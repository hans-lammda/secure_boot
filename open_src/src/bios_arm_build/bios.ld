OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(bl1_entrypoint)

/* 
$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_arm_build/branches/haspoc/hikey/bios_arm_build/bios.ld $
$Revision: 27146 $
*/ 


SECTIONS
{

    . = ROMBASE;

    .ro ALIGN(0x8) : {
        __RO_START__ = .;
        /* This module must be allocated at very beginning */ 
        *bl1_entrypoint.o(.text*)


        *(.text*)
        *(.rodata*)

        /* Exception handler */ 
        *(.vectors*)
	 . = ALIGN(8);

        /* Support for ATF   */ 
        __CPU_OPS_START__ = .;
        KEEP(*(cpu_ops))
        __CPU_OPS_END__ = .;


        . = ALIGN(0x1000);
	*(.xlat_l2*)

        KEEP(*(.keywords))
        KEEP(*(.ident)) 
        __RO_END__ = .;
    } 



     /* Data section from ROM must be copied to SRAM by C-runtime
     * LMA = location in ROM , VMA location in runtime (AT)
     */

    __DATA_ROM_START__ =  ROMBASE +  ( __RO_END__ -  __RO_START__ );

    .data  : AT (__DATA_ROM_START__)  {
        __DATA_RAM_START__ = .;

      . = ALIGN(0x1000);
	*(.xlat_l1*)

        *(.data*)
        __DATA_RAM_END__ = .;
    } 



    .bss ALIGN (4096) : {
        __BSS_START__ = .;
        *(.bss*)
        *(COMMON)
        __BSS_END__ = .;
    } 

    .param_block ALIGN(4096) : {
      __PARAMS_BASE__ = . ;
      .  =  .  + 0x1000 ;
     }

    /* Stack starts at high address and grows towards lower */
    .stack :
     {
      _LOW_LEVEL_SRAM_STACK_END =  . ;
      .  =  .  + STACK_SIZE  ;
      . = ALIGN(16);
      _LOW_LEVEL_SRAM_STACK  =  . ;
     }



    /* Heap starts at low address and grows towards higher */
    .heap_sram ALIGN(4096) :
     {
       __sram_heap_start = . ;
       . = . + SRAM_HEAP_SIZE ;
      __sram_heap_end = . ;
     }
     

     ASSERT((.  <= BL31_START), "Overlaps reserved area for BL31")

     /* The leftover section should be empty 
        since it will be removed later    
      */
     .leftover . : { 
        *(*)
        *(.*)
     }

     ASSERT((.  <= (SRAMBASE + SRAM_SIZE)), "Secure Boot exceeds SRAM_SIZE")


    __DATA_SIZE__ = __DATA_RAM_END__   - __DATA_RAM_START__ ;
    __BSS_SIZE__  = __BSS_END__        - __BSS_START__ ;
    __RO_SIZE__   = __RO_END__ -  __RO_START__ ;
    __SRAM_BASE__ = SRAMBASE; 
    __SRAM_TOP__  = SRAMBASE + SRAM_SIZE; 

}
