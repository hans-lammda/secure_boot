#include <plf/platform.h>
#include <plf/file.h>
#include <stdio.h>
int main() 
{
  char *source = "/etc/passwd";
  char *target = "/tmp/passwd";
  /* Copy a file to tmp */
  if (plf_cp(source,target) != PLF_OK)
    return 1;
  /* Try to copy a non existing file */
  if (plf_cp(source,"/urk") == PLF_OK)
    return 1;
  printf("File copying OK\n");
  return 0;
}
