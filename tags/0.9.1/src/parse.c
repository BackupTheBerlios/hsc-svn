/*
** parse.c
**
** parse file: handle for entities & tags
**
** updated:  8-Oct-1995
** created:  1-Jul-1995
**
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/prginfo.h"
#include "ugly/infile.h"
#include "ugly/dllist.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"
#include "skip.h"
#include "status.h"
#include "tagargs.h"

#include "entity.h"
#include "tag.h"
#include "vars.h"

#define NOEXTERN_PARSE_H
#include "parse.h"

INFILE *parse_end_file; /* file for do_list-methods called in parse_end() */

/*
**---------------------------
** overloaded methods for
** INFILE class
**---------------------------
*/

/*
** hsc_normch
**
** decides if an char is an 'normal' char
**
** params: ch...char to test
** result: TRUE, if ch is a 'normal' ch
**
** NOTE: this function is used as is_nc-methode
**       for the infile class
*/
BOOL hsc_normch( int ch )
{
    if ( isalnum(ch) || (ch=='_') || (ch=='-') )
        return TRUE;
    else return FALSE;
}

/*
** hsc_whtspc
**
** decides if an char is a white-space
**
** params: ch...char to test
** result: TRUE, if ch is a 'normal' ch
**
** NOTE: this function is used as is_ws-methode
**       for the infile class
*/
BOOL hsc_whtspc( int ch )
{
    if ( strchr(" \t",ch) != NULL )
        return TRUE;
    else return FALSE;
}

/*
**---------------------------
** misc. functions
**---------------------------
*/

/*
** message_rplc
**
** message that tells user that a special char
** was replaced by its entity
*/

void message_rplc( STRPTR what, STRPTR by, INFILE *inpf )
{
    message( MSG_RPLC_ENT, inpf );
    errstr( "replaced " );
    errqstr( what );
    errstr( " by " );
    errqstr( by );
    errlf();
}

/*
**---------------------------
** parse tag functions
**---------------------------
*/

/*
** parse_tag
**
** parse tag (after "<")
*/
BOOL parse_tag( INFILE *inpf)
{

    STRPTR  nxtwd;
    DLNODE *nd    = NULL;
    HSCTAG *tag   = NULL;
    ULONG   tci   = 0;   /* tag_call_id returned by set_tag_args() */
    BOOL   (*hnd)(INFILE *inpf, HSCTAG *tag) = NULL;
    BOOL    open_tag;
    DLLIST *taglist = deftag;
    BOOL    rplc_lt = FALSE;  /* TRUE, if replace spc. char "<" */

    if ( smart_ent && strlen( infgetcws( inpf ) ) ) {

        /*
        ** check for special char "<"
        */
        int ch = infgetc( inpf );

        if ( hsc_whtspc( ch ) ) {

            rplc_lt = TRUE;

            /* write "&lt;" and white spaces */
            message_rplc( "<", "&lt;", inpf );
            outstr( infgetcws( inpf ) );
            outstr( "&lt;" );
            outch( ch );

        } else
            inungetc( ch, inpf );

    }

    if ( !rplc_lt ) {

        /* get tag id */
        nxtwd = infget_tagid( inpf );

        if ( !fatal_error ) {


            /* check for hsctag; if not, enable output */
            if ( upstrncmp( nxtwd, HSC_TAGID, strlen(HSC_TAGID) ) )
                suppress_output = FALSE;         /* enable output */
            if ( !suppress_output )
                D( fprintf( stderr, "** tag <" ) );

        }
    }

    if ( !fatal_error && !rplc_lt ) {

        BOOL write_tag = FALSE; /* flag: write tag text & attrs to output? */

        if ( strcmp( "/", nxtwd ) ) {            /* is it a closing tag? */

            /*
            **
            ** process non-closing tag
            **
            */

            open_tag = TRUE;
            if ( !suppress_output )
                D( fprintf( stderr, "%s>\n", nxtwd ) );

            /* search for tag in list */
            nd = find_dlnode( taglist->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                message( MSG_UNKN_TAG, inpf );      /* tag not found */
                errstr( "Unknown tag " );
                errtag( nxtwd );
                errlf();

                skip_until_eot( inpf );

            } else {

                tag = (HSCTAG*)nd->data;

                /* set handle */
                hnd = tag->o_handle;


                /*
                ** handle options
                */

                /* check for obsolete tag */
                if ( tag->option & HT_OBSOLETE ) {

                    message( MSG_TAG_OBSOLETE, inpf );
                    errstr( "tag " );
                    errtag( nxtwd );
                    errstr( " is obsolete\n" );

                }

                /* check for jerk-tag */
                if ( tag->option & HT_JERK ) {

                    message( MSG_TAG_JERK, inpf );
                    errstr( "tag " );
                    errtag( nxtwd );
                    errstr( " is only used by jerks\n" );

                }

                /* only-once-tag occured twice? */
                if ( (tag->option & HT_ONLYONCE ) && (tag->occured) ) {

                    message( MSG_TAG_TOO_OFTEN, inpf );
                    errstr( "tag " );
                    errtag( nxtwd );
                    errstr( "occured too often\n" );

                }

                /* set occured-flag */
                if ( tag->option & (HT_ONLYONCE | HT_REQUIRED) )
                    tag->occured = TRUE;

                /* closing tag required? */
                if ( tag->option & HT_CLOSE ) {

                    if ( !app_dlnode( cltags, (APTR) tag->name ) )
                        err_mem( inpf );
                }
            }

        } else {

            /*
            **
            ** process closing tag
            **
            */

            /* get tag id */
            nxtwd = infget_tagid( inpf );           /* get tag id */
            open_tag = FALSE;

            if ( !suppress_output )
                D( fprintf( stderr, "/%s>\n", nxtwd ) );

            /* search for tag in taglist */
            /* (see if it exists at all) */
            nd = find_dlnode( taglist->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                /* closing tag is absolutely unknown */
                message( MSG_UNKN_TAG, inpf );      /* tag not found */
                errstr( "Unknown closing tag " );
                errctag( nxtwd );
                errlf();

                skip_until_eot( inpf );

            } else {

                tag = (HSCTAG*)nd->data; /* fitting tag in taglist */

                /* set closing handle */
                hnd = tag->c_handle;

                /* remove closing tag from stack */
                remove_ctag( tag, inpf );
#if 0
                /* search for tag on stack of occured tags */
                nd = find_dlnode( cltags->first, (APTR) nxtwd, cmp_strctg );
                if ( nd == NULL ) {

                    /* closing tag not found on stack */
                    /* ->unmatched closing tag without previous opening tag */
                    message( MSG_UNMA_CTAG, inpf );
                    errstr( "Unmatched closing tag " );
                    errctag( nxtwd );
                    errlf();

                } else {

                    /* closing tag found on stack */
                    STRPTR foundnm = (STRPTR) nd->data;
                    STRPTR lastnm  = (STRPTR) cltags->last->data;

                    /* check if name of closing tag is -not- equal
                    /* to the name of the last tag last on stack */
                    /* ->illegal tag nesting */
                    if ( upstrcmp(lastnm, foundnm)
                         && !(tag->option | HT_MACRO) )
                    {

                        message( MSG_CTAG_NESTING, inpf );
                        errstr( "Illegal closing tag nesting (expected " );
                        errctag( lastnm );
                        errstr( ", found " );
                        errctag( nxtwd );
                        errch( ')' );
                        errlf();

                    }

                }

                /* remove node for closing tag from cltags-list */
                del_dlnode( cltags, nd );
#endif
            }
        }

        /*
        ** processed for opening AND closing tag
        */

        /* set attributes */
        if ( tag && !(tag->option & HT_IGNOREARGS) )
            tci = set_tag_args( tag, inpf, open_tag );
        else if ( !hnd )
            skip_until_eot( inpf );

        write_tag = ( !(tag) || !(tag->option & HT_NOCOPY) );

        /* call handle if available */
        if ( hnd && !fatal_error )
            (*hnd)( inpf, tag );

        /* write whole tag out */
        if ( write_tag )
            outstr( infget_log( inpf ) );

        /* skip LF if requested */
        if ( tag && (tag->option & HT_SKIPLF) )
            skip_lf( inpf );
    }

    return (BOOL)( !fatal_error );
}

/*
**---------------------------
** other parse functions
**---------------------------
*/

/*
** parse_amp
**
** parse ampersand ("&")
*/
BOOL parse_amp( INFILE *inpf )
{
    if ( !fatal_error ) {

        BOOL   rplc = smart_ent;       /* TRUE, if "&" should be replaced */
        STRPTR txtout = NULL;          /* text to write to output */

        suppress_output = FALSE;

        if ( rplc ) {

            /*
            ** test if char before and
            ** after ">" is white-space
            */
            int ch = infgetc( inpf );

            inungetc( ch, inpf );

            if ( !( hsc_whtspc( ch ) && strlen( infgetcws( inpf ) ) ) )
                rplc = FALSE;

        }

        if ( rplc ) {

            /* replace ampersand */
            message_rplc( "&", "&amp;", inpf );
            txtout = "&amp;";

        } else {

            /*
            ** get entity-id, test for unknwon entity
            */
            char *nxtwd;
            char nxtch;
            DLNODE *nd;

            /* get entity id */
            nxtwd = infgetw( inpf );

            if ( !strcmp( nxtwd, "#" ) ) {

                /*
                ** process numeric entity
                */
                nxtwd = infgetw( inpf );
                errno = 0;
                strtoul( nxtwd, NULL, 0 );
                if ( errno || strlen( infgetcws( inpf ) ) ) {

                    message( MSG_ILLEGAL_NUM, inpf );
                    errstr( "Illegal numeric entity " );
                    errqstr( nxtwd );
                    errlf();

                }

            } else {

                /*
                ** process text entity
                */

                /* search for entity in list */
                nd = find_dlnode( defent->first, (APTR) nxtwd, cmp_strent );
                if ( nd == NULL ) {

                    message( MSG_UNKN_ENTITY, inpf );
                    errstr( "Unknown entity " );
                    errqstr( nxtwd );
                    errlf();

                }
            }

            /* check for closing ';' */
            nxtch = infgetc( inpf );
            if ( nxtch != ';' ) {

                message( MSG_EXPT_SEMIK, inpf );
                errqstr( ";" );
                errstr( " expected (found: " );
                errqstr( ch2str( nxtch ) );
                errstr( ")\n" );
            }

            /* set output text */
            txtout = infget_log( inpf );

        }

        /* output whole entity */
        outstr( txtout );
    }

    return (BOOL)( !fatal_error );
}

/*
** parse_text
*/
BOOL parse_text( INFILE *inpf )
{
    STRPTR  nw = infgetcw( inpf );

    suppress_output = FALSE;

    /*
    ** check unmatched ">"
    */
    if ( !strcmp(nw, ">") ) {          

        BOOL rplc = smart_ent; /* TRUE, if ">" should be replaced */

        if ( rplc ) {

            /*
            ** test if char before and
            ** after ">" is white-space
            */
            int ch = infgetc( inpf );

            inungetc( ch, inpf );

            if ( !( hsc_whtspc( ch ) && strlen( infgetcws( inpf ) ) ) )
                rplc = FALSE;

        }

        if ( rplc ) {

            /* replace gt */
            message_rplc( nw, "&gt;", inpf );
            nw = "&gt;";

        } else {

            message( MSG_UNMA_GT, inpf );
            errstr( "unmatched \">\"\n" );

        }

    /*
    ** check for quote
    */
    } else if ( !strcmp( nw, "\"" ) ) {

        if ( smart_ent ) {

            /* replace quote */
            message_rplc( nw, "&quot;", inpf );
            nw = "&quot;";

        } else {

            message( MSG_UNMA_QUOTE, inpf );
            errstr( "quote found inside text" );
            errlf();

        }


    /*
    ** check for entities to replace
    */
    } else if ( rplc_ent ) {

        DLNODE *nd = find_dlnode( defent->first, (APTR) nw, cmp_rplcent );

        if ( nd ) {

            BOOL ok = TRUE;

            /* copy replaced entity to buffer */
            ok &= set_estr( tmpstr, "&" );
            ok &= app_estr( tmpstr, ((HSCENT*)nd->data)->name );
            ok &= app_estr( tmpstr, ";" );

            if ( ok ) {

                /* replace-message */
                message_rplc( nw, estr2str( tmpstr ), inpf );
                nw = estr2str( tmpstr );

            }
        }
    }

    if ( nw )
        outstr( nw );                  /* output word */

    return (BOOL)( !fatal_error );
}

/*
** parse_hsc
**
** parse input chars with full hsc support
**
** params: inpf...input file
**
** result: TRUE, if no error
*/
BOOL parse_hsc( INFILE *inpf )
{
    if ( !fatal_error ) {
        char *nxtwd;

        nxtwd = infgetw( inpf );

        if ( nxtwd ) {

            /* write white-spaces */
            outstr( infgetcws( inpf ) );

            /*
            ** clear and enable log,
            ** append current word WITHOUT white-spaces to log
            */
            if ( !inflog_clear( inpf ) || !inflog_app( inpf, nxtwd ) )
                err_mem( inpf );
            else
                inflog_enable( inpf );


            if ( !strcmp(nxtwd, "<") )                     /* parse tag */
                parse_tag( inpf );
            else if ( !strcmp(nxtwd, "&") )                /* parse entity */
                parse_amp( inpf );

            else {                                         /* handle text */
                parse_text( inpf );
            }
        }
        inflog_disable( inpf );

    }

    return (BOOL)( !fatal_error );
}

/*
**---------------------------
** parse end functions
**---------------------------
*/

/*
** check_tag_missing
**
** checks if a required tag has not occured
** (methode for do_dllist())
*/
void check_tag_missing( APTR data )
{
    HSCTAG *tag = (HSCTAG *)data;

    if ( (tag->option & HT_REQUIRED
         && (tag->occured == FALSE) ) )
    {
        message( MSG_MISS_REQTAG, parse_end_file );
        errstr( "required tag missing: " );
        errtag( tag->name );
        errlf();

    }
}

/*
** check_not_closed
**
** check if a closing tag is missing
** (methode for do_dllist())
*/
void check_not_closed( APTR data )
{
    STRPTR name = (STRPTR)data;

    message( MSG_MISS_CTAG, parse_end_file );
    errstr( "closing tag missing: " );
    errctag( name );
    errlf();

}

/*
** parse_end
**
** check for all tags closed and required
** tags occured
*/
BOOL parse_end( INFILE *inpf )
{
    if ( !fatal_error ) {

        parse_end_file = inpf;

        /* check for tags not closed */
        do_dllist( cltags, check_not_closed );

        /* check for required tags missing */
        do_dllist( deftag, check_tag_missing );

    }

    return (BOOL)( !fatal_error );
}

/*
**---------------------------
** main include function
**---------------------------
*/


/*
** include_hsc
**
** read from inpf, parse for hsc-commands and execute them,
** check for html-error,
** write all out to outf and close input file.
**
** params: inpfnm...input file name
**         outf.....output file structure, already opended
**
** result: TRUE, if all worked well, else FALSE
*/
BOOL include_hsc( STRPTR filename, INFILE *inpf, FILE *outf, ULONG optn )
{

    BOOL    ok;            /* result */
    ok   = (inpf!=NULL);

    if ( inpf ) {                                /* file opened? */

        inpf->is_nc = hsc_normch;                /*    set is_nc-methode */
        inpf->is_ws = hsc_whtspc;                /*    set is_ws-methode */

        while ( !infeof(inpf) && ok ) {          /*    parse file */

            if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC) ) )
                status_infile( inpf, FALSE );    /*    status message */
            ok = parse_hsc( inpf );
        }

        if ( ok && (optn & IH_PARSE_END) ) {     /*    parse end */
                                                 /*    (unclosed tags etc) */
            ok = parse_end( inpf );

        }

        /* end of file status */
        if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)) ) {

            status_infile( inpf, TRUE );         /*    status message */
            status_lf();                         /*    (display num of line) */

        }

        infclose( inpf );                        /*    close file */
                                                 
    } else {                                     /* N-> error message */

        message( MSG_NO_INPUT, NULL );
        errstr( "can not open " );
        errqstr( filename );
        errstr( " for input: " );
        errstr( strerror( errno ) );
        errlf();
        ok = FALSE;
    }

    return ( ok );
}

/*
** include_hsc_file
**
** open input file and include it
*/
BOOL include_hsc_file( STRPTR filename, FILE *outf, ULONG optn )
{
    INFILE *inpf;
    BOOL    ok;
    STRARR msg[ MAX_PATHLEN ];

    /* status message: reading input */
   if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)) ) {

        sprintf( msg, "Reading \"%s\"", filename );
        status_msg( msg );

    }

    /* open & read input file */
    inpf = infopen( filename, ES_STEP_INFILE );

    /* include opened file */
    ok = include_hsc( filename, inpf, outf, optn );

    return( ok );
}

/*
** include_hsc_str
**
** open input file and include it
*/
BOOL include_hsc_string( STRPTR filename, STRPTR s, FILE *outf, ULONG optn )
{
    INFILE *inpf;
    BOOL    ok;

    inpf = infopen_str( filename, s, 0 );        /* try to open input file */
    ok = include_hsc( filename, inpf, outf, optn );

    return( ok );
}

