/*
    This file is part of Reindeer.

    Copyright (C) 2008, 2009 - Patrik Olsson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ren/util.h>

#include <stdlib.h>

_RenDList*
_ren_dlist_prepend (_RenDList *list, void *data)
{
    _RenDList *new_list;

    new_list = (_RenDList *) calloc (1, sizeof (struct _RenDList));
    new_list->data = data;
    new_list->next = list;

    if (list)
    {
        new_list->prev = list->prev;
        if (list->prev)
	        list->prev->next = new_list;
        list->prev = new_list;
    }
    else
        new_list->prev = NULL;

    return new_list;
}

_RenDList*
_ren_dlist_remove_link (_RenDList *list, _RenDList *link)
{
    if (link)
    {
        if (link->prev)
	        link->prev->next = link->next;
        if (link->next)
	        link->next->prev = link->prev;

        if (link == list)
	        list = list->next;

        link->next = NULL;
        link->prev = NULL;
    }

    return list;
}

_RenDList*
_ren_dlist_delete_link (_RenDList *list, _RenDList *link)
{
    list = _ren_dlist_remove_link (list, link);
    free (link);
    return list;
}
