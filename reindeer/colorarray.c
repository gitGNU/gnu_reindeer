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

#include "vertexarray.h"

#include <ren/ren.h>
#include <ren/impl.h>

#include <stdlib.h>

struct _RenColorArray
{
    RenVertexArray parent;

    RenType type;
    ren_ubyte num;
};

RenColorArray*
ren_color_array_new (RenType type, ren_ubyte num,
    RenDataBlock *datablock, ren_offset start, ren_size count, ren_size stride)
{
    RenColorArray *vxarray;

    vxarray = (RenColorArray *) calloc (1, sizeof (struct _RenColorArray));
    if (!vxarray)
        goto FAIL;

    if (!_ren_vertex_array_init (&(vxarray->parent),
        datablock, start, count, stride))
    {
        goto FAIL;
    }

    /* FIXME: Check type and num input... */
    vxarray->type = type;
    vxarray->num = num;

    return vxarray;

    FAIL:
    if (vxarray) free (vxarray);
    return NULL;
}

void
ren_color_array_destroy (RenColorArray *vxarray)
{
    _ren_color_array_unref (vxarray);
}

void
ren_color_array_set_size (RenColorArray *vxarray, ren_size count)
{
    vxarray->parent.count = count;
}

void
_ren_color_array_ref (RenColorArray *vxarray)
{
    ++(vxarray->parent.ref);
}

void
_ren_color_array_unref (RenColorArray *vxarray)
{
    if (--(vxarray->parent.ref) == 0)
    {
        free (vxarray);
    }
}

void
_ren_color_array_type (RenColorArray *vxarray, RenType *typep, ren_uint *nump)
{
    if (typep)
        (*typep) = vxarray->type;
    if (nump)
        (*nump) = vxarray->num;
}
