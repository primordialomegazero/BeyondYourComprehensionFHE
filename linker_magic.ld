SECTIONS
{
  .text : { *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}

/* CUSTOM SYMBOL INJECTION — add = a + b LOGIC AT LINKER LEVEL */
PROVIDE(add = 0x8);
