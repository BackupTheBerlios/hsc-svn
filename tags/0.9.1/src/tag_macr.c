/*
** tag_macr.c
**
** tag handles for "<$MACRO>" and "<macro>"
**
** updated:  8-Oct-1995
** created:  5-Aug-1995
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/fname.h"
#include "ugly/infile.h"
#include "ugly/memory.h"
#include "ugly/string.h"

#include "global.h"

#include "tag.h"
#include "vars.h"

#include "config.h"
#include "cleanup.h"
#include "error.h"
#include "msgid.h"
#include "parse.h"
#include "output.h"
#include "skip.h"
#include "tagargs.h"

EXPSTR *rmt_str = NULL;      /* temp. buffer to read macro text */

/* states for read_macro_text */
#define MAST_TEXT   0 /* inside text      */
#define MAST_LT     1 /* after "<"        */
#define MAST_SLASH  2 /* after "</"       */
#define MAST_CMACRO 3 /* after "</$MACRO" */
#define MAST_TAG    8 /* inside a tag     */
#define MAST_ERR   99 /* error occured    */

extern BOOL parse_wd( INFILE *inpf, STRPTR expstr ); /* TODO: what a pervert! */

/*
**-------------------------------------
** handlers for opening/closing macro
**-------------------------------------
*/

/*
** handle_macro
**
** handle for macro: add local attributes to global attributes,
** include macro text, remove local attributes
**
** params: open_mac..TRUE, if called by an openning macro
**         inpf......input file
*/
BOOL handle_macro( BOOL open_mac, INFILE *inpf, HSCTAG *macro )
{
    BOOL    ok = FALSE;
    EXPSTR *text;       /* macro text */
    EXPSTR *fname;      /* pseudo-filename */
    DLLIST *args;
    ULONG   mci = 0;    /* tag-call-id returned by set_tag_args() */

    /* determine filename & args */
    if ( open_mac ) {
        text  = macro->op_text ;
        args  = macro->op_args;
    } else {
        text  = macro->cl_text ;
        args  = macro->cl_args; /* TODO: remove this */
        args  = macro->op_args;
    }
    fname = init_estr( 0 );

    if ( fname ) {

        /* debugging message */
        DMC( {
            fprintf( stderr, "**-MACRO <" );
            if ( !open_mac )
                fprintf( stderr, "/" );
            fprintf( stderr, "%s> from %p\n", macro->name, text );
        } );

        /* create pseudo-filename */
        ok = set_estr( fname, "[macro " );
        ok &= app_estr( fname, macro->name );
        ok &= app_estr( fname, "]" );

    }

    if ( ok ) {

        /* parse args */
        mci = set_tag_args( macro, inpf, open_mac );
        if ( mci == MCI_ERROR ) ok = FALSE;
        DMC( prt_varlist( args, "macro args (after set_macro_args)" ) );
    }

    if ( ok ) {

        /* copy local vars to global list */
        ok = copy_local_varlist( args, mci );
        DMC( prt_varlist( vars, "global vars (after copy_local_vars)" ) );

    }

    if ( ok ) {
        /* include macro file */
        ok = include_hsc_string( estr2str( fname ), estr2str( text ),
                                 outfile, IH_PARSE_MACRO );
    }

    /* cleanup */
    if ( mci != MCI_ERROR )
        remove_local_varlist( mci );   /* remove local vars */
    clr_varlist( args );               /* clear macro vars */
    del_estr( fname );                 /* release pseudo-filename */

    /* debugging message */
    DMC( fprintf( stderr, "**-ENDMACRO <%s>\n", macro->name ) );


    return ( ok );
}

/*
** handle_op_macro
**
** handle for opening macro
*/
BOOL handle_op_macro( INFILE *inpf, HSCTAG *tag )
{
    return( handle_macro(TRUE, inpf, tag) );
}

/*
** handle_cl_macro
**
** handle for closing macro
*/
BOOL handle_cl_macro( INFILE *inpf, HSCTAG *tag )
{
    return( handle_macro(FALSE, inpf, tag) );
}

/*
**-------------------------------------
** macro creation functions
**-------------------------------------
*/

#if 0
/*
** rmt_check_for
**
** read next word from input, compare it with cmpstr,
** append word to rmt_str
*/
BOOL rmt_check_for( INFILE *inpf, STRPTR cmpstr )
{
    STRPTR  nw = infgetw( inpf );
    BOOL    eq = FALSE;

    if ( nw ) {

        eq = !upstrcmp( nw, cmpstr );
        app_estr( rmt_str, infgetcws( inpf ) );
        app_estr( rmt_str, infgetcw( inpf ) );

    } else
        err_eof( inpf );

    return( eq );
}
#endif

/*
** read_macro_text
**
*/
BOOL read_macro_text( HSCTAG *macro, INFILE *inpf, BOOL open_mac )
{
    BOOL    ok = FALSE;
    EXPSTR *macstr = NULL;

    /* init an EXPSTR for macro text */
    if ( open_mac ) {
        macro->op_text = init_estr( ES_STEP_MACRO );
        macstr         = macro->op_text;
    } else {
        macro->cl_text = init_estr( ES_STEP_MACRO );
        macstr         = macro->cl_text;
    }

    if ( macstr ) {                    /* init sucessfull? */
#if 0
        BOOL   quote     = FALSE;    /* TRUE, if between single quotes */
        BOOL   dquote    = FALSE;    /* TRUE, if between double quotes */
#endif
        STRPTR nw        = NULL;     /* word read from input */
        BYTE   state     = MAST_TEXT;/* current state */

        /* skip first LF if any */
        skip_lf( inpf );

        /* init rmt_str */
        rmt_str = init_estr( ES_STEP_MACRO );
        if ( !rmt_str )
            err_mem( inpf );


        while ( !(fatal_error || (state==MAST_CMACRO) ) ) {

            /* read next word */
            if ( state == MAST_SLASH )
                nw = infget_tagid( inpf  );
            else if ( state != MAST_TAG )
                nw = infgetw( inpf  );

            if ( nw ) {

                if ( state == MAST_TAG ) {

                    /*
                    ** skip inside tags
                    */
                    BYTE   tag_state = TGST_TAG; /* state var passe to */
                                                 /*     eot_reached() */

                    do {

                        if ( eot_reached( inpf, &tag_state ) );
                            state = MAST_TEXT;

                        if ( !( app_estr( macstr, infgetcws( inpf ) )
                                && app_estr( macstr, infgetcw( inpf ) )
                        ) )
                            err_mem( inpf );

                    } while ( (tag_state!=TGST_END) && !fatal_error );

                } else {

                    switch ( state ) {

                        case MAST_TEXT:
                            if ( !strcmp( nw, "<" ) )
                                state = MAST_LT;
                            break;

                        case MAST_LT:
                            if ( !strcmp( nw, "/" ) )
                                state = MAST_SLASH;
                            else
                                state = MAST_TAG;

                            break;

                        case MAST_SLASH:
                            if ( !upstrcmp( nw, HSC_MACRO_STR ) )
                                state = MAST_CMACRO;
                            else
                                state = MAST_TAG;
                            break;

                    }


                    if ( state == MAST_TEXT ) {
                        /* add current white spaces & word to macstr */
                        if ( !( app_estr( macstr, infgetcws( inpf ) )
                                && app_estr( macstr, infgetcw( inpf ) )
                        ) )
                            err_mem( inpf );

                    } else if ( state != MAST_TAG ) {

                        /* add current white spaces & word to rmt_str */
                        if ( !( app_estr( rmt_str, infgetcws( inpf ) )
                                && app_estr( rmt_str, infgetcw( inpf ) )
                        ) )
                            err_mem( inpf );

                    } else {

                        /* append rmt_str to macstr, clear rmt_str,
                        ** append current word to macstr
                        */
                        if ( !( app_estr( macstr, estr2str(rmt_str) )
                                && set_estr( rmt_str, "" )
                                && app_estr( macstr, infgetcws( inpf ) )
                                && app_estr( macstr, infgetcw( inpf ) )
                        ) )
                            err_mem( inpf );

                    }
                }
            } else {

                err_eof( inpf );
                state = MAST_ERR;

            }

#if 0
            if ( rmt_check_for( inpf, "<" )
                 &&  rmt_check_for( inpf, "/" )
                 &&  rmt_check_for( inpf, HSC_TAGID )
                 &&  rmt_check_for( inpf, HSC_MACRO_STR )
               )
            {

                ok = TRUE;
                parse_gt( inpf );

            } else {

                if ( !app_estr( macstr, estr2str(rmt_str) ) )
                    err_mem( inpf );
                if ( !set_estr( rmt_str, "" )
                    err_mem( inpf );

            }
#endif

        } /* while */

        /* check for legal end state */
        if ( state == MAST_CMACRO ) {

            ok = parse_wd( inpf, ">" );

        }

    } else
        err_mem( inpf );


    /* release rmt_str */
    del_estr( rmt_str );
    rmt_str = NULL;

    return ( ok );
}

/*
**-------------------------------------
** main handler function for creation
**-------------------------------------
*/


/*
** handle_hsc_macro
**
** define a new macro tag
*/
BOOL handle_hsc_macro( INFILE *inpf, HSCTAG *tag )
{
    BOOL    ok = FALSE;
    BOOL    open_mac;

    /* get name and argumets */
    tag = def_tag_name( deftag, inpf, &open_mac );
    if ( tag ) {

        /* enable macro-flag */
        tag->option |= HT_MACRO;
        DDT( fprintf( stderr, "** def macro %s\n", tag->name ) );

    }

    ok = ( tag && def_tag_args( deftag, tag, inpf, &open_mac ) );

    if ( ok ) {
        /* copy macro text to temp. file */
        ok = read_macro_text( tag, inpf, open_mac );
        if ( ok ) {

            /* set tag handles & flags */
            tag->option |= HT_NOCOPY | HT_IGNOREARGS ;
            tag->o_handle = handle_op_macro;
            tag->c_handle = handle_cl_macro;

        }

    }

    return ( ok );
}

