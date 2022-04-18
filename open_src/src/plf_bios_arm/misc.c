#include <plf/platform.h>
#include "misc.h"

const char __attribute__ ((section (".ident")))  plf_misc_header_plf[] = PLF_BIOS_MISC_H;
const char __attribute__ ((section (".ident")))  plf_misc_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/misc.c $ $Revision: 20822 $";



size_t strlen(const char *src)
{
    size_t i = 0;

    while (*src++)
        i++;
    return i;
}

int strcmp(const char * cs,const char * ct)
{
        register signed char __res;

        while (1) {
                if ((__res = *cs - *ct++) != 0 || !*cs++)
                        break;
        }

        return __res;
}

int strncmp(const char * cs,const char * ct,size_t count)
{
        register signed char __res = 0;
 
        while (count) {
                if ((__res = *cs - *ct++) != 0 || !*cs++)
                        break;
                count--;
        }

        return __res;
}



/* 
 * This function is part of the runtime 
 * division by zero etc 
 */ 
int raise (int signum)
{
        trace_dl(1,"raise: Signal # %d caught\n", signum);
        return 0;
}


unsigned long log2( unsigned long x )
{
  unsigned long ans = 0 ;
  while( x>>=1 ) ans++;
  return ans ;
}

