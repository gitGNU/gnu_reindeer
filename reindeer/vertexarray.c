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

ren_bool
_ren_vertex_array_init (RenVertexArray *vxarray,
    RenDataBlock *datablock, ren_offset start, ren_size count, ren_size stride)
{
    vxarray->ref = 1;
    vxarray->datablock = datablock;
    vxarray->start = start;
    vxarray->count = count;
    vxarray->stride = stride;

    /* TODO: Initialize the shares part...  */

    return REN_TRUE;
}

void
_ren_vertex_array_data (RenVertexArray *vxarray, RenDataBlock **datablockp,
    ren_offset *startp, ren_size *countp, ren_size *stridep)
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