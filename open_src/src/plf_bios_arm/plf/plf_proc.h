/*! \file plf_proc.h
    \brief Process and Thread 

     Header file for an API that provides process and thread
     routines in a platform-independent manner.
    
 */
/* 
    Copyright (C) 2008  Oniteo AB
*/ 
#ifndef __PMILL_WINDOWS_PROC_HEADER__
#define __PMILL_WINDOWS_PROC_HEADER__ "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/plf_proc.h $ $Revision: 13531 $"


#ifdef __cplusplus
extern "C" {
#endif
/*! \name Process  
 
    @{

*/

/*! \brief Max number of tasks per process  */
#define PLF_MAX_THREADS_PER_PROCESS           300 
/*! \brief OK */
#define PLF_PROC_OK                           0 
/*! \brief Failed to create task  */
#define PLF_PROC_ERROR_MEM                    12 

/*! \brief Checks if a process is alive.
    \param[in] pid Process id.
    \return 1 if active, 0 if not active.

     This function reports if a process still is active. 
 */    
int CheckProcess(int pid);

// @}
/*! \name Threads  
 
    @{

*/

/*! \brief Spawns a new thread.
    \param [out] thread Handle to new thread.
    \param [in] attr Parameters to contol thread.
    \param [in] start_routine Function to execute in new thread.
    \param [in] arg Pointer to single value or struct containing arguments.
    \return 0 upon success.

    This function creates a new thread.  
    The parameters to contol a thread depends on implemenentation.
 */    
int NThreadCreate(void  *thread, const void *attr,
void* (*start_routine)(), void* arg );

/*! \brief Detaches a thread.
    \param [in]  thread Handle to thread to detach.
    \return 0 upon success 

    This function makes the thread independent of the main thread.
 */    
int NThreadDetach(void  *thread);

/*! \brief Terminates a thread.
    \param[in] val_ptr Pointer to exit return status.

    This function terminates a thread.
 */    
void NThreadExit(void  *val_ptr);

// @}

/*! \brief Handle to thread */
typedef unsigned long int  NThreadType ; 

#ifdef __cplusplus
}
#endif

#endif
