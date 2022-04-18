/*! \file stdinclude.h
    \brief Generic ANSI Definitions
    
    
    In implementations of the platform library, this file must be included 
    by the platform-specific platform.h header files.

    The include order for applications is:
      <plf/platform.h>
      <plf/memory.h>
      ...
 */
  
#ifndef __PMILL_STDINCLUDE_H__
#define __PMILL_STDINCLUDE_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/stdinclude.h $ $Revision: 17763 $"



/*! \def LF 
    ASCII value for linefeed character.
*/
#define LF 10
/*! \def CR 
    Ascii value for carriage return character.
*/
#define CR 13
/*! \def HTML_NEWL
    Newline/carriage return HTML code.
*/
#define HTML_NEWL "<BR/>"


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include <ctype.h>
#include <math.h>




/* ------------------------------------------------------------------
 * The following things must be defined by the platform.h files, 
 * unless the type or the definition already exists in the standard
 * base of the platform.
 * ------------------------------------------------------------------
 */
 

#endif
