/*! \file h_mmu.h
    \brief MMU Setup

    @\n

    This module provides a simple and static configuration 

    of the memory system for ARM v8 


    @\n


    Translation tables are tailored for storage in ROM 


*/

#ifndef _MMU_H
#define _MMU_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_memory/branches/haspoc/hikey/bios_memory/mmu/h_mmu.h $ $Revision: 26651 $"

#define MAIR_DEVICE	0x04
#define MAIR_MEM	0xFF

#define SCTLR_C_BIT		(1 << 2)
#define SCTLR_C_BIT_MASK        0x4

#define SCTLR_M_BIT		(1 << 0)
#define SCTLR_M_BIT_MASK        0x2 

#define SCTLR_WXN_BIT_DO_NOT_FORCE (0 << 19)
#define SCTLR_WXN_BIT_DO_NOT_FORCE_MASK 0x80000

#define DISABLE_DCACHE          (1 << 0)
#define DISABLE_DCACHE_MASK     0x2 

#define TABLE_ENTRY_POINTER     0x003  


#define TCR_EL3_RESERVED     (  \
                               (0x1 << 31) | (0x1 << 23) | (0x0 << 19) |  \
                               (0x0 << 7)  | (0x0 << 6)  \
                             )

#define TCR_TBI_USE             (0x0 << 20)
#define TCR_TBI_IGNORE          (0x1 << 20)
#define TCR_TBI_MASK            0x100000

#define TCR_ADDRESS_SIZE_4GB    (0x0 << 16)
#define TCR_ADDRESS_SIZE_64GB   (0x1 << 16)
#define TCR_ADDRESS_SIZE_1TB    (0x2 << 16)
#define TCR_ADDRESS_SIZE_4TB    (0x3 << 16)
#define TCR_ADDRESS_SIZE_16TB   (0x4 << 16)
#define TCR_ADDRESS_SIZE_256TB  (0x5 << 16)
#define TCR_ADDRESS_SIZE_MASK   0x10000

#define TCR_GRANULE_SIZE_4KB    (0x0 << 14)
#define TCR_GRANULE_SIZE_64KB   (0x1 << 14)
#define TCR_GRANULE_SIZE_16KB   (0x2 << 14)
#define TCR_GRANULE_SIZE_MASK   0x4000

#define TCR_SH_NON_SHAREABLE    (0x0 << 12)
#define TCR_SH_OUTER_SHAREABLE  (0x2 << 12)
#define TCR_SH_INNER_SHAREABLE  (0x3 << 12)
#define TCR_SH_MASK             0x1000

#define TCR_RGN_OUTER_NC        (0x0 << 10)
#define TCR_RGN_OUTER_WBA       (0x1 << 10)
#define TCR_RGN_OUTER_WT        (0x2 << 10)
#define TCR_RGN_OUTER_WBNA      (0x3 << 10)
#define TCR_RGN_OUTER_MASK      0x400

#define TCR_RGN_INNER_NC        (0x0 << 8)
#define TCR_RGN_INNER_WBA       (0x1 << 8)
#define TCR_RGN_INNER_WT        (0x2 << 8)
#define TCR_RGN_INNER_WBNA      (0x3 << 8)
#define TCR_RGN_INNER_MASK      0x100

/*! \brief  Memory operation successfull 
 */
#define MEMORY_OK 1 

/*! \brief  Memory operation failed 
 */
#define MEMORY_ERROR 0 



#define REGION_SIZE_OFFSET      (0x1 << 5)
#define REGION_SIZE_MASK        0x20

/*! \name Configure
@{
*/
/*! \brief SDRAM setup 



    @\n



    Configuration of memory depends on actual platform 

    

    \retval MEMORY_OK 
    \retval MEMORY_ERROR
    \return status

    

*/

int setup_sdram(void); 
/*!
@}
*/

/*! \name MMU 
@{
*/

/*! \brief Configure MMU 



    @\n



    This function setup and enable memory management unit 

    The page table translation used is transparent , which means 

    virtual addresses maps directly to physical 

    @\n

    No parameters and no exit code. 


*/

void bios_memory_mmu_setup(void); 


/*! \brief Disable TLB 



    @\n


    This function is called when Secure boot pass control to BL31. 

    It purpose is to assure a well defined state. 


*/
void __mmu_tlb_inv(void); 

/*!
@}
*/

/*! \name Cache
@{
*/


/*! \brief Disable Instruction cache 



    @\n


    This function is called when Secure boot pass control to BL31. 

    It purpose is to assure a well defined state. 


*/
void __cache_inv_icache(void); 

/*! \brief Flush entire data cache 



    @\n



    This function write content of caches to associated 

    memory attached to cache. 


*/
void __cache_flush_dcache(void); 

/*! \brief Flush region of  data cache



    @\n



    This function write  a subset of  cache content  to associated 

    memory attached to cache. 

    \param[in] start of region to flush 
    \param[in] size  of region to flush 

*/
void __cache_flush_dcache_range(uint32_t start, uint32_t size); 

/*!
@}
*/

#endif

