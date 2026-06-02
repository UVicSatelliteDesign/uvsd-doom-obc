# =============================================================================
# tests/Makefile — Host-based unit test build for UVSD DOOM OBC
# =============================================================================
#
# Usage
# -----
#   make          — build & run all tests (default)
#   make build    — compile only
#   make run      — run compiled test binaries
#   make clean    — remove build artefacts
#   make coverage — run with gcov/lcov HTML coverage report
#
# Adding a new test file
# ----------------------
#   1. Create tests/unit/test_<module>.c  (see test_example.c for the pattern)
#   2. Append it to the TESTS list below
#   3. If the test exercises a real source file in Core/Src, add it to
#      the SRC_FILES list below so it gets compiled into that test binary.
#
# =============================================================================

CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c11 \
            -Wno-unused-parameter \
            -g \
            -DUNIT_TEST \
            -DSTM32H733xx \
            -DUSE_HAL_DRIVER

# Coverage flags — only applied when 'make coverage' is called
COVFLAGS := --coverage -O0

# ── Include paths ─────────────────────────────────────────────────────────────
#
#  1. mocks/   — stub headers that shadow the real HAL/FreeRTOS/FatFs
#  2. unity/   — Unity test framework header
#  3. ../Core/Inc — the project's own application headers
#
INCLUDES := \
    -I./mocks \
    -I./unity \
    -I../Core/Inc

# ── Unity source ──────────────────────────────────────────────────────────────

UNITY_SRC := unity/unity.c

# ── Source files from the real firmware that tests need ──────────────────────
#
# Add Core/Src/<module>.c entries here when a test exercises that module.
# Do NOT add files that are entry points (main.c) or ISR/HAL hooks — those
# call hardware and will not compile cleanly on the host.
#
# Example (uncomment when you have a module to test):
#   SRC_FILES := ../Core/Src/comms_router.c \
#                ../Core/Src/telemetry.c    \
#                ../Core/Src/compression.c  \
#                ../Core/Src/boot_sequence.c \
#                ../Core/Src/logger.c
#
SRC_FILES :=

# ── Test files ────────────────────────────────────────────────────────────────
#
# Add every tests/unit/test_*.c file here.
#
TESTS := \
    unit/test_example.c        \
    unit/test_comms_router.c   \
    unit/test_boot_sequence.c  \
    unit/test_compression.c    \
    unit/test_telemetry.c      \
    unit/test_logger.c
	
# ── Build directory ───────────────────────────────────────────────────────────

BUILD_DIR := build

# ── Derived targets ───────────────────────────────────────────────────────────

TEST_BINS := $(patsubst unit/%.c,$(BUILD_DIR)/%,$(TESTS))

# =============================================================================
# Targets
# =============================================================================

.PHONY: all build run clean coverage

all: build run

build: $(TEST_BINS)

# Each test binary links Unity + the shared SRC_FILES + the test file itself.
$(BUILD_DIR)/%: unit/%.c $(UNITY_SRC) $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

run: build
	@echo ""
	@echo "══════════════════════════════════════════════════"
	@echo "  Running OBC unit tests"
	@echo "══════════════════════════════════════════════════"
	@PASS=0; FAIL=0; \
	for bin in $(TEST_BINS); do \
	    echo ""; \
	    echo "── $$bin ──────────────────────────────────────"; \
	    if $$bin; then \
	        PASS=$$((PASS + 1)); \
	    else \
	        FAIL=$$((FAIL + 1)); \
	    fi; \
	done; \
	echo ""; \
	echo "══════════════════════════════════════════════════"; \
	echo "  Results: $$PASS passed, $$FAIL failed"; \
	echo "══════════════════════════════════════════════════"; \
	[ $$FAIL -eq 0 ]

coverage: CFLAGS += $(COVFLAGS)
coverage: build run
	@echo "Generating coverage report..."
	lcov --capture --directory $(BUILD_DIR) --output-file $(BUILD_DIR)/coverage.info \
	     --ignore-errors mismatch 2>/dev/null || \
	lcov --capture --directory $(BUILD_DIR) --output-file $(BUILD_DIR)/coverage.info
	lcov --remove $(BUILD_DIR)/coverage.info \
	     '*/unity/*' '/usr/*' \
	     --output-file $(BUILD_DIR)/coverage_filtered.info
	genhtml $(BUILD_DIR)/coverage_filtered.info \
	        --output-directory $(BUILD_DIR)/coverage_html
	@echo "Coverage report: $(BUILD_DIR)/coverage_html/index.html"

clean:
	rm -rf $(BUILD_DIR)