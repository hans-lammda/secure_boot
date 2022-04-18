#include <plf/platform.h>
#include <plf/ngetopt.h>
#include <strings.h>
int main() 
{
  int index = 0;
  int help = 0;
  int x = 0;
  char *xarg = NULL;
  char *value = NULL;
  int c;
  char *date = NULL;
  char *file = NULL;
  /* Structure holding valid options */
  static struct option options[] =
    {
      {"help",       no_argument,         0, 'h'},
      {"value",      optional_argument,   0, 'v'},
      {"x",          optional_argument,   0, 'x'},
      {"date",       required_argument,   0, 'd'},
      {0, 0, 0, 0}
    };
  /* This is test data for the NGetOpt call */
  char *argv[] = {"command","--help","-d2008","--value=4711","-x","file.txt",NULL};
  int argc = 6;
  /* Go through the test argv above */
  while (1) {
    c = NGetOpt (argc, argv, "hd:v::x::",
		 options, &index);
    if (c == -1)
      break;
    switch(c) {
    case 'h':
      help = 1;
      break;
    case 'v':
      value = NGetOptarg();
      break;
    case 'd':
      if (NGetOptarg())
	date = NGetOptarg();
      break;
    case 'x':
      if (NGetOptarg())
	xarg = NGetOptarg();
      else
	x = 1;
    }
  }
  if ( !(help == 1 &&
	 strcmp(date,"2008") == 0 &&
	 strcmp(value,"4711") == 0 &&
	 x == 1 && xarg == NULL))
    return 1;
  /* Get argument */
  file = argv[NGetOptind()];
  if (strcmp(file,"file.txt") != 0)
    return 1;
  printf("NGetopt handling OK\n");
  return 0;
}
