/*
**
** test_fn.c
**
** test filename manipulaton functions
**
** (W) by Tommy-Saftw�rx in 1994,95,96
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "string.h"
#include "expstr.h"
#include "fname.h"

#define NUM_NAME 6

char newp[ MAX_FPATH ];   /* new path (result of get_relfname() ) */

EXPSTR *dest = NULL;

void test_link_fname( STRPTR dir, STRPTR fn )
{
    BOOL ok = link_fname( dest, dir, fn );

    if ( !dir ) dir = "NULL";
    if ( !fn  ) fn  = "NULL";
    printf( "file \"%s\" in \"%s\" -> [%d] \"%s\"\n",
            fn, dir, ok, estr2str( dest ) );


}

void test_getfne( void )
{
    STRPTR name[ NUM_NAME ] = {
        "c:/hugo/sucks.txt", 
        "//silly/file.tar.gz",
        "test:dir/no_ext",
        "test:no_ext",
        "no_ext",
        "test:emptyext."
        };
    STRPTR res;
    int i;
    BOOL ok;

    for ( i=0; i < NUM_NAME; i++ ) {
        ok = get_fext( dest, name[i] );
        printf( "get_fext (\"%-25s\") -> [%d] \"%s\"\n",
                name[i], ok, estr2str( dest ) );
        ok = get_fname( dest, name[i]  );
        printf( "get_fname(\"%-25s\") -> [%d] \"%s\"\n",
                name[i], ok, estr2str( dest ) );

        printf( "\n" );

    }

}


void test_reldir( STRPTR absn, STRPTR curp )
{
    BOOL ok = get_relfname( dest, absn, curp );
    printf( "\"%s\" and \"%s\" -> [%d] \"%s\"\n",
            absn, curp, ok, estr2str( dest ) );

}

void test_fsdir( STRPTR path )
{
    BOOL ok = get_fsdir( dest, path );

    printf( "get_fsdir: \"%s\" -> [%d]\"%s\"\n",
            path, ok, estr2str( dest ) );

}

void test_fpath( STRPTR path )
{
    BOOL ok = get_fpath( dest, path );

    printf( "get_fpath: \"%s\" -> [%d]\"%s\"\n",
        path, ok, estr2str( dest ) );

}


void test_setfext( STRPTR fn, STRPTR ext )
{
    BOOL ok;

    set_estr( dest, fn );
    ok = set_fext( dest, ext );

    if ( !fn  ) fn  = "NULL";
    if ( !ext ) ext = "NULL";

    printf( "set_fext: \"%s\" with \"%s\" -> [%d] \"%s\"\n",
            fn, ext, ok, estr2str( dest ) );
}

int main( void )
{
    LONG i;
    STRPTR nam;

    /* init global destination */
    dest = init_estr( 20 );

    printf( "Testing ugly fname functions:\n\n" );

#if 0
    printf( "temp names (%d):", L_tmpnam );
    for ( i=0; i<10; i++ ) {

        nam = tmpnamstr();
        if ( nam ) {
            printf( " \"%s\"", nam );
            ufreestr( nam );
        } else
            printf( " <NULL>" );

    }
    printf( "\n" );
#endif

#if 0
    /*
    ** test get_fext & get_fname
    */
    test_getfne();
#endif

#if 1
    /*
    ** test app_fname
    */
    test_link_fname( "c:tools", "sucktool.exe" );
    test_link_fname( "c:", "sucktool.exe" );
    test_link_fname( "", "sucktool.exe" );
    test_link_fname( NULL, "sucktool.exe" );
    test_link_fname( "tools", "" );
    test_link_fname( "tools/", "" );
    test_link_fname( NULL, NULL );
#endif

#if 0
    /*
    ** test set_fext: set a new file extension
    */
    test_setfext( "hugo.tmp", "suck" );
    test_setfext( "hugo", "suck" );
    test_setfext( "hugo.oh.yeah.honey", "sucker" );
    test_setfext( "", "suck" );
#endif

#if 0
    /*
    ** test get_fpath: get only dirs from path
    */
    test_fpath( "quite/long/path/for/hugo" );
    test_fpath( "hugo/heini/" );
    test_fpath( "hugo/heini" );
    test_fpath( "hugo/" );
    test_fpath( "hugo" );
    test_fpath( "" );
#endif

#if 0
    /*
    ** test get_fsdir: get first subdir from path
    */
    test_fsdir( "hugo/heini/" );
    test_fsdir( "hugo/hein" );
    test_fsdir( "hugo/" );
    test_fsdir( "hugo" );
    test_fsdir( "" );
#endif

#if 0
#if 1
    test_reldir( "image/back.gif", "image/hugo/" );
#endif
    test_reldir( "image/back.gif", "" );
#if 1
    test_reldir( "image/back.gif", "people/" );
    test_reldir( "image/image.gif", "image/" );

    test_reldir( "image/symbol/main.gif", "people/hugo/picture/" );
    test_reldir( "image/symbol/main.gif", "image/hugo/picture/" );

    test_reldir( "image.gif", "people/" );
#endif
    test_reldir( "image.gif", "people/hugo/" );

#endif

    return( 0 );

}

