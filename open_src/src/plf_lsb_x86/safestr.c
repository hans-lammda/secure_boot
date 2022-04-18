#include <plf/platform.h>
#include <plf/safestr.h>

char plf_safestr_header_version[] = __PMILL_SAFESTR_H__;
const char plf_safestr_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_safestr_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/safestr.c $ $Revision: 16124 $";



int NStrSPrintf(char* target, int targetSize, const char* format, ...)
{
   va_list v;
   int nWritten;

   if (targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   va_start(v,format);
   nWritten = vsnprintf(target,targetSize,format,v);
   va_end(v);

   if (nWritten<targetSize)
      return SAFESTR_OK;
   else if (nWritten>=targetSize-1)
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;
   else
      return SAFESTR_ERROR_UNKNOWN;
}


int NStrSCopy(char* target, const char* source, int targetSize)
{
   char* pszPtrT;
   const char* pszPtrS;
   int nCopied;

   if (!target || targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   if (!source)
   {
      target[0]=0;  
      return SAFESTR_OK;
   }

   nCopied = 0;
   pszPtrS = source;
   pszPtrT = target;

   while (*pszPtrS && nCopied<targetSize-1)
   {
      *pszPtrT = *pszPtrS;
      pszPtrT++;
      pszPtrS++;
      nCopied++;
   }

   if ( *pszPtrS == '\0' )
      *pszPtrT = '\0';      
   else
   {
      *pszPtrT = '\0';      
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;
   }      

   return SAFESTR_OK;
}

int NStrSCat(char* target, const char* source, int targetSize)
{
   char* pszPtrT;
   const char* pszPtrS;
   int nFilled;

   if (!target || targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   if (!source)
   {
      target[0]=0;  
      return SAFESTR_OK;
   }

   nFilled = 0;
   pszPtrT = target;
   while (*pszPtrT && nFilled<targetSize-1)
   {
      pszPtrT++;
      nFilled++;
   }

   pszPtrS = source;
   while (*pszPtrS && nFilled<targetSize-1)
   {
      *pszPtrT = *pszPtrS;
      pszPtrT++;
      pszPtrS++;
      nFilled++;
   }

   if (nFilled<targetSize) {
      *pszPtrT = 0;
      return SAFESTR_OK;
   }
   else if (nFilled>=targetSize-1)
   {
      *pszPtrT = 0;
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;;
   }
   else
      return SAFESTR_ERROR_UNKNOWN;
}

