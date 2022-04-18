/* 
  Copyright Oniteo 2007 - 2008 
*/

/*! \file memory.c
    \brief Implements platform independent memory functions.
 */

#include <plf/platform.h>
#include <plf/memory.h>

const char plf_memory_header_version[] = __PMILL_MEMORY_H__;
const char plf_memory_header_plf[] = __ONITEO_PLATFORM_H__;
const char plf_memory_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/memory.c $ $Revision: 25901 $";



void* NMemAlloc_align(size_t bytes,size_t align)
{
   return calloc(1,bytes);
}

void* NMemAlloc(size_t bytes)
{
   return calloc(1,bytes);
}

void* NMemRealloc(void* pMemBlk, size_t bytes)
{
   if (!pMemBlk)
      return NMemAlloc(bytes);

   return realloc(pMemBlk,bytes);
}

void NMemFree(void* pMemBlk)
{
   if (pMemBlk)
      free(pMemBlk);
}

void* NMemCopy(void* dest, void* src, size_t count)
{
   return memcpy(dest,src,count);
}

int NMemReadable(void* pMemBlk,size_t blkSize)
{
   return (pMemBlk==NULL)?TRUE:FALSE;
}

int NMemWriteable(void* pMemBlk,size_t blkSize)
{
   return (pMemBlk==NULL)?TRUE:FALSE;
}

char* NStrDup(const char* str)
{
   char* pszDup;

   if (!str)
      return NULL;

   if (strlen(str)<1)
   	pszDup=calloc(1,1);
   else
      pszDup=strdup(str);
   return pszDup; 
}

void* NMemDup(void* source,size_t size)
{
   void* m = NMemAlloc(size);
   if (m == NULL)
      return NULL;
   NMemCopy(m,source,size);
   return m;
}

void* NMemMove(void* dest, void* src, size_t count){
  char *d = dest;
  const char *s = src;
  size_t i;

  if (d < s) {
      for (i = 0; i < count; i++)
          d[i] = s[i];
  } else {
      for (i = count-1; i != (size_t)(-1); i--)
          d[i] = s[i];
  }
  return dest;
}

void* NMemSet(void *ptr, int value, size_t count){
  size_t i;

  for (i = 0; i < count; i++)
      ((char *) ptr)[i] = value;
  return ptr;
}

int NMemCmp(const void *ptr1, const void *ptr2, size_t count)
{
    size_t i;
    int result;

    for (i = 0; i < count; i++) {
      result = ((unsigned char *) ptr1)[i] - ((unsigned char *) ptr2)[i];
      if (result) {
        return result;
      }
    }
    return 0;
}



