/* $Id
 *	Copyright (c) 2001-2002 ADTEC Ltd.
 *	All rights reserved
 *
 *	This is unpublished proprietary
 *	source code of ADTEC Ltd.
 *	The copyright notice above does not
 *	evidence any actual or intended
 * 	publication of such source code.
 *
 * 	NOTICE: UNAUTHORIZED DISTRIBUTION, ADAPTATION OR USE MAY BE
 *	SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
 *
 *
 *	FILE:        wrapunix.h
 *	DESCRIPTION: The head file contains all unix header files needed by
 *                   eSWITCH system. OS verion defference should been 
 *                   eliminated by lots of macros that generated by configure.
 *                                                           
 *	AUTHOR:
 *              WangNan 03/20/2001 - Developed for eSWITCH system
 *	MODIFIED:
 *
 */

#ifndef __WRAPUNIX_H
#define __WRAPUNIX_H

#include "config.h"

#if defined( HAVE_SYS_TYPES_H )
#include <sys/types.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STDIO_H )
#include <stdio.h>
#endif

#if defined( HAVE_STDDEF_H )
#include <stddef.h>
#endif

#if defined( HAVE_STDARG_H )
#include <stdarg.h>
#endif

#if defined( HAVE_FCNTL_H )
#include <fcntl.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_SIGNAL_H )
#include <signal.h>
#endif

#if defined( HAVE_SETJMP_H )
#include <setjmp.h>
#endif

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_SYS_TIMES_H )
    #if defined( TIME_WITH_SYS_TIME )
        #include <sys/times.h>
        #if defined( HAVE_TIME_H )
            #include <time.h>
        #endif
    #endif
#else
    #if defined( HAVE_TIME_H )
    #include <time.h>
    #endif
#endif

#if defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#endif

#if defined( HAVE_SYS_TIMEB_H )
#include <sys/timeb.h>
#endif

#if defined( HAVE_SEARCH_H )
#include <search.h>
#endif

#if defined( HAVE_SYS_WAIT_H )
#include <sys/wait.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_CTYPE_H )
#include <ctype.h>
#endif

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( HAVE_MEMORY_H )
#include <memory.h>
#endif

#if defined( HAVE_MALLOC_H )
#include <malloc.h>
#endif

#if defined( HAVE_ERRNO_H )
#include <errno.h>
#endif

#if defined( HAVE_SYS_IPC_H )
#include <sys/ipc.h>
#endif

#if defined( HAVE_SYS_SHM_H )
#include <sys/shm.h>
#endif

#if defined( HAVE_SYS_MSG_H )
#include <sys/msg.h>
#endif

#if defined( HAVE_SYS_SEM_H )
#include <sys/sem.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#ifdef __INTERIX
extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;
#endif
#endif

#if defined( HAVE_LIMITS_H )
#include <limits.h>
#ifdef __INTERIX
#include <sys/resource.h>
#endif
#endif

#if defined( HAVE_VALUES_H )
#include <values.h>
#endif

#if defined( HAVE_SYS_PARAM_H )
#include <sys/param.h>
#endif

#if defined( HAVE_SYS_SOCKET_H )
#include <sys/socket.h>
#endif

#if defined( HAVE_SYS_SELECT_H )
#include <sys/select.h>
#endif

#if defined( HAVE_NETINET_IN_H )
#include <netinet/in.h>
#endif

#if defined( HAVE_ARPA_INET_H )
#include <arpa/inet.h>
#endif

#if defined( HAVE_NET_IF_H )
#include <net/if.h>
#endif

#if defined( HAVE_NETDB_H )
#include <netdb.h>
#endif

#if defined( HAVE_SYS_UN_H )
#include <sys/un.h>
#endif

#if defined( HAVE_SYS_UIO_H )
#include <sys/uio.h>
#endif

#if defined( HAVE_SYS_LOCK_H )
#include <sys/lock.h>
#endif


#if defined( HAVE_MATH_H )
#include <math.h>
#endif

#if defined( HAVE_NL_TYPES_H )
#include <nl_types.h>
#endif

#if !defined( IPC_ALLOC )
#define IPC_ALLOC       0
#endif

#if defined( HAVE_REGEX_H )
#include <regex.h>
#endif

#if defined( HAVE_PTHREAD_H )
#include <pthread.h>
#endif

#if defined( HAVE_DIRENT_H )
#include <dirent.h>
#endif

#if defined( HAVE_POLL_H )
#include <poll.h>
#endif

#if !defined( HAVE_SEM_SEMUN )
#ifndef __INTERIX
       /* X/OPEN semun */
       union semun {
               int val;                    /* value for SETVAL */
               struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
               unsigned short int *array;  /* array for GETALL, SETALL */
               struct seminfo *__buf;      /* buffer for IPC_INFO */
       };
#endif
#endif

#ifdef HAVE_MSGHDR_MSG_CONTROL
    #ifndef CMSG_SPACE
    #define CMSG_SPACE(size)    (sizeof(struct cmsghdr) + (size))
    #endif

    #ifndef CMSG_LEN
    #define CMSG_LEN(size)      (sizeof(struct cmsghdr) + (size))
    #endif
#endif

#ifndef INADDR_NONE
#define INADDR_NONE     0xffffffff
#endif

#ifndef HAVE_FUNC_MAX
    #ifndef max
    #define max(a,b)            ((a)<(b) ? (b) : (a))
    #endif
#endif

#ifndef HAVE_FUNC_MIN
    #ifndef min
    #define min(a,b)            ((a)>(b) ? (b) : (a))
    #endif
#endif

#ifndef HAVE_FUNC_ABS
    #ifndef abs
    #define abs(x)                      ((x)>0 ? (x) : -(x))
    #endif
#endif




#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#define Trace(type,var)  fprintf( stderr, "%s.%d:*** %s %d ***." #var "= %" #type "\n", getenv("_"), getpid(),__FILE__, __LINE__, var );

#if defined( __STDC__ ) || defined( __cplusplus )
    #ifndef _
        #define _(a)    a
    #endif
#else
    #ifndef _
        #define _(a)    ()
    #endif
#endif


#define _FL_ __FILE__, __LINE__

#endif
