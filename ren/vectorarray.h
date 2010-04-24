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

#ifndef REN_COORDARRAY_H
#define REN_COORDARRAY_H

#include <ren/types.h>

/* Public */

extern RenVectorArray*
ren_vector_array_new (RenType type, ren_uint08 num,
	RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);

extern RenVectorArray*
ren_vector_array_ref (RenVectorArray *array);

extern void
ren_vector_array_unref (RenVectorArray *array);

extern void
ren_vector_array_set_size (RenVectorArray *array, ren_size count);

/* Backend */

extern void
ren_vector_array_data (RenVectorArray *array, RenDataBlock **data_block_p,
	ren_size *start_p, ren_size *count_p, ren_size *stride_p);

extern void
ren_vector_array_type (RenVectorArray *array,
	RenType *type_p, ren_uint08 *num_p);

#endif /* REN_COORDARRAY_H */
