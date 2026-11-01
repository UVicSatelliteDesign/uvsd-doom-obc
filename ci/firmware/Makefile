# =============================================================================
# ci/firmware/Makefile
# Cross-compile the OBC firmware with arm-none-eabi-gcc, outside CubeIDE.
# =============================================================================
#
# Mirrors the Debug configuration in .cproject (same defines, include paths,
# CPU flags and linker script) so a PR that breaks the CubeIDE build also
# breaks CI.
#
# Prerequisite: the CubeMX folders must exist. Either generate code in CubeMX
# or run ci/firmware/fetch_cube_deps.sh once.
#
# Targets:
#   make -C ci/firmware          build build/uvsd-doom-obc.{elf,hex,bin}
#   make -C ci/firmware clean    remove ci/firmware/build
# =============================================================================

ROOT   := ../..
BUILD  := build
TARGET := uvsd-doom-obc

PREFIX  ?= arm-none-eabi-
CC      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE    := $(PREFIX)size

# ── Sources ───────────────────────────────────────────────────────────────────
HAL_DIR   := $(ROOT)/Drivers/STM32H7xx_HAL_Driver/Src
RTOS_DIR  := $(ROOT)/Middlewares/Third_Party/FreeRTOS/Source
FATFS_DIR := $(ROOT)/Middlewares/Third_Party/FatFs/src

# Every hand-written and CubeMX-owned file in Core/
CORE_SRCS := $(wildcard $(ROOT)/Core/Src/*.c)

# HAL: all modules except the *_template.c examples (the HAL config header
# decides which ones actually compile to anything)
HAL_SRCS := $(filter-out %_template.c,$(wildcard $(HAL_DIR)/*.c))

RTOS_SRCS := \
  $(RTOS_DIR)/croutine.c \
  $(RTOS_DIR)/event_groups.c \
  $(RTOS_DIR)/list.c \
  $(RTOS_DIR)/queue.c \
  $(RTOS_DIR)/stream_buffer.c \
  $(RTOS_DIR)/tasks.c \
  $(RTOS_DIR)/timers.c \
  $(RTOS_DIR)/CMSIS_RTOS_V2/cmsis_os2.c \
  $(RTOS_DIR)/portable/MemMang/heap_4.c \
  $(RTOS_DIR)/portable/GCC/ARM_CM4F/port.c

FATFS_SRCS := \
  $(FATFS_DIR)/diskio.c \
  $(FATFS_DIR)/ff.c \
  $(FATFS_DIR)/ff_gen_drv.c \
  $(FATFS_DIR)/option/syscall.c \
  $(ROOT)/FATFS/App/fatfs.c \
  $(ROOT)/FATFS/Target/user_diskio.c

C_SRCS   := $(CORE_SRCS) $(HAL_SRCS) $(RTOS_SRCS) $(FATFS_SRCS)
ASM_SRCS := $(ROOT)/Core/Startup/startup_stm32h733vgtx.s

LDSCRIPT := $(ROOT)/STM32H733VGTX_FLASH.ld

# ── Flags (from .cproject, Debug) ─────────────────────────────────────────────
MCU := -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb

DEFS := -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H733xx

INCS := \
  -I$(ROOT)/Core/Inc \
  -I$(ROOT)/FATFS/Target \
  -I$(ROOT)/FATFS/App \
  -I$(ROOT)/Drivers/STM32H7xx_HAL_Driver/Inc \
  -I$(ROOT)/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy \
  -I$(RTOS_DIR)/include \
  -I$(RTOS_DIR)/CMSIS_RTOS_V2 \
  -I$(RTOS_DIR)/portable/GCC/ARM_CM4F \
  -I$(FATFS_DIR) \
  -I$(ROOT)/Drivers/CMSIS/Device/ST/STM32H7xx/Include \
  -I$(ROOT)/Drivers/CMSIS/Include

CFLAGS  := $(MCU) $(DEFS) $(INCS) -std=gnu11 -g3 -O0 -Wall \
           -ffunction-sections -fdata-sections -fstack-usage -MMD -MP
ASFLAGS := $(MCU) -g3 -x assembler-with-cpp -MMD -MP
LDFLAGS := $(MCU) -T$(LDSCRIPT) --specs=nosys.specs --specs=nano.specs \
           -Wl,-Map=$(BUILD)/$(TARGET).map -Wl,--gc-sections -static \
           -Wl,--start-group -lc -lm -Wl,--end-group

# Object files mirror the source tree under build/ (strip the ../../ prefix)
obj = $(patsubst $(ROOT)/%,$(BUILD)/%,$(1))
C_OBJS   := $(call obj,$(C_SRCS:.c=.o))
ASM_OBJS := $(call obj,$(ASM_SRCS:.s=.o))
OBJS     := $(C_OBJS) $(ASM_OBJS)

.PHONY: all clean check-deps
all: check-deps $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).hex $(BUILD)/$(TARGET).bin

check-deps:
	@test -d $(HAL_DIR) -a -d $(RTOS_DIR) -a -f $(LDSCRIPT) -a -d $(ROOT)/FATFS || \
	  { echo "CubeMX folders missing: run ci/firmware/fetch_cube_deps.sh or generate code in CubeMX"; exit 1; }

$(BUILD)/%.o: $(ROOT)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(ROOT)/%.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
