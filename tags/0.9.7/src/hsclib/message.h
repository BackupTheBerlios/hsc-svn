/*
** hsclib/message.h
**
** message funcs for hsc
**
*/

#ifndef HSCLIB_MESSAGE_H
#define HSCLIB_MESSAGE_H

/*
** global funcs
*/
#ifndef NOEXTERN_HSCLIB_MESSAGE_H

extern VOID hsc_message( HSCPRC *hp, HSCMSG_ID msg_id,
                         const char *format, ... );

extern VOID hsc_msg_eof( HSCPRC *hp, STRPTR descr );
extern VOID hsc_msg_illg_whtspc( HSCPRC *hp );
extern VOID hsc_msg_stripped_tag( HSCPRC *hp, HSCTAG *tag, STRPTR why );
extern VOID hsc_msg_unkn_attr( HSCPRC *hp, STRPTR attr );
extern VOID hsc_msg_eol( HSCPRC *hp );

#endif /* NOEXTERN_HSCLIB_MESSAGE */
#endif /* HSCLIB_MESSAGE_H */
