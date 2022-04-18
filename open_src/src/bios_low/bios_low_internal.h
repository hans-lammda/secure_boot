/*! \file bios_low_internal.h
    \brief ATF functions

    @\n

    This module contais code from ARM Trusted Firmware

    URL: github.com/96boards

    Revision: 1.1 tag: ca9f7ed 



    @\n

    This module contains the framework of ATF 


 */

/*
  Copyright (C) 2016 Oniteo
 */

#ifndef __ONITEO_BIOS_LOW_INTERNAL_H__
#define __ONITEO_BIOS_LOW_INTERNAL_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_low/branches/haspoc/arm64_v8_sb/bios_low/bios_low.h $ $Revision: 26627 $"



/*! \name Boot Mode 
@{
*/
/*! \brief Warm or cold boot

    Determination of boot path

    @\n


    This functions checks the stored program counter 

    for the actual core, zero means cold boot, 

    otherwise warmboot. 


    \param[in] X0  

    \return X0  

*/
int platform_get_entrypoint(uint64_t x0);


/*! \brief Identify core

    Determination of primary core

    @\n

    This function indicates if current core is the 

    primary 


    \param[in] X0  content of mpidr_el1

    \return X0  

*/ 
int platform_is_primary_cpu(uint64_t x0); 

/*!
@}
*/

/*! \name Platform Setup 
@{
*/

/*! \brief secondary core setup

    Placing the executing secondary CPU in a platform-specific state
    until the primary CPU performs the necessary actions
    to bring it out of that state and
    allow entry into the OS. 

    @\n

    This function is called with the MMU and data caches disabled 
*/ 

void plat_secondary_cold_boot_setup(void); 

/*! \brief bl2 early setup

    This function assigns board id and type to 
    sram.  It also calls function 

    init_acpu_dvfs

    @\n

    NOTE: This function is migrated from bl2 which 

    violates the firmware design according to ARM

*/ 

void bl2_early_platform_setup(void); 

/*!
@}
*/

#endif

