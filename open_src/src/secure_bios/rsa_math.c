#include <plf/platform.h>
#include "bignum.h"
#include "rsa_math.h"

char secure_bios_rsa_header[] __attribute__ ((section (".keywords"))) = RSA_MATH_H;
char secure_bios_rsa_code[]   __attribute__ ((section (".keywords")))   = \
     "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/rsa_math.c $ $Revision: 25403 $";


/*! \brief Public Key decryption 
 
     This function require the RSA public key exponent (e) and the RSA 
     modulus (n). The e value is common for keys used in X509 
     certificates and could therefore be excluded for key storage. 

     \param[in] e RSA public key exponent 
     \param[in] e_len Length of RSA public key exponent 
     \param[in] n RSA modulus 
     \param[in] n_len Length of RSA modules
     \param[in] data Data to be decrypted  
     \param[in] data_len Length of data to be decrypted  
     \param[out] decData_len Length of decrypted data
     \return NULL if failed of pointer to data

*/ 
unsigned char *rsa_decrypt_public_key(unsigned char *e,
                                      unsigned short e_len,
                                      unsigned char *n,
                                      unsigned short n_len,
                                      unsigned char *data,
                                      unsigned short data_len,
                                      unsigned short *decData_len)
{
   Bignum n_bn;
   Bignum e_bn;
   Bignum data_bn;
   Bignum decData_bn;
   unsigned char *decData;
    
   n_bn = bignum_from_bytes(n, n_len);
   e_bn = bignum_from_bytes(e, e_len);
   data_bn = bignum_from_bytes(data, data_len);
   
   decData_bn = modpow(data_bn, e_bn, n_bn);
   
   decData = bytes_from_bignum(decData_bn,decData_len);
   
   freebn(n_bn);
   freebn(e_bn);
   freebn(data_bn);
   freebn(decData_bn);
   
   return decData;

}
