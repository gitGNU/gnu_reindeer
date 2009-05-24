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

struct _RenNormalArray
{
    ren_uint32 ref_count;

    RenDataBlock *datablock;
    ren_size start;
    ren_size count;
    ren_size stride;

    RenType type;
    ren_uint08 num;
};

RenNormalArray*
ren_normal_array_new (RenType type, ren_uint08 num,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride)
{
    /* FIXME: Check type and num input... */

    RenNormalArray *vxarray = g_new0 (RenNormalArray, 1);

    vxarray->ref_count = 1;

    vxarray->datablock = datablock;
    vxarray->start = start;
    vxarray->count = count;
    vxarray->stride = stride;

    vxarray->type = type;
    vxarray->num = num;

    return vxarray;
}

void
ren_normal_array_destroy (RenNormalArray *vxarray)
{
    ren_normal_array_unref (vxarray);
}

void
ren_normal_array_set_size (RenNormalArray *vxarray, ren_size count)
{
    vxarray->count = count;
}

void
ren_normal_array_ref (RenNormalArray *vxarray)
{
    ++(vxarray->ref_count);
}

void
ren_normal_array_unref (RenNormalArray *vxarray)
{
    if (--(vxarray->ref_count) > 0)
        return;

    g_free (vxarray);
}

void
ren_normal_array_data (RenNormalArray *vxarray, RenDataBlock **datablockp,
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
ren_normal_array_type (RenNormalArray *vxarray,
    RenType *typep, ren_uint08 *nump)
{
    if (typep)
        (*typep) = vxarray->type;
    if (nump)
        (*nump) = vxarray->num;
}
