/*
 * $Id: tty.h 165 2006-10-04 01:09:47Z jpf@u.washington.edu $
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


#ifndef igluno_OSDEP_TTY_INCLUDED
#define igluno_OSDEP_TTY_INCLUDED


#include <general.h>


/* exported prototypes */
int	ttopen(void);
int	ttclose(void);
int	ttflush(void);
void	ttresize(void);
#ifndef _WINDOWS
int	ttgetc(int, int (*recorder)(int), void (*bail_handler)(void));
int	simple_ttgetc(int (*recorder)(int), void (*bail_handler)(void));
int	ttputc(UCS);
void	ttgetwinsz(int *, int *);
#endif /* !_WINDOWS */

#endif /* igluno_OSDEP_TTY_INCLUDED */
