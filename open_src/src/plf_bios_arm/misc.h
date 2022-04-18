#ifndef PLF_BIOS_MISC_H
#define PLF_BIOS_MISC_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/misc.h $ $Revision: 20589 $"


size_t strlen(const char *src);
int strcmp(const char * cs,const char * ct);
int strncmp(const char * cs,const char * ct,size_t count);
unsigned long log2 (unsigned long word);

int raise (int signum);



#endif
