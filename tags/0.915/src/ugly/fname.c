/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1997  Thomas Aglassinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * ugly/fname.c - filename processing functions
 *
 * updated: 22-Sep-1997
 * created: 24-May-1994
 *
 *-------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

#define NOEXTERN_UGLY_FNAME_H
#include "fname.h"

/*
 * get_fext
 *
 * get filename extension
 *
 * params: fn...full filename to examine
 * result: extension of _fn or NULL, if fn was also NULL
 *
 */
BOOL get_fext(EXPSTR * dest, CONSTRPTR fn)
{
    CONSTRPTR fn_ext = fn;      /* result var */

    /* search for end of string */
    while (fn_ext[0])
        fn_ext++;

    /* search string backwards for "." or PATH_SEPARATOR */
    do
        /* nufin */ ;
    while ((fn_ext != fn)       /* beginning of name reached? */
           && (fn_ext--)        /* process next char */
           && (fn_ext[0] != '.')
           && (strchr(PATH_SEPARATOR, fn_ext[0]) == NULL));

    /* copy extension to dest */
    if (fn_ext[0] == '.')
        set_estr(dest, ++fn_ext);
    else
        clr_estr(dest);

    return (ok_fnl_fext(dest));
}

/*
 * get_fname
 *
 *
 */
BOOL get_fname(EXPSTR * dest, CONSTRPTR fn)
{
    CONSTRPTR fn_name = fn;

    fn_name = ustrrpbrk(fn_name, PATH_SEPARATOR);

    /* copy extension to dest */
    if (fn_name)
        set_estr(dest, ++fn_name);
    else
        set_estr(dest, fn);

    return (ok_fnl_fname(dest));
}

/*
 * get_fpath
 *
 *
 */
BOOL get_fpath(EXPSTR * dest, CONSTRPTR fn)
{
    STRPTR pa_name = ustrrpbrk(fn, PATH_SEPARATOR);

    if (pa_name)
    {
        set_estrn(dest, fn, strlen(fn) - strlen(pa_name) + 1);
    }
    else
        clr_estr(dest);

    return (ok_fnl_fpath(dest));
}

/*
 * get_fsdir: get next subdirectory (including separator)
 *
 */
BOOL get_fsdir(EXPSTR * dest, CONSTRPTR fn)
{
    STRPTR fn_name = strpbrk(fn, PATH_SEPARATOR);

    if (fn_name)
    {
        set_estrn(dest, fn, strlen(fn) - strlen(fn_name) + 1);
    }
    else
        clr_estr(dest);

    return (ok_fnl_fpath(dest));
}

/*
 * get_fdrive
 *
 *
 */
BOOL get_fdrive(EXPSTR * dest, CONSTRPTR fn)
{
    set_estr(dest, fn);         /* dummy data */
    clr_estr(dest);
    return (ok_fnl_fpath(dest));
}

/*
 * fextidx: get filename extension index of string
 *
 * params: fn...filename to examine
 * result: index in _fn, where extension starts
 *
 * NOTE: if _fn has an extension at all, _fn[i] is equal to '.', else
 *       _fn has no extension (e.g. "TESTFILE") and _fn[i] is equal
 *       to '\0'.
 *
 * (internal function)
 *
 */
static size_t fextidx(CONSTRPTR fn)
{
    size_t i;                   /* string index counter */

    i = strlen(fn) - 1;         /* scan string backwards... */
    while ((i) && (fn[i] != '.')
           && (strchr(PATH_SEPARATOR, fn[i]) == NULL))
        i--;
    if (fn[i] != '.')
        i = strlen(fn);

    return i;                   /* return result */
}

/*
 * clr_fext: clear filename extension
 *
 * params: dest...string that contains filename to strip
 *
 * EXAMPLE: "testfile.txt" -> "testfile"
 *
 */
BOOL clr_fext(EXPSTR * dest)
{
    BOOL ok;
    size_t extidx = fextidx(estr2str(dest));
    /* index, where extension starts */
    ok = set_estrn(dest, estr2str(dest), extidx);
    ok &= ok_fnl_fpath(dest);

    return (ok);
}

/*
 * set_fext: set new (last) extension for filename
 *
 * params: dest....string that contains old filename
 *         newext..new extension to set
 *
 * EXAMPLE: "testfile.txt", "lha" -> "testfile.lha"
 *          "hugo.tar.gz" , "lha" -> "hugo.tar.lha"
 */
BOOL set_fext(EXPSTR * dest, CONSTRPTR newext)
{
    BOOL ok;

    ok = clr_fext(dest);
    ok &= app_estrch(dest, '.');
    ok &= app_estr(dest, newext);

    ok &= ok_fnl_fpath(dest);

    return (ok);
}

/*
 * app_fext: append extension to filename
 *
 * EXAMPLE: ("testfile.txt", "lha") -> "testfile.txt.lha"
 *                                  -> "testfile.lha"     (msdos)
 */
BOOL app_fext(EXPSTR * dest, CONSTRPTR newext)
{
    BOOL ok = app_estrch(dest, '.');

    ok &= app_estr(dest, newext);
    ok &= ok_fnl_fpath(dest);

    return (ok);
}

/*
 * set_fextIdx: set file name index extension
 *
 * params: dest..destination string with new extension
 *         fn....source string with extension to replace
 *         idx...extension index
 */
BOOL set_fnameIdx(EXPSTR * dest, int idx)
{
    char fn_ext[12];            /* index extension */
    sprintf(fn_ext, "%03d", idx);       /* create index string */

    return (set_fext(dest, fn_ext));
}

/*
 * link_fname: link directory and filename together
 *
 * params: dest..where to store result
 *         dir...directoryname
 *         fn....filename to append
 *
 * NOTE: a PATHSEPARATOR[0] is append to dir, if none exists
 * NOTE: dir and fn MUST NOT be part of dest,
 *       when invoking this funtion
 */
BOOL link_fname(EXPSTR * dest, STRPTR dir, STRPTR fn)
{
    BOOL anydir;                /* TRUE, if any dir passed as arg */
    BOOL ok = TRUE;

    /* is a dir passed? */
    anydir = (dir != NULL);
    if (anydir)
        anydir = (strlen(dir) != 0);

    if (anydir)
    {
        /* clone dir, if any
         *
         * NOTE: it's neccesarry to work with a copy do `dir',
         * because if `dir' is part or `dest', this could lead
         * to a mungwall hit */
        STRPTR dir_clone = strclone(dir);       /* clone of `dir' */

        set_estr(dest, dir_clone);
        /* check, if the last char of dir is a path separator */
        /* ->if not, append a direcory separator */
        if (!strchr(PATH_SEPARATOR, dir_clone[strlen(dir_clone) - 1]))
            app_estrch(dest, DIR_SEPARATOR);

        /* free cloned dir */
        ufreestr(dir_clone);
    }
    else
        clr_estr(dest);

    /* append filename */
    if (fn)
        app_estr(dest, fn);

    ok &= ok_fnl_fpath(dest);

    return ok;
}

/*
 * link_envfname: link content of an environment variable,
 *                directory and filename together
 *
 * params: dest..where to store result
 *         env...name of environment variable
 *         dir...directoryname
 *         fn....filename to append
 *
 * result: FALSE, if envvar could not be found or
 *         filename got too long
 */
BOOL link_envfname(EXPSTR * dest, STRPTR envname, STRPTR dir, STRPTR fn)
{
    BOOL ok = FALSE;
    STRPTR env = getenv(envname);

    if (env)
    {
        STRPTR env1 = strclone(env);        /* copy envvar to own memory area */
        EXPSTR *tmpstr = init_estr(32);

        /* strip linefeeds from hscenv */
        while (strlen(env1) && (env1[strlen(env1)] == '\n'))
            env1[strlen(env1)] = '\0';

        if (dir)
        {
            link_fname(tmpstr, env1, dir);
        }
        else
        {
            set_estr(tmpstr, env1);
        }

        ok = link_fname(dest, estr2str(tmpstr), fn);

        del_estr(tmpstr);
        ufreestr(env1);
    }
    else
    {
        clr_estr(dest);
    }

    return ok;
}

/*
 * tmpnamstr: alloc & create a string with a temp. filename
 *
 * result: string containig filename;
 *         MUST be release using ufreestr() by caller
 * errors: return NULL; this can be because auf no more
 *         temp. files available or out of mem
 *
 * IMPORTANT: you need to copy the filename, if you
 *           call this function frquently
 */
static size_t adjust_prefixlen(size_t prefixlen)
{
#ifdef MSDOS
    if (prefixlen > MAX_FNAME - 4)
        prefixlen = MAX_FNAME - 4;
#else
    if (prefixlen > MAX_FNAME - 8)
        prefixlen = MAX_FNAME - 8;
#endif

    return (prefixlen);
}

STRPTR tmpnamstr(STRPTR prefix)
{
    static LONG fileidx = 0;
    STRARR buf[256 + 32];
    STRPTR s = NULL;
    FILE *file = NULL;
    size_t prefixlen = 0;

    if (prefix)
        /* copy prefix to buffer */
        prefixlen = adjust_prefixlen(strlen(prefix));
    else
        prefix = "";

    /* copy prefix to buffer */
    strcpy(buf, prefix);

    /* add address of buffer to prefix */
    sprintf(&(buf[strlen(prefix)]), "%p", tmpnamstr);

    prefixlen = adjust_prefixlen(strlen(buf));
    buf[prefixlen] = '\0';

    /* try to open tmpfile for input until it fails */
    do
    {
        fileidx++;
        sprintf(&(buf[prefixlen]), "%04lx", fileidx);
        strcat(buf, ".tmp");
        file = fopen(buf, "r");
        if (file)
        {
            fclose(file);
            if (fileidx == 0xffff)
                fileidx = 0;    /* ran out of names */
        }
    }
    while (fileidx && file);

    if (fileidx)
    {
        s = buf;
    }

    return (s);
}

/*
 * get_relfname: get relative filename, according to given path
 *
 * params: absn..absolute filename
 *         curp..current path
 *
 * EXAMPLE:
 *  "image/back.gif" and "image/hugo/" -> "/back.gif"
 *  "image/back.gif" and ""            -> "image/back.gif"
 *  "image/back.gif" and "people/"     -> "/image/back.gif"
 */
BOOL get_relfname(EXPSTR * dest, STRPTR absn, STRPTR curp)
{
    EXPSTR *fname = init_estr(32);      /* file name only */
    EXPSTR *abspa = init_estr(32);      /* absolute path only */
    EXPSTR *tmpp1 = init_estr(32);      /* temp pointer */
    EXPSTR *tmpp2 = init_estr(32);
    STRPTR rest_absp = NULL;    /* rest of current path */
    STRPTR absp = NULL;         /* path processing */
    int cmp_result;             /* stores result returned by upstrcmp */

    /* init string array */
    clr_estr(dest);
    get_fname(fname, absn);
    get_fpath(abspa, absn);
    absp = estr2str(abspa);

    /*
     * skip all equal subdirs
     */
    do
    {
        get_fsdir(tmpp1, absp);
        get_fsdir(tmpp2, curp);
        cmp_result = upstrcmp(estr2str(tmpp1), estr2str(tmpp2));

        if (!cmp_result)
        {
            absp += estrlen(tmpp1);
            curp += estrlen(tmpp2);
        }
    }
    while (estrlen(tmpp1)
           && estrlen(tmpp2)
           && (!cmp_result));

    /* remember equal part of path */
    rest_absp = absp;

    /*
     * for every subdir in absp unequal to
     * corresponding subdir curp, insert a parent dir
     */
    if (curp[0])
        do
        {
            get_fsdir(tmpp1, absp);
            get_fsdir(tmpp2, curp);
            cmp_result = upstrcmp(estr2str(tmpp1), estr2str(tmpp2));

            if (cmp_result)
            {
                absp += estrlen(tmpp1);
                curp += estrlen(tmpp2);
                app_estr(dest, PARENT_DIR);
#if 0
                printf("absp: (tmpp1) [%d] `%s'\n", estrlen(tmpp1), absp);
                printf("curp: (tmpp2) [%d] `%s'\n", estrlen(tmpp2), curp);
                printf("dest: `%s'\n", estr2str(dest));
#endif
            }
        }
        while (strlen(curp) && cmp_result);

    /* append equal part of path */
    app_estr(dest, rest_absp);

    /* append name of file */
    app_estr(dest, estr2str(fname));

    /* relaese resources */
    del_estr(fname);
    del_estr(abspa);
    del_estr(tmpp1);
    del_estr(tmpp2);

    return (ok_fnl_fpath(dest));
}

