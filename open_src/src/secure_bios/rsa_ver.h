/* Copyright Oniteo 2008 */ 

/*! \file rsa_ver.h
    \brief RSA Signature Verification
 
 
    This is a minimal implementation of RSA that could used in embedded 
    application such as a BIOS. It checks if a signature is valid or 
    not.
    
*/ 

/*! \brief Signature verified successfully */
#define SIGNATURE_VALID 1
/*! \brief Signature failure */
#define SIGNATURE_INVALID 0


/*! \name ignore
    @{
        */
#ifndef RSA_VER_H
#define RSA_VER_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/rsa_ver.h $ $Revision: 24674 $"
// @}

/*! \name Verify 
 
  @{

*/ 



/*! \brief Signature verification
     
     This function uses the RSA decrypt- and SHA-1 hash- functions to 
     decrypt the signature, hash the data and then compare them to see 
     if the signature is valid or not. It requires the RSA public key 
     exponent (e) and the RSA modulus (n). The e value is common for 
     keys used in X509 certificates and could therefore be excluded 
     for key storage. 
      
     \param[in] e RSA public key exponent 
     \param[in] e_len Length of RSA public key exponent 
     \param[in] n RSA modulus 
     \param[in] n_len Length of RSA modulus
     \param[in] sig Signature to be decrypted 
     \param[in] sig_len Length of signature to be decrypted 
     \param[in] data Data to be hashed
     \param[in] data_len Length of data to be hashed
     \return 1 if signature is valid and 0 if invalid

*/
int rsa_signature_check(unsigned char *e,
                        unsigned short e_len,
                        unsigned char *n,
                        unsigned short n_len,
                        unsigned char *sig,
                        unsigned short sig_len,
                        unsigned char *data,
                        unsigned long  data_len);


/*! \brief SHA-256 hash function

     Uses the SHA-256 algorithm on the input data. The output hash will
     have a length of 65 bytes, 64 bytes hex-values and a null-terminator.

     \param[in] p Pointer to data to be hashed
     \param[in] len Length of data
     \param[out] output binary sha256 hash

*/
void sha_256(void *p, int len, unsigned char *output); 


// @}

#endif //RSA_VER_H
