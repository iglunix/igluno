/*
 * $Id: color.h 1007 2008-03-24 19:35:31Z hubert@u.washington.edu $
 *
 * ========================================================================
 * Copyright 2006-2008 University of Washington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * ========================================================================
 */


#ifndef igluno_OSDEP_COLOR_INCLUDED
#define igluno_OSDEP_COLOR_INCLUDED


/* exported prototypes */
void	 StartInverse(void);
void     flip_inv(int);
void	 EndInverse(void);
int	 InverseState(void);
int	 StartBold(void);
void	 EndBold(void);
void	 StartUnderline(void);
void	 EndUnderline(void);
int      igluno_trans_color(void);
void	 igluno_endcolor(void);
void	 igluno_toggle_color(int);
void	 igluno_set_nfg_color(void);
void	 igluno_set_nbg_color(void);


#endif /* igluno_OSDEP_COLOR_INCLUDED */
