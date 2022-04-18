/* Copyright Oniteo 2008 */ 

/*! \file sha1.h
    \brief SHA-1 Hash functions
    
    Minimal implementation of SHA-1 hash functions used for RSA 
    signature verification.

*/

/*! \name ignore 
 @{
 */ 
#ifndef SHA_H_DEF
#define SHA_H_DEF "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/sha1.h $ $Revision: 24339 $"

/********************* Start of code from Putty *********************/

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
 
 //@}
 
/*! \name Hash

  @{
*/

/*! \brief SHA-1 hash function
 
     Uses the SHA-1 algorithm on the input data. The output hash will 
     have a length of 20 bytes.

     \param[in] p Pointer to data to be hashed
     \param[in] len Length of data to be hashed
     \param[out] output Hashed data

*/ 
void SHA_Simple(void *p, int len, unsigned char *output);

#define SIGNATURE_LENGTH_BYTES (20)
// @}
/*********************** End of code from Putty ************************/

#endif 
