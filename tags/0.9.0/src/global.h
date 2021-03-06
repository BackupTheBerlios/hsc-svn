/*
** global.h
**
** global variables and functions for hsc
**
*/

#ifndef HSC_GLOBAL_H
#define HSC_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"

#include "debug.h"                     /* debugging defines */
                                       /* (used by all modules) */
/*
** defines
*/
#define HSCPREFS_ENVVAR "HSCPREFS"

#ifdef AMIGA
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "", ":", "s:"
#endif

#ifdef UNIX
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "./", "~/", "/"
#endif

#ifdef MSDOS
#define CONFIG_FILE "HSC.PRE"
#define CONFIG_PATH ".\\", "\\"
#endif

/* step sizes for expstr's */
#define ES_STEP_VARARG    64 /* config.c */
#define ES_STEP_RMTSTR    32 /* tag_macr.c */
#define ES_STEP_MACRO   1024 /* tag_macr.c */
#define ES_STEP_INFILE  4096 /* input file buffer */

#define MAXLINELEN  1023               /* input line */
#define MAX_ARGLEN   511               /* arguments */
#define MAX_TAGLEN    63               /* tag id */
#define MAX_PATHLEN  255               /* directory path & filename */
#define MAX_URILEN   255               /* URIs */


/*
** char constants
*/
#define CH_QUOTE '\"'
#define CH_SPACE ' '
#define CH_LF    '\n'
#define CH_CR    '\r'
#define CH_ESC   27
#define CH_NULL  '\0'

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_GLOBAL_H

/*
** global vars
*/
extern STRPTR inpfilename;
extern STRPTR outfilename;
extern STRPTR errfilename;
extern STRPTR destdir;
extern STRPTR projdir;

extern ULONG max_error;
extern DLLIST *ignore;

extern BOOL absuri;
extern BOOL chkuri;
extern BOOL debug;
extern BOOL insanch;
extern BOOL need_help;
extern BOOL pipe_in;
extern BOOL rplc_ent;
extern BOOL stripuri;
extern BOOL statusmsg;
extern BOOL verbose;


extern FILE *outfile;
extern time_t now;

extern STRPTR destfname;
extern STRPTR rel_destdir;

extern EXPSTR *tmpstr;

/*
** global funcs
*/
extern int fprintf_spc( FILE *file, size_t num );


#endif /* NOEXTERN_HSC_GLOBAL_H */

#endif /* HSC_GLOBAL_H */
