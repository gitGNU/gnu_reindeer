/*
    This file is part of Reindeer.

    Copyright (C) 2010 - Patrik Olsson

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

#ifndef REN_COLORARRAY_H
#define REN_COLORARRAY_H

#include <ren/types.h>

/* Public */

extern RenColorArray*
ren_color_array_new (RenType type, RenColorFormat format,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);

extern RenColorArray*
ren_color_array_ref (RenColorArray *vx_array);

extern void
ren_color_array_unref (RenColorArray *vx_array);

extern void
ren_color_array_set_size (RenColorArray *vxarray, ren_size count);

/* Backend */

extern void
ren_color_array_data (RenColorArray *vx_array, RenDataBlock **data_block_p,
    ren_size *start_p, ren_size *count_p, ren_size *stride_p);

extern void
ren_color_array_type (RenColorArray *vx_array,
    RenType *type_p, RenColorFormat *format_p);

#endif /* REN_COLORARRAY_H */
