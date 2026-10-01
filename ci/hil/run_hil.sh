#!/usr/bin/env bash
# =============================================================================
# ci/hil/run_hil.sh
# Hardware-in-the-loop smoke test: flash the OBC firmware onto a real
# STM32H733 over ST-Link and check that it boots.
# =============================================================================
#
# Runs on the Raspberry Pi self-hosted runner (see docs/hil-setup.md), but works
# on any Linux/macOS machine with openocd and an ST-Link attached.
#
# Checks, in order:
#   1. Target identity — the chip's DBGMCU_IDCODE must be 0x483 (STM32H72x/73x).
#      Anything else aborts BEFORE flashing so a wrong board is never written.
#   2. Flash + verify — openocd programs the ELF and verifies it read-back.
#   3. Boot — after BOOT_WAIT seconds the core is halted and its PC must be
#      inside flash and NOT inside a fault/error handler (HardFault, Error_Handler…).
#   4. UART (optional) — if HIL_SERIAL_PORT is set, read it for HIL_UART_SECS and
#      require HIL_UART_EXPECT (an extended regex) to appear.
#
# Usage:
#   ci/hil/run_hil.sh path/to/uvsd-doom-obc.elf
#
# Environment (all optional):
#   HIL_LOG_DIR       where logs go                      (default: hil-logs)
#   BOOT_WAIT         seconds to let firmware run        (default: 3)
#   HIL_SERIAL_PORT   e.g. /dev/ttyACM0 (ST-Link VCP)    (default: unset = skip)
#   HIL_BAUD          UART baud rate                     (default: 115200)
#   HIL_UART_SECS     seconds to capture UART            (default: 5)
#   HIL_UART_EXPECT   regex that must appear on UART     (default: .+ = any output)
#   OPENOCD_ADAPTER   openocd interface config           (default: interface/stlink.cfg)
#
# Exit code is 0 only if every enabled check passes.
# =============================================================================
set -euo pipefail

ELF="${1:?usage: run_hil.sh path/to/firmware.elf}"
LOG_DIR="${HIL_LOG_DIR:-hil-logs}"
BOOT_WAIT="${BOOT_WAIT:-3}"
BAUD="${HIL_BAUD:-115200}"
UART_SECS="${HIL_UART_SECS:-5}"
UART_EXPECT="${HIL_UART_EXPECT:-.+}"
ADAPTER="${OPENOCD_ADAPTER:-interface/stlink.cfg}"
NM="${NM:-arm-none-eabi-nm}"

EXPECTED_DEV_ID="0x483"   # STM32H72x/73x (RM0468 DBGMCU_IDCODE.DEV_ID)
FLASH_START=$((0x08000000))
FLASH_END=$((0x08100000))  # 1 MB on the H733VG

mkdir -p "$LOG_DIR"
[[ -f "$ELF" ]] || { echo "FAIL: firmware not found: $ELF"; exit 1; }

ocd() {
    # Run one openocd session; stdout+stderr go to the log and are returned.
    local name="$1"; shift
    openocd -f "$ADAPTER" -f target/stm32h7x.cfg "$@" >"$LOG_DIR/openocd-$name.log" 2>&1 || {
        cat "$LOG_DIR/openocd-$name.log"
        echo "FAIL: openocd ($name) exited non-zero"
        exit 1
    }
    cat "$LOG_DIR/openocd-$name.log"
}

# ── 1. Target identity ────────────────────────────────────────────────────────
echo "== 1/4 Checking target identity"
out="$(ocd probe -c "init; reset halt; mdw 0x5C001000; shutdown")"
idcode="$(grep -oiE '0x5c001000: [0-9a-f]+' <<<"$out" | awk '{print $2}' | tail -1)"
[[ -n "$idcode" ]] || { echo "FAIL: could not read DBGMCU_IDCODE"; exit 1; }
dev_id=$(printf '0x%03x' $((0x$idcode & 0xFFF)))
echo "DBGMCU_IDCODE=0x$idcode DEV_ID=$dev_id"
if [[ "$dev_id" != "$EXPECTED_DEV_ID" ]]; then
    echo "FAIL: expected an STM32H72x/73x ($EXPECTED_DEV_ID), found $dev_id. Not flashing."
    exit 1
fi

# ── 2. Flash + verify ─────────────────────────────────────────────────────────
echo "== 2/4 Flashing $ELF"
ocd flash -c "program $ELF verify reset exit" >/dev/null
echo "Flash + verify OK"

# ── 3. Boot check ─────────────────────────────────────────────────────────────
echo "== 3/4 Letting firmware run for ${BOOT_WAIT}s"
sleep "$BOOT_WAIT"
out="$(ocd boot -c "init; halt; reg pc; resume; shutdown")"
pc_hex="$(grep -oiE 'pc[^:]*: 0x[0-9a-f]+' <<<"$out" | grep -oiE '0x[0-9a-f]+' | tail -1)"
[[ -n "$pc_hex" ]] || { echo "FAIL: could not read PC"; exit 1; }
pc=$((pc_hex))
echo "PC=$pc_hex"

if (( pc < FLASH_START || pc >= FLASH_END )); then
    echo "FAIL: PC is outside flash, the firmware is not running from flash"
    exit 1
fi

# Any of these means the firmware crashed or gave up.
FAULT_SYMBOLS="HardFault_Handler MemManage_Handler BusFault_Handler UsageFault_Handler NMI_Handler Default_Handler Error_Handler"
while read -r addr size _type name; do
    for f in $FAULT_SYMBOLS; do
        [[ "$name" == "$f" ]] || continue
        start=$((0x$addr & ~1)); len=$((0x$size))
        if (( pc >= start && pc < start + len )); then
            echo "FAIL: firmware is stuck in $name"
            exit 1
        fi
    done
done < <("$NM" -S --defined-only "$ELF" | awk 'NF==4')
echo "Boot OK: running, not in a fault handler"

# ── 4. UART (optional) ────────────────────────────────────────────────────────
if [[ -z "${HIL_SERIAL_PORT:-}" ]]; then
    echo "== 4/4 UART check skipped (HIL_SERIAL_PORT not set)"
    exit 0
fi
echo "== 4/4 Reading $HIL_SERIAL_PORT @ $BAUD for ${UART_SECS}s"
if [[ "$(uname)" == "Darwin" ]]; then
    stty -f "$HIL_SERIAL_PORT" "$BAUD" raw -echo
else
    stty -F "$HIL_SERIAL_PORT" "$BAUD" raw -echo
fi
# Reset so the capture starts from boot.
ocd reset -c "init; reset run; shutdown" >/dev/null &
timeout "$UART_SECS" cat "$HIL_SERIAL_PORT" >"$LOG_DIR/uart.log" || true
wait
cat "$LOG_DIR/uart.log"
if grep -qE "$UART_EXPECT" "$LOG_DIR/uart.log"; then
    echo "UART OK: matched /$UART_EXPECT/"
else
    echo "FAIL: UART output did not match /$UART_EXPECT/"
    exit 1
fi
