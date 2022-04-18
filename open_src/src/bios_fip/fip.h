/*! \file fip.h
    \brief Arm Trusted Firmware Interface

    @\n 
   
    This module enables compability with Arm Trusted Firmware. 

    The firmware image package file ( fip.bin ) is loaded into memory 

    and then parsed to find the following items. 

    @\n 
   
    bl31.bin: Runtime support including Power State Control Interface 

    bl33.bin: Bootloader , hypervisor , etc 


    @\n 

    Secure Boot contain functionally covering both bl1.bin and bl2.bin. 
    
    Load address for bl31.bin and bl33.bin must be specified in the 

    HSBF FIP header as well as Exception level for bl33.bin 

     

*/

#ifndef FIP_H
#define FIP_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_fip/trunk/bios_fip/fip.h $ $Revision: 26375 $"

#include <spd/spd.h>



/*! \brief  registers  */
typedef struct aapcs64_params {
        uint64_t arg0; //!< X0 
        uint64_t arg1; //!< X1
        uint64_t arg2; //!< X2
        uint64_t arg3; //!< X3
        uint64_t arg4; //!< X4
        uint64_t arg5; //!< X5
        uint64_t arg6; //!< X6
        uint64_t arg7; //!< X7
} aapcs64_params_t;

/*! \brief  Version header  */
typedef struct param_header {
        uint8_t type;    //!<  type of the structure 
        uint8_t version; //!<  version of this structure 
        uint16_t size;   //!<  size of this structure in bytes 
        uint32_t attr;   //!<  attributes: unused bits SBZ
} param_header_t;

/*! \brief  Processor state */
typedef struct entry_point_info {
        param_header_t h; //!<  version 
        uint64_t pc;      //!< start address for link register 
        uint32_t spsr;    //!< Processor state 
        aapcs64_params_t args; //!< X0 - X7 parameters 
} entry_point_info_t;

/*! \brief FIP Image info   */
typedef struct image_info {
        param_header_t h;     //!<  version
        uint64_t image_base;  //!< location of image  
        uint32_t image_size;  //!< bytes read from image file 
} image_info_t;

/*! \brief BL31 Call Interface    */
typedef struct bl31_params {
        param_header_t h;  //!<  version

        image_info_t *bl31_image_info;     //!< Image info pointer to  bl31
        entry_point_info_t *bl32_ep_info;  //!< Execution info pointer to  bl31

        image_info_t *bl32_image_info;     //!< Image info pointer to bl32
        entry_point_info_t *bl33_ep_info;  //!< Execution info pointer to bl33

        image_info_t *bl33_image_info;      //!< Image info pointer to bl33
} bl31_params_t;

/*! \brief Bl1 and BL31 shared data    */
typedef struct bl2_to_bl31_params_mem {
        bl31_params_t bl31_params;          //!< BL31 Call Interface

        image_info_t bl31_image_info;       //!< Image info about Runtime
        image_info_t bl32_image_info;       //!< Image info about Trusted OS 
        image_info_t bl33_image_info;       //!< Image info about Bootloader

        entry_point_info_t bl33_ep_info;    //!< Execution info about Bootloader
        entry_point_info_t bl32_ep_info;    //!< Execution info about Trusted OS
        entry_point_info_t bl31_ep_info;    //!< Execution info about Runtime

} bl2_to_bl31_params_mem_t;




#define RUN_IMAGE       0xC0000000

/*! \brief  Verify platform parameters from BL2 to BL3-1 */
#define BL31_PLAT_PARAM_VAL 0x0f1e2d3c4b5a6978ULL

/*! \brief  Verison of parameter block */
#define VERSION_1               0x01

/*! \brief  Version of Entry point structure  */
#define PARAM_EP     0x01
/*! \brief  Version of image structure  */
#define PARAM_IMAGE_BINARY  0x02
/*! \brief  Version of bl31 module  */
#define PARAM_BL31       0x03

/*! \brief  Secure state and EL3 for bl31   */
#define  BL31_SPSR_STATE 0x3cd


/*! \brief Key to find bl2.bin in FIP   */
#define FIRMWARE_BL2  0
/*! \brief Key to find bl30.bin in FIP   */
#define FIRMWARE_BL30 1
/*! \brief Key to find bl31.bin in FIP   */
#define FIRMWARE_BL31 2
/*! \brief Key to find bl32.bin in FIP   */
#define FIRMWARE_BL32 3
/*! \brief Key to find bl33.bin in FIP   */
#define FIRMWARE_BL33 4


/*! \brief Failed to call __boot_bl31
 */
#define FIP_ERROR_BOOT_FAILED  -1

/*! \brief Load address of bl31 out of range 
 */
#define FIP_ERROR_BL31_INVALID_LOAD_ADDRESS -2 

/*! \brief bl31 not present in fip image  
 */
#define FIP_ERROR_BL31_NOT_FOUND -3 

/*! \brief Load address of bl33 out of range 
 */
#define FIP_ERROR_BL33_INVALID_LOAD_ADDRESS -4

/*! \brief bl33 not present in fip image  
 */
#define FIP_ERROR_BL33_NOT_FOUND -5 

/*! \brief Problem 
 */
#define FIP_ERROR  0

/*! \brief Success 
 */
#define FIP_OK      1 


/*! \name Setup
@{
*/

/*! \brief post configuration 


    This function allows migration of overlapping 

    setup from bl31 to Secure Boot  


     
    @\n

    bl31_early_platform_setup()

    bl31_plat_arch_setup()

    bl31_arch_setup()

    bl31_platform_setup()

    \retval FIP_OK  
    \retval FIP_ERROR 

*/
int bl31_setup(void); 

/*!
@}
*/

/*! \name Control
@{
*/


/*! \brief Prepare parameters  

    This function prepares the parameter block needed 

    for boot compliance with ATF 1.1 

     
    @\n


    The Header contains load addresses for each object, 

    for bl33 the exception level is specified in the header 

     
    @\n

    The location of the parameter block is platform 

    dependent and defined by the linker script __PARAMS_BASE__.

    

    \param[in] e_level  exception level for bl33
    \param[in] bl31_load_address where to load bl31
    \param[in] bl31_size size of atf runtime
    \param[in] bl33_load_address where to load bl33
    \param[in] bl31_size size of atf payload
    \param[in] bl33_X0 first argument to bl33
    \param[in] bl33_X1 second argument to bl33


    \retval FIP_ERROR_BOOT_FAILED
    \retval FIP_ERROR_BL31_INVALID_LOAD_ADDRESS 
    \retval FIP_ERROR_BL33_INVALID_LOAD_ADDRESS 
    \retval FIP_ERROR_BL31_NOT_FOUND 
    \retval FIP_ERROR_BL33_NOT_FOUND 
    \return Never upon success  
*/

int bl2main(uint32_t e_level, uint64_t *bl31_load_address, int bl31_size,  uint64_t *bl33_load_address,  int bl33_size, uint64_t  bl33_X0, uint64_t  bl33_X1); 

/*! \brief start bl31 

    This assembler function pass control

    to ATF bl31 runtime service version 1

    @\n

    X0 contains the address to bl31 parameter block 

    X1 contain BL31_PLAT_PARAM_VAL


    @\n

    call mechanism is eret  where 

    elr_el3 contains bl31_ep_info->pc 

    spsr_el3 contains bl31_ep_info->spsr 
    
    @\n

    Instruction cache is disable prior to eret call 

    \param[in] x0 point to  ATF1.1 structure
    \param[in] bl31_entry start address of bl31 
*/
extern void __boot_bl31(uint64_t *x0, uint64_t *bl31_entry); 

/*!
@}
*/


#endif /* FIP_H */


