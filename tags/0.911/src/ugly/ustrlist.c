/*
 * ugly/ustrlist.c
 *
 * string list functions
 *
 * Copyright (C) 1996  Thomas Aglassinger
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
 * updated: 14-Oct-1996
 * created: 14-Oct-1996
 *
 *-------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define NOEXTERN_UGLY_USTRLIST_H
#include "ustrlist.h"

/*
 * del_string_node
 */
VOID del_string_node(APTR data)
{
    STRPTR s = (STRPTR) data;
    ufreestr(s);
}

/*
 * new_string_node
 */
STRPTR new_string_node(STRPTR data)
{
#if 0
    D(fprintf(stderr, DHL "new string `%s'\n", data);
#endif
    return (strclone(data));
}

/*
 * cmp_string_node
 */
int cmp_string_node(APTR cmp_data, APTR lst_data)
{
    STRPTR s1 = (STRPTR) cmp_data;
    STRPTR s2 = (STRPTR) lst_data;

#ifdef DEBUG_UGLY
    if (!cmp_data)
        panic("cmp_data = NULL");
    if (!lst_data)
        panic("lst_data = NULL");
#endif

    if (!strcmp(s1, s2))
        return (-1);
    else
        return (0);
}

/*
 * del_strlist: cleanup whole list of strings
 */
VOID del_strlist(DLLIST *list)
{
    del_dllist(list);
}

/*
 * del_strlist: cleanup whole list of strings
 */
VOID clr_strlist(DLLIST *list)
{
    del_all_dlnodes(list);
}

/*
 * init_strlist: set up new list of strings
 */
DLLIST *init_strlist(VOID)
{
    return( init_dllist(del_string_node) );
}


