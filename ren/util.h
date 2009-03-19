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

#ifndef REN_LIST_H_
#define REN_LIST_H_

#include <ren/types.h>

typedef struct _RenSList _RenSList;
typedef struct _RenDList _RenDList;
typedef struct _RenArray _RenArray;

typedef struct _RenSList _RenStack;

struct _RenSList
{
    void *data;
    _RenSList *next;
};

struct _RenDList
{
    void *data;
    _RenDList *next;
    _RenDList *prev;
};

struct _RenArray
{
    ren_size length;
    void *data[];
};

/* TODO: _RenCList (cycle list) for backend.  */

extern _RenDList*
_ren_dlist_prepend (_RenDList *list, void *data);

extern _RenDList*
_ren_dlist_remove_link (_RenDList *list, _RenDList *link);

extern _RenDList*
_ren_dlist_delete_link (_RenDList *list, _RenDList *link);

/*
RenArray*
_ren_array_new (ren_size reserved_size);
*/

/*
extern _RenStack*
_ren_stack_push (_RenStack *stack, void *data);

extern _RenStack*
_ren_stack_pop (_RenStack *stack, void **datap);
*/

#endif /* REN_LIST_H_ */
