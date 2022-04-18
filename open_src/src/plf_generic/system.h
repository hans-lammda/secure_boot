/*! \file system.h
    \brief System Functions

    Header file for an API that provides system functions 
    platform independent
                
*/

/*
 Copyright Oniteo 2007 - 
*/  
#ifndef __PMILL_WINDOWS_SYSTEM_HEADER__
#define __PMILL_WINDOWS_SYSTEM_HEADER__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/system.h $ $Revision: 24679 $"


#ifdef __cplusplus
extern "C" {
#endif


/*! \brief Successfull operation

 */
#define SYSTEM_OK          1 

/*! \brief Problem

 */
#define SYSTEM_FAIL        -1  

/*! \brief Memory problem 

 */
#define MEMORY_FAIL        -2  



/*! \name OS 
 
    @{

*/

/*! \brief Reboot system  

    Immidiate reboot of target base operating system  
 */
void target_reboot(void);

/*! \brief Retrieve system id 
    \param[in/out] address of identity
    \param[in/out] length of identity 
    \retval SYSTEM_OK
    \retval SYSTEM_FAIL
    \retval MEMORY_FAIL


    Returns the operational identity of target. 
    The caller is responsible for providing memory.
    The returned char pointer is a NULL terminated string.
    If memory assigned for the result is to short,
    the function will return MEMORY_FAIL
    
 */
int target_identity(char *id_string, int length);

// @}
#ifdef __cplusplus
}
#endif


#endif

