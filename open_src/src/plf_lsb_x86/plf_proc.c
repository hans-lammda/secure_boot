#include <plf/platform.h>
#include <plf/plf_proc.h>
#include <pthread.h>

#define BUFLEN 100

char plf_proc_header_version[] = __PMILL_WINDOWS_PROC_HEADER__;
const char plf_proc_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_proc_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/plf_proc.c $ $Revision: 16123 $";


int CheckProcess(int pid)
{

 char buffer[BUFLEN];
 int pidfd,ret;
 
 snprintf(buffer,BUFLEN,"/proc/%d",pid);
 if ( ( pidfd = open(buffer, O_RDONLY ))  != -1) {
   close(pidfd);
   ret = 1;
 }
 else 
   ret = 0;
 return ret;
}

int NThreadCreate(void  *thread, const void *attr,void* (*start_routine)(), void* arg )
{
  int ret,rc; 
  pthread_t *thread_id = ( pthread_t *) thread;
  pthread_attr_t *attribute = ( pthread_attr_t* ) attr;
  
  ret = rc = pthread_create(thread_id, attribute, start_routine, arg);
  switch (rc) {
    case 0: 
             ret = PLF_PROC_OK ; 
             break;
    case 12: 
             ret = PLF_PROC_ERROR_MEM ; 
             break;
    default:
             break;
  }
  return ret; 
}

int NThreadDetach(void  *thread)

{
  int ret,rc; 
  pthread_t *thread_id = ( pthread_t* ) thread;
  rc = ret = pthread_detach(*thread_id);
  switch (rc) {
    case 0: 
             ret = PLF_PROC_OK ; 
             break;
    default:
             break;
  }
  return ret; 
}

void NThreadExit(void  *thread)
{

  pthread_exit(thread);

}

