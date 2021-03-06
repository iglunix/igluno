/*
 * $Id: read.h 219 2006-11-06 19:55:41Z jpf@u.washington.edu $
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


#ifndef igluno_OSDEP_READ_INCLUDED
#define igluno_OSDEP_READ_INCLUDED


#include <general.h>


/* exported prototypes */
#ifndef _WINDOWS
UCS	input_ready(int);
int	read_one_char(void);
#else /* _WINDOWS */
int set_time_of_last_input(void);
#endif /* _WINDOWS */

time_t	time_of_last_input(void);


#endif /* igluno_OSDEP_READ_INCLUDED */
