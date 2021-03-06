/*
** tag_hsc.h
**
** tag handles for "<HSC_xx>"
**
*/

#ifndef HSC_TAG_HSC_H
#define HSC_TAG_HSC_H

#include "ugly/types.h"
#include "ugly/infile.h"

#include "tag.h"

/*
**
** extern references
**
*/
extern BOOL handle_hsc_comment( INFILE *inpf, HSCTAG *tag );

extern BOOL handle_hsc_tag( INFILE *inpf, HSCTAG *tag ); /* TODO: remove this */
extern BOOL handle_hsc_defent( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_deftag( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_insert( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_include( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_onlycopy( INFILE *inpf, HSCTAG *tag );

#endif /* HSC_TAG_HSC_H */
