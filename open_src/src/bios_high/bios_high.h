/*! \file bios_high.h
    \brief High level boot logic 

 @\n

    This module retrieves the HSBF image from storage

    and process each object. 


    @\n

    The object is first verified



    @\n

    Upon failure the module returns a error code to C runtime. 


*/

/*
  Copyright (C) 2016 Oniteo
 */

#ifndef __ONITEO_BIOS_HIGH_H__
#define __ONITEO_BIOS_HIGH_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_high/branches/haspoc/hikey/bios_high/bios_high.h $ $Revision: 26907 $"


/*! \brief  Unable to allocate heap in SRAM  */
#define BIOS_HIGH_MALLOC_SRAM_FAILED 1

/*! \brief  Unable to verify ROOTPK against hash  */
#define BIOS_HIGH_FAILED_VERIFY_ROOTPK 2 

/*! \brief  ROOTPK object not found */
#define BIOS_HIGH_ROOTPK_NOT_FOUND  3

/*! \brief  Unable to verify  DRAM object  */
#define BIOS_HIGH_FAILED_VERIFY_DRAM  4

/*! \brief  DRAM object  not found */
#define BIOS_HIGH_DRAM_NOT_FOUND  5 

/*! \brief  Unable to allocate heap in DRAM  */
#define BIOS_HIGH_MALLOC_DRAM_FAILED 6

/*! \brief  Unable to verify ATF Boot  */
#define BIOS_HIGH_FAILED_VERIFY_ATF_BOOT  7

/*! \brief  Unable to verify  ramdisk  */
#define BIOS_HIGH_FAILED_VERIFY_RAMDISK 8 

/*! \brief  unknown object in HSBF image   */
#define BIOS_HIGH_FAILED_UNKNOWN_OBJECT  9

/*! \brief   Unable to verify  ATF Payload bl33.bin   */
#define BIOS_HIGH_FAILED_VERIFY_ATF_PAYLOAD  10

/*! \brief   Unable to verify  ATF Runtime bl31.bin   */
#define BIOS_HIGH_FAILED_VERIFY_ATF_RUNTIME  11

/*! \brief   Size of HSBF image could not be obtained   */
#define BIOS_HIGH_UNKNOWN_SIZE_HSBF 12 

/*! \brief   No object in HSBF that boot  */
#define  BIOS_HIGH_FAILED_NO_BOOT_OBJECT 13 

/*! \brief   Failed to install hash value of public root key   */
#define BIOS_HIGH_FAILED_INSTALL_ROOT_PK_HASH 14 

/*! \brief   Failed to retrieve hash value of public root key   */
#define BIOS_HIGH_FAILED_RETRIEVE_ROOT_PK_HASH 15 

/*! \brief   Arm Trusted Firmware final boot step failed */
#define BIOS_HIGH_FAILED_ATF_BOOT 16 

/*! \brief   Failed to retrieve image from external storage */
#define BIOS_HIGH_BAD_STORAGE_READ 17 

/*! \brief   Failed to configure MCU/SDRAM */
#define BIOS_HIGH_SDRAM_SETUP_ERROR 18 

/*! \brief   Failed to configure storage */
#define BIOS_HIGH_FAILED_INIT_STORAGE 19 

/*! \brief   Number of sectors to read in pass 1 */
#define INITIAL_READ 300

/*! \brief   First sector to read  */
#define BOOT_START_SECTOR 8192

/*! \name Boot
@{
*/

/*! \brief retrieve HSBF and boot

    This function retrieves each object 

    in the HSBF image and then boot

    Upon sucess it never returns

    \retval BIOS_HIGH_FAILED_RETRIEVE_ROOT_PK_HASH
    \retval BIOS_HIGH_FAILED_INSTALL_ROOT_PK_HASH
    \retval BIOS_HIGH_MALLOC_SRAM_FAILED
    \retval BIOS_HIGH_FAILED_VERIFY_ROOTPK
    \retval BIOS_HIGH_ROOTPK_NOT_FOUND
    \retval BIOS_HIGH_FAILED_VERIFY_DRAM
    \retval BIOS_HIGH_DRAM_NOT_FOUND
    \retval BIOS_HIGH_MALLOC_DRAM_FAILED
    \retval BIOS_HIGH_FAILED_VERIFY_ATF_BOOT
    \retval BIOS_HIGH_FAILED_VERIFY_RAMDISK
    \retval BIOS_HIGH_FAILED_VERIFY_ATF_PAYLOAD 
    \retval BIOS_HIGH_FAILED_VERIFY_ATF_RUNTIME
    \retval BIOS_HIGH_FAILED_UNKNOWN_OBJECT
    \retval BIOS_HIGH_UNKNOWN_SIZE_HSBF 
    \retval BIOS_HIGH_FAILED_NO_BOOT_OBJECT 
    \retval BIOS_HIGH_FAILED_ATF_BOOT 
    \retval BIOS_HIGH_BAD_STORAGE_READ
    \retval BIOS_HIGH_SDRAM_SETUP_ERROR 
    \retval BIOS_HIGH_FAILED_INIT_STORAGE 

    \param[in] boot_start_sector first sector of HSBF image 
    \param[in] initial_read limited read in pass 1 

*/
int _main(int boot_start_sector, int initial_read); 




/*!
@}
*/


/*! \name C-Runtime
@{
*/

/*! \brief invoke boot logic 


    This function assigns a small heap 

    residing in static ram and then invoke main

    @\n

*/


void bios_high_runtime(void); 

/*!
@}
*/



#endif

