/*! \file bios_low.h
    \brief low level functions

    @\n

    This module contais code from ARM Trusted Firmware

    URL: github.com/96boards

    Revision: 1.1 tag: ca9f7ed 


    @\n

    TSFI_POWERON_COLD 

    TSFI_STORAGE



    @\n

    The structure of the code conforms to Arm Trusted Firmware 

    Static linker script 

    Static configuration of MMU 


 */

/*
  Copyright (C) 2016 Oniteo
 */

#ifndef __ONITEO_BIOS_LOW_H__
#define __ONITEO_BIOS_LOW_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_low/branches/haspoc/arm64_v8_sb/bios_low/bios_low.h $ $Revision: 27092 $"


/*! \brief  Successfully read from SD card  */
#define SECTOR_READ_OK 0 

/*! \brief  Successfully setup of mmc driver  */
#define MMC_INIT_OK 0 

/*! \brief  Error during setup of mmc driver  */
#define MMC_INIT_ERROR 1 

/*! \brief  Error when  reading from SD card  */
#define SECTOR_READ_ERROR  -1

/*! \brief  Successfully mcu image load   */
#define MCU_LOAD_OK 0 

/*! \brief  mcu image load error  */
#define MCU_LOAD_ERROR -1 

/*! \brief  Block size on SD media  */
#define BLOCK_SIZE 512

/*! \name MMU  
@{
*/

/*! \brief disable instruction cache

    Disable MMU and instruction cache 

  
*/
void disable_mmu_icache_el3(void); 
/*!
@}
*/

/*! \name Storage
@{
*/

/*! \brief init mmc 

    This functions configures mmc drivers 

    needed for access of SD drivers

    \retval MMC_INIT_OK
    \retval MMC_INIT_ERROR
*/

int init_mmc(void); 

/*! \brief high level read

    This functions retrieves 
    sectors from a SD card. 

    Actual constraint defined by avaliable memory for target

    \param[in] sector to start read from 
    \param[in] nr number of sectors to read  
    \param[in] buffer to write retrieved data 

    \retval SECTOR_READ_OK
    \retval SECTOR_READ_ERROR

*/

int sector_read(unsigned int sector, unsigned int nr, unsigned char *buffer);
/*!
@}
*/

/*! \name IGNORE 
@{
*/

/*! \brief low level read 

    This functions only reads a limited 
    number of sectors. 


    \param[in] sector to start read from 
    \param[in] nr number of sectors to read  
    \param[in] buffer to write retrieved data 

    \retval SECTOR_READ_OK
    \retval SECTOR_READ_ERROR
*/
int mmc_sector_read(unsigned int sector, unsigned int nr, unsigned char *buffer);


/*!
@}
*/

/*! \name MCU   
@{
*/
/*! \brief enable mcu memory 

    The actual purpose of this function is poorly 
    
    documented by Hikey. 

    @\n

    The implementation writes 

    values to 32 bit registers that power on 

    memory needed to manage mcu. 
*/
void hisi_mcu_enable_sram(void);

/*! \brief load mcu image 

    The bl30.image contains proprietary code that is 

    loaded into a dedicated region of memory
 
    @\n

    The actual copying is divided into sections. 

    prior to copying a section its headers is verified 

    if not valid it will return an error code 

    \retval MCU_LOAD_OK 
    \retval MCU_LOAD_ERROR
    
*/
int hisi_mcu_load_image(uint64_t image_base, uint32_t image_size);

/*! \brief start mcu 

    The actual purpose of this function is poorly 
    
    documented by Hikey. 

    @\n

    Some registers that controls ddr remap configuration
*/
void hisi_mcu_start_run(void);

/*!
@}
*/

/*! \name IGNORE 
/*! 
@{
*/
/*! \brief Init acpu  

    The actual purpose of this function is poorly 
    
    documented by Hikey. 

    Configuration of clocks by writing to registers and then call to   
    @\n

    acpu_dvfs_volt_init();

    @\n

    acpu_dvfs_set_freq();

    @\n


*/
void init_acpu_dvfs(void);
/*!
@}
*/


#endif

