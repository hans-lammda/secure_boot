/*! \file bios_memory.h
    \brief MMU and DRAM support functions

    @\n

    This module contais code from ARM Trusted Firmware

    URL: github.com/96boards

    Revision: 1.1 tag: ca9f7ed


 */

/*
  Copyright (C) 2016 Oniteo
 */

#ifndef __ONITEO_BIOS_MEMORY_H__
#define __ONITEO_BIOS_MEMORY_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_memory/branches/haspoc/hikey/bios_memory/bios_memory.h $ $Revision: 27103 $"


extern char __sdram_heap_start, __sdram_heap_end;
#define SDRAM_HEAP_START (uint64_t)  (&__sdram_heap_start)
#define SDRAM_HEAP_END   (uint64_t)  (&__sdram_heap_end)

/*! \brief  Successfully configured SDRAM  */
#define SDRAM_SETUP_OK 1

/*! \brief  Failed to configure SDRAM  */
#define SDRAM_SETUP_ERROR 0


/*! \name Cache 
@{
*/
/*! \brief Flush cache 

    Assure that all data is written to 
    
    memory prior to boot of payload 


    \param[in] start of region to flush 
    \param[in] size of region 

*/

void flush_dcache_range(uint64_t start, uint64_t size);

/*! \brief cache support 

     High level cache operation 

*/
void dcsw_op_all(void);

/*! \brief cache level 1

     Cache operation level 1 

*/
void dcsw_op_level1(void);

/*! \brief cache level 2

     Cache operation level 2

*/
void dcsw_op_level2(void);


/*!
@}
*/


/*! \name DRAM 
@{
*/

/*! \brief Configure memory 

    This is a placeholder function that may 

    be used to configure memory. 

    The bl30.bin image retrived from storage 

    is passed as start and size. 

    \param[in] start of buffer  
    \param[in] size of  buffer

*/

int setup_sdram(uint64_t start, uint32_t size);
/*!
@}
*/

/*! \name MMU 
@{
*/

/*! \brief Configure MMU 

     @\n

    Secure Boot use one translation table 

    during all phases 
 
*/

void bios_memory_mmu_setup(void);
/*!
@}
*/


#endif

