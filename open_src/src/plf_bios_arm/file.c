#include <plf/platform.h>
#include <plf/file.h>
#include <plf/memory.h>
#include <plf/safestr.h>
#include <vfs.h>


/*
   Copyright Oniteo (C) 2008 - 2013
*/ 
  


const char __attribute__ ((section (".rom.ident"))) plf_file_header_version[] = __PLF_FILE_HEADER__;
const char __attribute__ ((section (".rom.ident"))) plf_file_header_plf[] = __ONITEO_PLATFORM_H__;
const char __attribute__ ((section (".rom.ident"))) plf_file_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/file.c $ $Revision: 24733 $";

#define DBG_LEVEL 2

file_io   plf_new_file(void)
{

 file_io p = NULL;

 MALLOC(p,sizeof(struct file_io_struct));
 if ( p != NULL ) {
   p->size = 0;
   p->buffer = NULL;
   p->progress  = NULL;
   p->sync  = 0;
   p->mem_flag  = 0;
   return p;
 }
 else
 return NULL;
}

void   plf_free_file(file_io in)
{
 NMemFree(in->buffer);
 NMemFree(in);
}


int   plf_attr_file(file_io in)
{
   FP fp; 
   fp = in->h;

   if ( fp == NULL ) {
    return PLF_FILE_ATTR_FAIL;
   }

   in->size = fp->filemax;
   in->mode = 0;
   return PLF_FILE_OK;
}


int  plf_ioctl_file(file_io in, void *data)
{

   FP fp; 

   trace_dl(DBG_LEVEL,"About to call file_open[%s]\n",in->path);
   fp  = file_open(in->path,(init_data*) data);
   trace_dl(DBG_LEVEL,"file_open returned\n");

   if ( fp == NULL ) {
    return PLF_FILE_BAD_PATH;
   }
    
   if ( fp->status != 1  ) {
    trace_e("%s\n",fp->info); 
    return PLF_FILE_BAD_PATH;
   }
    

   in->h = fp ;
   in->size  = fp->filemax ;
    return PLF_FILE_OK;
}

int   plf_read_file(file_io in)
{
   int rc ; 

   /* If buffer is assigned dont allocate memory on heap */ 
   if (in->buffer  == NULL ) {
     MALLOC(in->buffer,in->size + 1);
     if (in->buffer  == NULL ) {
      return PLF_FILE_ERR_OPEN;
     }
   }

   trace_dl(DBG_LEVEL,"About to call file_read\n");
   rc = file_read(in->h,in->buffer, in->size,in->progress);
   trace_dl(DBG_LEVEL,"file_read returned\n");

   if (rc  == in->size) 
    return PLF_FILE_OK;
   else {
    NStrSPrintf(in->err,ERR_MSG_LENGTH, "expected size [%d] actual size [%d]\n", rc,in->size); 
    return PLF_FILE_BAD_READ;
   }
}

file_io plf_load_file(char *path, int len , void (*progress)(int),void *data)
{

  file_io tmp;

  if ((tmp = plf_new_file()) == NULL ) {
   trace_d(DBG_LEVEL,"Failed to allocate handle \n");
   return NULL;
  }

  tmp->path = path;
  tmp->progress = progress;

  if ( plf_ioctl_file(tmp,data) != PLF_FILE_OK ) {
   trace_d(DBG_LEVEL,"Failed to call plf_ioctl_file \n");
   return NULL;
  }

  if ( plf_attr_file(tmp) != PLF_FILE_OK ) {
   trace_d(DBG_LEVEL,"Failed to call plf_attr_file \n");
   return NULL;
  }

  if ( len > 0 )
   tmp->size = len ;

  tmp->buffer  = NULL; /* Use malloc */
  if ( plf_read_file(tmp) != PLF_FILE_OK ) {
   trace_d(DBG_LEVEL,"Failed to call plf_read_file \n");
   return NULL;
  }

  return tmp;

}

