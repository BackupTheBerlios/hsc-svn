/*
** ugly/string.c
**
** ugly string functions
**
** (W) by Tommy-Saftw�rx in 1993,94,95
**
** updated:  1-Aug-1995
** created: 31-Jul-1993
**
** $VER: string.c 1.1.0 (1.8.95)
*/


#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "memory.h"

#define NOEXTERN_UGLY_STRING_H
#include "string.h"



char ch2str_buffer[2];                 /* internal buffer for _ch2str() */
char num2str_buffer[10];               /* internal buffer for _long2str() */



/*
** strclone
**
** create a clone of a string 
**     (copy of string using its own memory area)
**
** params: *oldstr...ptr to string to clone
** result: ptr to copy of string;
** errors: out of memory: returns NULL
**
*/
STRPTR ugly_strclone( CONSTRPTR oldstr, STRPTR file, ULONG line )
{
    STRPTR newstr = NULL;

    if ( oldstr ) {

        newstr =                           /* alloc mem for clone */
#ifdef UMEM_TRACKING
            ugly_malloc( strlen( oldstr ) +1, file, line );
#else
            malloc(  strlen( oldstr ) +1 );
#endif

        if ( newstr )                      /* alloc sucessful? */
            strcpy( newstr, oldstr );      /* Y-> copy data */

    }

    return( newstr );                  /* return result */
}


/*
** upstr: convert a string to upper case
**
** params: *s...string to convert
**
*/
STRPTR upstr( STRPTR s )
{
    STRPTR s_old = s;

    if (s)
        for (; *s != '\0'; s++)
            *s = toupper(*s);

    return s_old;
}

/*
** upstrcmp
**
** string compare, ignore case
**
** params: *s1, *s2...ptr to strings to compare
** result: -1: s1 < s2
**          0: s1 = s2
**          1: s1 > s2
**
*/
int upstrcmp( CONSTRPTR s1, CONSTRPTR s2 )
{
    int equal;                         /* result of _strcp() */
    unsigned char c1, c2;              /* chars currently comparing */
    size_t i = 0;                      /* string index counter */

    do {
        c1 = toupper( s1[i] );
        c2 = toupper( s2[i] );
        i++;
    } while ( c1 && c2 && ( c1 == c2 ) );

    if ( c1 < c2 )
        equal = -1;                    /* s1 < s2 */
    else if ( c1 > c2 )
        equal = +1;                    /* s1 > s2 */
    else
        equal =  0;                    /* s1 = s2 */

    return( equal );                   /* return result */
}


/*
** upstrncmp
**
** string compare, ignore case, max. n chars
**
** params: *s1, *s2...ptr to strings to compare
**         n..........max. nr. of chars to compare
** result: -1: s1 < s2
**          0: s1 = s2
**          1: s1 > s2
**
*/
int upstrncmp( CONSTRPTR s1, CONSTRPTR s2, size_t n )
{
    int equal;                         /* result of _strcp() */
    unsigned char c1, c2;              /* char of string currently comparing */
    size_t i = 0;                      /* string index counter */

    do {
        c1 = toupper( s1[i] );
        c2 = toupper( s2[i] );
        i++;
    } while ( c1 && c2 && ( c1 == c2 ) && ( i < n) );

    if ( c1 < c2 )
        equal = -1;                    /* s1 < s2 */
    else if ( c1 > c2 )
        equal = +1;                    /* s1 > s2 */
    else
        equal =  0;                    /* s1 = s2 */

    return(equal);                     /* return result */
}




/*
** freestr
**
** free memory used by a string
**
** params: s..string to free
**
** NOTE: strings created with strclone() or reallocstr
**       should be freed with this function instead of
**       ufree(). this will avoid problems when using
**       the memory-tracking feature of ugly.o
*/
void ugly_freestr( STRPTR s, STRPTR file, ULONG line )
{
#ifdef UMEM_TRACKING
    ugly_free( s, file, line );
#else
    ufree( s );
#endif
}

/*
** reallocstr
**
** free memory used by string, clone new data
**
** params: **oldstr...prt to ptr of old string
**          *newstr...ptr to new string data
** errors: out of mem: returns NULL (old data are lost)
**
** IMPORTANT: old string has to be initialies like "oldstr = NULL;" before !!
**            first call like "reallocstr( &oldstr, "<new data>" );"
*/
void ugly_reallocstr( STRPTR *oldstr,  CONSTRPTR newstr, STRPTR file, ULONG line )
{
#ifdef UMEM_TRACKING
    ugly_freestr( *oldstr, file, line );                   /* free old string */
    *oldstr = ugly_strclone( newstr, file, line );         /* clone new string */
#else
    ufree( *oldstr );                            /* free old string */
    *oldstr = strclone( newstr );                /* clone new string */
#endif
}
/*
** ch2str
**
** convert single char to zero terminated string
**
** params: ch...char to convert
** result: ptr to internal buffer, where a string containning
**         "<ch>\0" is located. 
**
** USAGE : buffer is rewritten at next call of _chstr(), so
**         be sure to clone string if you need it for a longer
**         time.
**         _ch2str() is mostly designed for parameter conversion
**         on string funtions, e.g s = strclone( ch2str( 'x' ) );
**
*/
STRPTR ch2str( const char ch )
{
    ch2str_buffer[0] = ch;
    ch2str_buffer[1] = '\0';

    return ch2str_buffer;
}

/*
** ustrrpbrk
**
** searches for last occurence of a char of _set in _str
**
** params: str...string to examine
**         set...chars to search for
** result: ptr where char of _set occured in _str
**         or NULL if none occurence found
**
** NOTE:   this is a clone of none-ansi-c function _strrpbrk()
**
*/
STRPTR ustrrpbrk( CONSTRPTR str, CONSTRPTR set )
{
    size_t i;
    STRPTR result = NULL;

    if ( str ) {

        i = strlen( str )-1;

        if ( i >= 0 ) {
        
            while ( ( i ) && ( strchr( set, str[i] ) == NULL ) )
                i--;

            if ( strchr( set, str[i] ) )
                result = ( STRPTR ) &(str[i]);
        }

    }

    return result;

}


/*
** str2long
**
** convert string expression to long value
**
** params: s....string to convert
**         num..ptr to destination long var
** result: TRUE, if sucessful
** errors: returen FALSE
**
*/
BOOL str2long( STRPTR s, LONG *num )
{
    BOOL conv_ok = FALSE;

    if ( sscanf( s, "%d", num ) ) {

        conv_ok = TRUE;

    }

    return conv_ok;
}


/*
** long2str
**
** convert long value to string expresion
**
** params: num...value to convert
** result: ptr to converted string
** errors: returns NULL
**
*/
STRPTR long2str( LONG num )
{
    STRPTR result_str = NULL;

    if ( sprintf( num2str_buffer, "%d", num ) ) {

        result_str = num2str_buffer;

    }

    return result_str;
}

