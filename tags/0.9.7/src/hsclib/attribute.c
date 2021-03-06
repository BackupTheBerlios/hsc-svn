/*
** hsclib/attribute.c
**
** hsc-variable funcs for hsc
**
** Copyright (C) 1995,96  Thomas Aglassinger
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
** updated: 11-Apr-1996
** created:  2-Sep-1995
**
*/


#define NOEXTERN_HSCLIB_ATTRIBUTE_H

#include "hsclib/inc_base.h"

#include "hsclib/eval.h"
#include "hsclib/uri.h"

/*
**-------------------------------------
** debugging functions
**-------------------------------------
*/

/*
** ptr_var
*/
static VOID prt_var( FILE *stream, APTR data )
{
    HSCVAR *var = (HSCVAR*) data;

    if ( var ) {

        int varquote = var->quote;
        if ( varquote == VQ_NO_QUOTE )
            varquote = '.';

        /* name & type & macro_id */
        fprintf( stream, "%s (type:%u,mci:%lu) ",
                 var->name, var->vartype, var->macro_id );
        /* text value */
        if ( var->text )
            fprintf( stream, "cur:%c%s%c",
                     var->quote, var->text, varquote );
        else
            fprintf( stream, "<NULL>" );
        fprintf( stream, " " );
        /* default text */
        if ( var->deftext )
            fprintf( stream, "def:%c%s%c",
                     var->quote, var->deftext, varquote );
        else
            fprintf( stream, "<NULL>" );
        fprintf( stream, "\n" );


    } else
        fprintf( stream, "<NULL>\n" );
}

VOID prt_varlist( DLLIST *varlist, STRPTR title )
{
    fprintf( stderr, DHL "%s\n", title );
    fprintf_dllist( stderr, varlist, prt_var );
}

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** del_hscattr
**
** delete attribute
*/
VOID del_hscattr( APTR data )
{
    HSCVAR *var = (HSCVAR *)data;

#if DEBUG_ATTR
    fprintf( stderr, DHL "   del_attr %s (mci=%d)\n",
             var->name, var->macro_id );
#endif

    /* release mem */
    ufreestr( var->name );
    ufreestr( var->deftext );
    ufreestr( var->text );
    ufreestr( var->enumstr );

    var->macro_id = 0;
    var->varflag  = 0;
    var->vartype  = VT_NONE;
    var->quote    = EOF;

    ufree( var );

}

/*
** new_hscattr
**
** alloc & init a new variable
*/
HSCVAR *new_hscattr( STRPTR newname )
{

    HSCVAR *newvar = (HSCVAR*) umalloc( sizeof(HSCVAR) );

#if DEBUG_ATTR
    fprintf( stderr, DHL "   new_attr %s\n", newname );
#endif

    if (newvar) {

        /* init new varument item */
        newvar->vartype   = VT_NONE;
        newvar->name      = strclone( newname );
        newvar->deftext   = NULL;
        newvar->text      = NULL;
        newvar->enumstr   = NULL;
        newvar->macro_id  = 0;
        newvar->varflag   = 0;
        newvar->quote     = EOF;

    }

    if ( !( newvar->name ) ) {

        del_hscattr( (APTR) newvar );
        newvar = NULL;

    }

    return (newvar);

}


/*
** cpy_hscattr
**
** create copy of an already existing attribute
*/
HSCVAR *cpy_hscattr( HSCVAR *oldvar )
{
    HSCVAR *newvar = new_hscattr( oldvar->name );

    if ( newvar ) {

        newvar->vartype   = oldvar->vartype;
        if ( oldvar->deftext )
            newvar->deftext   = strclone( oldvar->deftext );
        if ( oldvar->text )
            newvar->text   = strclone( oldvar->text );
        if ( oldvar->enumstr )
            newvar->enumstr   = strclone( oldvar->enumstr );
        newvar->macro_id  = oldvar->macro_id;
        newvar->varflag   = oldvar->varflag;
        newvar->quote     = oldvar->quote;

    }

    if ( !( newvar->name ) ) {

        del_hscattr( (APTR) newvar );
        newvar = NULL;

    }

    return (newvar);

}


/*
** app_var
**
** append a new var to the var list AT BEGINNING
*/
HSCVAR *app_var( DLLIST *varlist, STRPTR newname )
{
    HSCVAR *var = new_hscattr( newname );
    BOOL    ok = FALSE;

    if ( var )
        ok = ( ins_dlnode( varlist, varlist->first, (APTR) var ) != NULL );

    if ( !ok ) {
        del_hscattr( (APTR) var );
        var = NULL;
    }

    return ( var );
}


/*
**-------------------------------------
** search functions
**-------------------------------------
*/


/*
** cmp_varname
**
** compares a var-name with the name
** of a HSCVAR-entry
*/
int cmp_varname( APTR cmpstr, APTR vardata )
{
    STRPTR varstr = NULL;

    if ( vardata )
        varstr = ((HSCVAR*)vardata)->name;

    if (varstr)
        if ( !upstrcmp( cmpstr, varstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}

/*
** find_attrnode
*/
DLNODE *find_attrnode( DLLIST *varlist, STRPTR name )
{
    DLNODE *nd = find_dlnode( varlist->first, (APTR) name, cmp_varname );

    return ( nd );
}

/*
** find_varname
*/
HSCVAR *find_varname( DLLIST *varlist, STRPTR name )
{
    DLNODE *nd = find_dlnode( varlist->first, (APTR) name, cmp_varname );
    HSCVAR *var = NULL;

    if ( nd )
        var = (HSCVAR*)nd->data;

    return ( var );
}


/*
**-------------------------------------
** misc. functions
**-------------------------------------
*/

/*
** set_vartext
**
** set value of a var
**
** params: var......var to set
**         newtext..new text to set
** result: value of new text set
** errors: return NULL if out of mem, display error message
*/
STRPTR set_vartext( HSCVAR *var, STRPTR newtext )
{
    if ( newtext != var->text ) {

        ufreestr( var->text );
        var->text = NULL;
        if ( newtext )
            var->text = strclone( newtext );

    }

    return (var->text);
}


/*
** set_vartext_bool
**
** set value of a boolean attr
**
** params: var....var to set
**         value..new value to set
** result: value
** errors: if out of mem, display error message
*/
BOOL set_vartext_bool( HSCVAR *attr, BOOL value )
{
    if ( value )
        set_vartext( attr, VC_TRUE );
    else
        set_vartext( attr, VC_FALSE );

    return ( value );
}


/*
** clr_vartext
**
** clear or reset var to default text
**
** params: var..var to clear
** result: TRUE, if new value set correctly
*/
BOOL clr_vartext( HSCVAR *var )
{
    BOOL ok = TRUE;

    if ( var->deftext ) {
        if ( !(set_vartext( var, var->deftext )) )
            ok = FALSE;
    } else {

        ufreestr( var->text );
        var->text = NULL;

    }

    return( ok );
}

/*
** clr_attrdef
**
** clear attributes default text
**
** params: attr..attr to clear
*/
VOID clr_attrdef( HSCVAR *attr )
{
    ufreestr( attr->deftext );
    attr->deftext = NULL;
}

/*
** clr_varlist
**
** clear all vars to default text
**
** params: varlist..varlist to clear
*/
BOOL clr_varlist( DLLIST *varlist )
{
    DLNODE *nd = varlist->first;
    BOOL    ok = TRUE;

    while ( nd && ok ) {

        ok &= clr_vartext( (HSCVAR*) nd->data );
        nd = nd->next;
    }

    return( ok );
}

/*
** clr_varlist_bool
**
** set all "empty" (=NULL) boolean attributes of
** an attr list to FALSE
**
** params: varlist..varlist to process
*/
VOID clr_varlist_bool( DLLIST *varlist )
{
    DLNODE *nd = varlist->first;
    BOOL    ok = TRUE;

    while ( nd && ok ) {

        HSCVAR *attr = (HSCVAR*) nd->data;

        if ( ( attr->vartype == VT_BOOL ) && !( attr->text ) )
            set_vartext_bool( attr, FALSE );

        nd = nd->next;
    }
}

/*
** get_vartext_byname: get text value of a var
**
**
*/
STRPTR get_vartext_byname( DLLIST *varlist, STRPTR name )
{
    HSCVAR *var     = find_varname( varlist, name );
    STRPTR  vartext = NULL;

    if ( var )
        vartext = var->text;

    return( vartext );
}

/*
** get_vartext: get text value of a var
**
*/
STRPTR get_vartext( HSCVAR *var )
{
    STRPTR text = NULL;

    if ( var )
        text = var->text;

    return( text );
}

/*
** get_varbool: get valueof a boolean attr
*/
BOOL get_varbool( HSCVAR *attr )
{
    BOOL set = FALSE;
    if ( attr && ( attr->text[0] ) )
        set = TRUE;

    return( set );
}

/*
** get_varbool_byname: get value of a boolean var
*/
BOOL get_varbool_byname( DLLIST *varlist, STRPTR name )
{
    HSCVAR *var  = find_varname( varlist, name );

    return( get_varbool( var ) );
}


/*
** get_vardeftext: get default text value of a var
**
*/
STRPTR get_vardeftext( HSCVAR *var )
{
    STRPTR deftext = NULL;

    if ( var )
        deftext = var->deftext;

    return( deftext );
}
