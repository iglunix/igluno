/*
 * $Id: getkey.h 530 2007-04-19 02:37:54Z mikes@u.washington.edu $
 *
 * ========================================================================
 * Copyright 2006 University of Washington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * ========================================================================
 */


#ifndef igluno_OSDEP_GETKEY_INCLUDED
#define igluno_OSDEP_GETKEY_INCLUDED


#include <general.h>


/* exported prototypes */
UCS	      GetKey(void);
void	      kpinsert(char *, int, int);
#if	TYPEAH
int	      typahead(void);
#endif /* TYPEAH */
#ifndef _WINDOWS
UCS           kbseq(int (*getcfunc)(int (*recorder)(int ), void (*bail_handler)(void )),
		    int (*recorder)(int), void (*bail_handler)(void), void *, UCS *);
void          kbdestroy(KBESC_T *);
#endif

#endif /* igluno_OSDEP_GETKEY_INCLUDED */
