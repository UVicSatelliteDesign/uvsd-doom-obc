# Hardware-in-the-loop (HIL) test rig

The `HIL` workflow (`.github/workflows/hil.yml`) builds the OBC firmware on a
GitHub runner, then flashes it onto a real STM32H733 from a Raspberry Pi that
is registered as a self-hosted GitHub Actions runner. This page covers the
parts, wiring, Pi setup, runner registration, and common failures.

```
GitHub ──(push to main / manual run)──▶ ubuntu-24.04: build .elf
                                              │ artifact
                                              ▼
                                   Raspberry Pi (self-hosted runner)
                                              │ USB
                                              ▼
                                   ST-Link ──SWD──▶ STM32H733 (OBC or dev board)
                                      └── VCP UART ◀── USART (optional)
```

## What the test checks

`ci/hil/run_hil.sh` runs four checks, and stops at the first failure:

| # | Check | Fails when |
|---|-------|------------|
| 1 | **Target identity**: reads `DBGMCU_IDCODE` | The device ID isn't `0x483` (STM32H72x/73x). Nothing is flashed. |
| 2 | **Flash + verify**: `openocd program … verify` | The probe can't connect, or the read-back doesn't match |
| 3 | **Boot**: lets it run `BOOT_WAIT` s, halts, reads PC | PC is outside flash, or inside `HardFault_Handler`, `Error_Handler`, `Default_Handler`, etc. |
| 4 | **UART** (optional): reads the serial port | `HIL_UART_EXPECT` doesn't appear in the output |

Check 4 is skipped until the firmware prints something over a UART. The `.ioc`
doesn't enable a USART yet. When one is added, set the repo variables below.

Every openocd session and the UART capture are saved to `hil-logs/`, which is
uploaded as the `hil-logs` workflow artifact even when the run fails.

## Parts

| Part | Notes |
|------|-------|
| Raspberry Pi 4 or 5 (2 GB+) | Runs 64-bit Raspberry Pi OS Lite (Bookworm). A Pi 3B+ works but is slow. |
| 32 GB+ microSD card, A1/A2 rated | Holds the OS and the runner work directory |
| Official Pi PSU (USB-C, 5 V 3 A / 5 A for Pi 5) | Undervoltage causes random USB disconnects |
| ST-Link V2/V3 or the on-board ST-Link of a Nucleo/DK board | V3 recommended. Its VCP gives you UART for free. |
| STM32H733 target | Spare OBC board, or a **NUCLEO-H723ZG** / **STM32H735G-DK** (same family, same 0x483 device ID) |
| Ethernet cable (recommended) | Steadier than Wi-Fi for a runner that's always on |

> Use a **spare** board. The rig reflashes it on every push to `main`.

## Wiring

**Dev board with a built-in ST-Link** (Nucleo / DK): one USB cable from the
board's ST-Link port to the Pi. SWD and the VCP UART are already connected.

**OBC board + standalone ST-Link**: connect the ST-Link to the OBC's SWD header:

| ST-Link | OBC (STM32H733) |
|---------|-----------------|
| SWDIO | PA13 |
| SWCLK | PA14 |
| GND | GND |
| T_VCC / VREF | 3V3 (sense only, the ST-Link does **not** power the board) |
| NRST | NRST (recommended, lets openocd reset a hung chip) |
| VCP RX (V3 only, optional) | MCU USART TX, once one is configured |

Power the OBC from its own supply. Put a photo of the finished rig here once
it's built:

<!-- ![HIL rig](img/hil-rig.jpg) -->

## Pi setup

1. Flash **Raspberry Pi OS Lite (64-bit)** with Raspberry Pi Imager. In the
   imager settings, set a hostname (e.g. `uvsd-hil`), enable SSH and create a
   user (e.g. `runner`).
2. SSH in and install the tools:
   ```bash
   sudo apt update && sudo apt full-upgrade -y
   sudo apt install -y git curl openocd binutils-arm-none-eabi
   ```
3. Let the user access the ST-Link and serial port without sudo:
   ```bash
   sudo usermod -aG plugdev,dialout "$USER"
   # udev rules for ST-Link V2 / V2-1 / V3
   sudo tee /etc/udev/rules.d/49-stlink.rules >/dev/null <<'RULES'
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0660", GROUP="plugdev"
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0660", GROUP="plugdev"
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374e", MODE="0660", GROUP="plugdev"
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374f", MODE="0660", GROUP="plugdev"
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3753", MODE="0660", GROUP="plugdev"
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3754", MODE="0660", GROUP="plugdev"
   RULES
   sudo udevadm control --reload-rules && sudo udevadm trigger
   sudo reboot
   ```
4. Check that the Pi can see the board (it should report an STM32H72x/73x):
   ```bash
   openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "init; reset halt; mdw 0x5C001000; shutdown"
   ```
   The low three hex digits of the value printed after `0x5c001000:` must be
   `483`.

## Register the runner

You need **admin** on `UVicSatelliteDesign/uvsd-doom-obc` (or ask someone who
has it).

1. On GitHub: **Settings → Actions → Runners → New self-hosted runner**, pick
   **Linux / ARM64**, and copy the `config.sh` token.
2. On the Pi, follow the download commands GitHub shows, then configure
   it with the `obc-hil` label:
   ```bash
   mkdir ~/actions-runner && cd ~/actions-runner
   # curl … + tar … (copy these lines from the GitHub page)
   ./config.sh --url https://github.com/UVicSatelliteDesign/uvsd-doom-obc \
               --token <TOKEN> --name uvsd-hil --labels obc-hil --unattended
   sudo ./svc.sh install "$USER"
   sudo ./svc.sh start
   ```
   The runner now shows as **Idle** under Settings → Actions → Runners.
3. Under **Settings → Secrets and variables → Actions → Variables**, add:

   | Variable | Value | Required |
   |----------|-------|----------|
   | `HIL_ENABLED` | `true` | Yes. Without it the HIL workflow skips both jobs. |
   | `HIL_SERIAL_PORT` | e.g. `/dev/ttyACM0` | No. Turns on the UART check. |
   | `HIL_UART_EXPECT` | regex, e.g. `OBC boot` | No. Defaults to "any output". |

4. Run it: **Actions → HIL → Run workflow**.

### Security

The repo is public. **Never** add `pull_request` (or `pull_request_target`) to
`hil.yml`'s triggers, because a fork's PR would then run its own code on the
Pi. The workflow only runs on pushes to `main` and manual runs, which need
write access. Also keep the Pi off any network with sensitive machines, and
don't store credentials on it.

To take the rig offline, set `HIL_ENABLED` to `false` rather than leaving jobs
queued.

## Running the test without GitHub

From any machine with openocd and `arm-none-eabi-nm`:

```bash
ci/firmware/fetch_cube_deps.sh          # once: fetch ST HAL/CMSIS/FreeRTOS/FatFs
make -C ci/firmware                     # → ci/firmware/build/uvsd-doom-obc.elf
ci/hil/run_hil.sh ci/firmware/build/uvsd-doom-obc.elf
HIL_SERIAL_PORT=/dev/ttyACM0 ci/hil/run_hil.sh ci/firmware/build/uvsd-doom-obc.elf   # with UART
```

## Common failures

| Symptom in the log | Cause / fix |
|--------------------|-------------|
| `open failed` / `LIBUSB_ERROR_ACCESS` | udev rule missing or the user isn't in `plugdev`. Redo Pi setup step 3 and reboot. |
| `Error: init mode failed (unable to connect to the target)` | SWD wiring, target unpowered, or firmware disabled the SWD pins. Wire NRST and retry. Hold the board in reset while connecting if needed. |
| `expected an STM32H72x/73x (0x483), found 0x450` | Wrong board attached (0x450 = H743/H753). The script refuses to flash it. |
| `firmware is stuck in HardFault_Handler` | Firmware crashed at boot. Download the `.elf` artifact and debug it in CubeIDE. |
| `firmware is stuck in Error_Handler` | A HAL init call failed, usually clock or peripheral config in the `.ioc`. |
| `UART output did not match` | Wrong `HIL_SERIAL_PORT`, baud mismatch (`HIL_BAUD`), or the firmware hasn't printed yet. Raise `HIL_UART_SECS`. |
| Step fails asking for a newer runner (e.g. `2.327.1`) | The actions run on Node 24, which needs a recent runner. Runners update themselves; if auto-update is off, re-download it from the New self-hosted runner page. |
| Job stuck in **Queued** | The runner is offline. Check `sudo ./svc.sh status` on the Pi, or set `HIL_ENABLED=false`. |
| USB drops mid-run, `under-voltage detected` in `dmesg` | Use the official PSU, or a powered USB hub for the ST-Link. |
