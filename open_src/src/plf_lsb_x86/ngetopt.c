#include <plf/platform.h>
#include <plf/ngetopt.h>


char plf_ngetopt_header_version[] = __PMILL_GETOPT_H__;
const char plf_ngetopt_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_ngetopt_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/ngetopt.c $ $Revision: 16123 $";



char* NGetOptarg()
{
   return optarg;
}

int NGetOptind()
{
   return optind;
}

int NGetOpt (int ___argc, char ** ___argv,
                 const char *__shortopts,
                 const struct option *__longopts, int *__longind)
{
   return getopt_long (___argc, ___argv, __shortopts, __longopts, __longind);
}

