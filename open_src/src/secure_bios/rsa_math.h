/* Copyright Oniteo 2008 */ 

/*! \file rsa_math.h
    \brief RSA Math Functions 
    
    Minimal implementation of RSA mathematics needed for RSA signature 
    verification.
    
*/ 

/*! \name ignore 
 @{
*/ 
#ifndef RSA_MATH_H
#define RSA_MATH_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/rsa_math.h $ $Revision: 17411 $"
// @}
 
/*! \name Decrypt 

  @{
*/

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
                                      unsigned short *decData_len);

// @}

#endif //RSA_MATH_H
