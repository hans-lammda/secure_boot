#include <plf/platform.h>
#include <plf/memory.h>
#include <spd/spd.h>
#include <rsa_ver.h>

#define DBG_LEVEL 5


const char spdlib_code_install_version[]    __attribute__ ((section (".keywords")))     = \
   "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/spdlib/branches/haspoc/juno/spdlib/spd_install.c $ $Revision: 26962 $";


int  install_key(struct sobject ** trust, struct sobject * sp)
{

  if (sp == NULL ) 
   return SPD_INVALID_POINTER_HSBF_IMAGE; 

  switch  (sp->type ) {
    case ROOT_PK_HASH : 
	      trace_dl(DBG_LEVEL,"ROOT_PK_HASH: sha256 length =%d\n",sp->p.rkh_p->size);
              break;  
    case ROOT_PK : 
	      trace_dl(DBG_LEVEL,"ROOT_PK: n=%d\n",sp->p.rk_p->size_0);
	      trace_dl(DBG_LEVEL,"ROOT_PK: e=%d\n",sp->p.rk_p->size_1);
              break;  
    default: 
              return SPD_UNSUPPORTED_ROOT_PK_TYPE; 
              break; 
  }
  *trust = sp ; 
  return SPD_OK; 
}


