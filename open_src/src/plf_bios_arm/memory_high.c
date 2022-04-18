/*
  Copyright Oniteo 2007 - 2010
  TestVer.20101020A
*/

/*! \file memory.c
    \brief Implements platform independent memory functions.
 */

#include <plf/platform.h>
#include <plf/memory.h>

const char __attribute__ ((section (".rom.ident")))  plf_memory_header_version[] = __PMILL_MEMORY_H__;
const char __attribute__ ((section (".rom.ident")))  plf_memory_header_plf[] = __ONITEO_PLATFORM_H__;
const char __attribute__ ((section (".rom.ident")))  plf_memory_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/memory_high.c $ $Revision: 26500 $";


static char   *mem_start = 0 ; // Start of memory  
static int    mem_size = 0 ;    // Total available memory
static int    mem_used = 0 ;    // Used memory  

#define DBG_LEVEL 8 


int    NMemPool(char *start,size_t size,int test)
{
   char *p; 
   int data, i;

   mem_size = size;  
   mem_start = start; 
   mem_used = 0 ; 


   trace_dl(DBG_LEVEL,"mem_start %x\n",mem_start);
   trace_dl(DBG_LEVEL,"mem_size %d\n",mem_size);
   trace_dl(DBG_LEVEL,"mem_used %d\n",mem_used);
   trace_dl(DBG_LEVEL,"test enable %d\n",test);

   if ( test == 1 ) {
     for ( p = mem_start , i = 0 ; i < mem_size ; i = i + 1000 ) {
       data = i % 256 ; 
       // trace_dl(DBG_LEVEL,"*(0x%x + %d) = %d\n",p,i,data);
       *(p + i) = data ; 
       if ( *(p + i) != data ) {
         trace_dl(1,"error on addess %x\n",p + i );
         return 0;
       }
     }
   }
   if ( test == 1 ) {
     trace_dl(DBG_LEVEL,"Test passed \n");
     trace_dl(DBG_LEVEL,"mem_end %x\n",(p + i));
   }
   else 
     trace_dl(DBG_LEVEL,"Notice: Memory not tested\n");
   return 1;
}


void*  NMemAlloc(size_t bytes)
{
   bytes += (size_t)3;
   char *temp; 

   trace_dl(DBG_LEVEL,"bytes %d\n",bytes);
   mem_used = mem_used + bytes + (size_t)3; 

   if (mem_used >= mem_size)
   {
      trace_dl(DBG_LEVEL,"No memory left in pool total size %d used memory %d\n",mem_size,mem_used);
      return NULL;
   }

   temp = mem_start + ( mem_used - bytes ) ;
   trace_dl(DBG_LEVEL,"%d bytes allocated at %x\n",bytes,temp);

   /* temp = ((unsigned int) temp & ~3); */ 
   return (void*) temp  ;
}


void *NMemAlloc_align(size_t bytes, size_t align)

{

    char *tmp = NULL;
    int extra = 0;
    int mod, used = 0;

    if (!bytes) {
	return NULL;
    }

    if (!(align && !(align & (align - 1)))) {
	trace_dl(DBG_LEVEL,"Wrong alignment size\n");
	return NULL;
    }



    /* Calculate extra bytes for aligned start address */
    mod = (unsigned long ) mem_start % align;
    if (mod) {
	extra = align - mod;
    }

    // trace_dl(DBG_LEVEL,"extra bytes %d\n", extra);
    used = mem_used + bytes + extra;
    if (used >= mem_size) {
	trace_dl(DBG_LEVEL,"No memory left in pool total size %d used memory %d\n", mem_size, used);
	return NULL;
    }



    tmp = mem_start + extra;

    /* Adjust mem_start */

    mem_start = tmp + bytes;
    /* Adjust mem usage */
    mem_used = used;

    trace_dl(DBG_LEVEL,"Allocating %d bytes with %d byte alignment, used %d bytes\n", bytes, align,mem_used);

    return (void*)tmp;
}

void  NMemFree(void* pMemBlk)
{
   return;
}

void  NMemFree_align(void* pMemBlk)
{
   return;
}


void   NMemAddMallocStat(void *ptr)
{
}

void   NMemAddFreeStat(void *ptr)
{
}

int  NMemPrintStats(void)
{
  return mem_used;
}
