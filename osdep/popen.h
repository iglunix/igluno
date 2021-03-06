/*
 * $Id: popen.h 135 2006-09-22 18:56:49Z hubert@u.washington.edu $
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


#ifndef igluno_OSDEP_POPEN_INCLUDED
#define igluno_OSDEP_POPEN_INCLUDED


/* exported prototypes */
int	 P_open(char *);
void	 P_close(void);


#endif /* igluno_OSDEP_POPEN_INCLUDED */
