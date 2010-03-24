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

#ifndef REN_MATRIX_H
#define REN_MATRIX_H

#include <ren/types.h>

/* Public */

extern RenMatrix*
ren_matrix_new (ren_size width, ren_size height,
    RenType type, ren_bool transposed);

extern RenMatrix*
ren_matrix_ref (RenMatrix *matrix);

extern void
ren_matrix_unref (RenMatrix *matrix);

extern void*
ren_matrix_begin_edit (RenMatrix *matrix);

extern void
ren_matrix_end_edit (RenMatrix *matrix);

/* Backend */

extern void
ren_matrix_data (RenMatrix *matrix,
    const void **data_p, ren_size *width_p, ren_size *height_p,
    RenType *type_p, ren_bool *transposed_p);

typedef struct _RenMatrixBackData RenMatrixBackData;
typedef struct _RenMatrixBackDataKey RenMatrixBackDataKey;
typedef void (* RenMatrixBackDataInitFunc) (RenMatrix *matrix,
    RenMatrixBackData *back_data, void* user_data);
typedef void (* RenMatrixBackDataFiniFunc) (RenMatrix *matrix,
    RenMatrixBackData *back_data, void* user_data);
typedef void (* RenMatrixBackDataUpdateFunc) (RenMatrix *matrix,
    RenMatrixBackData *back_data, void* user_data);

extern RenMatrixBackDataKey*
ren_matrix_back_data_key_new (ren_size data_size,
    RenMatrixBackDataInitFunc init,
    RenMatrixBackDataFiniFunc fini,
    RenMatrixBackDataUpdateFunc update);

extern void
ren_matrix_back_data_key_user_data (RenMatrixBackDataKey *key,
    void *user_data);

extern RenMatrixBackDataKey*
ren_matrix_back_data_key_ref (RenMatrixBackDataKey *key);

extern void
ren_matrix_back_data_key_unref (RenMatrixBackDataKey *key);

extern RenMatrixBackData*
ren_matrix_back_data (RenMatrix *matrix, RenMatrixBackDataKey *key);

#endif /* REN_MATRIX_H */
