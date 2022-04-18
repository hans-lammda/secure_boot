#include <plf/platform.h>
#include <plf/safestr.h>
#include <plf/memory.h>
#include <plf/ngetopt.h>
#define BUF_SIZE 512

int main(int argc, char** argv)
{
  char hello_buf_short[5];
  char *hello_buf;
  char hello_world[] = "Hello, world\n";
  int ret ;
  int index = 0;
  int help = 0;
  int size = BUF_SIZE;
  int c;
  /* Structure holding valid options */
  static struct option options[] =
    {
      {"help", no_argument,       0, 'h'},
      {"size", optional_argument, 0, 's'},
      {0, 0, 0, 0}
    };

  while (1) {
    c = NGetOpt (argc, argv, "hs::", options, &index);
    if (c == -1)
      break;
    switch(c) {
    case 'h':
      help = 1;
      break;
    case 's':
      size = atoi(NGetOptarg());
      break;
    }
  }

  /*
  if (size < strlen(hello_world) + 1) {
    printf("Size of buffer too small.\n");
    return 1;
  }
  */

  hello_buf = NMemAlloc(size);

  ret = NStrSCopy(hello_buf_short, hello_world, sizeof hello_buf_short);
  if (ret != SAFESTR_ERROR_INSUFFICIENT_BUFFER) {
    printf("NStrSCopy does not catch too short buffer\n");
    return 1;
  }

  ret = NStrSCopy(hello_buf, hello_world, size);
  if (ret != SAFESTR_OK) {
    printf("NStrSCopy reports too short buffer (%d)\n", size);
    return 1;
  }

  printf("%s", hello_buf);
  NMemFree(hello_buf);
  return 0;
}
