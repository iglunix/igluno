/*
 * $Id: shell.h 788 2007-11-06 23:51:13Z hubert@u.washington.edu $
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


#ifndef igluno_OSDEP_SHELL_INCLUDED
#define igluno_OSDEP_SHELL_INCLUDED


/* exported prototypes */
#if defined(SIGTSTP) || defined(_WINDOWS)
int	bktoshell(int, int);
#endif


#endif /* igluno_OSDEP_SHELL_INCLUDED */
