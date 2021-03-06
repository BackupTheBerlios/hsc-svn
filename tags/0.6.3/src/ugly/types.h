#ifndef UGLY_TYPES_H
#define UGLY_TYPES_H


/*
** ugly/types.h
**
** ugly data typing.
**      (boys from commodore might recognise some parts of exec/types.h)
**
** NOTE: contains also UGLY_VER and UGLY_REV
**
** Version 1.0.2, (W) by Tommy-Saftw�rx
**
** updated:  4-Jul-94
** created: 25-Jan-94
**
** $VER: types.h 1.0.2 (4.7.94)
**
*/


/*
** are you running a amiga?
*/

#ifdef AMIGA

/*
** on amiga system, just include system types 
*/

#include <exec/types.h>


/*
**
** no amiga -> normal typedefs
**
*/

#else

#ifndef APTR_TYPEDEF
#define APTR_TYPEDEF
typedef void           *APTR;       /* 32-bit untyped pointer */
#endif
typedef long            LONG;       /* signed 32-bit quantity */
typedef unsigned long   ULONG;      /* unsigned 32-bit quantity */
typedef short           WORD;       /* signed 16-bit quantity */
typedef unsigned short  UWORD;      /* unsigned 16-bit quantity */
#if __STDC__
typedef signed char     BYTE;       /* signed 8-bit quantity */
#else
typedef char            BYTE;       /* signed 8-bit quantity */
#endif
typedef unsigned char   UBYTE;      /* unsigned 8-bit quantity */

#ifdef __cplusplus
typedef char           *STRPTR;     /* string pointer (NULL terminated) */
#else
typedef unsigned char  *STRPTR;     /* string pointer (NULL terminated) */
#endif


/* Types with specific semantics */
typedef short           BOOL;
typedef unsigned char   TEXT;

#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#ifndef NULL
#define NULL            0L
#endif

#define BYTEMASK        0xFF
#define WORDMASK        0xFFFF


#endif  /* AMIGA */


/*
**
** global typedefs (on any system)
**
*/

typedef const unsigned char *CONSTRPTR;          /* string constants */

/*
** UPTR as an generic pointer. C-math will not operate on UPTR.
** UPTR can be converted to any other pointer and the other way round.
** It is used by ugly functions, especially the umx-functions
*/
typedef void *UPTR;                    /* generic pointer ( ANSI-def ) */


/*
** compare/delete function type
*/
typedef int  cmp_func( UPTR data1, UPTR data2 );
typedef void del_func( UPTR data );


#endif  /* UGLY_TYPES_H */

