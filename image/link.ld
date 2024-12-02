ENTRY(loader) /* entry label on loader.s */

SECTIONS {
       . = 0x00100000; /* the code should be loaded at 1MB */
       .text ALIGN (0x1000) :
       {
	       *(.text)
       }
       .rodata ALIGN (0x1000) :
       {
	      *(.rodata*)
       }
       .data ALIGN (0x1000) :
       {
	       *(.data)
       }
       .bss ALIGN (0x1000) :
       {
	       *(COMMON)
	       *(.bss*)
       }
}
