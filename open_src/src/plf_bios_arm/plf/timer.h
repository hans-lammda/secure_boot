/*! \file timer.h
    \brief Timer

   Userland implementation of high resolution timers 

*/

#ifndef __PMILL_TIMER_H__
#define __PMILL_TIMER_H__ "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/timer.h $ $Revision: 16182 $"



#ifdef __cplusplus
extern "C" {
#endif

/*! \name Timer

    @{

*/


/*! \brief microsecond sleep 
    \param[in] useconds

    This function sleeps specified number of microseconds
 */
void udelay(int useconds);

/*! \brief millisecond sleep 
    \param[in] mseconds

    This function sleeps specified number of milliseconds
 */
void mdelay(int mseconds);

// @}

#ifdef __cplusplus
}
#endif

#endif

