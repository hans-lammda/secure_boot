/*! \file system.h
    \brief System Functions

    Header file for an API that provides system functions 
    platform independent
                
*/

/*
 Copyright Oniteo 2007 - 
*/  
#ifndef __PMILL_WINDOWS_SYSTEM_HEADER__
#define __PMILL_WINDOWS_SYSTEM_HEADER__ "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/system.h $ $Revision: 8122 $"


#ifdef __cplusplus
extern "C" {
#endif



/*! \name OS 
 
    @{

*/

/*! \brief Reboot system  

    Immidiate reboot of target base operating system  
 */
void target_reboot(void);

// @}
#ifdef __cplusplus
}
#endif


#endif

