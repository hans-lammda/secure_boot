/*! \file bignum.h
    \brief Bignum routines for RSA Signature Verification
 
 
    This is a minimal implementation of a Bignum format for RSA 
    signature verification. The Bignum format is an array of "unsigned 
    short". The first element of the array counts the remaining 
    elements. The remaining elements express the actual number, base 
    2^16, least significant digit first (so it's trivial to extract the 
    bit with value 2^n for any n). All Bignums in this module are 
    positive. Negative numbers must be dealt with outside it. 
    INVARIANT: the most significant word of any Bignum must be nonzero.
    
*/ 

/*! \name ignore */ 
#ifndef BIGNUM_H_DEF
#define BIGNUM_H_DEF "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/bignum.h $ $Revision: 17411 $"

/********************* Start of code from Putty ***********************/

/* The crypto code in this file is shamelessly stolen from Putty which
 * is licensed under the following terms:
 *
 * PuTTY is copyright 1997-2001 Simon Tatham.
 *
 * Portions copyright Robert de Bath, Joris van Rantwijk, Delian
 * Delchev, Andreas Schultz, Jeroen Massar, Wez Furlong, Nicolas
 * Barry, Justin Bradford, and CORE SDI S.A.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
 /*************************** Bignum **********************************/
// @}

/*! \brief Bignum
*/ 
typedef unsigned short *Bignum;


/*! \name Memory

  @{
*/
/*! \brief Allocates memory for a Bignum
   
     Allocates memory for a Bignum
     
     \param[in] length length of the Bignum
     \return the newly allocated Bignum
 */
Bignum newbn(int length);


/*! \brief Deallocates memory for a Bignum
    
     Deallocates memory for a Bignum
    
     \param[in] b Bignum to be deallocated
 */
void freebn(Bignum b);

// @}

/*! \name Convertion


  @{
*/

/*! \brief Create a Bignum from an array of unsigned char
    
     This function takes an array of unsigned char and converts it to a 
     Bignum. It does not deallocate the input data.
    
     \param[in] data data array
     \param[in] nbytes length of data array in bytes
     \return a Bignum converted from the data array
 */
Bignum bignum_from_bytes(const unsigned char *data, int nbytes);


/*! \brief Create a data array of unsigned char from a Bignum.
    
     This function takes a Bignum and converts it to an array of 
     unsigned char. It does not deallocate the input Bignum.
    
     \param[in] bn Bignum to be converted
     \param[out] nbytes length of data array in bytes
     \return a data array converted from the Bignum 
 */
unsigned char *bytes_from_bignum(Bignum bn, unsigned short *nbytes);

// @}

/*! \name Math
  
  
  @{
 */
/*! \brief Computes (base ^ exp) % mod.
 
     Computes (base ^ exp) % mod. The base MUST be smaller than the 
     modulus. The most significant word of mod MUST be non-zero. We 
     assume that the result array is the same size as the mod array.
     
     \param[in] base base
     \param[in] exp exponent
     \param[in] mod modulus
     \return a Bignum with the result from (base ^ exp) % mod
 */
Bignum modpow(Bignum base, Bignum exp, Bignum mod);

/*********************** End of code from Putty ***********************/


// @}


#endif 
