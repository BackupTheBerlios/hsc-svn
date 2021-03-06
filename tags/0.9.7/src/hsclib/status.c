/*
** hsclib/status.c
**
** status message functions for hsc
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
** updated: 15-Mar-1996
** created: 30-Jul-1995
**
*/

#include "hsclib/inc_base.h"

VOID hsc_status_misc( HSCPRC *hp, STRPTR s )
{
    if ( hp->CB_status_misc )
        (*hp->CB_status_misc)( hp, s );
}

VOID hsc_status_file_begin( HSCPRC *hp )
{
    if ( hp->CB_status_file_begin )
        (*hp->CB_status_file_begin)( hp );
}

VOID hsc_status_file_end( HSCPRC *hp )
{
    if ( hp->CB_status_file_end )
        (*hp->CB_status_file_end)( hp );
}

VOID hsc_status_line( HSCPRC *hp )
{
    if ( hp->CB_status_line )
        (*hp->CB_status_line)( hp );
}

