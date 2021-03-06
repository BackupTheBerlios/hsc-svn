/*
** test ugly list functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "memory.h"
#include "string.h"
#include "dllist.h"

/*
** destructor
*/
void del_str( APTR data )
{
    ufree( data );
}

void prt_str( FILE *stream, APTR data )
{
    if ( data )
        fprintf( stream, "%s\n", (STRPTR) data );
    else
        fprintf( stream, "<NULL>\n" );
}

int cmp_str( APTR data1, APTR data2 )
{
    if (data1 && data2)
        if ( !strcmp( (STRPTR) data1, (STRPTR) data2 ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}


/*
** main: create a list of strings and play with it
*/
void main( int argc, char *argv[] )
{

    DLLIST *tl = init_dllist( del_str );
    DLNODE *nd = NULL;
    STRPTR str = NULL;

#ifdef UMEM_TRACKING
    atexit( atexit_uglymemory );
#endif

    if ( tl ) {

        printf( "APPEND sepp,NULL,hugo,resi\n" );
        str = strclone( "sepp" );
        app_dlnode( tl, str );
        app_dlnode( tl, NULL );
        str = strclone( "hugo" );
        app_dlnode( tl, str );
        str = strclone( "resi" );
        app_dlnode( tl, str );
        fprintf_dllist( stdout, tl, prt_str );

        printf( "SEARCHING for suck\n" );
        nd = find_dlnode( tl->first, (APTR) "suck", cmp_str );
        if (nd)
            printf( "  ERROR: suck found\n" );
        else
            printf( "  suck not found\n" );

        nd = find_dlnode( tl->first, (APTR) "sepp", cmp_str );
        if (nd) {

            printf( "INSERT susi before sepp\n" );
            str = strclone( "susi" );
            ins_dlnode( tl, nd, str );
            fprintf_dllist( stdout, tl, prt_str );

        }

        nd = find_dlnode( tl->first, (APTR) "hugo", cmp_str );
        if (nd) {

            printf( "REMOVING hugo\n" );
            del_dlnode( tl, nd );
            fprintf_dllist( stdout, tl, prt_str );

        }

        nd = find_dlnode( tl->first, (APTR) "resi", cmp_str );
        if (nd) {

            printf( "INSERT jack before resi\n" );
            str = strclone( "jack" );
            ins_dlnode( tl, nd, str );
            fprintf_dllist( stdout, tl, prt_str );

        }

        nd = find_dlnode( tl->first, (APTR) "susi", cmp_str );
        if (nd) {

            printf( "REMOVING susi\n" );
            del_dlnode( tl, nd );
            fprintf_dllist( stdout, tl, prt_str );

        }

        nd = find_dlnode( tl->first, (APTR) "resi", cmp_str );
        if (nd) {

            printf( "REMOVING resi\n" );
            del_dlnode( tl, nd );
            fprintf_dllist( stdout, tl, prt_str );

        }

        printf( "REMOVING rest of list\n" );
        del_dllist( tl );
        fprintf_dllist( stdout, tl, prt_str );


    }

    umem_report( "testargs - end" );


}

