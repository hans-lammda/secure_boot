#include <plf/platform.h>
#include <plf/safestr.h>
#include <plf/dynload.h>

#define USE_DL 1 

char plf_dynload_header_version[] = __PMILL_DYNLOAD_H__;
const char plf_dynload_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_dynload_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/dynload.c $ $Revision: 16123 $";


HMODULE NDldLoadLibrary(const char* pszLibraryPath)
{
#ifdef USE_DL
   return dlopen(pszLibraryPath, RTLD_LAZY);
#else
   return 0 ;
#endif
}

int NDldUnloadLibrary(HMODULE hLib)
{
   if (!hLib)
      return -1;
   return dlclose(hLib);
}

FARPROC NDldLoadSymbol(HMODULE hLib, const char* pszName)
{
   return dlsym(hLib,pszName);
}

FARPROC NDldLoadFunc(HMODULE hLib, const char* pszName)
{
   return dlsym(hLib,pszName);
}

char* NDldGetError(char* pszDLError, int nMax)
{
   char* pszMsg = NULL;

   pszMsg = dlerror();

   memset(pszDLError,0,nMax);
   if (pszMsg != NULL)
      NStrSCopy(pszDLError,pszMsg,nMax);
   else
      NStrSCopy(pszDLError,"Unknown error.",nMax);

   return pszDLError;
}

char* NDldGetExtension(void)
{
  return PLF_SO_EXTENSION;
}

