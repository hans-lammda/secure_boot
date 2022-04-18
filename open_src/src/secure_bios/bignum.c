#include <plf/platform.h>
#include <plf/memory.h>
#include "bignum.h"

#define MEM_ALIGN 16

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
 
 /*************************** Bignum ***********************************/
 /*
 * Bignum routines for RSA and DH and stuff.
 */
 
 /*
 * The Bignum format is an array of `unsigned short'. The first
 * element of the array counts the remaining elements. The
 * remaining elements express the actual number, base 2^16, _least_
 * significant digit first. (So it's trivial to extract the bit
 * with value 2^n for any n.)
 *
 * All Bignums in this module are positive. Negative numbers must
 * be dealt with outside it.
 *
 * INVARIANT: the most significant word of any Bignum must be
 * nonzero.
 */
char secure_bios_bignum_header[]  __attribute__ ((section (".keywords")))  = BIGNUM_H_DEF;
char secure_bios_bignum_code[]    __attribute__ ((section (".keywords")))  = \
     "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/secure_bios/branches/haspoc/hikey/secure_bios/pki/bignum.c $ $Revision: 26005 $";


#define BIGNUM_INT_BITS  (32)
#define BIGNUM_INT_BYTES (BIGNUM_INT_BITS / 8)

#define BITS_PER_BYTE (8)

/*
 * Compute c = a * b.
 * Input is in the first len words of a and b.
 * Result is returned in the first 2*len words of c.
 */
static void internal_mul(unsigned short *a, unsigned short *b,
             unsigned short *c, int len)
{
    int i, j;
    unsigned long ai, t;

    for (j = 0; j < 2 * len; j++)
    c[j] = 0;

    for (i = len - 1; i >= 0; i--) {
    ai = a[i];
    t = 0;
    for (j = len - 1; j >= 0; j--) {
        t += ai * (unsigned long) b[j];
        t += (unsigned long) c[i + j + 1];
        c[i + j + 1] = (unsigned short) t;
        t = t >> 16;
    }
    c[i] = (unsigned short) t;
    }
}

static void internal_add_shifted(unsigned short *number,
                unsigned n, int shift)
{
    int word = 1 + (shift / 16);
    int bshift = shift % 16;
    unsigned long addend;

    addend = n << bshift;

    while (addend) {
    addend += number[word];
    number[word] = (unsigned short) addend & 0xFFFF;
    addend >>= 16;
    word++;
    }
}

/*
 * Compute a = a % m.
 * Input in first alen words of a and first mlen words of m.
 * Output in first alen words of a
 * (of which first alen-mlen words will be zero).
 * The MSW of m MUST have its high bit set.
 * Quotient is accumulated in the `quotient' array, which is a Bignum
 * rather than the internal bigendian format. Quotient parts are shifted
 * left by `qshift' before adding into quot.
 */
static void internal_mod(unsigned short *a, int alen,
            unsigned short *m, int mlen,
            unsigned short *quot, int qshift)
{
    unsigned short m0, m1;
    unsigned int h;
    int i, k;

    m0 = m[0];
    if (mlen > 1)
    m1 = m[1];
    else
    m1 = 0;

    for (i = 0; i <= alen - mlen; i++) {
    unsigned long t;
    unsigned int q, r, c, ai1;

    if (i == 0) {
        h = 0;
    } else {
        h = a[i - 1];
        a[i - 1] = 0;
    }

    if (i == alen - 1)
        ai1 = 0;
    else
        ai1 = a[i + 1];

    /* Find q = h:a[i] / m0 */
    t = ((unsigned long) h << 16) + a[i];
    q = t / m0;
    r = t % m0;

    /* Refine our estimate of q by looking at
        h:a[i]:a[i+1] / m0:m1 */
    t = (long) m1 *(long) q;
    if (t > ((unsigned long) r << 16) + ai1) {
        q--;
        t -= m1;
        r = (r + m0) & 0xffff;     /* overflow? */
        if (r >= (unsigned long) m0 &&
        t > ((unsigned long) r << 16) + ai1) q--;
    }

    /* Subtract q * m from a[i...] */
    c = 0;
    for (k = mlen - 1; k >= 0; k--) {
        t = (long) q *(long) m[k];
        t += c;
        c = t >> 16;
        if ((unsigned short) t > a[i + k])
        c++;
        a[i + k] -= (unsigned short) t;
    }

    /* Add back m in case of borrow */
    if (c != h) {
        t = 0;
        for (k = mlen - 1; k >= 0; k--) {
        t += m[k];
        t += a[i + k];
        a[i + k] = (unsigned short) t;
        t = t >> 16;
        }
        q--;
    }
    if (quot)
        internal_add_shifted(quot, q, qshift + 16 * (alen - mlen - i));
    }
}


/*! \brief Allocates memory for a Bignum
   
     Allocates memory for a Bignum
     
     \param[in] length length of the Bignum
     \return the newly allocated Bignum
 */
Bignum newbn(int length)
{
    Bignum b = (Bignum)NMemAlloc_align((length + 1) * sizeof(unsigned short),MEM_ALIGN); 
    if (!b)
    return 0;   /* ABORT */
    NMemSet(b, 0, (length + 1) * sizeof(*b));
    b[0] = length;
    return b;
}


/*! \brief Deallocates memory for a Bignum
    
     Deallocates memory for a Bignum
    
     \param[in] b Bignum to be deallocated
 */
void freebn(Bignum b)
{
    /*
     * Burn the evidence, just in case.
     */
    NMemSet(b, 0, sizeof(b[0]) * (b[0] + 1));
    NMemFree(b); 
}

/*! \brief Create a Bignum from an array of unsigned char
    
     This function takes an array of unsigned char and converts it to a 
     Bignum. It does not deallocate the input data.
    
     \param[in] data data array
     \param[in] nbytes length of data array in bytes
     \return a Bignum converted from the data array
 */
Bignum bignum_from_bytes(const unsigned char *data, int nbytes)
{
    Bignum result;
    int w, i;

    w = (nbytes + 1) / 2;	       /* bytes -> words */

    result = newbn(w);
    for (i = 1; i <= w; i++)
    result[i] = 0;
    for (i = nbytes; i--;) {
    unsigned char byte = *data++;
    if (i & 1)
        result[1 + i / 2] |= byte << 8;
    else
        result[1 + i / 2] |= byte;
    }

    while (result[0] > 1 && result[result[0]] == 0)
    result[0]--;
    return result;
}


/*! \brief Create a data array of unsigned char from a Bignum.
    
     This function takes a Bignum and converts it to an array of 
     unsigned char. It does not deallocate the input Bignum.
    
     \param[in] bn Bignum to be converted
     \param[out] nbytes length of data array in bytes
     \return a data array converted from the Bignum 
 */
unsigned char *bytes_from_bignum(Bignum bn, unsigned short *nbytes)
{
    unsigned short i,j;
    unsigned short bn_len;
    unsigned char *data;

    //First cell of Bignum contains its length (in [unsigned short])
    bn_len = bn[0];
    
    //Each bn cell is two bytes (unsigned short)
    if( bn[bn_len] < 0x0100 )
        *nbytes = bn_len*2-1;
    else
        *nbytes = bn_len*2;
    
    //Allocate memory for data
    data = NMemAlloc_align(*nbytes,MEM_ALIGN);
    
    //The Bignum format is an array of `unsigned short'. The first
    //element of the array counts the remaining elements. The
    //remaining elements express the actual number, base 2^16, _least_
    //significant digit first.
    for(i=1, j=bn_len; i<*nbytes; i=i+2, j--)
    {
        //If MSB is less than 0x0100
        if( i == 1 && bn[bn_len] < 0x0100 )
        {
            data[i-1] = bn[j] & 0x00FF;
            i--;
        }
        else
        {
            data[i-1] = bn[j] >> 8;
            data[i]   = bn[j] & 0x00FF;
        }
    }
    
    return data;
}


/*! \brief Computes (base ^ exp) % mod.
 
     Computes (base ^ exp) % mod. The base MUST be smaller than the 
     modulus. The most significant word of mod MUST be non-zero. We 
     assume that the result array is the same size as the mod array.
     
     \param[in] base base
     \param[in] exp exponent
     \param[in] mod modulus
     \return a Bignum with the result from (base ^ exp) % mod
 */
Bignum modpow(Bignum base, Bignum exp, Bignum mod)
{
    unsigned short *a, *b, *n, *m, temp;
    int mshift;
    int mlen, i, j;
    Bignum result;

    /* Allocate m of size mlen, copy mod to m */
    /* We use big endian internally */
    mlen = mod[0];
    m = (unsigned short*)NMemAlloc_align(mlen * sizeof(unsigned short),MEM_ALIGN); //m = (unsigned short*)firmware_malloc(mlen * sizeof(unsigned short),MEM_ALIGN);
    for (j = 0; j < mlen; j++) {
    temp = mod[mod[0] - j];
    m[j] = temp;
    }

    /* Shift m left to make msb bit set */
    for (mshift = 0; mshift < 15; mshift++)
    if ((m[0] << mshift) & 0x8000)
        break;
    if (mshift) {
    for (i = 0; i < mlen - 1; i++)
        m[i] = (m[i] << mshift) | (m[i + 1] >> (16 - mshift));
    m[mlen - 1] = m[mlen - 1] << mshift;
    }

    /* Allocate n of size mlen, copy base to n */
    n = (unsigned short*)NMemAlloc_align(mlen * sizeof(unsigned short),MEM_ALIGN); //n = (unsigned short*)firmware_malloc(mlen * sizeof(unsigned short));
    i = mlen - base[0];
    for (j = 0; j < i; j++)
    n[j] = 0;
    for (j = 0; j < base[0]; j++)
    n[i + j] = base[base[0] - j];

    /* Allocate a and b of size 2*mlen. Set a = 1 */
    a = (unsigned short*)NMemAlloc_align(2 * mlen * sizeof(unsigned short),MEM_ALIGN); //a = (unsigned short*)firmware_malloc(2 * mlen * sizeof(unsigned short));
    b = (unsigned short*)NMemAlloc_align(2 * mlen * sizeof(unsigned short),MEM_ALIGN); //b = (unsigned short*)firmware_malloc(2 * mlen * sizeof(unsigned short));
    for (i = 0; i < 2 * mlen; i++)
    a[i] = 0;
    a[2 * mlen - 1] = 1;

    /* Skip leading zero bits of exp. */
    i = 0;
    j = 15;
    while (i < exp[0] && (exp[exp[0] - i] & (1 << j)) == 0) {
    j--;
    if (j < 0) {
        i++;
        j = 15;
    }
    }

    /* Main computation */
    while (i < exp[0]) {
    while (j >= 0) {
        internal_mul(a + mlen, a + mlen, b, mlen);
        internal_mod(b, mlen * 2, m, mlen, NULL, 0);
        if ((exp[exp[0] - i] & (1 << j)) != 0) {
        internal_mul(b + mlen, n, a, mlen);
        internal_mod(a, mlen * 2, m, mlen, NULL, 0);
        } else {
        unsigned short *t;
        t = a;
        a = b;
        b = t;
        }
        j--;
    }
    i++;
    j = 15;
    }

    /* Fixup result in case the modulus was shifted */
    if (mshift) {
    for (i = mlen - 1; i < 2 * mlen - 1; i++)
        a[i] = (a[i] << mshift) | (a[i + 1] >> (16 - mshift));
    a[2 * mlen - 1] = a[2 * mlen - 1] << mshift;
    internal_mod(a, mlen * 2, m, mlen, NULL, 0);
    for (i = 2 * mlen - 1; i >= mlen; i--)
        a[i] = (a[i] >> mshift) | (a[i - 1] << (16 - mshift));
    }

    /* Copy result to buffer */
    result = newbn(mod[0]);
    for (i = 0; i < mlen; i++)
    result[result[0] - i] = a[i + mlen];
    while (result[0] > 1 && result[result[0]] == 0)
    result[0]--;

    /* Free temporary arrays */
    for (i = 0; i < 2 * mlen; i++)
    a[i] = 0;
    NMemFree(a); 
    for (i = 0; i < 2 * mlen; i++)
    b[i] = 0;
    NMemFree(b); 
    for (i = 0; i < mlen; i++)
    m[i] = 0;
    NMemFree(m); 
    for (i = 0; i < mlen; i++)
    n[i] = 0;
    NMemFree(n); 

    return result;
}

/*********************** End of code from Putty ************************/
