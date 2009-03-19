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

#ifndef _REN_VERTEX_ARRAY_H
#define _REN_VERTEX_ARRAY_H

#include <ren/types.h>

struct _RenVertexArray
{
    ren_uint ref;
#if 0
    ren_array__t *shares; /* of ren_pointer */
#endif

    RenDataBlock *datablock;
    ren_offset start;
    ren_size count;
    ren_size stride;
};

extern ren_bool
_ren_vertex_array_init (RenVertexArray *vxarray,
    RenDataBlock *datablock, ren_offset start, ren_size count, ren_size stride);

#endif /* _REN_VERTEX_ARRAY_H */
