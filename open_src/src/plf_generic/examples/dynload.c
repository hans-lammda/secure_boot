#include <plf/platform.h>
#include <plf/dynload.h>
#include <plf/safestr.h>

#define BUFLEN 512

int main()
{
  char ldpath[BUFLEN];
  HMODULE h;
  char *ext = NULL;
  void *p;
  /* Get dynamic library extension */
  ext = NDldGetExtension();
  if (ext == NULL)
    return 1;
  /* Build up path to a dynamic library */
  NStrSCopy(ldpath,PREFIX,BUFLEN);
  NStrSCat(ldpath,"/lib/libns_plf",BUFLEN);
  NStrSCat(ldpath,ext,BUFLEN);
  /* Try to open up the library */
  h = NDldLoadLibrary(ldpath);
  if (h == NULL)
    return 1;
  /* Try to load a reference to a function name */
  if (NDldLoadFunc(h,"NDldLoadFunc") == NULL)
    return 1;
  /* Try to load a reference to a non existing function name */
  if (NDldLoadFunc(h,"urk") != NULL)
    return 1;
  /* Try to load a reference to a symbol we know should be there */
  if ((p = NDldLoadSymbol(h,"plf_dynload_header_version")) == NULL)
    return 1;
  /* Check if the variable starts with what we should expect */
  if (strstr(p,"$HeadURL") == NULL)
    return 1;
  /* This symbol should not be here */
  if ((p = NDldLoadSymbol(h,"urk")) != NULL)
    return 1;
  /* Unload the library */
  if (NDldUnloadLibrary(ldpath) != 0)
    return 1;
  printf("Dynamic library OK\n");
  return 0;
}
