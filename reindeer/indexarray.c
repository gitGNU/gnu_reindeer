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

#include <ren/ren.h>
#include <ren/impl.h>
#include <glib.h>

struct _RenIndexArray
{
    ren_uint32 ref_count;

    RenType type;
    RenDataBlock *datablock;
    ren_size start;
    ren_size count;
};

RenIndexArray*
ren_index_array_new (RenType type, RenDataBlock *datablock,
    ren_size start, ren_size count)
{
    RenIndexArray *ixarray = g_new0 (RenIndexArray, 1);

    ixarray->ref_count = 1;

    ixarray->type = type;
    ixarray->datablock = datablock;
    ixarray->start = start;
    ixarray->count = count;

    return ixarray;
}

RenIndexArray*
ren_index_array_new_from_range (ren_uint32 from, ren_uint32 to)
{
    g_warning ("ren_index_array_new_from_range not implemented.");
    return NULL;
}

void
ren_index_array_destroy (RenIndexArray *ixarray)
{
    ren_index_array_unref (ixarray);
}

void
ren_index_array_set_size (RenIndexArray *ixarray, ren_size count)
{
    ixarray->count = count;
}

void
ren_index_array_ref (RenIndexArray *ixarray)
{
    ++(ixarray->ref_count);
}

void
ren_index_array_unref (RenIndexArray *ixarray)
{
    if (--(ixarray->ref_count) > 0)
        return;

    g_free (ixarray);
}

void
ren_index_array_data (RenIndexArray *ixarray, RenType *typep,
    RenDataBlock **datablockp, ren_size *startp, ren_size *countp)
{
    if (typep)
        (*typep) = ixarray->type;
    if (datablockp)
        (*datablockp) = ixarray->datablock;
    if (startp)
        (*startp) = ixarray->start;
    if (countp)
        (*countp) = ixarray->count;
}
