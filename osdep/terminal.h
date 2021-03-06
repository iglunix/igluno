/*
 * $Id: terminal.h 530 2007-04-19 02:37:54Z mikes@u.washington.edu $
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


#ifndef igluno_OSDEP_TERMINAL_INCLUDED
#define igluno_OSDEP_TERMINAL_INCLUDED


/*
 * Useful definitions
 */
#define NROW	DEFAULT_LINES_ON_TERMINAL
#define NCOL	DEFAULT_COLUMNS_ON_TERMINAL

#define	TT_OPTIMIZE	0x01		/* optimize flag(cf line speed)	*/
#define	TT_EOLEXIST	0x02		/* does clear to EOL exist	*/
#define	TT_SCROLLEXIST	0x04		/* does insert line exist	*/
#define	TT_REVEXIST	0x08		/* does reverse video exist?	*/
#define	TT_INSCHAR	0x10		/* does insert character exist	*/
#define	TT_DELCHAR	0x20		/* does delete character exist	*/


#define	TERM_OPTIMIZE		(tthascap() & TT_OPTIMIZE)
#define	TERM_EOLEXIST		(tthascap() & TT_EOLEXIST)
#define	TERM_SCROLLEXIST	(tthascap() & TT_SCROLLEXIST)
#define	TERM_REVEXIST		(tthascap() & TT_REVEXIST)
#define	TERM_INSCHAR		(tthascap() & TT_INSCHAR)
#define	TERM_DELCHAR		(tthascap() & TT_DELCHAR)


/* exported prototypes */
unsigned tthascap(void);
#if	HAS_TERMINFO || HAS_TERMCAP
void	 putpad(char *);
#endif


#endif /* igluno_OSDEP_TERMINAL_INCLUDED */
