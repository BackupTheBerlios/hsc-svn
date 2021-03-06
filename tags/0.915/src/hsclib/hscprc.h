/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * hsclib/hscprc.h
 *
 * hsc process structure
 *
 */

#ifndef HSC_HSCPRC_H
#define HSC_HSCPRC_H

#include <time.h>

#include "hsclib/ldebug.h"
#include "hsclib/tag.h"

#include "hscprj/project.h"

/*
 * system-dependant defines
 */
#ifdef AMIGA
#define CONFIG_FILE "hsc.prefs"
#define OPTION_FILE "hsc.options", "env:hsc.options"
#define CONFIG_PATH "PROGDIR:"

#define UNIX 1                  /* utilise POSIX-layer of BeOS */
#elif defined RISCOS
#define CONFIG_FILE "hsc:hsc.prefs"
#define CONFIG_PATH "hsc:"
#define OPTION_FILE "hsc.options"

#elif (defined NEXTSTEP) || (defined UNIX) || (defined BEOS)
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "/usr/local/lib/", "/usr/lib/"
#define OPTION_FILE "hsc.options"

/* [3] */
#else
#error "Operating system not supported: config-file/path"
#endif

/* utilise POSIX-layer of BEOS */
#if defined BEOS
#define UNIX 1
#endif

/* step sizes for expstr's */
#define ES_STEP_MACRO   1024    /* tag_macro.c */
#define ES_STEP_INFILE  4096    /* input file buffer */

/*
 * modes for syntax check
 */
#define MODE_PEDANTIC 1
#define MODE_NORMAL   2
#define MODE_RELAXED  3

/*
 * modes for attribute quotes
 */
#define QMODE_KEEP   1          /* keep quotes from input */
#define QMODE_DOUBLE 2          /* always use double quotes (compatible) */
#define QMODE_SINGLE 3          /* always use single quotes */
#define QMODE_NONE   4          /* never use any quotes (compact) */

/*
 * modes for special characters/entity extraction
 */
#define EMODE_KEEP     1        /* do not replace */
#define EMODE_REPLACE  2        /* replace by prefered value */
                                /*   (depends wheter if a PREFNUM was used */
                                /*   within $DEFENT) */
#define EMODE_NUMERIC  3        /* always replace by numeric (&#123;) */
#define EMODE_SYMBOLIC 4        /* always replace by symbolic (&uuml;) */

/*
 * misc. defines for files & envvars
 */
#define ENV_HSCPATH    "HSCPATH"        /* envvar that contains path for prefs */
#define ENV_HSCSALARY  "HSCSALARY"      /* contains salary of user */
#define ENV_HSCUSER    "HSCUSER_PATH"   /* to substitute "~/" */
#define ENV_HSCUSERS   "HSCUSERS_PATH"  /* to substitute "~" */
#define ENV_HSCROOT    "HSCROOT_PATH"   /* to substitute "/" */

/* used as prefix in filename for
 * internal (pseudo-)files (macros,init) */
#define SPECIAL_FILE_ID "::s::"

/* prefix to be used by filenames if parent file on input-stack
 * should be used for message-pos.
 *
 * NOTE: this is set if IH_POS_PARENT is passed on hsc_include() */
#define PARENT_FILE_ID  "::p::"

/* pseudo-filename for stdin */
#define FILENAME_STDIN "STDIN"

/* prefix char used for special hsc-tags  */
#define HSC_SPECIAL_PREFIX "$"

/* names for special attributes */
#define CLICK_HERE_ATTR     "HSC.CLICK-HERE"    /* attribute that holds "click here" words */
#define COLOR_NAMES_ATTR    "HSC.COLOR-NAMES"
#define CONTENT_ATTR        "HSC.CONTENT"       /* keep macro content for content macros */
#define ANCHOR_ATTR         "HSC.ANCHOR"
#define RESULT_ATTR         "HSC.EXEC.RESULT"
#define FILESIZEFORMAT_ATTR "HSC.FORMAT.FILESIZE"
#define TIMEFORMAT_ATTR     "HSC.FORMAT.TIME"
#define LINEFEED_ATTR       "HSC.LF"

/* attribute that hold condition on <$if/$elseif> */
#define CONDITION_ATTR "COND"

/* attribute that tells operating system */
#define SYSTEM_ATTR     "hsc.System"
#ifdef AMIGA
#define SYSTEM_ATTR_ID "Amiga"

#elif defined BEOS
#define SYSTEM_ATTR_ID "BeOS"

#elif defined RISCOS
#define SYSTEM_ATTR_ID "RiscOS"

#elif defined NEXTSTEP
#define SYSTEM_ATTR_ID "NeXTStep"

#elif defined UNIX
#define SYSTEM_ATTR_ID "Unix"

/* dos4 */
#else
#error "system not supported: SYSTEM_ATTR_ID"
#endif

/*
 * some typedefs used inside the hsc_process
 */
typedef LONG HSCMSG_ID;         /* hsc message id */
typedef LONG HSCMSG_CLASS;      /* hsc message class */

/* enumerator values for hsc_process->msg_ignore */
enum hsc_ignore_status
{
    make_my_day,                /* ignore only, if whole class is ignored */
    ignore,                     /* always ignore */
    enable                      /* always show up (even if class ignored) */
};

typedef enum hsc_ignore_status HSCIGN;

/*
 * hsc process structure
 */
struct hsc_process
{
    INFILE *inpf;               /* current input file */
    DLLIST *inpf_stack;         /* stack of nested input files */
    DLLIST *deftag;             /* defined tags and macros */
    DLLIST *defattr;            /* defined attributes */
    DLLIST *defent;             /* defined special charcters & entities */
    DLLIST *deflazy;            /* defined lazy attribute lists */
    DLLIST *container_stack;    /* stack of container-tags currently open */
    DLLIST *content_stack;      /* stack of contents of container macros */
    DLLIST *include_dirs;       /* include directories */
    HSCPRJ *project;            /* project data */
    DLLIST *idrefs;             /* list of references to local IDs */
    EXPSTR *destdir;            /* destination root directory */
    EXPSTR *reldir;             /* relative destination directory */
    EXPSTR *tmpstr;             /* temp. string used by several functions */
    EXPSTR *curr_msg;           /* current message */
    EXPSTR *curr_ref;           /* current reference message */
    EXPSTR *iconbase;           /* base URI for icons */
    EXPSTR *server_dir;         /* root dir for server-relative URIs */
    time_t start_time;          /* time process has been started */

    DLLIST *select_stack;       /* stack for results of <$select> */
    EXPSTR *if_stack;           /* stack for results of <$if> */

    EXPSTR *tag_name_str;       /* strings for communication with tag-handlers */
    EXPSTR *tag_attr_str;
    EXPSTR *tag_close_str;

    STRPTR filename_document;   /* document-name to be stored in project */

    HSCIGN *msg_ignore;         /* messages to be ignored */
    BOOL msg_ignore_notes;      /* message-classes to be ignores */
    BOOL msg_ignore_style;
    BOOL msg_ignore_port;
    HSCMSG_CLASS *msg_class;    /* messages with remaped classes */
    ULONG msg_count;            /* numer of messages occured until now */

    BOOL chkid;                 /* flag: check existence of URIs/IDs */
    BOOL chkuri;
    BOOL compact;               /* flag: create compact output */
    BOOL debug;                 /* flag: display debuging info */
    BOOL getsize;               /* flag: get size of images/embedded docs */

    BOOL htmlonly;              /* flag: disable hsc-extensions */
    BOOL jens;                  /* flag: enable some experimental features */
    BOOL jerkvalues;            /* flag: interpret jerk values */
    BOOL rplc_ent;              /* flag: replace special chars */
    BOOL rplc_quote;            /* flag: replace quotes in text by "&quot;" */
    BOOL smart_ent;             /* flag: replace special entities "<>&" */
    BOOL strip_cmt;             /* flag: strip SGML-comments */
    BOOL strip_ext;             /* flag: strip external references */
    BOOL strip_badws;           /* flag: strip bad white spaces */
    BOOL weenix;                /* flag: unix compatibilty mode */
    BOOL suppress_output;       /* flag: TRUE, until outputable data occure */
    BOOL docbase_set;           /* <BASE HREF=".."> occured */
    BOOL inside_pre;            /* inside preformatted tag <PRE> & Co. */
    BOOL inside_anchor;         /* inside anchor-tag <A> */
    BOOL inside_title;          /* inside title-tag <TITLE> */
    BOOL prostitute;            /* use "prostitute" or "jerk"? */

    BOOL fatal;                 /* fatal error occured; abort process */

    LONG tag_call_id;
    ULONG prev_status_line;
    LONG prev_heading_num;      /* number of previous heading */
    LONG entmode;               /* entity replace mode */
    LONG quotemode;             /* quotes to use as output quotes */

    STRPTR click_here_str;      /* keywords for click-here syndrome */
    STRPTR color_names;         /* predifined names for colors */
    STRPTR strip_tags;          /* tags that should be stripped */
    EXPSTR *whtspc;             /* white spaces buffered */
    HSCTAG *tag_next_whtspc;    /* set, if a tag did not allow succ.whtspc */
    BOOL strip_next_whtspc;     /* flag: strip next whtite space
                                 * set by parse_tag(), if strip_badws = TRUE */
    BOOL strip_next2_whtspc;    /* flag: strip next but one white space */
    /* status callbacks */
      VOID(*CB_status_misc) (struct hsc_process * hp, STRPTR s);
    /* called for verbose messages */
      VOID(*CB_status_line) (struct hsc_process * hp);
    /* called after new line */
      VOID(*CB_status_file_begin) (struct hsc_process * hp, STRPTR filename);
    /* called when new file is going to be loaded */
      VOID(*CB_status_file_end) (struct hsc_process * hp);
    /* called after file has fully been processed */

    /* message callbacks */
      VOID(*CB_message) (struct hsc_process * hp,
                         HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                         STRPTR fname, ULONG x, ULONG y,
                         STRPTR msg_text);
      VOID(*CB_message_ref) (struct hsc_process * hp,
                             HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                             STRPTR fname, ULONG x, ULONG y,
                             STRPTR msg_text);

    /* syntax elements callbacks */
      VOID(*CB_start_tag) (struct hsc_process * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close);
      VOID(*CB_end_tag) (struct hsc_process * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close);
      VOID(*CB_text) (struct hsc_process * hp,
                      STRPTR white_spaces, STRPTR text);
      VOID(*CB_id) (struct hsc_process * hp,
                    HSCATTR * attr, STRPTR id);

#ifdef MSDOS
    /* some compilers seem to have problems with this line.. */
    enum content_called
    {
        no, once, often
    };
#endif

};

typedef struct hsc_process HSCPRC;

#if (!defined MSDOS) & ((defined WINNT))
/* handle several OS-es same as MSDOS */
#define MSDOS 1
#error "Fuchs du hast die Gans gestohlen, gib sie wieder her..."
#endif

/*
 * global funcs
 */
#ifndef NOEXTERN_HSC_HSCPRC

#define anyWhtspc(hp) (estrlen(hp->whtspc))

extern VOID del_hscprc(HSCPRC * hp);
extern HSCPRC *new_hscprc(void);
extern VOID reset_hscprc(HSCPRC * hp);

/* set-methodes for callbacks */
extern VOID hsc_set_status_file_begin(HSCPRC * hp, VOID(*status_file) (HSCPRC * hp, STRPTR filename));
extern VOID hsc_set_status_file_end(HSCPRC * hp, VOID(*status_file) (HSCPRC * hp));
extern VOID hsc_set_status_line(HSCPRC * hp, VOID(*status_line) (HSCPRC * hp));
extern VOID hsc_set_status_misc(HSCPRC * hp, VOID(*status_misc) (HSCPRC * hp, STRPTR s));
extern VOID hsc_set_message(HSCPRC * hp,
                            VOID(*message) (struct hsc_process * hp,
                                            HSCMSG_CLASS msg_class,
                                            HSCMSG_ID msg_id,
                                            STRPTR fname, ULONG x, ULONG y,
                                            STRPTR msg_text));
extern VOID hsc_set_message_ref(HSCPRC * hp,
                                VOID(*message_ref) (struct hsc_process * hp,
                                                    HSCMSG_CLASS msg_class,
                                                    HSCMSG_ID msg_id,
                                                    STRPTR fname,
                                                    ULONG x, ULONG y,
                                                    STRPTR msg_text));
extern VOID hsc_set_start_tag(HSCPRC * hp,
                              VOID(*CB_start_tag) (struct hsc_process * hp,
                                                   HSCTAG * tag,
                                                   STRPTR tag_name,
                                                   STRPTR tag_attr,
                                                   STRPTR tag_close));
extern VOID hsc_set_end_tag(HSCPRC * hp,
                            VOID(*CB_end_tag) (struct hsc_process * hp,
                                               HSCTAG * tag,
                                               STRPTR tag_name,
                                               STRPTR tag_attr,
                                               STRPTR tag_close));
extern VOID hsc_set_text(HSCPRC * hp,
                         VOID(*CB_text) (struct hsc_process * hp,
                                         STRPTR white_spaces, STRPTR text));
extern VOID hsc_set_id(HSCPRC * hp,
                       VOID(*id) (struct hsc_process * hp,
                                  HSCATTR * attr, STRPTR id));

/* set-methodes for flags */
extern VOID hsc_set_chkid(HSCPRC * hp, BOOL new_chkid);
extern VOID hsc_set_chkuri(HSCPRC * hp, BOOL new_chkuri);
extern VOID hsc_set_compact(HSCPRC * hp, BOOL new_compact);
extern VOID hsc_set_debug(HSCPRC * hp, BOOL new_debug);
extern VOID hsc_set_getsize(HSCPRC * hp, BOOL new_getsize);
extern VOID hsc_set_htmlonly(HSCPRC * hp, BOOL new_jens);
extern VOID hsc_set_jens(HSCPRC * hp, BOOL new_jens);
extern VOID hsc_set_jerkvalues(HSCPRC * hp, BOOL new_jens);
extern VOID hsc_set_rplc_ent(HSCPRC * hp, BOOL new_rplc_ent);
extern VOID hsc_set_rplc_quote(HSCPRC * hp, BOOL new_rplc_quote);
extern BOOL hsc_set_server_dir(HSCPRC * hp, STRPTR dir);
extern VOID hsc_set_smart_ent(HSCPRC * hp, BOOL new_smart_ent);
extern VOID hsc_set_strip_badws(HSCPRC * hp, BOOL new_strip_badws);
extern VOID hsc_set_strip_cmt(HSCPRC * hp, BOOL new_strip_cmt);
extern VOID hsc_set_strip_ext(HSCPRC * hp, BOOL new_strip_ext);

/* set-methodes for values */
extern BOOL hsc_set_destdir(HSCPRC * hp, STRPTR dir);
extern BOOL hsc_set_reldir(HSCPRC * hp, STRPTR fname);
extern BOOL hsc_set_iconbase(HSCPRC * hp, STRPTR uri);
extern BOOL hsc_set_strip_tags(HSCPRC * hp, STRPTR taglist);
extern BOOL hsc_set_filename_document(HSCPRC * hp, STRPTR filename);
extern VOID hsc_set_quote_mode(HSCPRC * hp, LONG new_mode);
extern VOID hsc_set_entity_mode(HSCPRC * hp, LONG new_mode);

/* methodes for include-directories */
extern BOOL hsc_add_include_directory(HSCPRC * hp, STRPTR dir);
extern VOID hsc_clr_include_directory(HSCPRC * hp);

/* get-methodes for flags */
extern BOOL hsc_get_chkid(HSCPRC * hp);
extern BOOL hsc_get_chkuri(HSCPRC * hp);
extern BOOL hsc_get_compact(HSCPRC * hp);
extern BOOL hsc_get_debug(HSCPRC * hp);
extern BOOL hsc_get_getsize(HSCPRC * hp);
extern BOOL hsc_get_htmlonly(HSCPRC * hp);
extern BOOL hsc_get_jerkvalues(HSCPRC * hp);
extern BOOL hsc_get_jens(HSCPRC * hp);
extern BOOL hsc_get_rplc_ent(HSCPRC * hp);
extern BOOL hsc_get_rplc_quote(HSCPRC * hp);
extern BOOL hsc_get_smart_ent(HSCPRC * hp);
extern BOOL hsc_get_strip_badws(HSCPRC * hp);
extern BOOL hsc_get_strip_cmt(HSCPRC * hp);
extern BOOL hsc_get_strip_ext(HSCPRC * hp);

/* get-methodes for internal flags */
extern BOOL hsc_get_fatal(HSCPRC * hp);
extern BOOL hsc_get_inside_anchor(HSCPRC * hp);
extern BOOL hsc_get_inside_pre(HSCPRC * hp);
extern BOOL hsc_get_suppress_output(HSCPRC * hp);

/* get-methodes for values */
extern STRPTR hsc_get_destdir(HSCPRC * hp);
extern STRPTR hsc_get_reldir(HSCPRC * hp);
extern STRPTR hsc_get_iconbase(HSCPRC * hp);
extern STRPTR hsc_get_server_dir(HSCPRC * hp);

/* get-methodes for internal values */
extern STRPTR hsc_get_click_here_str(HSCPRC * hp);
extern STRPTR hsc_get_file_name(HSCPRC * hp);
extern ULONG hsc_get_file_line(HSCPRC * hp);
extern ULONG hsc_get_file_column(HSCPRC * hp);
extern ULONG hsc_get_msg_count(HSCPRC * hp);

/* methodes for messages */
extern BOOL hsc_get_msg_ignore_notes(HSCPRC * hp);
extern BOOL hsc_get_msg_ignore_style(HSCPRC * hp);
extern BOOL hsc_get_msg_ignore_port(HSCPRC * hp);
extern BOOL hsc_set_msg_ignore_notes(HSCPRC * hp, BOOL value);
extern BOOL hsc_set_msg_ignore_style(HSCPRC * hp, BOOL value);
extern BOOL hsc_set_msg_ignore_port(HSCPRC * hp, BOOL value);
extern BOOL hsc_set_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id, HSCIGN value);
extern HSCIGN hsc_get_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id);
extern BOOL hsc_set_msg_class(HSCPRC * hp, HSCMSG_ID msg_id, HSCMSG_CLASS msg_class);
extern HSCMSG_CLASS hsc_get_msg_class(HSCPRC * hp, HSCMSG_ID msg_id);
extern VOID hsc_clear_msg_ignore(HSCPRC * hp);
extern VOID hsc_reset_msg_class(HSCPRC * hp);

/* output function */
extern STRPTR compactWs(HSCPRC * hp, STRPTR ws);
extern BOOL hsc_output_text(HSCPRC * hp, STRPTR wspc, STRPTR text);

/* misc. functions */
extern BOOL hsc_standard_nomem_handler(size_t size);

#endif /* NOEXTERN_HSC_HSCPRC */
#endif /* HSC_HSCPRC_H */
