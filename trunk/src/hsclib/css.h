/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2001-2003  Matthias Bethke
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
 * hsclib/css.h
 *
 * Support functions for Cascading Stylesheet values
 *
 * created: 23-Mar-2003
 */

#ifndef HSC_CSS_H
#define HSC_CSS_H

#ifndef NOEXTERN_HSCLIB_CSS_H
extern BOOL add_styleattr(HSCPRC *hp, CONSTRPTR property, CONSTRPTR value);
extern VOID del_styleattr(APTR data);
extern BOOL add_width_height_attrs(HSCPRC *hp, ULONG width, ULONG height);
#endif /* NOEXTERN_CSS_H */

#endif /* HSC_CSS_H */