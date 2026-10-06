# OBC unit tests

Host-based unit tests for the OBC firmware, using the
[Unity](https://github.com/ThrowTheSwitch/Unity) framework. They compile the
firmware's C code with your normal `gcc`, swap the STM32 HAL, CMSIS-RTOS and
FatFs out for small mock headers, and run on your laptop or a GitHub runner.
No ARM toolchain or board is needed.

Hardware tests are separate: see [../docs/hil-setup.md](../docs/hil-setup.md).

## Running the tests

You need `gcc` (or clang) and `make`. Linux, macOS and WSL all work.

```bash
cd tests
make            # build and run everything
```

The output ends with a summary per binary and an overall line:

```
══════════════════════════════════════════════════
  Results: 6 passed, 0 failed
══════════════════════════════════════════════════
```

`Results` counts **test binaries** (one per `unit/test_*.c` file), not single
test cases. Each binary prints its own Unity summary, such as
`7 Tests 0 Failures 7 Ignored`. A binary passes when none of its tests fail.
Ignored tests (`TEST_IGNORE_MESSAGE`) are placeholders for modules that don't
exist yet. They print as `IGNORE` with a TODO and don't fail the run.

| Command | What it does |
|---------|--------------|
| `make` / `make all` | Build and run every test |
| `make build` | Compile only |
| `make run` | Run already-built binaries |
| `make build/test_command && ./build/test_command` | Build and run one file |
| `make coverage` | Clean rebuild with gcov, run, write an lcov HTML report (needs `lcov`) |
| `make clean` | Delete `build/` |

## Layout

```
tests/
├── Makefile          build rules, list of tests, per-test firmware sources
├── unity/            Unity framework (vendored, don't edit)
├── mocks/            stand-ins for hardware headers
│   ├── stm32h7xx_hal.h   HAL types + GPIO/UART/SPI/I2C/TIM/NVIC/MPU stubs
│   ├── cmsis_os.h        CMSIS-RTOS v2: kernel, threads, mutex, semaphore, queue
│   └── fatfs.h           FatFs types and f_* stubs
└── unit/             one test_<module>.c per firmware module
```

The include order is `mocks/`, then `unity/`, then `../Core/Inc`. So when
firmware code does `#include "main.h"`, the real `main.h` is used, but its
`#include "stm32h7xx_hal.h"` gets the mock.

## Which tests exist

Each file's header comment says which OBC requirement it covers.

| File | Covers | Firmware linked |
|------|--------|-----------------|
| `test_example.c` | Template that shows the patterns | none |
| `test_command.c` | Req 1: command dispatch between TTC and subsystems | `Core/Src/command.c` |
| `test_boot_sequence.c` | Req 2: boot sequence and antenna deployment | none yet |
| `test_telemetry.c` | Req 3: telemetry storage and forwarding to TTC | `Core/Src/telemetry.c` |
| `test_logger.c` | Req 4: storing spacecraft logs | none yet |
| `test_burnwire.c` | Req 5: burn-wire release and parachute deploy | none yet |

Most tests are still `TEST_IGNORE` placeholders. When a module gets written,
replace the ignore with real assertions (see below).

## Adding a test file

1. Copy `unit/test_example.c` to `unit/test_<module>.c`.
2. Start the file with a doc comment covering the requirement or behaviour
   under test and any open questions. That comment is the test's
   documentation, so keep it current.
3. `#include` the module's header and write `void test_<behaviour>(void)`
   functions. Name each one after the behaviour it checks
   (`test_store_returns_error_when_full`, not `test_store_3`).
4. Call each test with `RUN_TEST(...)` in the file's `main()`.
5. Add the file to `TESTS` in `Makefile`.
6. If the test calls real firmware code, add the source files it needs:
   ```make
   SRC_test_<module> := ../Core/Src/<module>.c
   ```
   Each test binary links only its own list. Don't add `main.c`, the
   `stm32h7xx_*.c` files or `freertos.c`. They are entry points and
   interrupt or HAL hooks that need real hardware.

CI picks up the new file automatically on the next push.

## Turning a placeholder into a real test

```c
void test_store_is_empty_after_init(void)
{
    TEST_IGNORE_MESSAGE("TODO: implement when telemetry.c exists");
}
```
becomes, once `telemetry.h` exists:
```c
void test_store_is_empty_after_init(void)
{
    telemetry_init();
    TEST_ASSERT_EQUAL_UINT32(0, telemetry_count());
}
```
Then:
- move the placeholder types at the top of the test file into the module's
  real header, and include that header instead;
- make sure the module's `.c` file is in that test's `SRC_test_<module>`;
- if it fails to link because of a missing HAL/RTOS function, add a stub to the mock (next section).

## Extending the mocks

Mocks are `static inline` functions in a header, so they need no extra source
file and never cause duplicate-symbol errors. Add the smallest stub that lets
the code compile:

```c
/* mocks/stm32h7xx_hal.h */
static inline HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *h) { (void)h; return HAL_OK; }
```

If a test needs to **control** what the hardware returns, or check what the
code sent, give the mock a variable the test can set or read:

```c
/* mocks/stm32h7xx_hal.h */
extern GPIO_PinState mock_gpio_read_value;   /* define it in the test file */
static inline GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *g, uint16_t p)
{ (void)g; (void)p; return mock_gpio_read_value; }
```

```c
/* unit/test_burnwire.c */
GPIO_PinState mock_gpio_read_value;
void setUp(void) { mock_gpio_read_value = GPIO_PIN_RESET; }
```

Reset mock state in `setUp()` so tests don't affect each other.

## Coverage

```bash
sudo apt install lcov        # macOS: brew install lcov
cd tests && make coverage
open build/coverage_html/index.html     # Linux: xdg-open
```

The report only covers firmware sources under `Core/Src`, not the tests or
Unity. In CI, the **Coverage** job prints the same per-file table in the job
summary and uploads the HTML report as the `coverage-report` artifact
(Actions → the run → Artifacts). Coverage is reported only. It never fails a
build.

## In CI

`.github/workflows/ci.yml` runs these jobs on every push and PR to `main`:

| Job | Runs |
|-----|------|
| **Unit Tests** | `make all` in `tests/` |
| **Coverage** | `make coverage` + report artifact |
| **Static Analysis (cppcheck)** | cppcheck on `Core/Src` and `Core/Inc` |
| **Firmware Build** | cross-compiles the real firmware (see `ci/firmware/`) |

To reproduce a CI failure, run the same `make` command locally. CI uses
Ubuntu's `gcc`. macOS's `gcc` is actually clang and prints a few extra
warnings (e.g. `-Wnewline-eof`), which you can ignore.
