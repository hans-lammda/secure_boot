#include <plf/platform.h>
#include <plf/memory.h>
#include "rsa_math.h"
#include "sha2.h"
#include "rsa_ver.h"

char secure_bios_rsa_ver_header[]  __attribute__ ((section (".keywords"))) = RSA_VER_H;
char secure_bios_rsa_ver_code[]  __attribute__ ((section (".keywords")))   = \
    "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/rsa_ver.c $ $Revision: 26466 $";


#define PKCS1_PADDING 1 
#define PKCS1_PADDING_FILL_PATTERN 0xFF 
#define MEM_ALIGN 16


int rsa_signature_check(unsigned char *e,
                        unsigned short e_len,
                        unsigned char *n,
                        unsigned short n_len,
                        unsigned char *sig,
                        unsigned short sig_len,
                        unsigned char *data,
                        unsigned long  data_len)
{

    unsigned char *decSig;
    unsigned short decSig_len;
    unsigned char *hash;   
    int i,j,ret,start ;

    /* sig[sig_len] = '\0';  */ 

    ret = SIGNATURE_VALID;

    /* Decrypt encrypted signature hash */ 
    decSig = rsa_decrypt_public_key(e,e_len,
                                    n,n_len,
                                    sig,sig_len,
                                    &decSig_len);
    
    /* First bit in decrypted data indicate padding */  
    if ( decSig[0] != PKCS1_PADDING ) {
      /* Unsupported padding */ 
      return SIGNATURE_INVALID;
    }

    /* Let index point beyond padding  */  
    for ( i = 1 ; i < decSig_len ; i++)  
      if ( decSig[i] != PKCS1_PADDING_FILL_PATTERN ) 
         break;  
    i++;

    /* We could end up here if no encrypted data exists */  
    if(i == decSig_len ) {
      return SIGNATURE_INVALID;
    }

    start = i ; 

    
    /* Calculate message digest 20 bit SHA1 */  
    hash = NMemAlloc_align(SIGNATURE_LENGTH_BYTES,MEM_ALIGN);
    if ( hash == NULL ) {
      /* Bad signature is a side effect of memory problem */ 
      return SIGNATURE_INVALID;
    }
    sha_256(data, data_len, hash);


    /* Compare if hashed data and decrypted signature match */ 
    for(j=0, i = start ; j<SIGNATURE_LENGTH_BYTES; j++,i++)
    {
        if(decSig[i] != hash[j])
            ret = SIGNATURE_INVALID;
    }
    
    NMemFree(hash);
    NMemFree(decSig);
    
   return ret;
}
