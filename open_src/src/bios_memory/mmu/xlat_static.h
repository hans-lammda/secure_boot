#ifndef _MMU_STATIC_H
#define _MMU_STATIC_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_memory/branches/haspoc/hikey/bios_memory/mmu/xlat_static.h $ $Revision: 26902 $"

#define NUM_L1_ENTRIES 4
#define XLAT_TABLE_ENTRIES 512
#define XLAT_TABLE_SIZE 0x1000

#define RAM_ATTR	0x721
#define DEV_ATTR	0x725

/*************************************************************************


          xlat_tables_l1
          *********
TBBR0 ->  *       *
          *  [0]  *
          * DRAM  *
          *  [1]  *
          * DRAM  *
          *       *
          *       *
          *       *           l2_xlation_table
          *  [3]--*----------->*********** 
          *       *            * Devices *
          *********            *         *
                               *  XG2RAM *
                               *  SRAM   *
                               ***********


                executable memory(1GB):	0000_0000 - 4000_0000
		
                Devices	:               F400_0000 - F900_0000

                executable memory(4MB):	F980_0000 - F9C0_0000

                executable memory(72KB):FFF8_0000 - FFF9_2000


*************************************************************************/

/* This table covers DEVICE, XG2RAM and SoC-SRAM regions in physical address space */

static uint64_t l2_xlation_table[XLAT_TABLE_ENTRIES] __attribute__ ((section (".xlat_l2"))) = {


        /* DEVICE: 0xF400_0000 - 0xF802_0000 */ 
/*
 [63:52] Upper attributes = All 0's
 [51:48] Reserved
 [47:12] Output address
 [11:2]  Lower attributes = Inner sharable, NS-addr-map, Attrindx = 1	
 [1:0]   Type = 0b01( BLOCK ENTRY ) 			
*/

        [416] = 0x00000000F4000725,     [417] = 0x00000000F4200725,
        [418] = 0x00000000F4400725,     [419] = 0x00000000F4600725,
        [420] = 0x00000000F4800725,     [420] = 0x00000000F4a00725,
        [422] = 0x00000000F4c00725,     [423] = 0x00000000F4e00725,

        [424] = 0x00000000F5000725,     [425] = 0x00000000F5200725,
        [426] = 0x00000000F5400725,     [427] = 0x00000000F5600725,
        [428] = 0x00000000F5800725,     [429] = 0x00000000F5a00725,
        [430] = 0x00000000F5c00725,     [431] = 0x00000000F5e00725,

        [432] = 0x00000000F6000725,     [433] = 0x00000000F6200725,
        [434] = 0x00000000F6400725,     [435] = 0x00000000F6600725,
        [436] = 0x00000000F6800725,     [437] = 0x00000000F6a00725,
        [438] = 0x00000000F6c00725,     [439] = 0x00000000F6e00725,

        [440] = 0x00000000F7000725,     [441] = 0x00000000F7200725,
        [442] = 0x00000000F7400725,     [443] = 0x00000000F7600725,
        [444] = 0x00000000F7800725,     [445] = 0x00000000F7a00725,
        [446] = 0x00000000F7c00725,     [447] = 0x00000000F7e00725,

        [448] = 0x00000000F8000725,     [449] = 0x00000000F8200725,
        [450] = 0x00000000F8400725,     [451] = 0x00000000F8600725,
        [452] = 0x00000000F8800725,     [453] = 0x00000000F8a00725,
        [454] = 0x00000000F8c00725,     [455] = 0x00000000F8e00725,

        [456] = 0x00000000F9000725,     [457] = 0x00000000F9200725,
        [458] = 0x00000000F9400725,     [459] = 0x00000000F9600725,

        /* X2GRAM: 0xF980_0000 - 0xF9C0_0000 4MB */ 
/*
 [63:52] Upper attributes = All 0's
 [51:48] Reserved
 [47:12] Output address
 [11:2]  Lower attributes = Inner sharable, NS-addr-map, Attrindx = 0
 [1:0]   Type = 0b01( BLOCK ENTRY ) 			
*/
        [460] = 0x00000000F9800721,		[461] = 0x00000000F9a00721,

        /* SoC SRAM: 0xFFF8_0000 - 0xFFF9_2000 72KB */
	/* Here we do a 2MB section to avoid l3-table */
/*
 [63:52] Upper attributes = All 0's
 [51:48] Reserved
 [47:12] Output address
 [11:2]  Lower attributes = Inner sharable, NS-addr-map, Attrindx = 0
 [1:0]   Type = 0b01( BLOCK ENTRY ) 			
*/ 
        [511] = 0x00000000FFE00725

};



/*

        [128] = 0x0   0    0    0    0    0    0    0    9   0    0    0     0    7   2     1,
                0000 0000 0000 0000 0000 0000 0000 0000 1001 0000 0000 0000 0000 0111 0010 0001  
                63   59   55   51   47   43   39   35   31   27   23   19   15   11   7    3  0 
                ----------------------------------------||------------------------------------- VA[31:30] L1 Which L1 Entry 2**2 entries 
                ------------------------------------------|| |||| |||-------------------------- VA[29:21] Which entry in l2 2**9  entries  
                -----------------------------------------------------------------------------|| TABLE TYPE [ ENTRY, BLOCK , PAGE ] 
                -------------------------------------------------------------------------| ||-- Index into MAIR 
                ------------------------------------------------------------------------|- ---- Non Secure bit 0 = Secure , 1 = Normal  

                -----------------------------------------------------| |||| |||| |||| |||| |||| VA[20:0 ] Inside 2**20 block  



        0000.0000.0 | 0 0    0   0000 | output address 36 bits | 0 ,1 ,11 , 00 ,   0 ,000, 01
        0000.0000.0 | XN,PXN,CG| 0000 | output address 36 bits | NG,AF,SH[2],AP[2],NS,AI[3],01

*/ 
 


/* 
This table covers ( 4Gb ) each entry covers 1GB
 [63:48] Register-defined = 0				
 [47:12] Output address					
 [11:2]  Reserved = 0						
 [1:0]   Type = 0b00 and 0b01				

*/


static  uint64_t  l1_xlation_table[NUM_L1_ENTRIES]  __attribute__ ((section (".xlat_l1"))) = {
	[0] = 0x000000725, // DRAM
	[1] = 0x040000725, // Not used
	[2] = 0x080000720, // not 
	[3] = 0x000000000  // l2-pointer
};

#endif

