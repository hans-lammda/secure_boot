#include <plf/platform.h>
#include <plf/safestr.h>


const char bios_output_print_code_version[]    __attribute__ ((section (".keywords")))     = \
       "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_output/branches/haspoc/arm64_v8/bios_output/printf.c $ $Revision: 25866 $";


extern int console_putc(int c);

#define PRINTSTR_LEN 512

int printf(const char* format, ...) {

  va_list v;
  int retval = 0, rv = 0;
  char target[PRINTSTR_LEN];
  char* c = (char*)&target;
  int i = 0;

  va_start(v, format);
  rv = NStrVSPrintf(target, PRINTSTR_LEN, format, v);
  va_end(v);

  if (rv != SAFESTR_OK) {
    retval = 0;
    return retval;
  }

  for (i = 0; (*c != '\0') && (i < PRINTSTR_LEN); c++){
    if (*c == '\n') {
      console_putc('\r');
    }
    console_putc(*c);
  }

  retval = 1;
  return retval;
}







