#include <plf/platform.h>
#include <plf/memory.h>
#include <plf/safestr.h>
#include <plf/io.h>
#include <spd/spd.h>
#include <fip.h>
#include <spd/spd.h>
#include "bios_high.h"



const char bl1_bios_main_code_version[]    __attribute__ ((section (".keywords")))     = \
       "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_high/branches/haspoc/hikey/bios_high/runtime.c $ $Revision: 26989 $";


extern char __bl31_start, __bl31_end;
extern char __sram_heap_start, __sram_heap_end;
extern char __SRAM_BASE__,__SRAM_TOP__;
extern char __RO_START__, __RO_END__,__PARAMS_BASE__;
extern char __BSS_START__, __BSS_END__,__DATA_RAM_START__;
extern char __DATA_RAM_END__, __DATA_ROM_START__,_LOW_LEVEL_SRAM_STACK,_LOW_LEVEL_SRAM_STACK_END;
extern int __BSS_SIZE__, __DATA_SIZE__,__RO_SIZE__;


#define SRAM_HEAP_START (uint64_t)(&__sram_heap_start)
#define SRAM_HEAP_END (uint64_t)(&__sram_heap_end)

#define BL31_START (uint64_t)(&__bl31_start)
#define BL31_END (uint64_t)(&__bl31_end)

int main(void); 

#define DBG_LEVEL          1
#define DBG_LEVEL_VERBOSE  3


#if 0 
void stack(int i)
 {
 int st = i ;
  if ( i < 10 ) {
    trace_d(DBG_LEVEL,"stack 0x%x[0x%x]\n",&st,(uint32_t)(&_LOW_LEVEL_SRAM_STACK_END));
    stack(i+1);
  }
 }
#endif


static void vma_dump(void) 
{
        trace_d(DBG_LEVEL,"\nSecure Boot VMA\n\n");
        trace_d(DBG_LEVEL,"SRAM\n");
        trace_d(DBG_LEVEL,"\t__SRAM_BASE__             = 0x%x\n",(uint32_t)(&__SRAM_BASE__));
        trace_d(DBG_LEVEL,"\t__RO_START__              = 0x%x\n",(uint32_t)(&__RO_START__));
        trace_d(DBG_LEVEL,"\t__RO_END__                = 0x%x\n",(uint32_t)(&__RO_END__));
        trace_d(DBG_LEVEL,"\n");
        trace_d(DBG_LEVEL,"\t__DATA_RAM_START__        = 0x%x\n",(uint32_t)(&__DATA_RAM_START__));
        trace_d(DBG_LEVEL,"\t__DATA_RAM_END__          = 0x%x\n",(uint32_t)(&__DATA_RAM_END__));
        trace_d(DBG_LEVEL,"\t__BSS_START__             = 0x%x\n",(uint32_t)(&__BSS_START__));
        trace_d(DBG_LEVEL,"\t__BSS_END__               = 0x%x\n",(uint32_t)(&__BSS_END__));
        trace_d(DBG_LEVEL,"\t__PARAMS_BASE__           = 0x%x\n",(uint32_t)(&__PARAMS_BASE__));
        trace_d(DBG_LEVEL,"\t_LOW_LEVEL_SRAM_STACK_END = 0x%x\n",(uint32_t)(&_LOW_LEVEL_SRAM_STACK_END));
        trace_d(DBG_LEVEL,"\t_LOW_LEVEL_SRAM_STACK     = 0x%x\n",(uint32_t)(&_LOW_LEVEL_SRAM_STACK));
#if 0 
        trace_d(DBG_LEVEL,"\t__bl31_start              = 0x%x\n",(uint32_t)(&__bl31_start));
        trace_d(DBG_LEVEL,"\t__bl31_end                = 0x%x\n",(uint32_t)(&__bl31_end));
        trace_d(DBG_LEVEL,"\t__sram_heap_start         = 0x%x\n",(uint32_t)(&__sram_heap_start));
        trace_d(DBG_LEVEL,"\t__sram_heap_end           = 0x%x\n",(uint32_t)(&__sram_heap_end));
#endif
        trace_d(DBG_LEVEL,"\t__SRAM_TOP__              = 0x%x\n",(uint32_t)(&__SRAM_TOP__));
        trace_d(DBG_LEVEL,"\n");
        // trace_d(DBG_LEVEL,"__DATA_ROM_START__ =\t0x%x\n",(uint32_t)(&__DATA_ROM_START__));
        trace_d(DBG_LEVEL,"Size\n");
        trace_d(DBG_LEVEL,"\n");
        trace_d(DBG_LEVEL,"\tSRAM_SIZE                 = %d\n",(uint32_t) (&__SRAM_TOP__ - &__SRAM_BASE__));
        trace_d(DBG_LEVEL,"\t__RO_SIZE__               = %d\n",(uint32_t)(&__RO_SIZE__));
        trace_d(DBG_LEVEL,"\t__BSS_SIZE__              = %d\n",(uint32_t)(&__BSS_SIZE__));
        trace_d(DBG_LEVEL,"\t__DATA_SIZE__             = %d\n",(uint32_t)(&__DATA_SIZE__));
        trace_d(DBG_LEVEL,"\tSTACK_SIZE                = %d\n",(uint32_t) (&_LOW_LEVEL_SRAM_STACK - &_LOW_LEVEL_SRAM_STACK_END));
#if 0 
        trace_d(DBG_LEVEL,"\tBL31_SIZE                 = %d\n",(uint32_t) BL31_END - BL31_START);
#endif
        trace_d(DBG_LEVEL,"\tSRAM_HEAP_SIZE            = %d\n",(uint32_t) SRAM_HEAP_END - SRAM_HEAP_START);
        trace_d(DBG_LEVEL,"\n");
        trace_d(DBG_LEVEL,"\n");
}



void bios_high_runtime()
{
	int  rc; 
	uint64_t time, freq;
      
#if 1 
        vma_dump();
#endif
#if 0 
        stack(0);
#endif



        time = read_pmccntr_el0();
        freq = read_cntfrq_el0();
        trace_d(DBG_LEVEL,"Config Elapsed time(ms): %d\n\n",(time/(freq/1000)));

	NMemPool((char *) SRAM_HEAP_START, (size_t) (SRAM_HEAP_END - SRAM_HEAP_START) ,1);
        rc = _main(BOOT_START_SECTOR,INITIAL_READ);
        trace_e("Error: %d\n",rc);
        while(1);
}

