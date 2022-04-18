#include <plf/platform.h>
#include <plf/memory.h>
#include <trust.h>


#include "root_pk_hash.h"


#define DBG_LEVEL 5

struct sobject *get_root_pk_hash(void)
{

 static struct root_pk_hash root_pk_hash;
 static struct sobject *tp ;
 static struct sobject  sp_hash;
 int i; 
 char *cp; 




 root_pk_hash.type = ROOT_PK_HASH_TYPE ;
 root_pk_hash.size = ROOT_PK_HASH_SIZE ;
 root_pk_hash.start = (uint64_t)  root_pk_hash_value;

 tp = &sp_hash;
 tp->p.rkh_p = &root_pk_hash;

 sp_hash.version = 1;
 sp_hash.type  = ROOT_PK_HASH;

 for(i = 0 , cp = (char *) tp->p.rkh_p->start; i < tp->p.rkh_p->size; i++,cp++)
   trace_dl(DBG_LEVEL,"tp->p.rkh_p->start[%d]=0x%x \n",i,*cp);

 return tp ;

}



