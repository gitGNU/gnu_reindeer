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

#ifndef REN_VECTOR_H
#define REN_VECTOR_H

#include <ren/types.h>

/* Public */

extern RenVector*
ren_vector_new (ren_size length, RenType type);

extern RenVector*
ren_vector_ref (RenVector *vector);

extern void
ren_vector_unref (RenVector *vector);

extern void*
ren_vector_begin_edit (RenVector *vector);

extern void
ren_vector_end_edit (RenVector *vector);

/* Backend */

extern void
ren_vector_data (RenVector *vector,
    const void **data_p, ren_size *length_p, RenType *type_p);

typedef struct _RenVectorBackData RenVectorBackData;
typedef struct _RenVectorBackDataKey RenVectorBackDataKey;
typedef void (* RenVectorBackDataInitFunc) (RenVector *vector,
    RenVectorBackData *back_data);
typedef void (* RenVectorBackDataFiniFunc) (RenVector *vector,
    RenVectorBackData *back_data);
typedef void (* RenVectorBackDataUpdateFunc) (RenVector *vector,
    RenVectorBackData *back_data);

extern RenVectorBackDataKey*
ren_vector_back_data_key_new (ren_size data_size,
    RenVectorBackDataInitFunc init,
    RenVectorBackDataFiniFunc fini,
    RenVectorBackDataUpdateFunc update);

extern RenVectorBackDataKey*
ren_vector_back_data_key_ref (RenVectorBackDataKey *key);

extern void
ren_vector_back_data_key_unref (RenVectorBackDataKey *key);

extern RenVectorBackData*
ren_vector_back_data (RenVector *vector, RenVectorBackDataKey *key);

#endif /* REN_VECTOR_H */
