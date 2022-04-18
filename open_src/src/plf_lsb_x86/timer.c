/* 
  Copyright Oniteo 2010 
*/


#include <plf/platform.h>
#include <plf/timer.h>

const char plf_timer_header_version[] = __PMILL_TIMER_H__;
const char plf_timer_header_plf[] = __ONITEO_PLATFORM_H__;
const char plf_timer_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/timer.c $ $Revision: 16183 $";


void udelay(int i)
{
   usleep(i);
}

void mdelay(int i)
{
   usleep(i * 1000);
}

