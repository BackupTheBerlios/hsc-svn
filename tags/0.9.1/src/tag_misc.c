/*
** tag_misc
**
** misc tag handles
**
** updated:  5-Oct-1995
** created: 30-Jul-1995
*/

/*
** ansi includes
*/
#include <stdio.h>
#include <string.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/args.h"
#include "ugly/memory.h"
#include "ugly/prginfo.h"

/*
** local includes
*/
#include "global.h"
#include "error.h"
#include "msgid.h"
#include "output.h"
#include "parse.h"
#include "skip.h"
#include "tagargs.h"


#include "tag.h"

UBYTE prev_heading_num = 0; /* stores previous heading */

/*
**
** global funs
**
*/


/*
** handle_heading: tag handle for <H1>..<H6>
**
** compute number of haeding,
** compare it with previous heading,
** check first heading to be <H1>
**
*/
BOOL handle_heading( INFILE *inpf, HSCTAG *tag )
{
    BYTE num = (tag->name[1] - '0'); /* num of heading (1..6) */

    if ( debug )
        fprintf( stderr, "**   heading %d\n", num);

    if ( (prev_heading_num-num) < (-1) )
        if (prev_heading_num) {

            char hstr[4];

            message( MSG_WRONG_HEADING, inpf );
            errstr( "next heading should be " );
            sprintf( hstr, "H%d", prev_heading_num+1 );
            errtag( hstr );
            errlf();

        } else {

            message( MSG_EXPT_H1, inpf );
            errstr( "first heading should be " );
            errtag( "H1" );
            errlf();

        }

    prev_heading_num = num;

    return (TRUE);

}


/*
** handle_sgml_comment: tag handle for <!..>
**
**
*/
BOOL handle_sgml_comment( INFILE *inpf, HSCTAG *tag )
{
    /*
    ** TODO: - correct abort
    **       - copy chars
    */
    STRPTR nw = infgetw( inpf );

    if ( nw ) {

            /* check for whitespace after "!" */
            if ( strlen( infgetcws( inpf ) ) ) {

                message( MSG_ILLG_WHTSPC, inpf );
                errstr( "illegal white space" );
                errlf();

            }

        if ( strncmp( nw, "--", 2 ) )
            skip_until_eot( inpf );     /* unknown "!"-command: skip */
        else {

            /* handle comment */
            BOOL end_min  = FALSE;
            BOOL end_gt   = FALSE;
            BOOL in_quote = FALSE;

            while ( !fatal_error && !end_gt ) {

                nw = infgetw( inpf );
                if ( nw ) {

                    size_t slen = strlen( nw );

                    /*
                    **check for "--"
                    */
                    if ( slen >= 2 )
                        end_min =
                            ( (nw[slen-1]=='-') && (nw[slen-1]=='-') );

                    /*
                    ** check for end-of-comment
                    */
                    if ( !strcmp( nw , ">" ) )
                        if ( end_min )
                            end_gt = TRUE;
                        else {

                            message( MSG_GT_IN_COMMENT, inpf );
                            errstr( "\">\"inside SGML-comment" );
                            errlf();

                        }


                    /*
                    ** check for LF
                    */
                    else if ( !strcmp( nw, "\n" ) ) {

                        in_quote = TRUE;
                        message( MSG_LF_IN_COMMENT, inpf );
                        errstr( "line feed inside SGML-comment" );
                        errlf();

                    /*
                    ** check for quote
                    */
                    } else if ( !strcmp( nw, "\"" ) || !strcmp( nw, "'" ) )
                        in_quote = !in_quote;


                } else
                    err_eof( inpf );

                if ( end_gt && in_quote ) {

                    message( MSG_CMTEND_QUOTE, inpf );
                    errstr( "SGML-comment ends inside quotes" );
                    errlf();

                }
            }



        }
    } else
        err_eof( inpf );

    return (TRUE);
}
