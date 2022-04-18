/*! \file platform.h
    \brief Platform header file for Linux Standard Base (x86).

 */

/*
  Copyright (C) 2007-2010 Oniteo
 */

#ifndef __ONITEO_PLATFORM_H__
#define __ONITEO_PLATFORM_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/plf/platform.h $ $Revision: 18164 $"

#include <plf/stdinclude.h>


#define PLATFORM_UNIX
#define PLATFORM_LSB
#define PLATFORM_LSB_X86


#include <stdarg.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>


#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include <resolv.h>
int iopl(int level);

#include <linux/reboot.h>

extern char** environ;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define sockerror errno
#define SOCKERROR errno
#define FILEPATH_SEP '/'

#ifndef SOCKET
#define SOCKET int
#endif

#define stricmp strcasecmp
#define strnicmp strncasecmp

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define FILEHANDLE int

#define SOEXT ".so"

typedef void* HMODULE;
typedef void* FARPROC;


#ifdef DEBUG_LEVEL

#define trace_dl(level, ...)  if ((level) <= DEBUG_LEVEL) \
    (printf("%s:%s:%d ",__FILE__,__FUNCTION__, __LINE__), \
    printf(__VA_ARGS__))

# define trace_d(level, ...)  if ((level) <= DEBUG_LEVEL) \
    (printf("%s:%d: ", __FUNCTION__, __LINE__), \
    printf(__VA_ARGS__))

#endif /* ifdef DEBUG_LEVEL */

/* Trace errors always enabled */
# define trace_el(...) (printf("%s:%s:%d, ERR: ", __FILE__, __FUNCTION__, __LINE__), printf(__VA_ARGS__))
# define trace_e(...) (printf(__VA_ARGS__))

/* Trace warnings if enabled */
#ifdef TRACE_WARNING
#define trace_wl(...) (printf("%s:%s:%d, WARN: ", __FILE__, __FUNCTION__, __LINE__), printf(__VA_ARGS__))
#define trace_w(...) (printf(__VA_ARGS__))
#else
#define trace_wl(...)  /* nothing */
#define trace_w(...)  /* nothing */
#endif

/* Trace info if enabled */
#ifdef TRACE_INFO
#define trace_il(...) (printf("%s:%s:%d, INFO: ", __FILE__, __FUNCTION__, __LINE__), printf(__VA_ARGS__))
#define trace_i(...) (printf(__VA_ARGS__))
#else
#define trace_il(...)  /* nothing */
#define trace_i(...)  /* nothing */
#endif


/* Empty defines by default */
#ifndef trace_d
#define trace_d(level, ...)  /* nothing */
#endif

#ifndef trace_dl
#define trace_dl(level, ...)  /* nothing */
#endif


#endif


