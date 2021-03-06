/*
** output.h
**
** output functions for hsc
*/

#ifndef HSC_OUTPUT
#define HSC_OUTPUT

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/infile.h"

extern BOOL open_output( void );
extern int  outch( char ch );
extern int  outstr( CONSTRPTR str );
extern BOOL copy_until_gt( INFILE *inpf );
extern BOOL skip_until_gt( INFILE *inpf );

#endif /* HSC_OUTPUT */
