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

struct _RenColorArray
{
    ren_uint32 ref_count;

    RenDataBlock *datablock;
    ren_size start;
    ren_size count;
    ren_size stride;

    RenType type;
    RenColorFormat format;
};

RenColorArray*
ren_color_array_new (RenType type, RenColorFormat format,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride)
{
    /* FIXME: Check type and num input... */

    RenColorArray *vxarray = g_new0 (RenColorArray, 1);

    vxarray->ref_count = 1;

    vxarray->datablock = datablock;
    vxarray->start = start;
    vxarray->count = count;
    vxarray->stride = stride;

    vxarray->type = type;
    vxarray->format = format;

    return vxarray;
}

void
ren_color_array_destroy (RenColorArray *vxarray)
{
    _ren_color_array_unref (vxarray);
}

void
ren_color_array_set_size (RenColorArray *vxarray, ren_size count)
{
    vxarray->count = count;
}

void
_ren_color_array_ref (RenColorArray *vxarray)
{
    ++(vxarray->ref_count);
}

void
_ren_color_array_unref (RenColorArray *vxarray)
{
    if (--(vxarray->ref_count) > 0)
        return;

    g_free (vxarray);
}

void
_ren_color_array_data (RenColorArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep)
{
    if (datablockp)
        (*datablockp) = vxarray->datablock;
    if (startp)
        (*startp) = vxarray->start;
    if (countp)
        (*countp) = vxarray->count;
    if (stridep)
        (*stridep) = vxarray->stride;
}

void
_ren_color_array_type (RenColorArray *vxarray,
    RenType *typep, RenColorFormat *formatp)
{
    if (typep)
        (*typep) = vxarray->type;
    if (formatp)
        (*formatp) = vxarray->format;
}
