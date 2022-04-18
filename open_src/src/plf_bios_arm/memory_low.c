/*
  Copyright Oniteo 2007 - 2010
  TestVer.20101020A
*/

/*! \file memory_low.c
    \brief Implements platform independent memory functions.

     This module contains functions that could be used without heap 
     and static data section.
 */

#include <plf/platform.h>
#include <plf/memory.h>

const char __attribute__ ((section (".rom.ident")))  plf_memory_low_header_version[] = __PMILL_MEMORY_H__;
const char __attribute__ ((section (".rom.ident")))  plf_memory_low_header_plf[] = __ONITEO_PLATFORM_H__;
const char __attribute__ ((section (".rom.ident")))  plf_memory_code_low_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/memory_low.c $ $Revision: 26241 $";


void* FMemCopy(void* dest, void* src, size_t count)
{
  uint32_t i;

  for (i = 0; i < (count/4); i++) {
  ((uint32_t *) dest)[(i)] = ((uint32_t *) src)[(i)];
  }
  return dest;
}

void* NMemCopy(void* dest, void* src, size_t count)
{
  size_t i;

  for (i = 0; i < count; i++)
      ((char *) dest)[i] = ((char *) src)[i];
  return dest;
}


void* NMemMove(void* dest, void* src, size_t count)
{
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

void* NMemSet(void *ptr, int value, size_t count)

{
  int i;
  char *p; 

  p = (char*) ptr; 
  for (i = 0; i < count; i++)
      p[i] = value;
  return ptr; 
}



int NMemCmp(const void *ptr1, const void *ptr2, size_t count)
{
    size_t i;
    int result;

    for (i = 0; i < count; i++) {
      result  = ((unsigned char *) ptr1)[i] - ((unsigned char *) ptr2)[i];
      if (result) {
        return result;
      }
    }
    return 0;
}
