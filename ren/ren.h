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

#ifndef REN_REN_H
#define REN_REN_H

#include <ren/types.h>
#include <ren/tmpl.h>

extern ren_bool
ren_library_init (void);

extern void
ren_library_exit (void);

extern RenReindeer*
ren_load (RenBackend *backend);

extern void
ren_unload (RenReindeer *r);

extern RenBackend*
ren_backend (RenReindeer *r);

extern RenBackend*
ren_lookup_backend (const char *name);

static inline ren_size
ren_type_sizeof (RenType type)
{
    static const ren_size type_sizes[] =
    {
        0,                      /*REN_TYPE_NONE*/
        sizeof (ren_bool),      /*REN_TYPE_BOOL*/
        sizeof (ren_sint08),    /*REN_TYPE_SINT08*/
        sizeof (ren_uint08),    /*REN_TYPE_UINT08*/
        sizeof (ren_sint16),    /*REN_TYPE_SINT16*/
        sizeof (ren_uint16),    /*REN_TYPE_UINT16*/
        sizeof (ren_sint16),    /*REN_TYPE_SINT32*/
        sizeof (ren_uint16),    /*REN_TYPE_UINT32*/
        sizeof (ren_sint32),    /*REN_TYPE_SINT64*/
        sizeof (ren_uint32),    /*REN_TYPE_UINT64*/
        sizeof (ren_sfloat),    /*REN_TYPE_SFLOAT*/
        sizeof (ren_dfloat),    /*REN_TYPE_DFLOAT*/
    };
    if (type < (sizeof (type_sizes)/sizeof (type_sizes[0])))
        return type_sizes[type];
    else
        return 0;
}

extern RenMatrix*
ren_matrix_new (const void *data, ren_size width, ren_size height,
    RenType type, ren_bool transposed);

extern void
ren_matrix_destroy (RenMatrix *matrix);

extern void
ren_matrix_changed (RenMatrix *matrix);

extern RenVector*
ren_vector_new (const void *data, ren_size length, RenType type);

extern void
ren_vector_destroy (RenVector *vector);

extern void
ren_vector_changed (RenVector *vector);

extern RenColor*
ren_color_new (const void *data, RenColorFormat format, RenType type);

extern void
ren_color_destroy (RenColor *color);

extern void
ren_color_changed (RenColor *color);

extern RenDataBlock*
ren_data_block_new (const void *data, ren_size size, RenUsage usage);

extern void
ren_data_block_destroy (RenDataBlock *datablock);

extern void
ren_data_block_callback (
    RenDataBlock *datablock,
    RenDataBlockCallback reload_func,
    RenDataBlockCallback unload_func,
    void *user_data);

extern void
ren_data_block_relocated (RenDataBlock *datablock, const void *data);

extern void
ren_data_block_resized (RenDataBlock *datablock, ren_size size);

extern void
ren_data_block_changed (RenDataBlock *datablock, ren_size from, ren_size to);

extern RenCoordArray*
ren_coord_array_new (RenType type, ren_uint08 num,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);

extern void
ren_coord_array_destroy (RenCoordArray *vxarray);

extern void
ren_coord_array_set_size (RenCoordArray *vxarray, ren_size count);

extern RenColorArray*
ren_color_array_new (RenType type, RenColorFormat format,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);

extern void
ren_color_array_destroy (RenColorArray *vxarray);

extern void
ren_color_array_set_size (RenColorArray *vxarray, ren_size count);

extern RenNormalArray*
ren_normal_array_new (RenType type, ren_uint08 num,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);

extern void
ren_normal_array_destroy (RenNormalArray *vxarray);

extern void
ren_normal_array_set_size (RenNormalArray *vxarray, ren_size count);

/* Index arrays for primitives */

extern RenIndexArray*
ren_index_array_new (RenType type, RenDataBlock *datablock,
    ren_size start, ren_size count);

extern RenIndexArray*
ren_index_array_new_from_range (ren_uint32 from, ren_uint32 to);

extern void
ren_index_array_destroy (RenIndexArray *ixarray);

extern void
ren_index_array_set_size (RenIndexArray *ixarray, ren_size count);

/* Lighting */

extern RenLight*
ren_light_new (RenLightType type);

extern void
ren_light_destroy (RenLight *light);

extern void
ren_light_ambient (RenLight *light, RenColor *color);

extern void
ren_light_diffuse (RenLight *light, RenColor *color);

extern void
ren_light_specular (RenLight *light, RenColor *color);

extern void
ren_light_attenuation (RenLight *light, RenVector *k);

#define _REN_FUNC(F)\
    extern _REN_RET(F) _REN_SYM(F) _REN_PRM(F);
#include <ren/funcs.h>
#undef _REN_FUNC

#endif /* REN_REN_H */
