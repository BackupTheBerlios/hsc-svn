/*
** hsclib/size.c
**
** evaluate values for WIDTH and HEIGHT from file
**
** Copyright (C) 1996  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 15-May-1996
** created:  7-Jan-1996
*/

#include "hsclib/inc_base.h"

#include "hsclib/uri.h"


/* markers for JFIF/JPEG that contain
** information about image dimension
 */
UBYTE msof[] = {
    /* M_SOF0  */ 0xc0,
    /* M_SOF1  */ 0xc1,
    /* M_SOF2  */ 0xc2,
    /* M_SOF3  */ 0xc3,
    /* M_SOF5  */ 0xc5,
    /* M_SOF6  */ 0xc6,
    /* M_SOF7  */ 0xc7,
    /* M_SOF9  */ 0xc9,
    /* M_SOF10 */ 0xca,
    /* M_SOF11 */ 0xcb,
    /* M_SOF13 */ 0xcd,
    /* M_SOF14 */ 0xce,
    /* M_SOF15 */ 0xcf,
    /* end     */ 0x00 };


/*
**
** global funs
**
*/

/*
** try_set_attr
**
** if attribute exists and it's value is empty, set
** new value and update tag-attribute-string
*/
static VOID try_setattr( HSCPRC *hp, HSCVAR *attr, ULONG value )
{
    if ( attr && !get_vartext( attr ) ) {

        set_vartext( attr, long2str( value ) );

        /* append attribute name and "=" */
        app_estr( hp->tag_attr_str, " " );
        app_estr( hp->tag_attr_str, attr->name );
        app_estr( hp->tag_attr_str, "=" );

        /* append quotes and value */
        if ( (hp->quotemode==QMODE_KEEP) || (hp->quotemode==QMODE_DOUBLE) )
            app_estrch( hp->tag_attr_str, '\"' );
        else if (hp->quotemode==QMODE_SINGLE)
            app_estrch( hp->tag_attr_str, '\'' );
        app_estr( hp->tag_attr_str, long2str( value ) ); /* append value */
        if ( (hp->quotemode==QMODE_KEEP) || (hp->quotemode==QMODE_DOUBLE) )
            app_estrch( hp->tag_attr_str, '\"' );
        else if (hp->quotemode==QMODE_SINGLE)
            app_estrch( hp->tag_attr_str, '\'' );

    }
}

/*
** get_width_height
**
** tries to get values for WIDTH and HEIGHT attributes
** from file
**
** result: TRUE, if filetype has been recognised
*/
BOOL get_attr_size( HSCPRC *hp, HSCTAG *tag )
{
#define BUFSIZE  1024
#define WIDTH_PNG  16 /* file indeces for PiNG */
#define HEIGHT_PNG 20

    HSCVAR *asrc   = tag->uri_size;
    STRPTR  srcuri = NULL;

    if ( asrc )
        srcuri = get_vartext( asrc );
    else {
        panic( "no uri_size" );
    }

    if ( hp->getsize && srcuri && ( uri_kind(srcuri)!=URI_ext ) ) {

        STRARR  buf[BUFSIZE];
        EXPSTR *srcpath  = init_estr(64);
        EXPSTR *imgpath  = init_estr(64);
        ULONG   width    = 0;
        ULONG   height   = 0;
        STRPTR  filetype = NULL;
        FILE   *fref;                    /* file link references to */

        conv_uri2path( imgpath, srcuri );
        estrcpy( srcpath, hp->destdir );
        estrcat( srcpath, imgpath );

        DSZ( fprintf( stderr, DHL "   uri : \"%s\"\n**    path: \"%s\"\n",
            srcuri, estr2str( srcpath ) ) );

        fref = fopen( estr2str( srcpath ), "r" );

        if ( fref ) {

            /* TODO: support JPEG */

            /* fill buffer with zero */
            memset( buf, 0, BUFSIZE );

            /* read buffer  from file */
            fread( buf, BUFSIZE, 1, fref );

            if ( buf[0] == 0xff ) {

                /*
                ** JFIF/JPEG
                */
                BOOL found = FALSE;
                size_t   i = 0;

                while ( !found && ( i< BUFSIZE ) ) {

                    if ( buf[i] == 0xff ) {

                        BOOL is_msof = FALSE;
                        int  j = 0;

                        DSZ( printf( "%04x: %02x %02x: (%02x%02x %02x%02x) ",
                                 i, buf[i], buf[i+1],
                                 buf[i+2], buf[i+3], buf[i+4], buf[i+5] ) );

                        /* check if marker is of required type */
                        while ( !is_msof && msof[j] )
                            if ( buf[i+1] == msof[j] )
                                is_msof = TRUE;
                            else
                                j++;

                        if ( is_msof ) {


                            DSZ( {
                            for ( j=0; j<10; j++ ) {

                                printf( "\n  %-2d: $%02x %-3d",
                                        j, buf[i+j], buf[i+j] );
                                if ( buf[i+j] >= 32 )
                                    printf( " '%c'", buf[i+j] );

                            }} );

                            filetype = "JPG";
                            width  = buf[i+8] + (buf[i+7]<<8);
                            height = buf[i+6] + (buf[i+5]<<8);
                            found = TRUE;

                        } else {
                            DDA( printf( "ignore\n" ) );
                        }
                    }

                    i++;

                }

            } else if ( !strncmp( "GIF", buf, 3 ) ) {

                /*
                ** GIF
                */
                LONG use_global_colormap = ( buf[10] & 0x80 ) >> 7;
                LONG pixeldepth          = ( buf[10] & 0x07 ) + 1;
                LONG startimg            =
                     13 + use_global_colormap * 3 * ( 1 << pixeldepth );


                filetype = "GIF";
                width  = buf[startimg+5] + 256*buf[startimg+6];
                height = buf[startimg+7] + 256*buf[startimg+8];

                DDA( fprintf( stderr, DHL "  width : %lu\nheight: %lu\n",
                                     width, height ) );

            } else if ( !strncmp( "PNG", &(buf[1]), 3 ) ) {

                /*
                ** PNG
                */
                filetype = "PNG";
                width  = 0x00800000*buf[WIDTH_PNG]   +
                         0x00010000*buf[WIDTH_PNG+1] +
                         0x00000100*buf[WIDTH_PNG+2] +
                         0x00000001*buf[WIDTH_PNG+3];
                height = 0x00800000*buf[HEIGHT_PNG]   +
                         0x00010000*buf[HEIGHT_PNG+1] +
                         0x00000100*buf[HEIGHT_PNG+2] +
                         0x00000001*buf[HEIGHT_PNG+3];

                DSZ( {
                    int i;
                    for ( i=0; i<BUFSIZE; i++ ) {

                        fprintf( stderr, "%-2d: $%02x %-3d", i, buf[i], buf[i] );
                        if ( buf[i] >= 32 )
                            fprintf( stderr, " '%c'\n", buf[i] );
                        else
                            fprintf( stderr, "\n" );

                    }}
                );

                DDA( fprintf( stderr, DHL "  width : %lu\nheight: %lu\n",
                                     width, height ) );

            } else {

                /* unknown file type */
                hsc_message( hp, MSG_UNKN_FILETYPE,
                             "filetype of %q not recognised",
                             estr2str( srcpath ) );

            }

            DSZ( fprintf( stderr, DHL "  size: \"%s\" (%dx%d)\n",
                        filetype, width, height ) );

            fclose( fref );

        } else {
            DSZ( fprintf( stderr, DHL "  Can't open image \"%s\"\n",
                        estr2str( srcpath ) ) );
        }

        /* set values */
        if ( height && width ) {

            HSCVAR *awidth  = find_varname( tag->attr, "WIDTH" );
            HSCVAR *aheight = find_varname( tag->attr, "HEIGHT" );

            /* status message */
            app_estr( srcpath, ": " );
            app_estr( srcpath, filetype );
            app_estr( srcpath, ", " );
            app_estr( srcpath, long2str( width ) );
            app_estr( srcpath, "x" );
            app_estr( srcpath, long2str( height ) );
            hsc_status_misc( hp, estr2str( srcpath ) );

            try_setattr( hp, awidth , width );
            try_setattr( hp, aheight, height );

        }

        /* free local resources */
        del_estr( srcpath );
        del_estr( imgpath );
    }

    return (TRUE);
}


