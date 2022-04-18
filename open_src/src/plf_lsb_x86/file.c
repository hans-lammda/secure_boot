#include <plf/platform.h>
#include <plf/file.h>
#include <plf/memory.h>
#include <plf/safestr.h>

char plf_file_header_version[] = __PLF_FILE_HEADER__;
const char plf_file_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_file_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/file.c $ $Revision: 24738 $";


#define DBG_LEVEL 2

int plf_cp(const char *source, const char *target)
{


  int fp_in, fp_out;
  char line[1024];
  int bytes;


  if((fp_in = open(source, O_RDONLY)) == -1) {
    return PLF_NO_SOURCE;
  }

  if((fp_out = open(target, O_WRONLY | O_CREAT,S_IRWXU)) == -1) {
    return PLF_NO_TARGET;
  }

  while((bytes = read(fp_in, line, sizeof(line))) > 0) {
    if ( write(fp_out, line, bytes) == -1 ) {
      close(fp_in);
      close(fp_out);
      return PLF_WRITE_FAILED;
    }

  }

  fsync(fp_out);
  if ( close(fp_in) != 0 )
    return PLF_FILE_CLOSE_ERROR;

  if ( close(fp_out) != 0 )
    return PLF_FILE_CLOSE_ERROR;

  return PLF_OK ;
}




file_io plf_new_file(void)
{

 file_io p = NULL;
 p = (struct file_io_struct *) NMemAlloc(sizeof(struct file_io_struct));
 if ( p != NULL ) {
   p->buffer = NULL; 
   p->size  = 0; 
   p->sync  = 0; 
   p->mem_flag  = 0; 
   return p; 
 } 
 else 
 return NULL;
 
}

int plf_read_file(file_io in)
{
   int arc,fp;
   int n,nread;
   struct stat stat_p;
   
   in->read_mode = 0 ; 

  if ( strlen(in->path) == 0 ) 
    return PLF_FILE_BAD_PATH;
  

   
   if ( stat (in->path, &stat_p) == -1 ) {
     arc = NStrSCopy(in->err,"Unable to stat file",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_ATTR_FAIL; 
     return PLF_FILE_ATTR_FAIL ;
   }  
   in->size = stat_p.st_size;

   fp = open(in->path,O_RDONLY);
   if (fp <  0 ) {
     arc = NStrSCopy(in->err,"Unable to open file",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_ERR_OPEN; 
     return PLF_FILE_ERR_OPEN;
   }
   if ((in->buffer = ( char *) NMemAlloc(stat_p.st_size + 1)) != NULL ) {
     in->mem_flag  = 1; 
     for (nread = 0, n=0;  nread < in->size;  nread += n) 
       if (( n = read(fp, in->buffer, in->size)) <= 0 ) {
         close(fp);
         arc = NStrSCopy(in->err,"Error while reading file",ERR_MSG_LENGTH);
         assert(arc == SAFESTR_OK);
         in->rc = PLF_FILE_BAD_READ; 
         return PLF_FILE_BAD_READ;
       }
     in->rc = PLF_FILE_OK; 
     close(fp);
   }
   else {
     arc = NStrSCopy(in->err,"Unable to allocate memory",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_MEMORY_ALLOCATION_FAILED; 
   }
     in->read_mode = 1 ; 
     return PLF_FILE_OK; 
}

int plf_map_file(file_io in)
{
   int arc, fp;
   struct stat stat_p;

   in->read_mode = 0 ; 
   

  if ( strlen(in->path) == 0 ) 
    return PLF_FILE_BAD_PATH;
  

   
   if ( stat (in->path, &stat_p) == -1 ) {
     arc = NStrSCopy(in->err,"Unable to stat file",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_ATTR_FAIL; 
     return PLF_FILE_ATTR_FAIL ;
   }  
   in->size = stat_p.st_size;

   fp = open(in->path,O_RDONLY);
   if (fp <  0 ) {
     arc = NStrSCopy(in->err,"Unable to open file",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_ERR_OPEN; 
     return PLF_FILE_ERR_OPEN;
   }
  
  in->buffer =   mmap(0, in->size, PROT_READ, MAP_PRIVATE, fp, 0);

   if ( (int ) in->buffer == -1 ) 
     return PLF_FILE_ERR_MAP; 
   else  {
     in->read_mode = 2 ; 
     return PLF_FILE_OK; 
   }
}


int plf_write_file(file_io in)
{
   int arc, fp;
   int n , nwritten;

  if ( strlen(in->path) == 0 ) 
    return PLF_FILE_BAD_PATH;
  
   fp = open(in->path,O_WRONLY|O_CREAT|O_TRUNC,in->mode);
   if (fp <  0 ) {
     arc = NStrSCopy(in->err,"Unable to open file",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_ERR_OPEN; 
     return PLF_FILE_ERR_OPEN;
   }

   for ( nwritten = 0 , n = 0; nwritten < in->size ; nwritten += n ) 
     if (( n = write(fp, in->buffer + nwritten , in->size - nwritten)) < 0 )  {
       close(fp);
       unlink(in->path);
       arc = NStrSCopy(in->err,"Failed to write",ERR_MSG_LENGTH);
       assert(arc == SAFESTR_OK);
       in->rc = PLF_FILE_ERR_WRITE; 
       return PLF_FILE_ERR_WRITE;
     }

   in->rc = PLF_FILE_OK; 
#ifndef PLATFORM_WINDOWS
   if (in->sync) 
    fsync(fp); 
#endif
   close(fp);
   return PLF_FILE_OK; 
}


void plf_free_file(file_io in)
{

   switch (in->read_mode) {
    case 1: 
      if ((in->mem_flag ) && (in->buffer != NULL) ) 
        NMemFree(in->buffer);
      break; 
    case 2: 
        munmap(in->buffer, in->size); 
      break; 
   }


   NMemFree(in);
}

int plf_attr_file(file_io in)
{
   int arc ;
   struct stat stat_p;

   if ( strlen(in->path) == 0 ) 
    return PLF_FILE_BAD_PATH;
  
   if ( stat (in->path, &stat_p) == -1 ) {
     arc = NStrSCopy(in->err,"Unable to stat file",ERR_MSG_LENGTH);
     assert(arc == SAFESTR_OK);
     in->rc = PLF_FILE_ATTR_FAIL; 
     return PLF_FILE_ATTR_FAIL ;
   }
   else {
     in->size = stat_p.st_size;
     in->mode = stat_p.st_mode;
   }

   return PLF_FILE_OK;
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

#if 0 
  if ( plf_ioctl_file(tmp,data) != PLF_FILE_OK ) {
   trace_d(DBG_LEVEL,"Failed to call plf_ioctl_file \n");
   return NULL;
  }
#endif 

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






