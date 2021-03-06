/*
 * hscprc.c
 *
 * hsc process functions
 *
 * Copyright (C) 1995,96  Thomas Aglassinger
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
 * updated:  8-Sep-1996
 * created: 11-Feb-1996
 */

#define NOEXTERN_HSCLIB_HSCPRC

#include "hsclib/inc_base.h"

#include "ugly/fname.h"
#include "ugly/returncd.h"

#include "hscprj/project.h"

#include "hsclib/idref.h"
#include "hsclib/tag_if.h"

/*
 * del_inpf_stack_node
 *
 * remove node from input file stack
 */
static VOID del_inpf_stack_node(APTR data)
{
    /* do nufin */
}

/*
 * del_hscprc
 *
 * remove hsc process and all it's resources
 */
VOID del_hscprc(HSCPRC * hp)
{
    if (hp)
    {
        /* remove list */
        del_dllist(hp->defent);
        del_dllist(hp->deftag);
        del_dllist(hp->defattr);
        del_dllist(hp->container_stack);
        del_dllist(hp->inpf_stack);
        del_dllist(hp->idrefs);
        del_dllist(hp->select_stack);

        /* remove strings */
        del_estr(hp->destdir);
        del_estr(hp->reldir);
        del_estr(hp->iconbase);
        del_estr(hp->if_stack);
        del_estr(hp->tag_name_str);
        del_estr(hp->tag_attr_str);
        del_estr(hp->tag_close_str);
        del_estr(hp->tmpstr);
        del_estr(hp->rmt_str);
        del_estr(hp->curr_msg);
        del_estr(hp->curr_ref);

#if 0                           /* TODO:remove */
        ufreestr(hp->filename_project);
#endif
        ufreestr(hp->filename_document);

        /* remove project-data */
        del_project(hp->project);

        /* close input files */
        infclose(hp->inpf);

        /* remove message arrays */
        ufree(hp->msg_ignore);
        ufree(hp->msg_class);

        ufree(hp);
    }
}

/*
 * reset_hscprc
 *
 * reset all items of a hsc process
 */
static VOID reset_hscprc(HSCPRC * hp)
{
    int i;

    /* get current time */
    hp->start_time = time(NULL);

    /* reset strings */
    clr_estr(hp->destdir);
    clr_estr(hp->reldir);
    clr_estr(hp->if_stack);

    hp->suppress_output = TRUE;
    hp->fatal = FALSE;
    hp->prev_heading_num = 0;
    hp->prev_status_line = (ULONG) - 1;
    hp->msg_count = 0;

    hp->preceding_whtspc = FALSE;

    hp->inside_pre = FALSE;
    hp->inside_anchor = FALSE;
    hp->inside_title = FALSE;

    /* reset message arrays */
    for (i = 0; i < MAX_MSGID; i++)
    {
        hp->msg_ignore[i] = FALSE;
        hp->msg_class[i] = MSG_NOTE;
    }
}

/*
 * new_hscprc
 *
 * create and init a new hsc process
 */
HSCPRC *new_hscprc(void)
{
    HSCPRC *hp = NULL;

    hp = (HSCPRC *) umalloc(sizeof(HSCPRC));
    if (hp)
    {
        memset(hp, 0, sizeof(HSCPRC));

        /* init lists */
        hp->defent = init_dllist(del_entity);
        hp->deftag = init_dllist(del_hsctag);
        hp->defattr = init_dllist(del_hscattr);
        hp->container_stack = init_dllist(del_hsctag);
        hp->inpf_stack = init_dllist(del_inpf_stack_node);
        hp->project = NULL;
        hp->idrefs = init_dllist(del_idref);
        hp->select_stack = init_dllist(del_select_stack_node);

        /* init strings */
        hp->destdir = init_estr(0);
        hp->reldir = init_estr(0);
        hp->iconbase = init_estr(0);
        hp->if_stack = init_estr(0);
        hp->tag_name_str = init_estr(128);
        hp->tag_attr_str = init_estr(128);
        hp->tag_close_str = init_estr(0);
        hp->tmpstr = init_estr(0);
        hp->rmt_str = init_estr(128);
        hp->curr_msg = init_estr(64);
        hp->curr_ref = init_estr(64);

#if 0                           /* TODO:remove */
        hp->filename_project = NULL;
        hp->filename_document = NULL;
#endif

        /* alloc message arrays */
        hp->msg_ignore = (BOOL *)
            umalloc((MAX_MSGID + 1) * sizeof(BOOL));
        hp->msg_class = (HSCMSG_CLASS *)
            umalloc((MAX_MSGID + 1) * sizeof(HSCMSG_CLASS));

        reset_hscprc(hp);
    }
    return (hp);
}

/*
 *
 * GET-methodes for public item of HSCPRC
 *
 */

/*
 * get flags
 */
BOOL hsc_get_chkid(HSCPRC * hp)
{
    return (hp->chkid);
}

BOOL hsc_get_chkuri(HSCPRC * hp)
{
    return (hp->chkuri);
}

BOOL hsc_get_compact(HSCPRC * hp)
{
    return (hp->compact);
}

BOOL hsc_get_debug(HSCPRC * hp)
{
    return (hp->debug);
}

BOOL hsc_get_getsize(HSCPRC * hp)
{
    return (hp->getsize);
}

BOOL hsc_get_htmlonly(HSCPRC * hp)
{
    return (hp->htmlonly);
}

BOOL hsc_get_jens(HSCPRC * hp)
{
    return (hp->jens);
}

BOOL hsc_get_jerkvalues(HSCPRC * hp)
{
    return (hp->jerkvalues);
}

BOOL hsc_get_rplc_ent(HSCPRC * hp)
{
    return (hp->rplc_ent);
}

BOOL hsc_get_rplc_quote(HSCPRC * hp)
{
    return (hp->rplc_quote);
}

BOOL hsc_get_smart_ent(HSCPRC * hp)
{
    return (hp->smart_ent);
}

BOOL hsc_get_strip_cmt(HSCPRC * hp)
{
    return (hp->strip_cmt);
}

BOOL hsc_get_strip_ext(HSCPRC * hp)
{
    return (hp->strip_ext);
}

/*
 * get internal flags
 */
BOOL hsc_get_suppress_output(HSCPRC * hp)
{
    return (hp->suppress_output);
}

BOOL hsc_get_inside_pre(HSCPRC * hp)
{
    return (hp->inside_pre);
}

BOOL hsc_get_inside_anchor(HSCPRC * hp)
{
    return (hp->inside_anchor);
}

BOOL hsc_get_fatal(HSCPRC * hp)
{
    return (hp->fatal);
}

/*
 * get values
 */
STRPTR hsc_get_destdir(HSCPRC * hp)
{
    return (estr2str(hp->destdir));
}

STRPTR hsc_get_reldir(HSCPRC * hp)
{
    return (estr2str(hp->reldir));
}

STRPTR hsc_get_iconbase(HSCPRC * hp)
{
    return (estr2str(hp->iconbase));
}

/*
 * get internal values
 */
STRPTR hsc_get_click_here_str(HSCPRC * hp)
{
    return (hp->click_here_str);
}

STRPTR hsc_get_file_name(HSCPRC * hp)
{
    if (hp->inpf)
        return (infget_fname(hp->inpf));
    else
        return (NULL);
}

ULONG hsc_get_file_line(HSCPRC * hp)
{
    if (hp->inpf)
        return (infget_y(hp->inpf));
    else
        return (0);
}

ULONG hsc_get_file_column(HSCPRC * hp)
{
    if (hp->inpf)
        return (infget_x(hp->inpf));
    else
        return (0);
}

ULONG hsc_get_msg_count(HSCPRC * hp)
{
    return (hp->msg_count);
}

/*
 *
 * SET-methodes for public item of HSCPRC
 *
 */

/*
 * set values
 */
BOOL hsc_set_destdir(HSCPRC * hp, STRPTR dir)
{
    set_estr(hp->destdir, dir);
    /* append "/" if neccessary */
    link_fname(hp->destdir, estr2str(hp->destdir), "");
    D(fprintf(stderr, DHL "destdir=`%s'\n", estr2str(hp->destdir)));

    return (TRUE);
}

BOOL hsc_set_reldir(HSCPRC * hp, STRPTR dir)
{
#if 0
    set_estr(hp->reldir, dir);
    /* append "/" if neccessary */
    link_fname(hp->reldir, estr2str(hp->reldir), "");
    D(fprintf(stderr, DHL "reldir =`%s'\n", estr2str(hp->reldir)));
#endif
    /* append "/" if neccessary */
    link_fname(hp->reldir, dir, "");
    D(fprintf(stderr, DHL "reldir =`%s'\n", estr2str(hp->reldir)));

    return (TRUE);
}

BOOL hsc_set_iconbase(HSCPRC * hp, STRPTR uri)
{
    set_estr(hp->iconbase, uri);

    /* append "/" if neccessary
     * NOTE: in this case link_fname can't be used because
     * hp->iconbase refers to an URI, but not to a file
     */
    if (strlen(uri))
    {
        char lastch = uri[strlen(uri) - 1];
        if (lastch != '/')
            app_estrch(hp->iconbase, '/');
    }
    D(fprintf(stderr, DHL "iconbase=`%s'\n", estr2str(hp->iconbase)));

    return (TRUE);
}

#if 0 /* TODO: remove */
BOOL hsc_set_filename_project(HSCPRC * hp, STRPTR filename)
{
    BOOL ok = FALSE;

    D(fprintf(stderr, DHL "project =`%s'\n", filename));
    ok = hsc_project_set_filename(hp->project, filename);

    return (ok);
}
#endif

BOOL hsc_set_filename_document(HSCPRC * hp, STRPTR filename)
{
    BOOL ok = FALSE;

    D(fprintf(stderr, DHL "document=`%s'\n", filename));
    hp->filename_document = strclone( filename );

    return (ok);
}

/*
 * set flags
 */
VOID hsc_set_chkid(HSCPRC * hp, BOOL new_chkid)
{
    hp->chkid = new_chkid;
    D(fprintf(stderr, DHL "flag: chkid    =%d\n", new_chkid));
}

VOID hsc_set_chkuri(HSCPRC * hp, BOOL new_chkuri)
{
    hp->chkuri = new_chkuri;
    D(fprintf(stderr, DHL "flag: chkuri   =%d\n", new_chkuri));
}

VOID hsc_set_compact(HSCPRC * hp, BOOL new_compact)
{
    hp->compact = new_compact;
    D(fprintf(stderr, DHL "flag: compact  =%d\n", new_compact));
}

VOID hsc_set_debug(HSCPRC * hp, BOOL new_debug)
{
    hp->debug = new_debug;
    D(fprintf(stderr, DHL "flag: debug    =%d\n", new_debug));
}

VOID hsc_set_getsize(HSCPRC * hp, BOOL new_getsize)
{
    hp->getsize = new_getsize;
    D(fprintf(stderr, DHL "flag: getsize  =%d\n", new_getsize));
}

VOID hsc_set_htmlonly(HSCPRC * hp, BOOL new_htmlonly)
{
    hp->htmlonly = new_htmlonly;
    D(fprintf(stderr, DHL "flag: htmlonly =%d\n", new_htmlonly));
}

VOID hsc_set_jens(HSCPRC * hp, BOOL new_jens)
{
    hp->jens = new_jens;
    D(fprintf(stderr, DHL "flag: jens     =%d\n", new_jens));
}

VOID hsc_set_jerkvalues(HSCPRC * hp, BOOL new_jerkvalues)
{
    hp->jerkvalues = new_jerkvalues;
    D(fprintf(stderr, DHL "flag: jerkval  =%d\n", new_jerkvalues));
}

VOID hsc_set_rplc_ent(HSCPRC * hp, BOOL new_rplc_ent)
{
    hp->rplc_ent = new_rplc_ent;
    D(fprintf(stderr, DHL "flag: rplc_ent =%d\n", new_rplc_ent));
}

VOID hsc_set_rplc_quote(HSCPRC * hp, BOOL new_rplc_quote)
{
    hp->rplc_quote = new_rplc_quote;
    D(fprintf(stderr, DHL "flag: rplc_quote =%d\n", new_rplc_quote));
}

VOID hsc_set_smart_ent(HSCPRC * hp, BOOL new_smart_ent)
{
    hp->smart_ent = new_smart_ent;
    D(fprintf(stderr, DHL "flag: smart_ent=%d\n", new_smart_ent));
}

VOID hsc_set_strip_cmt(HSCPRC * hp, BOOL new_strip_cmt)
{
    hp->strip_cmt = new_strip_cmt;
    D(fprintf(stderr, DHL "flag: strip_cmt=%d\n", new_strip_cmt));
}

VOID hsc_set_strip_ext(HSCPRC * hp, BOOL new_strip_ext)
{
    hp->strip_ext = new_strip_ext;
    D(fprintf(stderr, DHL "flag: strip_ext=%d\n", new_strip_ext));
}

/*
 * set flags
 */
VOID hsc_set_quote_mode(HSCPRC * hp, LONG new_mode)
{
    hp->quotemode = new_mode;
    D(fprintf(stderr, DHL "quote_mode=%ld\n", new_mode));
}

VOID hsc_set_entity_mode(HSCPRC * hp, LONG new_mode)
{
    hp->entmode = new_mode;
    D(fprintf(stderr, DHL "entity_mode=%ld\n", new_mode));
}

/*
 * set call-backs
 */
VOID hsc_set_status_file_begin(HSCPRC * hp, VOID(*status_file) (HSCPRC * hp, STRPTR filename))
{
    hp->CB_status_file_begin = status_file;
}

VOID hsc_set_status_file_end(HSCPRC * hp, VOID(*status_file) (HSCPRC * hp))
{
    hp->CB_status_file_end = status_file;
}

VOID hsc_set_status_line(HSCPRC * hp, VOID(*status_line) (HSCPRC * hp))
{
    hp->CB_status_line = status_line;
}

VOID hsc_set_status_misc(HSCPRC * hp, VOID(*status_misc) (HSCPRC * hp, STRPTR s))
{
    hp->CB_status_misc = status_misc;
}

VOID hsc_set_message(
                        HSCPRC * hp,
                        VOID(*message) (struct hscprocess * hp,
                                        HSCMSG_CLASS msg_class, HSCMSG_ID,
                                        STRPTR fname, ULONG x, ULONG y,
                                        STRPTR msg_text))
{
    hp->CB_message = message;
}

VOID hsc_set_message_ref(
                            HSCPRC * hp,
                            VOID(*message_ref) (struct hscprocess * hp,
                                   HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                                             STRPTR fname, ULONG x, ULONG y,
                                                STRPTR msg_text))
{
    hp->CB_message_ref = message_ref;
}

VOID hsc_set_start_tag(HSCPRC * hp,
                       VOID(*start_tag) (struct hscprocess * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close))
{
    hp->CB_start_tag = start_tag;
}

VOID hsc_set_end_tag(HSCPRC * hp,
                     VOID(*end_tag) (struct hscprocess * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close))
{
    hp->CB_end_tag = end_tag;
}

VOID hsc_set_text(HSCPRC * hp,
                  VOID(*text) (struct hscprocess * hp,
                               STRPTR white_spaces, STRPTR text))
{
    hp->CB_text = text;
}

VOID hsc_set_id(HSCPRC * hp,
                VOID(*id) (struct hscprocess * hp,
                           HSCATTR * attr, STRPTR id))
{
    hp->CB_id = id;
}

/*
 * message methodes
 */
BOOL hsc_set_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id, BOOL value)
{
    BOOL set = FALSE;

    if ((msg_id & MASK_MESSAGE) <= MAX_MSGID)
        hp->msg_ignore[msg_id & MASK_MESSAGE] = value;

    return (set);
}

BOOL hsc_get_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id)
{
/*    HSCMSG_ID max_msgid = MAX_MSGID; */
    if ((msg_id & MASK_MESSAGE) <= MAX_MSGID)
        return (hp->msg_ignore[msg_id & MASK_MESSAGE]);
    else
        return (FALSE);
}

BOOL hsc_set_msg_class(HSCPRC * hp, HSCMSG_ID msg_id, HSCMSG_CLASS msg_class)
{
    BOOL set = FALSE;

    if ((msg_id & MASK_MESSAGE) <= MAX_MSGID)
        hp->msg_class[msg_id & MASK_MESSAGE] = msg_class;

    return (set);
}

HSCMSG_CLASS hsc_get_msg_class(HSCPRC * hp, HSCMSG_ID msg_id)
{
    if ((msg_id & MASK_MESSAGE) <= MAX_MSGID)
    {
        HSCMSG_CLASS mchp = hp->msg_class[msg_id & MASK_MESSAGE];
        HSCMSG_CLASS mcid = msg_id & MASK_MSG_CLASS;

        if (mcid > mchp)
            return (mcid);
        else
            return (mchp);
    }
    else
        return (MSG_NONE);
}

VOID hsc_clear_msg_ignore(HSCPRC * hp)
{
    size_t i;
    for (i = 0; i <= MAX_MSGID; i++)
        hp->msg_ignore[i] = FALSE;
}

VOID hsc_reset_msg_class(HSCPRC * hp)
{
    size_t i;
    for (i = 0; i <= MAX_MSGID; i++)
        hp->msg_class[i] = MSG_NONE;
}

/*
 * output text function
 *
 * output text to
 *
 * params: hp....hsc process to perform ouput with
 *         wspc..white spaces
 *         text..other text
 * result: true, if text has been outputted
 */
BOOL hsc_output_text(HSCPRC * hp, STRPTR wspc, STRPTR text)
{
    BOOL written = FALSE;
    if ((hp)->CB_text && !((hp)->suppress_output))
    {
        (*((hp)->CB_text)) ((hp), wspc, text);
#if DEBUG_HSCLIB_OUTPUT
        if (hp->debug)
            if (text)
                if (strcmp(text, "\n"))
                    fprintf(stderr, DHL "ouput: `%s', `%s'\n", wspc, text);
                else
                    fprintf(stderr, DHL "ouput: `%s', `\\n'\n", wspc);
#endif
        written = TRUE;
    }
    return (written);
}

/*
 * misc. functions
 */

/*
 * nomem-handler
 */
BOOL hsc_standard_nomem_handler(size_t size)
{                               /* TODO: think about this */
    fputs("\n*** out of memory\n\n", stderr);

    exit(RC_FAIL);

    return (FALSE);             /* abort immediatly */
}

