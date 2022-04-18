
#include <plf/platform.h>
#include <plf/io.h>


#include "xlat_static.h"
#include "h_mmu.h"


const char bios_memory_mmu_header_version[]  __attribute__ ((section (".keywords")))     = _MMU_H;
const char bios_memory_mmu_xlat_version[]  __attribute__ ((section (".keywords")))     = _MMU_STATIC_H;
const char bios_memory_mmu_code_version[]    __attribute__ ((section (".keywords")))     = \
       "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_memory/branches/haspoc/hikey/bios_memory/mmu/h_mmu.c $ $Revision: 26902 $";

#define DBG_LEVEL  1
#define DBG_LEVEL_VERBOSE  3

void bios_memory_mmu_setup(void)
{								

	uint32_t sctlr, tcr_el3;
	uint64_t mair;

	trace_dl(DBG_LEVEL,"SUITE_BUILD_ID %s\n", SUITE_BUILD_ID);

	/* Set attributes in the right indices of the MAIR */				
	mair = ((MAIR_DEVICE << 8) | (MAIR_MEM));
	// trace_d(DBG_LEVEL,"MAIR = %x\n",(unsigned int) mair);
	write_mair_el3(mair);

	/* Invalidate TLBs at the current exception level */	
	tlbialle3();						
									
	/* Translation Control Register EL3 */					
	tcr_el3 = 0x0;
	tcr_el3 = tcr_el3  | TCR_EL3_RESERVED; 
	tcr_el3 = SET_VALUE(tcr_el3, TCR_ADDRESS_SIZE_4GB, TCR_ADDRESS_SIZE_MASK); 
	tcr_el3 = SET_VALUE(tcr_el3, TCR_GRANULE_SIZE_4KB, TCR_GRANULE_SIZE_MASK);
	tcr_el3 = SET_VALUE(tcr_el3, REGION_SIZE_OFFSET, REGION_SIZE_MASK);  /* Region size  2**(64 --  REGION_SIZE_OFFSET */
	tcr_el3 = SET_VALUE(tcr_el3, TCR_SH_INNER_SHAREABLE, TCR_SH_MASK); 
	tcr_el3 = SET_VALUE(tcr_el3, TCR_RGN_OUTER_WBA, TCR_RGN_OUTER_MASK); 
	tcr_el3 = SET_VALUE(tcr_el3, TCR_RGN_INNER_WBA,TCR_RGN_INNER_MASK);
	tcr_el3 = SET_VALUE(tcr_el3, TCR_TBI_USE, TCR_TBI_MASK);

	trace_d(DBG_LEVEL_VERBOSE,"TCR_EL3 = %x\n",tcr_el3);
	write_tcr_el3(tcr_el3);

        trace_d(DBG_LEVEL_VERBOSE,"Start address of l1_xlation_table=%x\n",l1_xlation_table);
        trace_d(DBG_LEVEL_VERBOSE,"Start address of l2_xlation_table=%x\n",l2_xlation_table);   



        trace_d(DBG_LEVEL_VERBOSE,"Sample values before assignment: l1_xlat[1]: %8X l2_xlat[460]: %8X, l2_xlat[511]: %8X\n",l1_xlation_table[1],l2_xlation_table[460],l2_xlation_table[511]);


	l1_xlation_table[0] = 0x000000725; // DRAM 0-1GB
	l1_xlation_table[1] = 0x040000725; // DRAM 1-2GB
	l1_xlation_table[2] = 0x080000720; // Not used

        trace_d(DBG_LEVEL_VERBOSE,"Sample values  after assignment: l1_xlat[1]: %8X l2_xlat[460]: %8X, l2_xlat[511]: %8X\n",l1_xlation_table[1],l2_xlation_table[460],l2_xlation_table[511]);

	// Assign pointer to l2-table
	l1_xlation_table[3] = (TABLE_ENTRY_POINTER | (uint64_t) l2_xlation_table);


	write_ttbr0_el3((uint64_t) l1_xlation_table);				

	/* Ensure all translation table writes have drained */	
	/* into memory, the TLB invalidation is complete, */	
	/* and translation register writes are committed */	
	/* before enabling the MMU */				

	dsb();							
	isb();								
	sctlr = read_sctlr_el3();
	sctlr = SET_VALUE(sctlr, SCTLR_WXN_BIT_DO_NOT_FORCE, SCTLR_WXN_BIT_DO_NOT_FORCE_MASK);  /* Allow execution and write on same address */
	sctlr = SET_VALUE(sctlr, SCTLR_C_BIT, SCTLR_C_BIT_MASK);                                /* Enable Data Cache */
	sctlr = SET_VALUE(sctlr, SCTLR_M_BIT, SCTLR_M_BIT_MASK);                                /* Enable MMU */


	trace_d(DBG_LEVEL,"Enabling MMU & D-Cache with sctlr= %x\n",sctlr);
	write_sctlr_el3(sctlr);				
	/* Ensure the MMU enable takes effect immediately */	
	isb();
}

