# UVSD DOOM OBC
[![CI](https://github.com/UVicSatelliteDesign/uvsd-doom-obc/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/UVicSatelliteDesign/uvsd-doom-obc/actions/workflows/ci.yml)

Code for the On-Board Computer on the DOOM project. The OBC runs on a STM32H733VGT6. Currently code for the DOOM Balloon phase.

### Instructions for generating code
After cloning the repository, in order to compile the code you will need to generate Middleware and filesystem sources using CubeMX. To do this,

1. Open `uvsd-doom-obc.ioc` in CubeMX.
2. It should be configured correctly already, but it is important that under the `Project Manager` tab, `Toolchain / IDE` is set to `STM32CubeIDE` and `Generate Under Root` is checked.
3. Click the `GENERATE CODE` button. This will add all required source files and build configurations to the project. You can now open the project in STM32CubeIDE and build it.

### Building without CubeIDE (CI)
CI cross-compiles the firmware without CubeMX by fetching the same ST sources pinned in the `.ioc`:

```bash
ci/firmware/fetch_cube_deps.sh   # fills Drivers/, Middlewares/, FATFS/ and the linker script if missing
make -C ci/firmware              # → ci/firmware/build/uvsd-doom-obc.{elf,hex,bin}
```

Unit tests run on any Linux/macOS machine with `gcc` + `make`: see [tests/README.md](tests/README.md).
Hardware-in-the-loop tests (Raspberry Pi runner + STM32H733) are described in [docs/hil-setup.md](docs/hil-setup.md).
