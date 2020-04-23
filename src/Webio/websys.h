/* websys.h
 *
 * Part of the Webio Open Source lightweight web server.
 *
 * Copyright (c) 2007 by John Bartas
 * Portions Copyright (C) 2010 Renesas Electronics Corporation.
 * All rights reserved.
 *
 * Use license: Modified from standard BSD license.
 * 
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation, advertising 
 * materials, Web server pages, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by John Bartas. The name "John Bartas" may not be used to 
 * endorse or promote products derived from this software without 
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _WEBSYS_H_
#define _WEBSYS_H_    1

/* This file contains definitions intended for modification during porting */

/*********** Optional webio features - comment out to remove ***************/

#define WI_STDFILES  1     /* Use system "fopen" files */
#define WI_EMBFILES  1     /* Use embedded FS */
#define WI_THREAD    1     /* Drive webio with a thread rather than polling */


/*********** Webio sizes and limits ***************/

#define WI_RXBUFSIZE    1536  /* rxbuf[] total size */
#define WI_TXBUFSIZE    1400  /* txbuf[] section size */
#define WI_MAXURLSIZE   512   /* URL buffer size  */
#define WI_FSBUFSIZE    (1024 * 4) /* file read buffer size */
#define WI_PERSISTTMO   300   /* persistent connection timeout */
#define WI_LANG_BUFFER  64    /* Buffer for language string in get request */

/*********** OS portability ***************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compiler_settings.h"
#include "command.h"
#include "strstri.h"

/* Map Webio heap routine to system's */
#define WI_MALLOC(size)     R_OS_AllocMem(size, R_REGION_UNCACHED_RAM)
#define WI_FREE(mem)        R_OS_FreeMem(mem)

/* Map Webio string routines to system's */
extern  int strnicmp(const char *pszS1, const char *pszS2, size_t stLength);
extern  int stricmp(const char *pszS1, const char *pszS2);
extern  int pathCompare(const char *pszS1, const char *pszS2);

/* The types used by Webio */
typedef unsigned char u_char;
typedef unsigned long u_long;

/* Variable cticks replaced with function call */
extern u_long cticks(void);

/* Define TPS (Ticks Per Second). If this is contained an another project with 
 * TPS defined (eg Buster) then use the external definition.
 */
#ifndef TPS
#define TPS 1000
#endif /* no TPS */

/*********** Network portability ***************/

#include "lwip/sockets.h"
typedef int socktype;
/* Map Webio socket routines to system's */
#define accept(a,b,c)           lwip_accept(a,b,c)
#define bind(a,b,c)             lwip_bind(a,b,c)
#define shutdown(a,b)           lwip_shutdown(a,b)
#define closesocket(s)          lwip_close(s)
#define connect(a,b,c)          lwip_connect(a,b,c)
#define getsockname(a,b,c)      lwip_getsockname(a,b,c)
#define getpeername(a,b,c)      lwip_getpeername(a,b,c)
#define setsockopt(a,b,c,d,e)   lwip_setsockopt(a,b,c,d,e)
#define getsockopt(a,b,c,d,e)   lwip_getsockopt(a,b,c,d,e)
#define listen(a,b)             lwip_listen(a,b)
#define recv(a,b,c,d)           lwip_recv(a,b,c,d)
#define recvfrom(a,b,c,d,e,f)   lwip_recvfrom(a,b,c,d,e,f)
#define send(a,b,c,d)           lwip_send(a,b,c,d)
#define sendto(a,b,c,d,e,f)     lwip_sendto(a,b,c,d,e,f)
#define socket(a,b,c)           lwip_socket(a,b,c)
#define select(a,b,c,d,e)       lwip_select(a,b,c,d,e)
#define ioctlsocket(a,b,c)      lwip_ioctl(a,b,c)

/*********** File system mapping ***************/

#define  USE_EMFILES 1
#define  USE_SYSFILES 1

#include <stdio.h>

/*********** debug support **************/

#include <stdarg.h>
#include "Trace.h"
#ifndef _DEBUG_WEBIO_
#undef  TRACE
#define TRACE(_x_)
#endif
#define _ANSI_IO_
extern  void wsBreakPoint(void);
#define  dtrap()  wsBreakPoint()


#ifndef USE_ARG
#define USE_ARG(c) (c=c)
#endif  /* USE_ARG */

extern  void wsPanic(char *pszMessage);

#define panic wsPanic

extern u_long wi_totalblocks;

#endif   /* _WEBSYS_H_ */


