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

#include <stdlib.h>

struct _RenIndexArray
{
    ren_uint ref;
    RenType type;
    RenDataBlock *datablock;
    ren_offset start;
    ren_size count;
};

RenIndexArray*
ren_index_array_new (RenType type, RenDataBlock *datablock,
    ren_offset start, ren_size count)
{
    RenIndexArray *ixarray;

    ixarray = (RenIndexArray *) calloc (1, sizeof (struct _RenIndexArray));
    if (!ixarray)
        goto FAIL;

    ixarray->ref = 1;
    ixarray->type = type;
    ixarray->datablock = datablock;
    ixarray->start = start;
    ixarray->count = count;

    return ixarray;

    FAIL:
    if (ixarray) free (ixarray);
    return NULL;
}

RenIndexArray*
ren_index_array_new_from_range (ren_uint from, ren_uint to)
{
    _ren_throw_error ("ren_index_array_new_from_range not implemented.");
    return NULL;
}

void
ren_index_array_destroy (RenIndexArray *ixarray)
{
    _ren_index_array_unref (ixarray);
}

void
ren_index_array_set_size (RenIndexArray *ixarray, ren_size count)
{
    ixarray->count = count;
}

void
_ren_index_array_ref (RenIndexArray *ixarray)
{
    ++(ixarray->ref);
}

void
_ren_index_array_unref (RenIndexArray *ixarray)
{
    if (--(ixarray->ref) == 0)
    {
        free (ixarray);
    }
}

void
_ren_index_array_data (RenIndexArray *ixarray, RenType *typep,
    RenDataBlock **datablockp, ren_offset *startp, ren_size *countp)
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
