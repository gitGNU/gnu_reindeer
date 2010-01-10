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

#include <ren/indexarray.h>
#include <ren/datablock.h>
#include <glib.h>

struct _RenIndexArray
{
    ren_uint32 ref_count;

    RenType type;
    RenDataBlock *data_block;
    ren_size start;
    ren_size count;
};

RenIndexArray*
ren_index_array_new (RenType type, RenDataBlock *data_block,
    ren_size start, ren_size count)
{
    if (type != REN_TYPE_UINT08 && type != REN_TYPE_UINT16 &&
        type != REN_TYPE_UINT32 && type != REN_TYPE_UINT64)
    {
        return NULL;
    }
    if (data_block == NULL)
        return NULL;

    RenIndexArray *ix_array = g_new0 (RenIndexArray, 1);

    ix_array->ref_count = 1;

    ix_array->type = type;
    ix_array->data_block = ren_data_block_ref (data_block);
    ix_array->start = start;
    ix_array->count = count;

    return ix_array;
}

RenIndexArray*
ren_index_array_ref (RenIndexArray *ix_array)
{
    ++(ix_array->ref_count);
    return ix_array;
}

void
ren_index_array_unref (RenIndexArray *ix_array)
{
    if (--(ix_array->ref_count) > 0)
        return;

    ren_data_block_unref (ix_array->data_block);
    g_free (ix_array);
}

void
ren_index_array_set_size (RenIndexArray *ix_array, ren_size count)
{
    ix_array->count = count;
}

void
ren_index_array_data (RenIndexArray *ix_array, RenType *type_p,
    RenDataBlock **data_block_p, ren_size *start_p, ren_size *count_p)
{
    if (type_p)
        (*type_p) = ix_array->type;
    if (data_block_p)
        (*data_block_p) = ix_array->data_block;
    if (start_p)
        (*start_p) = ix_array->start;
    if (count_p)
        (*count_p) = ix_array->count;
}
