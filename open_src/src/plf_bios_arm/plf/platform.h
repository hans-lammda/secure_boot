/*! \file platform.h
    \brief Platform header file for BIOS (x86).

 */

/*
  Copyright (C) 2010 Oniteo
 */

/*
 * This is a minimal definition of functions normally found in runtime such as libC.
 */
#ifndef __ONITEO_PLATFORM_H__
#define __ONITEO_PLATFORM_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/plf/platform.h $ $Revision: 26947 $"

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>


#include <plf/safestr.h>


#ifndef NULL 
#define NULL ((void *)0)
#endif 

#define TRUE 1
#define FALSE 0

#define MAXINT 0x7fffffff

#define size_t unsigned long int


#if 0 
typedef unsigned char      uint8_t;

typedef unsigned short     uint16_t;

typedef unsigned int       uint32_t;

typedef unsigned long long uint64_t;

typedef long long           int64_t;
#endif 



/*! \name Output
    @{
*/


/*! \brief putchar 

    write on character on serial port 
    \param[in] port address
    \param[in] c character to print 
 */
void serial_putchar(unsigned int port, unsigned char c);

/*! \brief print 

    Formatted print 
    \param[in] format A printf-style format string.
 */
int printf(const char* format, ...);

// @}



#ifdef DEBUG_LEVEL
#define trace_dl(level, ...) if ((level) <= DEBUG_LEVEL) (printf("%s:%s:%d: ", __FILE__,__FUNCTION__, __LINE__), printf(__VA_ARGS__))
#define trace_d(level, ...)  if ((level) <= DEBUG_LEVEL) (printf(__VA_ARGS__))
#endif 

/* Trace errors always enabled  */ 
#if 1 
#define trace_el(...) (printf("%s:%s:%d, ERR: ", __FILE__,__FUNCTION__, __LINE__), printf(__VA_ARGS__))
#define trace_e(...) (printf(__VA_ARGS__))
#endif 

/* Trace warnings if enabled */
#ifdef TRACE_WARNING
#define trace_wl(...) (printf("%s:%s:%d, WARN: ", __FILE__,__FUNCTION__, __LINE__), printf(__VA_ARGS__))
#define trace_w(...) (printf(__VA_ARGS__))
#else
#define trace_wl(...) ((void)0)
#define trace_w(...) ((void)0) 
#endif

/* Trace info if enabled */
#ifdef TRACE_INFO
#define trace_il(...) (printf("%s:%s:%d, INFO: ", __FILE__,__FUNCTION__, __LINE__), printf(__VA_ARGS__))
#define trace_i(...) (printf(__VA_ARGS__))
#else
#define trace_il(...) ((void)0) /* nothing */
#define trace_i(...)  ((void)0)/* nothing */
#endif



/* Empty defines by default */
#ifndef trace_e
#define trace_e(...)  (void)0/* nothing */
#endif
#ifndef trace_el
#define trace_el(...)  (void)0/* nothing */
#endif
#ifndef trace_d
#define trace_d(level, ...)  (void)0/* nothing */
#endif
#ifndef trace_dl
#define trace_dl(level, ...) (void)0 /* nothing */
#endif


/* Macro for bitwise assign operation with mask */ 
#define SET_VALUE(p,v,m) (((p) & ~(m)) | (v))


#endif

