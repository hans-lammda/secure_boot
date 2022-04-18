/*! \file trust.h
    \brief Trust Anchoring module

    @\n 
   
    This module retrieve the hashed value for the 

    corresponding key being retrieved from untrusted storage. 

    @\n 

    For OTP/Fuse implemenentation this function provides only 

    a shallow interface. 


    @\n 

    For UEFI integration this function enables the ROOT_PK to be 

    embedded in the final image, stage 1 loader.


*/

#ifndef TRUST_H
#define TRUST_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_fip/trunk/bios_fip/fip.h $ $Revision: 26226 $"

#include <spd/spd.h>


/*! \name Anchor
@{
*/



/*! \brief Retrieve ROOT_PK_HASH 

    Returns pointer to structure containing 

    hashed value of public key. 

    \return pointer to sobject  or NULL

*/
struct sobject *get_root_pk_hash(void); 

/*
@}
*/


#endif /* FIP_H */


