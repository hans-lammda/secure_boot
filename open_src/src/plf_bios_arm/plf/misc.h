#ifndef PLF_BIOS_MISC_H
#define PLF_BIOS_MISC_H "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_bios_x86/trunk/plf_bios_x86/misc.h $ $Revision: 16580 $"


#define isdigit(c)	((c) >= '0' && (c) <= '9')
#define isxdigit(c)	(((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#define islower(c)	((c) >= 'a' && (c) <= 'z')

unsigned long log2 (unsigned long word);

size_t strlen(const char *src);

int strcmp(const char *s1, const char *s2);

char *strchr(const char *s, int c);

int isspace(int c);

int tolower(int c);

unsigned char toupper(unsigned char c);

int substring (const char *s1, const char *s2);

#endif
