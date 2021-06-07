#if !defined(lint) && !defined(DOS)
static char rcsid[] = "$Id: filesys.c 745 2007-10-11 18:03:32Z hubert@u.washington.edu $";
#endif

/*
 * ========================================================================
 * Copyright 2006-2007 University of Washington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * ========================================================================
 */

#include <system.h>
#include <general.h>

#if	HAVE_PWD_H
# include <pwd.h>
#endif

#include "../../pith/osdep/temp_nam.h"
#include "../../pith/charconv/filesys.h"

#include "../estruct.h"
#include "../mode.h"
#include "../pico.h"
#include "../edef.h"
#include "../efunc.h"
#include "../keydefs.h"

#include "fsync.h"
#include "filesys.h"


#if	HAVE_DIRENT_H
# include <dirent.h>
#else
# define dirent direct
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif


/*
 * fexist - returns TRUE if the file exists with mode passed in m,
 *          FALSE otherwise.  By side effect returns length of file in l
 *          File is assumed to be a UTF-8 string.
 */
/*int
fexist(char *file,
       char *m,
       off_t *l)
{
#ifndef _WINDOWS
    struct stat	sbuf;
    int rv;

    if(l)
      *l = (off_t)0;


    if(rv < 0){
	switch(errno){
	  case ENOENT :
	    rv = FIOFNF;
	    break;
#ifdef	ENAMETOOLONG
	  case ENAMETOOLONG :
	    rv = FIOLNG;
	    break;
#endif
	  case EACCES :
	    rv = FIOPER;
	    break;
	  default:
	    rv = FIOERR;
	    break;
	}

	return(rv);
    }

    if(l)
      *l = (off_t)sbuf.st_size;

    if((sbuf.st_mode&S_IFMT) == S_IFDIR)
      return(FIODIR);
    else if(*m == 't'){
	struct stat	sbuf2;

	if((sbuf.st_mode&S_IFMT) == S_IFLNK){
	    rv = our_stat(file, &sbuf2);
	    if(rv < 0){
		switch(errno){
		  case ENOENT :
		    rv = FIOSYM;
		    break;
#ifdef	ENAMETOOLONG
		  case ENAMETOOLONG :
		    rv = FIOLNG;
		    break;
#endif
		  case EACCES :
		    rv = FIOPER;
		    break;
		  default:
		    rv = FIOERR;
		    break;
		}

		return(rv);
	    }

	    if((sbuf2.st_mode&S_IFMT) == S_IFDIR)
	      return(FIODIR);
	}

	return(((sbuf.st_mode&S_IFMT) == S_IFLNK) ? FIOSYM : FIOSUC);
    }

    if(*m == 'r'){
	if(*(m+1) == 'w'){
    }
    else if(*m == 'w' && !*(m+1))

    return(rv);
#else
    struct stat	sbuf;

    if(l != NULL)
      *l = (off_t)0;

    if(our_stat(file, &sbuf) < 0){
	if(errno == ENOENT)
	  return(FIOFNF);
	else
	  return(FIOERR);
    }

    if(l != NULL)
      *l = (off_t)sbuf.st_size;

    if(sbuf.st_mode & S_IFDIR)
      return(FIODIR);
    else if(*m == 't')
      return(FIOSUC);

    if(m[0] == 'r')
      return((S_IREAD & sbuf.st_mode) ? FIOSUC : FIONRD);
    else if(m[0] == 'w')
      return((S_IWRITE & sbuf.st_mode) ? FIOSUC : FIONWT);
    else if(m[0] == 'x')
      return((S_IEXEC & sbuf.st_mode) ? FIOSUC : FIONEX);
    return(FIOERR);
#endif
}*/


/*
 * isdir - returns true if fn is a readable directory, false otherwise
 *         silent on errors (we'll let someone else notice the problem;)).
 */
/*int
isdir(char *fn, long *l, time_t *d)
{
    struct stat sbuf;

    if(l)
      *l = 0;

    if(our_stat(fn, &sbuf) < 0)
      return(0);

    if(l)
      *l = sbuf.st_size;

    if(d)
      *d = sbuf.st_mtime;

    return((sbuf.st_mode&S_IFMT) == S_IFDIR);
}*/


/*
 * gethomedir - returns the users home directory in UTF-8 string
 *              Note: home is malloc'd for life of pico
 */
/*char *
gethomedir(int *l)
{
    static char *home = NULL;
    static short hlen = 0;

    if(home == NULL){
	char buf[NLINE];

#ifndef _WINDOWS
	strncpy(buf, "~", sizeof(buf));
	buf[sizeof(buf)-1] = '\0';
	fixpath(buf, sizeof(buf));
#else
	if(Pmaster && Pmaster->home_dir)
	  snprintf(buf, sizeof(buf), "%s", Pmaster->home_dir);
	else
	  snprintf(buf, sizeof(buf), "%c:\\", _getdrive() + 'A' - 1);
#endif
	hlen = strlen(buf);
	if((home = (char *)malloc((hlen + 1) * sizeof(char))) == NULL){
	    emlwrite("Problem allocating space for home dir", NULL);
	    return(0);
	}

	strncpy(home, buf, hlen);
	home[hlen] = '\0';
    }

    if(l)
      *l = hlen;

    return(home);
}*/


/*
 * homeless - returns true if given file does not reside in the current
 *            user's home directory tree.
 */
/*int
homeless(char *f)
{
    char *home;
    int   len;

    home = gethomedir(&len);
    return(strncmp(home, f, len));
}*/


/*
 * getfnames - return all file names in the given directory in a single
 *             malloc'd string.  n contains the number of names
 */
/*char *
getfnames(char *dn, char *pat, int *n, char *e, size_t elen)
{
    size_t	   l, avail, alloced, incr = 1024;
    char          *names, *np, *p;
    struct stat    sbuf;
#if	defined(ct)
    FILE          *dirp;
    char           fn[DIRSIZ+1];
#else
#ifdef _WINDOWS
    char    buf[NLINE+1];
    struct _finddata_t dbuf;
    long   findrv;
#else
    DIR           *dirp;
#endif
#endif
#if	defined(ct) || !defined(_WINDOWS)
    struct dirent *dp;
#endif

    *n = 0;

    if(our_stat(dn, &sbuf) < 0){
	switch(errno){
	  case ENOENT :
	    if(e)
	      snprintf(e, elen, "\007File not found: \"%s\"", dn);

	    break;
#ifdef	ENAMETOOLONG
	  case ENAMETOOLONG :
	    if(e)
	      snprintf(e, elen, "\007File name too long: \"%s\"", dn);

	    break;
#endif
	  default:
	    if(e)
	      snprintf(e, elen, "\007Error getting file info: \"%s\"", dn);

	    break;
	}
	return(NULL);
    }
    else{

	avail = alloced = MAX(sbuf.st_size, incr);
	if((sbuf.st_mode&S_IFMT) != S_IFDIR){
	    if(e)
	      snprintf(e, elen, "\007Not a directory: \"%s\"", dn);

	    return(NULL);
	}
    }

    if((names=(char *)malloc(alloced * sizeof(char))) == NULL){
	if(e)
	  snprintf(e, elen, "\007Can't malloc space for file names");

	return(NULL);
    }

#ifndef _WINDOWS
    errno = 0;
    if((dirp=opendir(fname_to_locale(dn))) == NULL){
	if(e)
	  snprintf(e, elen, "\007Can't open \"%s\": %s", dn, errstr(errno));

	free((char *)names);
	return(NULL);
    }
#endif

    np = names;

#if	defined(ct)
    while(fread(&dp, sizeof(struct direct), 1, dirp) > 0) {
	if(dp.d_ino == 0)
	     continue;
	(*n)++;
	(void)strncpy(fn, dp.d_name, DIRSIZ);
	fn[14] = '\0';
	p = fname_to_utf8(fn);
	while((*np++ = *p++) != '\0')
	  ;
    }
#else
#ifdef _WINDOWS
    strncpy(buf, dn, sizeof(buf));
    buf[sizeof(buf)-1] = '\0';
    snprintf(buf, sizeof(buf), "%s%s%s*%s%s", dn,
	    (dn[strlen(dn)-1] == '\\') ? "" : "\\",
	    (pat && *pat) ? pat : "",
	    (pat && *pat && strchr(pat, '.')) ? "" : ".",
	    (pat && *pat && strchr(pat, '.')) ? "" : "*");
    if((findrv = _findfirst(buf, &dbuf)) < 0){
	if(e)
	  sprintf(e, "Can't find first file in \"%s\"", dn);

	free((char *) names);
	return(NULL);
    }

    do {
	p = fname_to_utf8(dbuf.name);
#else
    while((dp = readdir(dirp)) != NULL){
      p = fname_to_utf8(dp->d_name);
      if(!pat || !*pat || !strncmp(p, pat, strlen(pat))){
#endif
	  (*n)++;
	  l = strlen(p);
	  while(avail < l+1){
	      char *oldnames;

	      alloced += incr;
	      avail += incr;
	      oldnames = names;
	      if((names=(char *)realloc((void *)names, alloced * sizeof(char)))
		  == NULL){
		if(e)
		  snprintf(e, elen, "\007Can't malloc enough space for file names");

		return(NULL);
	      }

	      np = names + (np-oldnames);
	  }

	  avail -= (l+1);

	  while((*np++ = *p++) != '\0')
	    ;
#ifdef _WINDOWS
      }
      while(_findnext(findrv, &dbuf) == 0);
#else
      }
    }
#endif
#endif

#ifdef _WINDOWS
    _findclose(findrv);
#else
    closedir(dirp);
#endif
    return(names);
}*/


/*
 * fioperr - given the error number and file name, display error
 */
/*void
fioperr(int e, char *f)
{
    EML eml;

    eml.s = f;

    switch(e){
      case FIOFNF:
	emlwrite("\007File \"%s\" not found", &eml);
	break;
      case FIOEOF:
	emlwrite("\007End of file \"%s\" reached", &eml);
	break;
      case FIOLNG:
	emlwrite("\007File name \"%s\" too long", &eml);
	break;
      case FIODIR:
	emlwrite("\007File \"%s\" is a directory", &eml);
	break;
      case FIONWT:
	emlwrite("\007Write permission denied: %s", &eml);
	break;
      case FIONRD:
	emlwrite("\007Read permission denied: %s", &eml);
	break;
      case FIOPER:
	emlwrite("\007Permission denied: %s", &eml);
	break;
      case FIONEX:
	emlwrite("\007Execute permission denied: %s", &eml);
	break;
      default:
	emlwrite("\007File I/O error: %s", &eml);
    }
}*/



/*
 * pfnexpand - pico's function to expand the given file name if there is
 *	       a leading '~'. Converts the homedir from user's locale to UTF-8.
 *             Fn is assumed to already be UTF-8.
 */
/*char *
pfnexpand(char *fn, size_t fnlen)
{
    register char *x, *y, *z;
    char *home = NULL;
#ifndef _WINDOWS
    struct passwd *pw;
    char name[50];

    if(*fn == '~'){
        for(x = fn+1, y = name;
	    *x != '/' && *x != '\0' && y-name < sizeof(name)-1;
	    *y++ = *x++)
	  ;

        *y = '\0';
        if(x == fn + 1){
	    if (!(home = (char *) getenv("HOME")))
	      if ((pw = getpwuid(geteuid())) != NULL)
		home = pw->pw_dir;
	}
	else if(*name){
	    if((pw = getpwnam(name)) != NULL)
	      home = pw->pw_dir;
	}

        if(!home || (strlen(home) + strlen(fn) >= fnlen))
	  return(NULL);
#else
    char name[_MAX_PATH];

    if(*fn == '~' && *(x = fn + 1) == '\\') {
	if(!(home = (char *) getenv("HOME"))
	   && getenv("HOMEDRIVE") && getenv("HOMEPATH"))
	  snprintf(home = name, sizeof(name), "%s%s",
		  (char *) getenv("HOMEDRIVE"), (char *) getenv("HOMEPATH"));
#endif
	home = fname_to_utf8(home);

	for(z = x + strlen(x), y = fn + strlen(x) + strlen(home);
	    z >= x;
	    *y-- = *z--)
	  ;

	for(x = fn, y = home; *y != '\0' && x-fn < fnlen; *x++ = *y++)
	  ;
    }

    fn[fnlen-1] = '\0';

    return(fn);
} */


/*
 * fixpath - make the given pathname into an absolute path, incoming
 *           name is UTF-8.
 */
/*void
fixpath(char *name, size_t namelen)
{
#ifdef _WINDOWS
    char file[_MAX_PATH];
    int  dr;

    if(!namelen)
      return;

    if(name[1] == ':' && isalpha((unsigned char) name[0])){
	if(name[2] != '\\'){
	    dr = toupper((unsigned char)name[0]) - 'A' + 1;
	    if((void *)_getdcwd(dr, file, _MAX_PATH) != NULL){
		if(file[strlen(file)-1] != '\\')
		  strncat(file, "\\", sizeof(file)-1-strlen(file));

		strncat(file, &name[2], sizeof(file)-1-strlen(file));
	    }
	    else
	      return;
	}
	else
	  return;
    }
    else if(name[0] == '\\' && name[1] != '\\') {
	sprintf(file, "%c:%.*s", _getdrive()+'A'-1, namelen-3, name);
    }
    else if(name[0] == '\\' && name[1] == '\\'){
      return;
    }
    else{
	if(Pmaster && !(gmode & MDCURDIR)){
	  strncpy(file, ((gmode & MDTREE) || opertree[0])
			  ? opertree : gethomedir(NULL), sizeof(file)-1);
	  file[sizeof(file)-1] = '\0';
	}
	else if(!_getcwd(file, sizeof(file)))
	  return;

	if(*name){
	    if(*file && file[strlen(file)-1] != '\\')
	      strncat(file, "\\", sizeof(file)-1-strlen(file));

	    strncat(file, name, sizeof(file)-1-strlen(file));
	}
    }

    strncpy(name, file, namelen-1);
    name[namelen-1] = '\0';
#else
    char *shft;

    if(!((name[0] == '/')
          || (name[0] == '.'
              && (name[1] == '/' || (name[1] == '.' && name[2] == '/'))))){
	if(Pmaster && !(gmode&MDCURDIR)
                   && (*name != '~' && strlen(name)+2 < namelen)){

	    if(gmode&MDTREE && strlen(name)+strlen(opertree)+1 < namelen){
		int off = strlen(opertree);

		for(shft = strchr(name, '\0'); shft >= name; shft--)
		  shft[off+1] = *shft;

		strncpy(name, opertree, MIN(off,namelen-1));
		name[MIN(off,namelen-1)] = '/';
	    }
	    else{
		for(shft = strchr(name, '\0'); shft >= name; shft--)
		  shft[2] = *shft;

		name[0] = '~';
		name[1] = '/';
	    }
	}

	pfnexpand(name, namelen);
    }
#endif
} */


/*
 * compresspath - given a base path and an additional directory, collapse
 *                ".." and "." elements and return absolute path (appending
 *                base if necessary).
 *
 *                returns  1 if OK,
 *                         0 if there's a problem
 *                         new path, by side effect, if things went OK
 */
/*int
compresspath(char *base, char *path, size_t pathlen)
{
    register int i;
    int  depth = 0;
    char *p;
    char *stack[32];
    char  pathbuf[NLINE];

#define PUSHD(X)  (stack[depth++] = X)
#define POPD()    ((depth > 0) ? stack[--depth] : "")

#ifndef _WINDOWS
    if(*path == '~'){
	fixpath(path, pathlen);
	strncpy(pathbuf, path, sizeof(pathbuf));
	pathbuf[sizeof(pathbuf)-1] = '\0';
    }
    else if(*path != C_FILESEP)
      snprintf(pathbuf, sizeof(pathbuf), "%s%c%s", base, C_FILESEP, path);
    else{
      strncpy(pathbuf, path, sizeof(pathbuf));
      pathbuf[sizeof(pathbuf)-1] = '\0';
    }
#else
    strncpy(pathbuf, path, sizeof(pathbuf));
    pathbuf[sizeof(pathbuf)-1] = '\0';
    fixpath(pathbuf, sizeof(pathbuf));
#endif

    p = &pathbuf[0];
    for(i=0; pathbuf[i] != '\0'; i++){
	if(pathbuf[i] == C_FILESEP){
	    if(p != pathbuf)
	      PUSHD(p);

	    p = &pathbuf[i+1];
	    pathbuf[i] = '\0';
	    continue;
	}

	if(pathbuf[i] == '.'){
	    if(pathbuf[i+1] == '.'
	       && (pathbuf[i+2] == C_FILESEP || pathbuf[i+2] == '\0')){
		if(!strcmp(POPD(), ""))
		  return(0);

		i += 2;
		p = (pathbuf[i] == '\0') ? "" : &pathbuf[i+1];
	    }
	    else if(pathbuf[i+1] == C_FILESEP || pathbuf[i+1] == '\0'){
		i++;
		p = (pathbuf[i] == '\0') ? "" : &pathbuf[i+1];
	    }
	}
    }

    if(*p != '\0')
      PUSHD(p);

    path[0] = '\0';
    for(i = 0; i < depth; i++){
	strncat(path, S_FILESEP, pathlen-strlen(path)-1);
	path[pathlen-1] = '\0';
	strncat(path, stack[i], pathlen-strlen(path)-1);
	path[pathlen-1] = '\0';
    }

    return(1);
} */



/*
 * tmpname - return a temporary file name in the given buffer, the filename
 * is in the directory dir unless dir is NULL. The file did not exist at
 * the time of the temp_nam call, but was created by temp_nam.
 */
/*void
tmpname(char *dir, char *name)
{
    char *t;
#ifndef _WINDOWS
    if((t = temp_nam((dir && *dir) ? dir : NULL, "pico.")) != NULL){
#else
    char  tmp[_MAX_PATH];

    if(!((dir && *dir) ||
	 (dir = getenv("TMPDIR")) ||
	 (dir = getenv("TMP")) ||
	 (dir = getenv("TEMP"))))
      if(!(getcwd(dir = tmp, _MAX_PATH)
	   && fexist(dir, "w", (off_t *) NULL) == FIOSUC))
	dir = "c:\\";

    if((t = temp_nam_ext(dir, "ae", "txt")) != NULL){
#endif
	strncpy(name, t, NFILEN-1);
	name[NFILEN-1] = '\0';
	free((void *)t);
    }
    else {
	emlwrite("Unable to construct temp file name", NULL);
	name[0] = '\0';
    }
}*/


/*
 * Take a file name, and from it
 * fabricate a buffer name. This routine knows
 * about the syntax of file names on the target system.
 * I suppose that this information could be put in
 * a better place than a line of code.
 */
/*void
makename(char bname[], char fname[])
{
    register char   *cp1;
    register char   *cp2;

    cp1 = &fname[0];
    while (*cp1 != 0)
      ++cp1;

    while (cp1!=&fname[0] && cp1[-1]!=C_FILESEP)
      --cp1;

    cp2 = &bname[0];
    while (cp2!=&bname[NBUFN-1] && *cp1!=0 && *cp1!=';')
      *cp2++ = *cp1++;

    *cp2 = 0;
}*/


/*
 * copy - copy contents of file 'a' into a file named 'b'.  Return error
 *        if either isn't accessible or is a directory
 */
/*int
copy(char *a, char *b)
{
    int    in, out, n, rv = 0;
    char   *cb;
    struct stat tsb, fsb;
    EML    eml;
    extern int  errno;*/

    /*if(our_stat(a, &fsb) < 0){
	eml.s = errstr(errno);
	emlwrite("Can't Copy: %s", &eml);
	return(-1);
    }

    if(!(fsb.st_mode&S_IREAD)){
	eml.s = a;
	emlwrite("\007Read permission denied: %s", &eml);
	return(-1);
    }

    if((fsb.st_mode&S_IFMT) == S_IFDIR){
	eml.s = a;
	emlwrite("\007Can't copy: %s is a directory", &eml);
	return(-1);
}*/

    /*if(our_stat(b, &tsb) < 0){
	switch(errno){
	  case ENOENT:
	    break;
	  default:
	    eml.s = errstr(errno);
	    emlwrite("\007Can't Copy: %s", &eml);
	    return(-1);
	}
}*/
    /*else{
	/*if(!(tsb.st_mode&S_IWRITE)){
	    eml.s = b;
	    emlwrite("\007Write permission denied: %s", &eml);
	    return(-1);
	}

	if((tsb.st_mode&S_IFMT) == S_IFDIR){
	    eml.s = b;
	    emlwrite("\007Can't copy: %s is a directory", &eml);
	    return(-1);
	}

	if(fsb.st_dev == tsb.st_dev && fsb.st_ino == tsb.st_ino){
	    emlwrite("\007Identical files.  File not copied", NULL);
	    return(-1);
	}
    }

    if((in = our_open(a, O_RDONLY|O_BINARY, 0600)) < 0){
	eml.s = errstr(errno);
	emlwrite("Copy Failed: %s", &eml);
	return(-1);
} */

    /*if((out=our_creat(b, fsb.st_mode&0xfff)) < 0){
	eml.s = errstr(errno);
	emlwrite("Can't Copy: %s", &eml);
	close(in);
	return(-1);
}*/

    /*if((cb = (char *)malloc(NLINE*sizeof(char))) == NULL){
	emlwrite("Can't allocate space for copy buffer!", NULL);
	close(in);
	close(out);
	return(-1);
}*/

    /*while(1){
	if((n = read(in, cb, NLINE)) < 0){
	    eml.s = errstr(errno);
	    emlwrite("Can't Read Copy: %s", &eml);
	    rv = -1;
	    break;
	} */

/*	if(n == 0)
	  break; */

/*	if(write(out, cb, n) != n){
	    eml.s = errstr(errno);
	    emlwrite("Can't Write Copy: %s", &eml);
	    rv = -1;
	    break;
*/


/*    free(cb);
    close(in);
    close(out);
    return(rv);*/



/*
 * Open a file for writing. Return TRUE if all is well, and FALSE on error
 * (cannot create).
 */
/* int
ffwopen(char *fn, int readonly)
{
    extern FIOINFO g_pico_fio;
#ifndef _WINDOWS
    int		 fd;
    EML          eml;
#endif */
#ifndef	MODE_READONLY
#define	MODE_READONLY	(0600)
#endif

    /*
     * Call open() by hand since we don't want O_TRUNC -- it'll
     * screw over-quota users.  The strategy is to overwrite the
     * existing file's data and call ftruncate before close to lop
     * off bytes
     */

    /* g_pico_fio.flags = FIOINFO_WRITE; */
    /* g_pico_fio.name  = fn; */
/* #ifndef _WINDOWS
    if((fd = our_open(fn, O_CREAT|O_WRONLY, readonly ? MODE_READONLY : 0666)) >= 0
       && (g_pico_fio.fp = fdopen(fd, "w")) != NULL
       && fseek(g_pico_fio.fp, 0L, 0) == 0)
      return (FIOSUC);


    eml.s = errstr(errno);
    emlwrite("Cannot open file for writing: %s", &eml);
    return (FIOERR);
#else
    if ((g_pico_fio.fp = our_fopen(fn, "w")) == NULL) {
        emlwrite("Cannot open file for writing", NULL);
        return (FIOERR);
    } */

/* #ifdef	MODE_READONLY
    if(readonly)
      our_chmod(fn, MODE_READONLY);		* fix access rights */
/* #endif

    return (FIOSUC); */
/* #endif
} */


/*
 * Close a file. Should look at the status in all systems.
 */
/* int
ffclose(void)
{
    extern FIOINFO g_pico_fio; */
/* #ifndef _WINDOWS
    EML eml;

    errno = 0;
    if((g_pico_fio.flags & FIOINFO_WRITE)
       && (fflush(g_pico_fio.fp) == EOF
	   || ftruncate(fileno(g_pico_fio.fp),
			(off_t) ftell(g_pico_fio.fp)) < 0)){
	eml.s = errstr(errno);
	emlwrite("\007Error preparing to close file: %s", &eml);
	sleep(5);
} */

    /* if (fclose(g_pico_fio.fp) == EOF) {
	eml.s = errstr(errno);
        emlwrite("\007Error closing file: %s", &eml);
        return(FIOERR);
    }
#else
    if (fclose(g_pico_fio.fp) != FALSE) {
        emlwrite("Error closing file", NULL);
        return(FIOERR);
    } */
/* #endif

    return(FIOSUC);
} */



#define	EXTEND_BLOCK	1024


/*
 * ffelbowroom - make sure the destination's got enough room to receive
 *		 what we're about to write...
 */
/* int
ffelbowroom(void)
{ */
/* #ifndef _WINDOWS
    register LINE *lp;
    register long  n;
    int		   x;
    char	   s[EXTEND_BLOCK], *errstring = NULL;
    struct stat	   fsbuf;
    extern FIOINFO g_pico_fio; */

    /* Figure out how much room do we need */
    /* first, what's total */
    /* for(n=0L, lp=lforw(curbp->b_linep); lp != curbp->b_linep; lp=lforw(lp))
      n += (llength(lp) + 1); */

    /* errno = 0;			*/ /* make sure previous error are cleared */

    /* if(fstat(fileno(g_pico_fio.fp), &fsbuf) == 0){
	n -= fsbuf.st_size; */

	/* if(n > 0L){
	    memset(s, 'U', EXTEND_BLOCK);
	    if(fseek(g_pico_fio.fp, fsbuf.st_size, 0) == 0){
		for( ; n > 0L; n -= EXTEND_BLOCK){
		    x = (n < EXTEND_BLOCK) ? (int) n : EXTEND_BLOCK;
		    if(fwrite(s, x * sizeof(char), 1, g_pico_fio.fp) != 1){
			errstring = errstr(errno);
			break;
		    }
		}

		if(!errstring
		   && (fflush(g_pico_fio.fp) == EOF
		       || fsync(fileno(g_pico_fio.fp)) < 0))
		  errstring = errstr(errno);

		if(errstring)
		  (void) ftruncate(fileno(g_pico_fio.fp), (off_t) fsbuf.st_size);
		else if(fseek(g_pico_fio.fp, 0L, 0) != 0)
		  errstring = errstr(errno);
	    }
	    else
	      errstring = errstr(errno);
	}
} */
    /* else
      errstring = errstr(errno); */

    /* if(errstring){
	snprintf(s, sizeof(s), "Error writing to %s: %s", g_pico_fio.name, errstring);
	emlwrite(s, NULL);
	(void) fclose(g_pico_fio.fp);
	return(FALSE);
}*/
