#include <plf/platform.h>
#include <plf/memory.h>
#include <spd/spd.h>
#include <rsa_ver.h>

#define DBG_LEVEL 5


const char spdlib_header_version[]  __attribute__ ((section (".keywords")))  = _SPD_H_; 
const char spdlib_code_verify_version[]    __attribute__ ((section (".keywords")))     = \
   "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/spdlib/branches/haspoc/juno/spdlib/spd_verify.c $ $Revision: 26962 $";


int  verify_element(struct sobject * trust, struct sobject * sp)
{
 int rc , sig_len, hash_len, i ; 
 uint32_t  payload_size ;
 uint64_t  sig_start,  payload_start ;
 unsigned char *calc_hash, *trust_hash;


 trace_dl(10,"start\n"); 
 trace_dl(10,"sizeof(struct sobject)  %d\n",sizeof(struct sobject));
 trace_dl(10,"sizeof(struct root_pk)  %d\n",sizeof(struct root_pk));
 trace_dl(10,"sizeof(struct boot_ldr_2)  %d\n",sizeof(struct boot_ldr_2));
 trace_dl(10,"sizeof(struct sdram)  %d\n",sizeof(struct sdram));
 trace_dl(10,"sizeof(struct kernel)  %d\n",sizeof(struct kernel));
 trace_dl(10,"sizeof(struct devtree)  %d\n",sizeof(struct devtree));
 trace_dl(10,"sizeof(struct atf_payload)  %d\n",sizeof(struct atf_payload));
 trace_dl(10,"sizeof(struct atf_runtime)  %d\n",sizeof(struct atf_runtime));

 if (sp != 0 ) {


  trace_dl(DBG_LEVEL,"sp->type %d\n",sp->type); 
  switch  (sp->type ) {


   case ROOT_PK     : 
                 rc = SPD_OK;  
                 sp->p.rk_p  =  (struct root_pk *) ( (uint64_t) sp + (uint64_t) sp->p.rk_p)  ; 
	         payload_size   =   sp->p.rk_p->size_0 +  sp->p.rk_p->size_1 ;
		 payload_start = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct root_pk) ;
                 switch ( sp->sigtype ) {
                    case SHA256_BIN : 
                                      hash_len = 32; 

				      if  ( trust->type != ROOT_PK_HASH ) 
                                        rc =  SPD_UNSUPPORTED_HASH_TYPE; 

                                      if ( trust->p.rkh_p->size != hash_len ) 
                                        rc =  SPD_HASH_LENGTH_MISMATCH; 

                                      trust_hash = (unsigned char *)   trust->p.rkh_p->start ; 

                                      break ; 
                    default         : 
                                      rc =  SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                      break ; 
                 }

                 if ( rc == SPD_OK ) { 

                  if ( (calc_hash = NMemAlloc_align(hash_len,8)) == NULL ) 
                    return SPD_MEMORY_ALLOCATION_FAILURE ; 

                  sha_256((void*) payload_start, payload_size, calc_hash);

                  /* Compare trusted hash with calculated hash */
                  for(i = 0 ; i < hash_len; i++)
    		  {
                   if(trust_hash[i] != calc_hash[i])
                      rc = SPD_ERROR;
                  }

                  /* n */ 
	          sp->p.rk_p->value_0 = payload_start ;
                  /* e */ 
	          sp->p.rk_p->value_1 = payload_start + sp->p.rk_p->size_0;
                 }

		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct root_pk) +  sp->p.rk_p->size_0 +  sp->p.rk_p->size_1 +  sp->cert_len + sp->siglen + sp->padding; 

 	         return rc; 
                 break; 

   case ATF_BOOT  : 

		 payload_start = (uint64_t) sp ; 
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }

		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,( unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           ( unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *)  payload_start,
                                           payload_size);

                 sp->p.bl2_p  =  (struct boot_ldr_2 *) ( (uint64_t) sp + (uint64_t) sp->p.bl2_p)  ; 
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct boot_ldr_2) + sp->p.bl2_p->size + sp->cert_len + sp->siglen + sp->padding; 
                 sp->p.bl2_p->start =   (uint64_t) sp  + sp->p.bl2_p->start; 

    		 if(rc == SIGNATURE_VALID )
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 

   case SDRAM       : 
		 payload_start =  (uint64_t) sp ;
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }
		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,(unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           (unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *) payload_start,
                                           payload_size);

                 sp->p.sdp  =  (struct sdram *) ( (uint64_t) sp + (uint64_t) sp->p.sdp)  ;
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct sdram) +  sp->p.sdp->size + sp->cert_len + sp->siglen + sp->padding; 
                 sp->p.sdp->start =   (uint64_t) sp  + sp->p.sdp->start; 

    		 if(rc == SIGNATURE_VALID ) 
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 

   case RAMDISK     : 
		 payload_start =  (uint64_t) sp ;
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }
		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,(unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           (unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *) payload_start,
                                           payload_size);

                 sp->p.rp  =  (struct ramdisk *) ( (uint64_t) sp + (uint64_t) sp->p.rp)  ;
                 sp->p.rp->start = payload_start + sizeof(struct sobject) + sizeof(struct ramdisk) ; 
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct ramdisk) +  sp->p.rp->size + sp->cert_len + sp->siglen + sp->padding; 

    		 if(rc == SIGNATURE_VALID ) 
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 
   case KERNEL    : 
		 payload_start =  (uint64_t) sp ;
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }
		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,(unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           (unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *) payload_start,
                                           payload_size);

                 sp->p.kernel_p  =  (struct kernel *) ( (uint64_t) sp + (uint64_t) sp->p.kernel_p)  ;
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct kernel) +  sp->p.kernel_p->size + sp->cert_len + sp->siglen + sp->padding; 
                 sp->p.kernel_p->start = payload_start  + sizeof(struct sobject) +  sizeof(struct kernel); 

    		 if(rc == SIGNATURE_VALID )
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 

   case DEVTREE   : 
		 payload_start =  (uint64_t) sp ;
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }
		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,(unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           (unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *) payload_start,
                                           payload_size);

                 sp->p.devtree_p  =  (struct devtree *) ( (uint64_t) sp + (uint64_t) sp->p.devtree_p)  ;
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct devtree) +  sp->p.devtree_p->size + sp->cert_len + sp->siglen + sp->padding; 
                 sp->p.devtree_p->start = payload_start  + sizeof(struct sobject) +  sizeof(struct devtree); 

    		 if(rc == SIGNATURE_VALID )
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 

   case ATF_PAYLOAD   : 
		 payload_start = (uint64_t) sp ; 
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }
		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,(unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           (unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *) payload_start,
                                           payload_size);

                 sp->p.atf_payload_p  =  (struct atf_payload *) ( (uint64_t) sp + (uint64_t) sp->p.atf_payload_p)  ;
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct atf_payload) +  sp->p.atf_payload_p->size + sp->cert_len + sp->siglen + sp->padding; 
                 sp->p.atf_payload_p->start = payload_start  +  sizeof(struct sobject) + sizeof(struct atf_payload); 

    		 if(rc == SIGNATURE_VALID ) 
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 

   case ATF_RUNTIME   : 
		 payload_start = (uint64_t) sp ; 
		 payload_size   =  (uint32_t) sp->sigstart  ; 
		 sig_start = (uint64_t) sp  +   (uint64_t) sp->sigstart ;
		 sig_len = sp->siglen;

                 switch ( sp->sigtype ) {
                    case RSA : 

				      if  ( trust->type != ROOT_PK ) 
                                        return SPD_TRUST_EXPECTED_ROOT_PK; 
				      if  ( trust->p.rk_p->type != RSA ) 
                                        return SPD_EXPECTED_RSA; 
                                      break ; 
                    default         : 
                                      return SPD_UNSUPPORTED_SIGNATURE_TYPE; 
                                        break ; 
                 }
		 rc = rsa_signature_check((unsigned char *) trust->p.rk_p->value_1,trust->p.rk_p->size_1,(unsigned char *) trust->p.rk_p->value_0,trust->p.rk_p->size_0,
                                           (unsigned char *) sig_start,
                                           sig_len,
                                           (unsigned char *) payload_start,
                                           payload_size);

                 sp->p.atf_runtime_p  =  (struct atf_runtime *) ( (uint64_t) sp + (uint64_t) sp->p.atf_runtime_p)  ;
		 sp->next = (uint64_t) sp + sizeof(struct sobject) +  sizeof(struct atf_runtime) +  sp->p.atf_runtime_p->size + sp->cert_len + sp->siglen + sp->padding; 
                 sp->p.atf_runtime_p->start = payload_start +  sizeof(struct sobject) +  sizeof(struct atf_runtime); 

    		 if(rc == SIGNATURE_VALID ) 
 	           return SPD_OK; 
                 else
                   return SPD_ERROR; 
                 break; 

   default :     
                 return SPD_HSBF_UNSUPPORTED_OBJECT; 
                 break; 
   }
  }
  else {
   trace_dl(DBG_LEVEL,"invalid sp\n"); 
   return SPD_INVALID_POINTER_HSBF_IMAGE; 
  }
}


