/*
** error.h
**
** error funcs for hsc
**
*/

#ifndef HSC_ERROR_H
#define HSC_ERROR_H

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/infile.h"

#include "global.h"

/* function called if invalid state didected */
#define panic(text) call_panic(text,__FILE__,__LINE__)

/*
** define values for return code
*/
#ifdef AMIGA

#define RC_OK     0
#define RC_WARN   5
#define RC_ERROR 10
#define RC_FATAL 20

#else
#ifdef UNIX

#define RC_OK     0 /* TODO: insert standard unix rc's */
#define RC_WARN   5
#define RC_ERROR 10
#define RC_FATAL 20

#else
#error "system not supported: return codes"

#endif /* UNIX */
#endif /* AMIGA */


/*
** global funcs
*/
#ifndef NOEXTERN_HSC_ERROR

extern int return_code;

extern ULONG message_limit;
extern BOOL fatal_error;

extern BOOL open_error( void );
extern void close_error ( void );

extern int errch( char ch );
extern int errlf( void );
extern int errstr( CONSTRPTR str );
extern int errqstr( CONSTRPTR str );
extern int errtag( CONSTRPTR str );
extern int errctag( CONSTRPTR str );
extern int errsym( CONSTRPTR str );
extern int errlong( ULONG num );

extern int message( ULONG id, INFILE *f );

extern VOID err_eof( INFILE *inpf, STRPTR descr );
extern VOID err_eol( INFILE *inpf );
extern VOID err_streol( INFILE *inpf );
extern VOID err_mem( INFILE *inpf );
extern VOID err_write( FILE *outf );
extern VOID err_longstr( INFILE *inpf );
extern VOID err_illgwspc( INFILE *inpf );
extern VOID err_wst( INFILE *inpf );

extern void call_panic( STRPTR text, STRPTR file, ULONG line );

#endif /* NOEXTERN_HSC_ERROR */
#endif /* HSC_ERROR_H */
