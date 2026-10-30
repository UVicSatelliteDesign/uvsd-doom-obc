#!/usr/bin/env bash
# =============================================================================
# ci/firmware/fetch_cube_deps.sh
# Populate the CubeMX-generated folders (Drivers/, Middlewares/, FATFS/ and the
# linker script) without running CubeMX.
# =============================================================================
#
# The team rule is that generated code is never committed, but CubeMX can't be
# installed on a GitHub runner. Instead this script pulls the exact same ST
# sources CubeMX would copy in, pinned to the firmware package in the .ioc
# (STM32Cube FW_H7 V1.12.1), straight from ST's GitHub.
#
# FATFS/ and STM32H733VGTX_FLASH.ld come from ci/firmware/ and are only copied
# if missing, so a tree that already has real CubeMX output is left alone.
#
# Usage (from the repo root):  ci/firmware/fetch_cube_deps.sh
# Then build with:             make -C ci/firmware
# =============================================================================
set -euo pipefail

CUBE_TAG="v1.12.1"
CMSIS_DEVICE_SHA="e8d40ae6e2fa06afe5b46d24756f141b363342a7"   # Drivers/CMSIS/Device/ST/STM32H7xx @ v1.12.1
HAL_DRIVER_SHA="57ed86b4c358e2333a71fd895b17f1ccdb68739e"     # Drivers/STM32H7xx_HAL_Driver     @ v1.12.1

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
HERE="$ROOT/ci/firmware"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
cd "$ROOT"

# Shallow-fetch a single commit of a repo into a directory.
fetch_commit() {
    local url="$1" dest="$2" sha="$3"
    rm -rf "$dest"
    mkdir -p "$dest"
    git -C "$dest" init -q
    git -C "$dest" fetch -q --depth 1 "$url" "$sha"
    git -C "$dest" checkout -q FETCH_HEAD
    rm -rf "$dest/.git"
}

if [[ ! -d Drivers/STM32H7xx_HAL_Driver/Src ]]; then
    echo "Fetching STM32H7 HAL driver ($HAL_DRIVER_SHA)"
    fetch_commit https://github.com/STMicroelectronics/stm32h7xx_hal_driver.git \
        Drivers/STM32H7xx_HAL_Driver "$HAL_DRIVER_SHA"
fi

if [[ ! -d Drivers/CMSIS/Device/ST/STM32H7xx/Include ]]; then
    echo "Fetching CMSIS device headers ($CMSIS_DEVICE_SHA)"
    fetch_commit https://github.com/STMicroelectronics/cmsis_device_h7.git \
        Drivers/CMSIS/Device/ST/STM32H7xx "$CMSIS_DEVICE_SHA"
fi

if [[ ! -d Drivers/CMSIS/Include || ! -d Middlewares/Third_Party/FreeRTOS || ! -d Middlewares/Third_Party/FatFs ]]; then
    echo "Fetching CMSIS core, FreeRTOS and FatFs from STM32CubeH7 $CUBE_TAG"
    git clone -q --filter=blob:none --no-checkout --depth 1 -b "$CUBE_TAG" \
        https://github.com/STMicroelectronics/STM32CubeH7.git "$TMP/cube" 2>/dev/null
    git -C "$TMP/cube" sparse-checkout set --no-cone \
        /Drivers/CMSIS/Include/ \
        /Middlewares/Third_Party/FreeRTOS/Source/ \
        /Middlewares/Third_Party/FatFs/src/
    git -C "$TMP/cube" checkout -q
    mkdir -p Drivers/CMSIS Middlewares/Third_Party/FreeRTOS Middlewares/Third_Party/FatFs
    [[ -d Drivers/CMSIS/Include ]] || cp -R "$TMP/cube/Drivers/CMSIS/Include" Drivers/CMSIS/
    [[ -d Middlewares/Third_Party/FreeRTOS/Source ]] || cp -R "$TMP/cube/Middlewares/Third_Party/FreeRTOS/Source" Middlewares/Third_Party/FreeRTOS/
    [[ -d Middlewares/Third_Party/FatFs/src ]] || cp -R "$TMP/cube/Middlewares/Third_Party/FatFs/src" Middlewares/Third_Party/FatFs/
fi

# FatFs glue that CubeMX would generate for "FATFS: User-defined" with FreeRTOS.
for f in App/fatfs.c App/fatfs.h Target/user_diskio.c Target/user_diskio.h; do
    if [[ ! -f "FATFS/$f" ]]; then
        mkdir -p "FATFS/$(dirname "$f")"
        cp "$HERE/FATFS/$f" "FATFS/$f"
    fi
done

# ffconf.h is ST's template with the settings CubeMX writes when FreeRTOS is on.
if [[ ! -f FATFS/Target/ffconf.h ]]; then
    sed -e 's/^#define _FS_REENTRANT[[:space:]].*/#define _FS_REENTRANT	1/' \
        -e 's/^#define[[:space:]]_USE_LFN[[:space:]].*/#define	_USE_LFN	0/' \
        -e 's/^#define _VOLUMES[[:space:]].*/#define _VOLUMES	1/' \
        Middlewares/Third_Party/FatFs/src/ffconf_template.h > FATFS/Target/ffconf.h
fi

[[ -f STM32H733VGTX_FLASH.ld ]] || cp "$HERE/STM32H733VGTX_FLASH.ld" STM32H733VGTX_FLASH.ld

echo "CubeMX dependencies ready."
