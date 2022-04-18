/* 
  Copyright Oniteo 2007 - 2008 
*/

/*! \file system.c
    \brief Implements platform independent system functions.
 */

#include <plf/platform.h>
#include <plf/system.h>
#include <plf/safestr.h>
#include <linux/reboot.h>

char plf_system_header_version[] = __PMILL_WINDOWS_SYSTEM_HEADER__;
const char plf_system_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_system_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/system.c $ $Revision: 24361 $";


extern void reboot(int);

void target_reboot(void)
{
  sync();
  reboot(LINUX_REBOOT_CMD_RESTART);
}

int target_identity(char *id_string, int length)
{

    /* 
       Operational ID is represented by the Common Name in 
       a X509 certificate. During deployment the attribute is 
       stored in a plain text file 

       PREFIX/sysconfig.db/security/cert/tk/host/target.cn
    */ 

    if ( NStrSCopy(id_string,"dummy",length) == SAFESTR_OK  ) 
      return SYSTEM_OK; 
    else 
      return MEMORY_FAIL; 

}

