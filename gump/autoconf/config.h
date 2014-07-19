/* config.h.  Generated from config.h.in by configure.  */
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
 *	FILE:           sysautocf.h.in
 *	DESCRIPTION:    OS specified MACROS defined here and maintained
 *                      by configure
 *	AUTHOR:		
 *		WangNan   03/20/2001 - Developed for eSWITCH system
 *	MODIFIED:
 *		configure 
 *
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* #undef CPU_VENDOR_OS */

/* Define if we have the header file sys/types.h */
#define HAVE_SYS_TYPES_H 1

/* Define if we have the header file stdlib.h */
#define HAVE_STDLIB_H 1

/* Define if we have the header file stdio.h */
#define HAVE_STDIO_H 1

/* Define if we have the header file stddef.h */
#define HAVE_STDDEF_H 1

/* Define if we have the header file stdarg.h */
#define HAVE_STDARG_H 1

/* Define if we have the header file fcntl.h */
#define HAVE_FCNTL_H 1

/* Define if we have the header file string.h */
#define HAVE_STRING_H 1

/* Define if we have the header file signal.h */
#define HAVE_SIGNAL_H 1

/* Define if we have the header file setjmp.h */
#define HAVE_SETJMP_H 1

/* Define if we have the header file sys/ioctl.h */
#define HAVE_SYS_IOCTL_H 1

/* Define if we have the header file sys/times.h */
#define HAVE_SYS_TIMES_H 1

/* Define if we have the header file sys/timeb.h */
#define HAVE_SYS_TIMEB_H 1

/* Define if we have the header file time.h */
#define HAVE_TIME_H 1

/* Define if we have the header file sys/time.h */
#define HAVE_SYS_TIME_H 1

/* Define if we have the header file search.h */
#define HAVE_SEARCH_H 1

/* Define if we have the header file sys/wait.h */
#define HAVE_SYS_WAIT_H 1

/* Define if we have the header file string.h */
#define HAVE_STRING_H 1

/* Define if we have the header file ctype.h */
#define HAVE_CTYPE_H 1

/* Define if we have the header file sys/stat.h */
#define HAVE_SYS_STAT_H 1

/* Define if we have the header file memory.h */
#define HAVE_MEMORY_H 1

/* Define if we have the header file malloc.h */
/* #undef HAVE_MALLOC_H */

/* Define if we have the header file errno.h */
#define HAVE_ERRNO_H 1

/* Define if we have the header file sys/ipc.h */
#define HAVE_SYS_IPC_H 1

/* Define if we have the header file sys/shm.h */
#define HAVE_SYS_SHM_H 1

/* Define if we have the header file sys/msg.h */
#define HAVE_SYS_MSG_H 1

/* Define if we have the header file sys/sem.h */
#define HAVE_SYS_SEM_H 1

/* Define if we have the header file unistd.h */
#define HAVE_UNISTD_H 1

/* Define if we have the header file limits.h */
#define HAVE_LIMITS_H 1

/* Define if we have the header file values.h */
/* #undef HAVE_VALUES_H */

/* Define if we have the header file sys/param.h */
/* #undef HAVE_SYS_PARAM_H */

/* Define if we have the header file sys/socket.h */
#define HAVE_SYS_SOCKET_H 1

/* Define if we have the header file sys/select.h */
#define HAVE_SYS_SELECT_H 1

/* Define if we have the header file netinet/in.h */
#define HAVE_NETINET_IN_H 1

/* Define if we have the header file net/if.h */
/* #undef HAVE_NET_IF_H */

/* Define if we have the header file arpa/inet.h */
#define HAVE_ARPA_INET_H 1

/* Define if we have the header file netdb.h */
#define HAVE_NETDB_H 1

/* Define if we have the header file sys/un.h */
/* #undef HAVE_SYS_UN_H */

/* Define if we have the header file sys/uio.h */
#define HAVE_SYS_UIO_H 1

/* Define if we have the header file sys/lock.h */
/* #undef HAVE_SYS_LOCK_H */

/* Define if we have the header file math.h */
#define HAVE_MATH_H 1

/* Define if we have the header file nl_types.h */
#define HAVE_NL_TYPES_H 1

/* Define if we can include <time.h> with <sys/time.h> */
#define TIME_WITH_SYS_TIME 1

/* Define if we have the header file regex.h */
#define HAVE_REGEX_H 1

/* Define if we have semun union */
/* #undef HAVE_SEM_SEMUN */

/* Define if we have msg_control and msg_controllen in struct msghdr */
/* #undef HAVE_MSGHDR_MSG_CONTROL */

/* Define if we have restartable system-calls */
/* #undef HAVE_RESTARTABLE_SYSCALLS */

/* Define if semctl(SEM_SETALL) can update semid_ds.sem_otim */
/* #undef SEMCTL_CHG_SEM_OTIME */

/* define if we have const */
/* #undef const */

/* define if we have max() */
/* #undef HAVE_FUNC_MAX */

/* define if we have min() */
/* #undef HAVE_FUNC_MIN */

/* define if we have abs() */
/* #undef HAVE_FUNC_ABS */

/* define if we have snprintf */
/* #undef HAVE_NOT_SNPRINTF */

/* define if SIOGCIFCONF ioctl command supported */
/* #undef HAVE_IFACE_IFCONF */

/* define if AIX style SIOGCIFCONF ioctl supported */
/* #undef HAVE_IFACE_AIX */

/* define if stream based ioctl method supported */
/* #undef HAVE_IFACE_IFREQ */

/* definitions of arg type for select() */
#define SELECT_ARG1 int
#define SELECT_ARG234 fd_set *
#define SELECT_ARG5 struct timeval *

/* define the length of a string whitch is converted from 
   max double value (include space for sign) */
/* #undef MAX_DOUBLE_STR_LEN */

/* #undef MAX_LONG_STR_LEN */

/* define if we find the atof() have problem to convert 
   "10111.0199999999999990" */
/* #undef HAVE_BUG_ATOF */

/* define the maximum length of command line. */
/* #undef ES_ARG_MAX */

/* define the maximum length of name. */
/* #undef ES_NAME_MAX */

/* define the maximum length of path. */
/* #undef ES_PATH_MAX */

#define HAVE_PTHREAD_H 1

#define HAVE_DIRENT_H 1

#define HAVE_POLL_H 1

#endif
