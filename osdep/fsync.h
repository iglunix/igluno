/*
 * $Id: fsync.h 113 2006-09-01 21:54:31Z hubert@u.washington.edu $
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


#ifndef igluno_OSDEP_FSYNC_INCLUDED
#define igluno_OSDEP_FSYNC_INCLUDED

#ifndef	HAVE_FSYNC

#define	fsync(F)	our_fsync(F)

/* exported prototypes */
int	our_fsync(int);


#endif /* !HAVE_FSYNC */

#endif /* igluno_OSDEP_FSYNC_INCLUDED */
