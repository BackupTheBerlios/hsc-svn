/*
** tag_a.h
**
** tag handle for "<A xx>"
**
*/

#ifndef HSC_TAG_A_H
#define HSC_TAG_A_H

#include "ugly/types.h"
#include "ugly/infile.h"

#include "tag.h"

extern BOOL inside_anchor;

extern BOOL handle_anchor( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_canchor( INFILE *inpf, HSCTAG *tag);

#if 0
extern BOOL handle_anchor( INFILE *inpf );
extern BOOL handle_canchor( INFILE *inpf );
#endif

#endif /* HSC_TAG_A_H */