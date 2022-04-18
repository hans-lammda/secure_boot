/*
  Copyright Oniteo (C) 2013
 
*/

/*! \file spd.h
    \brief HSBF Format library


     This library is based on result from Artemis N-Shield 

     where components are assigned Security , privacy and dependency parameters. 

     @\n

     The format is described in C header files and therefore requires little endian architecture. 


     Key management is adapted for compliance with Arm Trusted Firmware where the assymetric key 

     is verified against a hash value , then installed as the trust anchor for the remaining objects. 

     @\n

     
     Verification reads from a memory buffer and returns the verification result for each object. 

     For each object being processed a pointer to next object is updated. 


     
*/

/*! \brief IGNORE */
#ifndef _SPD_H_
#define _SPD_H_ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/spdlib/branches/haspoc/juno/spdlib/spd.h $ $Revision: 26876 $"


/*! \brief  Invalid size of HSBF image 
 */
#define HSBF_SIZE_UNKNOWN 0 

/*! \brief  Success 
 */
#define SPD_OK 1 

/*! \brief  Verification failed 
 */
#define SPD_ERROR 2 

/*! \brief  Unable to allocate memory 
 */
#define SPD_MEMORY_ALLOCATION_FAILURE 3

/*! \brief  Attempt to use unsupported hash algoritm  
 */
#define SPD_UNSUPPORTED_HASH_TYPE 4

/*! \brief  Mismatch between hash length 
 */
#define SPD_HASH_LENGTH_MISMATCH 5

/*! \brief Unsupported signature type
 */
#define SPD_UNSUPPORTED_SIGNATURE_TYPE 6

/*! \brief Unsupported ROOT_PK type
 */
#define SPD_UNSUPPORTED_ROOT_PK_TYPE 7

/*! \brief Could not verify against ROOT_PK 
 */
#define SPD_TRUST_EXPECTED_ROOT_PK 8

/*! \brief RSA key not installed as trust anchor
 */
#define SPD_EXPECTED_RSA 9

/*! \brief Invalid address of HSBF image 
 */
#define SPD_INVALID_POINTER_HSBF_IMAGE 10

/*! \brief Unsupported object in HSBF image 
 */
#define SPD_HSBF_UNSUPPORTED_OBJECT 11






/*! \brief  Public root key hash object 
 */
#define ROOT_PK_HASH 0

/*! \brief  Public root key object 
 */
#define ROOT_PK 1 

/*! \brief Boot configuration object 
 */
#define BOOT_CFG  2

/*! \brief Parameter block needed for ATF Boot  
 */
#define ATF_BOOT 3

/*! \brief SDRAM object key
 */
#define SDRAM  4

/*! \brief Ramdisk object key
 */
#define RAMDISK  5

/*! \brief KERNEL object key
 */
#define KERNEL 6 

/*! \brief DEVTREE object key
 */
#define DEVTREE 7 

/*! \brief ATF_RUNTIME SMC based services
 */
#define ATF_RUNTIME 8

/*! \brief ATF_PAYLOAD Boot interface
 */
#define ATF_PAYLOAD 9

/*! \name IGNORE 
     @{
*/
/* Required for test */ 
#define TEST_OF_UNKNOWN_TYPE 100
/*!  
     @}
*/


/*! \brief Signed by RSA  
 */
#define RSA  1 

/*! \brief binary hash SHA256  
 */
#define SHA256_BIN 2 

/*! \brief Size of HSBF identity string 
 */
#define  SPD_ID_SIZE 32

/*! \brief Public Root key hash
*/
struct   __attribute__((__packed__)) root_pk_hash {

 /*! \brief Type of hash */
 uint32_t type; 

 /*! \brief Size of hash */
 uint32_t size; 

 /*! \brief Start of hash*/
 uint64_t  start; 

};

/*! \brief Public Root key   
*/
struct   __attribute__((__packed__)) root_pk {

 /*! \brief Type of object */
 uint32_t type; 


 /*! \brief Type of first element  */
 uint32_t tag_0; 

 /*! \brief Start of first element   */
 uint64_t  value_0;

 /*! \brief Size of first element*/
 uint32_t size_0; 

 /*! \brief Type of second element  */
 uint32_t tag_1; 

 /*! \brief Start of second element   */
 uint64_t  value_1;

 /*! \brief Size of second element*/
 uint32_t size_1; 

 /*! \brief Identifier */
 uint8_t id[SPD_ID_SIZE]; 

 /*! \brief IGNORE  */
 uint32_t padding ;

};

/*! \brief Boot configuration  
*/
struct  __attribute__((__packed__)) boot_cfg {

 /*! \brief To be defined  */
 uint32_t tbd_1; 

 /*! \brief To be defined */
 uint32_t tbd_2; 

};

/*! \brief Second boot loader
*/
struct   __attribute__((__packed__)) boot_ldr_2 {

 /*! \brief Start of fip.bin relative start of this header  */
 uint64_t start; 

 /*! \brief Where to load bl31 */
 uint64_t bl31_load_address;

 /*! \brief Where to load bl32 */
 uint64_t bl32_load_address;

 /*! \brief Where to load bl33 */
 uint64_t bl33_load_address;

 /*! \brief X0 argument to bl33  */
 uint64_t bl33_X0; 

 /*! \brief X1 argument to bl33  */
 uint64_t bl33_X1; 

 /*! \brief Exception level of bl33 [1-2]*/
 uint32_t e_level; 

 /*! \brief Size of payload image */
 uint32_t size; 
};

/*! \brief ATF runtime 
*/
struct   __attribute__((__packed__)) atf_runtime {

 /*! \brief Start of fip.bin relative start of this header  */
 uint64_t start; 

 /*! \brief Where to load bl31 */
 uint64_t bl31_load_address;

 /*! \brief Size of payload image */
 uint32_t size; 
};

/*! \brief ATF payload 
*/
struct   __attribute__((__packed__)) atf_payload {

 /*! \brief Start of fip.bin relative start of this header  */
 uint64_t start; 

 /*! \brief Where to load bl33 */
 uint64_t bl33_load_address;

 /*! \brief Size of payload image */
 uint32_t size; 
};

/*! \brief SDRAM header
*/


struct  __attribute__((__packed__)) sdram {

 /*! \brief Start of image relative start of this header  */
 uint64_t  start; 

 /*! \brief Start of heap in SDRAM   */
 uint64_t  heap_start; 

 /*! \brief Size of heap in SDRAM */
 uint32_t heap_size;

 /*! \brief Size of image */
 uint32_t size;

 /*! \brief type */
 uint32_t  type;

 /*! \brief Total size of HSBF image*/
 uint32_t  image_size;
};


/*! \brief Ramdisk header
*/
struct  __attribute__((__packed__))  ramdisk {


 /*! \brief start of ramdisk  image */
 uint64_t start;

 /*! \brief Where to load ramdisk  */
 uint64_t load_address;

 /*! \brief Size of ramdisk */
 uint32_t size;

 /*! \brief IGNORE  */
 uint32_t padding ;
};

/*! \brief Kernel header
*/
struct  __attribute__((__packed__))  kernel {


 /*! \brief start of kernel  image */
 uint64_t start;

 /*! \brief Where to load kernel  */
 uint64_t load_address;

 /*! \brief Size of kernel */
 uint32_t size;

 /*! \brief X0 argument   */
 uint64_t X0; 

 /*! \brief X1 argument  */
 uint64_t X1; 

 /*! \brief Exception level of bl33 [1-2]*/
 uint32_t e_level; 

 /*! \brief IGNORE  */
 uint32_t padding ;
};

/*! \brief Device Tree header
*/
struct  __attribute__((__packed__))  devtree {


 /*! \brief start of devtree  image */
 uint64_t start;

 /*! \brief Where to load device tree  */
 uint64_t load_address;

 /*! \brief Size of device tree */
 uint32_t size;

 /*! \brief IGNORE  */
 uint32_t padding ;
};




/*! \brief Header for firmware objects  
*/

struct __attribute__((__packed__)) sobject {

/*! \brief Version of format */
    uint32_t version;
/*! \brief Object identifier */
    uint32_t type;

/*  brief Offset in bytes to next object or zero  */
/*! \brief IGNORE  */
    uint64_t   next;

/* brief Union pointer to object  */
/*! \brief IGNORE  */
    union   p {
/*! \brief root_key_hash member    */
      struct root_pk_hash *rkh_p;
/*! \brief root_key member    */
      struct root_pk *rk_p;
/*! \brief Boot config  member  */
      struct boot_cfg *bc_p;
/*! \brief Bootloader 2 member  */
      struct boot_ldr_2 *bl2_p;
/*! \brief Ramdisk member in union   */
      struct ramdisk *rp;
/*! \brief SDRAM member     */
      struct sdram *sdp;
/*! \brief Kernel member    */
      struct kernel *kernel_p;
/*! \brief Device Tree member    */
      struct devtree *devtree_p;
/*! \brief ATF Runtime bl31    */
      struct atf_runtime *atf_runtime_p;
/*! \brief ATF Payload bl33    */
      struct atf_payload *atf_payload_p;

/*! \brief IGNORE  */
    uint64_t  force_64;
    } p;

/*brief Start of X509v3 certificate   */
    uint64_t cert_start;
/*! \brief Start of signature   */
    uint64_t  sigstart;

/*! \brief Memory alignment   */
    uint32_t  padding;
/* brief Length of certificate   */
    uint32_t cert_len;
/*! \brief Length of signature   */
    uint32_t  siglen;
/*! \brief Signature type  */
    uint32_t  sigtype;

};


/*! \name Key Management 
     @{
*/

/*! \brief Obtain root of trust 
  
  The actual implementation of this function is platform dependent 

  and has been assigned to a dedicated module. 


 
  @\n 

  
  This function returns the hashed 
  value for the ROOT_PK embedded in a structure 
  
 

  \retval  pointer to struct sobject
  \retval  NULL 
  \return  pointer or  NULL

*/ 

struct sobject *get_root_pk_hash(void); 

/*! \brief Add verification key
  
    
  This function manages the trust anchor by installing either 

  the ROOT_PK_HASH or the asymmetric key ROOT_PK

 
  \param[in] trust pointer to root 
  \param[in] sp pointer to trust data 

  \retval  SPD_OK 
  \retval  SPD_UNSUPPORTED_ROOT_PK_TYPE 
  \retval  SPD_INVALID_POINTER_HSBF_IMAGE

*/ 

int  install_key(struct sobject ** trust, struct sobject * sp); 
/*
  @}
*/ 

/*! \name Verify  
   @{
*/

/*! \brief Verify object in Boot image
  
  This function verifies HSBF element in a sequence starting 

  at address passed as second parameter.  Verification is performed 

  against the trust parameter. 

  Upon success the next pointer in the HSBF is updated to point to 

  next element , or NULL if no more element being found. 


  
 
  \param[in] trust pointer to root 
  \param[in] sp pointer to boot object

  \retval  SPD_OK  
  \retval  SPD_ERROR  
  \retval  SPD_UNSUPPORTED_HASH_TYPE
  \retval  SPD_HASH_LENGTH_MISMATCH
  \retval  SPD_UNSUPPORTED_SIGNATURE_TYPE
  \retval  SPD_MEMORY_ALLOCATION_FAILURE
  \retval  SPD_UNSUPPORTED_ROOT_PK_TYPE
  \retval  SPD_TRUST_EXPECTED_ROOT_PK
  \retval  SPD_EXPECTED_RSA
  \retval  SPD_INVALID_POINTER_HSBF_IMAGE
  \retval  SPD_HSBF_UNSUPPORTED_OBJECT
*/ 

int  verify_element(struct sobject * trust, struct sobject * sp);

/*
  @}
*/ 

#endif
