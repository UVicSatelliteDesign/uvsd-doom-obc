/*
 * Linker script for the STM32H733VGTx (1 MB flash), used by the CI firmware
 * build. CubeMX generates its own STM32H733VGTX_FLASH.ld at the repo root;
 * fetch_cube_deps.sh only copies this one there when that file is missing.
 *
 * Code and constants live in flash, .data/.bss/heap/stack live in AXI SRAM
 * (RAM_D1), matching the CubeIDE default layout and the symbols that
 * Core/Startup/startup_stm32h733vgtx.s and Core/Src/sysmem.c expect.
 */

ENTRY(Reset_Handler)

_estack = ORIGIN(RAM_D1) + LENGTH(RAM_D1);

/* Must match ProjectManager.HeapSize / StackSize in uvsd-doom-obc.ioc */
_Min_Heap_Size  = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
  ITCMRAM (xrw) : ORIGIN = 0x00000000, LENGTH = 64K
  DTCMRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH   (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM_D1  (xrw) : ORIGIN = 0x24000000, LENGTH = 320K
  RAM_D2  (xrw) : ORIGIN = 0x30000000, LENGTH = 32K
  RAM_D3  (xrw) : ORIGIN = 0x38000000, LENGTH = 16K
}

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } >FLASH

  /* Load address of .data, copied to RAM by the startup code */
  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    *(.RamFunc)
    *(.RamFunc*)
    . = ALIGN(4);
    _edata = .;
  } >RAM_D1 AT> FLASH

  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM_D1

  /* Fails the link if the heap and stack don't fit in RAM_D1 */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    PROVIDE(_end = .);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM_D1

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
