/*
** ugly/infile.c
**
** ugly input file functions
**
** Version 1.4.2, (W) by Tommy-Saftw�rx in 1995
**
** updated:  3-Oct-1995
** created:  8-Jul-1995
**
** $VER: infile.c 1.4.2 (3.10.1995)
**
*/

/*
** TODO:
**
** - handle errors within expstr (no mem)
** - more elegant handling of inunget( '\n' );
** - assign logstr to user-definable expstr
*/

/*
** includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "expstr.h"
#include "string.h"

#define NOEXTERN_UGLY_FILE_H
#include "infile.h"
#include "memory.h"

/* buffer size for fgets() in infgetc() */
#define INF_FGETS_BUFSIZE 32

/*
** global vars
*/
STRPTR FNAME_STDIN="<stdin>";          /* filename for stdin (CONSTANT) */


/*
** local vars
*/

/* forward references */
BOOL infget_skws( INFILE *inpf );

/*
**
** local functions (not exported into header)
**
*/

/*
** default_whtspc, default_normch
**
** default methods for infgetw() to determine if
** a char is a whitespace or normal char.
*/
BOOL default_whtspc( int ch )
{
    if ( strchr(" \t",ch) != NULL )
        return TRUE;
    else return FALSE;
}

BOOL default_normch( int ch )
{
    if ( isalnum(ch) || (ch=='_') )
        return TRUE;
    else return FALSE;
}

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** reset_inpf
**
** reset a INFILE struct (set all items to NULL)
** (only called by con/destructor)
*/
void reset_infile( INFILE *inpf )
{
    inpf->infile      = NULL;
    inpf->filename    = NULL;
    inpf->lnbuf       = NULL;
    inpf->wordbuf     = NULL;        
    inpf->wspcbuf     = NULL;
    inpf->logstr      = NULL;
    inpf->filepos     = 0;
    inpf->lnctr       = 0;
    inpf->flnctr      = 0;
    inpf->eof_reached = FALSE;
    inpf->out_of_mem  = FALSE;
    inpf->skipped_ws  = FALSE;
    inpf->log_enable  = FALSE;
    inpf->is_nc       = NULL;
    inpf->is_ws       = NULL;
}


/*
** remove INFILE structure
*/
void del_infile( INFILE *inpf )
{
    if ( inpf ) {

        /* close file */
        if ( inpf->infile )
            fclose( inpf->infile );

        /* release mem */
        ufreestr( inpf->filename );
        del_estr( inpf->lnbuf );
        del_estr( inpf->wordbuf );
        del_estr( inpf->wspcbuf );
        del_estr( inpf->logstr );

        /* reset all items */
        reset_infile( inpf );

        /* release whole structure */
        ufree( inpf );
    }

}

/*
** init INFILE structure
*/
INFILE *init_infile( CONSTRPTR name, size_t step_size )
{
    INFILE *inpf = (INFILE *) malloc( sizeof(INFILE) );

    if ( inpf ) {

        /* check for buffer-stepsize */
        if ( !step_size )
            step_size = IF_BUFFER_VALUE;

        /* reset all items */
        reset_infile( inpf );

        /* clone filename (NULL=stdin) */
        if ( name )
            inpf->filename = strclone( name );

        /* init wordbuffers */
        inpf->lnbuf       = init_estr( step_size );
        inpf->wordbuf     = init_estr( step_size );
        inpf->wspcbuf     = init_estr( step_size );
        inpf->logstr      = init_estr( step_size );

        /* check if init ok */
        if ( !( (inpf->filename || !(name))
                && inpf->lnbuf
                && inpf->wordbuf
                && inpf->wspcbuf
                && inpf->logstr ) )
        {
            /* remove infile, */
            /* set return value to NULL */
            del_infile( inpf );
            inpf = NULL;
        }
    }

    return( inpf );
}


/*
**=====================================
**
** exported functions
**
**=====================================
*/


/*
**-------------------------------------
** functions to get info about infile
**-------------------------------------
*/

/*
** infget_x
**
** get colum of current line
*/
ULONG infget_x( INFILE *inpf )
{
    return (inpf->lnctr +1);
}


/*
** infget_y
**
** get colum of current line
*/
ULONG infget_y( INFILE *inpf )
{
    return (inpf->flnctr +1);
}


/*
** infget_skws
**
** get status of skipped_ws
*/
BOOL infget_skws( INFILE *inpf )
{
    return (inpf->skipped_ws);
}


/*
** infget_fname
**
** get name of file
*/
STRPTR infget_fname( INFILE *inpf )
{
    if ( inpf->filename )
        return (inpf->filename);
    else
        return( FNAME_STDIN );
}


/*
** infeof
**
** check if end of input file reached
**
** params: inpf...input file to check
** result: if end of input file reached, 0 is returned,
**         else a value < 0 (compare feof() of some
**         compilers)
*/
int infeof( INFILE *inpf )
{
    if ( inpf->eof_reached == TRUE )
        return EOF;
    else
        return 0;
}

/*
** infget_cws (get current whites spaces)
**
** get string that contains all white-spaces
** skipped within the last call of infgetw()
*/
STRPTR infgetcws( INFILE *inpf )
{
    return ( estr2str( inpf->wspcbuf ) );
}

/*
** infget_cw (get current word)
**
** get string that contains all chars
** read within the last call of infgetw()
*/
STRPTR infgetcw( INFILE *inpf )
{
    return ( estr2str( inpf->wordbuf ) );
}


/*
**-------------------------------------
** functions to set methods
**-------------------------------------
*/

/*
** set_inf_whtspc
**
** set function to check if a char is a whitespace.
** this func is called by infgetw() to determine if
** the begining of a word is reached.
*/
void set_whtspc( INFILE *inpf, BOOL (*iswsfn)( int ch ) )
{
    if ( inpf ) inpf->is_ws = iswsfn;
}


/*
** set_inf_normch
**
** set function to check if a char is a "normal" char.
** this function is called by infgetw() to determine if
** the end of a word is reached
*/
void set_normch( INFILE *inpf, BOOL (*isncfn)( int ch ) )
{
    if ( inpf ) inpf->is_nc = isncfn;
}


/*
**-------------------------------------
** functions for open & close
**-------------------------------------
*/

/*
** infclose1
**
** close INFILE entry, free all mem allocated by it
**
** result: always 0
**
*/
int infclose1( INFILE *inpf )
{
    del_infile( inpf );

    return( 0 );
}


/*
** infopen
**
** params: name.......name of input file to open
**         step_size..portion of memory buffers should be increased with
** result: ptr to INFILE entry or NULL if error;
**
** NOTE: if result = NULL, but errno=0, too, then there
**       wasn't enough mem for the line-buffer. in this
**       case, calling perror() will not produce a proper
**       error message.
**
** NOTE: don't forget to set errno=0 before calling infopen().
*/
INFILE *infopen( CONSTRPTR name, size_t step_size )
{
    INFILE *inpf = init_infile( name, step_size ); /* return file */

    if ( inpf )
        /* open input file or assign stdin to input file */
        if ( name ) {

            inpf->infile = fopen( name, "r" );
            if ( !(inpf->infile) ) {

                del_infile( inpf );
                inpf = NULL;

            }
        } else
            inpf->infile = stdin;


    /* read whole file into file lnbuf */
    if ( inpf ) {

        STRARR buf[ INF_FGETS_BUFSIZE ];
        STRPTR restr = buf; /* result of fgets() (dummy init)*/
        BOOL   ok = TRUE;

        while ( !feof(inpf->infile) && ok ) {

            restr = fgets( buf, INF_FGETS_BUFSIZE, inpf->infile );
            if ( restr )
                ok &= app_estr( inpf->lnbuf, restr );

        }
    }

    return( inpf );                    /* return input file */
}


/*
** infopen_str
**
** open a string as an input file
**
** params: fname......pseudo filename the string should have
**         s..........string that should be handled as a file
**         step_size..portion of memory buffers should be increased with
** result: ptr to INFILE entry or NULL if error;
**
** NOTE: a copy of the passed string is created. so you can
**       modify or release the string after calling inopen_str()
*/
INFILE *infopen_str( CONSTRPTR name, CONSTRPTR s, size_t step_size )
{
    INFILE *inpf = init_infile( name, step_size ); /* return file */

    if ( inpf ) {

        /* copy string to line buffer */
        BOOL ok = set_estr( inpf->lnbuf, s );

        if ( !ok ) {

            del_infile( inpf );
            inpf = NULL;
        }
    }

    return( inpf );                    /* return input file */
}

/*
**-------------------------------------
** functions to get text from infile
**-------------------------------------
*/

/*
** infgetc
**
**
*/
int infgetc( INFILE *inpf )
{
    int result = EOF;

#if 0
    fprintf( stderr, "** ingetch( \"%s\" at %p\n", inpf->filename, inpf );
#endif
    if ( inpf && (!inpf->eof_reached) ) {

        STRPTR lnbuf_str = estr2str( inpf->lnbuf );
        BOOL   ok;          /* result of expstr-funcs */

        /*
        ** if at end of line buffer, scan next line
        ** before proceding
        */
        if ( lnbuf_str[inpf->filepos] == 0 ) {

            inpf->eof_reached = TRUE;

        }

        /*
        ** check wether to return EOF or char from
        ** line buffer
        */
        if ( inpf->eof_reached == FALSE ) {

            lnbuf_str = estr2str( inpf->lnbuf );
            result = lnbuf_str[inpf->filepos];   /* set last char as result */
            if ( result ) {                      /* goto next char in buf */

                inpf->lnctr++;
                inpf->filepos++;

            }
        }

        /* update log-string if neccessary */
        if ( (result != EOF) && (inpf->log_enable) )
            ok = app_estrch( inpf->logstr, result );

        /* update line number */
        if ( result == '\n' )
            inpf->flnctr++;

        if ( !ok )
            /* TODO: handle out of mem */;
    }

    return( result );
}

/*
**-------------------------------------
** functions to unget text from infile
**-------------------------------------
*/

/*
** inungetc
**
** write char back to stream; comparable to ansi's ungetc()
**
** params: inpf...input file
**         ch.....char to write back
** result: ch, if sucessful, else EOF
*/
int inungetc( int ch, INFILE *inpf )
{
    int result = EOF;

    if ( inpf && ( inpf->lnctr ) ) {

        STRPTR lnbuf_str = estr2str( inpf->lnbuf );

        /* update file position */
        inpf->filepos--;

        /* write back char */
        lnbuf_str[inpf->filepos] = ch;
        result = ch;

        /* unget in logstr */
        if ( (inpf->log_enable) && ( estrlen( inpf->logstr ) ) )
            if ( !get_left_estr( inpf->logstr, inpf->logstr, estrlen(inpf->logstr)-1 ) )
                /* TODO: handle out of mem */;

        /* handle LF */
        if ( ch == '\n' ) {

            result = ch;
            inpf->flnctr--;
            inpf->lnctr = 0;

        } else
            inpf->lnctr--;

    }

    return( result );
}

/*
** inungets
** write string back to stream
**
** params: s......string to write back
**         inpf...input file
** result: num of chars written back
*/
size_t inungets( STRPTR s, INFILE *inpf )
{
    size_t ctr = 0;               /* counter, how many chars already written */
    size_t slen = strlen(s);
    STRPTR p=s+(strlen(s)-1);     /* ptr to current char in string */
    int    ch = 0;                /* current char written, dummy init */

    if ( slen > 0 ) {

        ctr = 1;                            /* unget first char */
        ch = inungetc( p[0], inpf );

        while ( (p!=s) && (ch!=EOF) ) {

            ctr++;                          /* inc counter */
            p--;                            /* goto next char */
            ch = inungetc( p[0], inpf );    /* unget current char */

        } 

    }

    return (ctr);
}


/*
** inungetcw
** write current word back to stream
**
** params: inpf...input file
** result: num of chars written back
*/
size_t inungetcw( INFILE *inpf )
{
    size_t ctr; /* counter how many chars written */

    /* unget word */
    ctr = inungets( infgetcw( inpf ), inpf );

    return (ctr);
}

/*
** inungetcwws
** write current word & whitespaces back to stream
**
** params: inpf...input file
** result: num of chars written back
*/
size_t inungetcwws( INFILE *inpf )
{
    size_t ctr; /* counter how many chars written */

    /* unget word & white spaces */
    ctr = inungets( infgetcw( inpf ), inpf );
    ctr += inungets( infgetcws( inpf ), inpf );

    return (ctr);
}

/*
** inf_isws
**
** checks if a char is a white space
**
** params: ch...char to check for white space
** result: TRUE if ch was white space
*/
BOOL inf_isws( char ch, INFILE *inpf )
{
    BOOL   (*isws)(int ch) = inpf->is_ws;

    if (isws==NULL)                    /* if no func for is_ws, */
        isws = default_whtspc;         /*   set default function */

    return ( (*isws)(ch) );
}

/*
** infskip_ws
**
** skip white spaces; update wspbuf; clear wordbuf
**
** TODO: handle wspcbuf-overflow
*/
size_t infskip_ws( INFILE *inpf )
{
    char   nxtch;                      /* next char to read */
    size_t ctr = 0;                    /* num of ws skipped */
    BOOL ok;

    /*
    ** set function to determine if a
    ** specified char is a white space
    */
    inpf->skipped_ws = FALSE;          /* clear skippe-flag */

    /* clear wspcbuf */
    ok = clr_estr( inpf->wspcbuf );

    /*
    ** loop:  skip white spaces
    */
    nxtch = infgetc(inpf);             /* read next char */
    while ( (!infeof(inpf))            /* while not at end of file.. */
            && ok
            && inf_isws(nxtch,inpf) )  /* ..and current char is a whtspc */
    {
        ok &=app_estrch( inpf->wspcbuf, nxtch );
        ctr++;
        nxtch = infgetc(inpf);         /*   read next char */
    }

    if ( ctr )                         /* any whtspcs skipped? */
        inpf->skipped_ws = TRUE;       /* Y-> set skippe-flag */

    if ( !ok )
        /* TODO: error */;

    /*
    ** write back last char read
    */
    inungetc( nxtch, inpf );

    return( ctr );
}




/*
** infgetw
**
** read word
*/
STRPTR infgetw( INFILE *inpf )
{
    /* TODO: handle expstr errors */
    int ch = EOF;
    BOOL   wordread = FALSE;
    STRPTR thisword = NULL;
    BOOL   ok = TRUE;

    /* set function for normal chars */
    BOOL (*isnc)(int ch) = inpf->is_nc;
    if (isnc==NULL) isnc = default_normch;

    /* skip all white spaces */
    infskip_ws( inpf );

    ok = clr_estr( inpf->wordbuf );

    /*
    ** read word until non-normal char is reached
    */
    if ( !infeof(inpf) ) {

        ch = infgetc( inpf );

        if ( ((*isnc)(ch)) )
            do  {

                ok &= app_estrch( inpf->wordbuf, ch );
                ch = infgetc( inpf );
                wordread = TRUE;
                /* todo: set out-of-mem-flag */

            } while ( (ch!=EOF) && ok && ((*isnc)(ch)) );
        else
            ok &= app_estrch( inpf->wordbuf, ch );

        if ( (ch!=EOF) && (wordread) )
            inungetc( ch, inpf );

        if ( ch != EOF );
        thisword = estr2str( inpf->wordbuf );

    } else
        thisword = NULL;

    return ( thisword );
}


/*
** infgotoeol
**
** read all chars until CR or EOF
*/
int infgotoeol( INFILE *inpf )
{
    int ch = EOF;

    if ( !infeof( inpf ) ) {

        /*
        ** read all chars until CR appears
        */
        do {

            ch = infgetc( inpf );

        } while ( (ch > 0) && (ch!=0x0a) );

        /*
        ** read LF
        */
        if ( ch == 0x0a ) {

            ch = infgetc( inpf );
            if ( ch != 0x0d )
                inungetc( ch, inpf );
            ch = 0x0a;
        }

    }

    /*
    ** return last char read
    */
    return (ch);
}

/*
**-------------------------------------
** functions for logstr
**-------------------------------------
*/

/* enable log */
void inflog_enable( INFILE *inpf )
{
    inpf->log_enable = TRUE;
}

/* disable log */
void inflog_disable( INFILE *inpf )
{
    inpf->log_enable = FALSE;
}

/* clear log-string */
BOOL inflog_clear( INFILE *inpf )
{
    /* TODO: handle out of mem */
    return ( clr_estr( inpf->logstr ) );
}

/* append string to log-string */
BOOL inflog_app( INFILE *inpf, STRPTR s )
{
    /* TODO: handle out of mem */
    return ( app_estr( inpf->logstr, s ) );
}

/* get log-string */
STRPTR infget_log( INFILE *inpf )
{
    return ( estr2str( inpf->logstr ) );
}


