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

struct _RenCoordArray
{
    RenVertexArray parent;

    RenType type;
    ren_ubyte num;
};

RenCoordArray*
ren_coord_array_new (RenType type, ren_ubyte num,
    RenDataBlock *datablock, ren_offset start, ren_size count, ren_size stride)
{
    RenCoordArray *vxarray;

    vxarray = (RenCoordArray *) calloc (1, sizeof (struct _RenCoordArray));
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
ren_coord_array_destroy (RenCoordArray *vxarray)
{
    _ren_coord_array_unref (vxarray);
}

void
ren_coord_array_set_size (RenCoordArray *vxarray, ren_size count)
{
    vxarray->parent.count = count;
}

void
_ren_coord_array_ref (RenCoordArray *vxarray)
{
    ++(vxarray->parent.ref);
}

void
_ren_coord_array_unref (RenCoordArray *vxarray)
{
    if (--(vxarray->parent.ref) == 0)
    {
        free (vxarray);
    }
}

void
_ren_coord_array_type (RenCoordArray *vxarray, RenType *typep, ren_uint *nump)
{
    if (typep)
        (*typep) = vxarray->type;
    if (nump)
        (*nump) = vxarray->num;
}
