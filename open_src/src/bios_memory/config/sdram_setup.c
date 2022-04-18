#include <plf/platform.h>
#include <plf/io.h>
#include <bios_memory.h>
#include <bios_low.h>




const char bios_memory_setup_code_version[]    __attribute__ ((section (".keywords")))     = \
       "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_memory/branches/haspoc/hikey/bios_memory/config/sdram_setup.c $ $Revision: 26632 $";



int setup_sdram(uint64_t start, uint32_t size)				
{								
 int rc; 
 hisi_mcu_enable_sram();
 rc = hisi_mcu_load_image(start,size);

 if ( rc == MCU_LOAD_OK ) {
   hisi_mcu_start_run();
   return SDRAM_SETUP_OK; 
 }
 else 
   return SDRAM_SETUP_ERROR; 
}

