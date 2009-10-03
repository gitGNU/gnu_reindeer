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

#ifndef REN_IMPL_H
#define REN_IMPL_H

#include <ren/types.h>
#include <ren/tmpl.h>

#ifdef REN_IMPL_NAME
    #define _REN_IMPL_MANGLE3(NAME, F) ren__##NAME##__##F
    #define _REN_IMPL_MANGLE2(NAME, F) _REN_IMPL_MANGLE3(NAME, F)
    #define REN_IMPL_MANGLE(F) _REN_IMPL_MANGLE2(REN_IMPL_NAME, F)
    #define _REN_IMPL_PRIV_MANGLE3(NAME, F) _ren__##NAME##__##F
    #define _REN_IMPL_PRIV_MANGLE2(NAME, F) _REN_IMPL_PRIV_MANGLE3(NAME, F)
    #define REN_IMPL_PRIV_MANGLE(F) _REN_IMPL_PRIV_MANGLE2(REN_IMPL_NAME, F)
    /* I hate C preprocessor.  */
#endif

typedef ren_bool (* RenBackendInitFunc) (RenBackend *backend);
typedef ren_bool (* RenBackendFiniFunc) (void);

typedef struct _RenReindeerBackData RenReindeerBackData;
typedef void (* RenReindeerInitFunc) (RenReindeer *r, RenReindeerBackData *back_data);
typedef void (* RenReindeerFiniFunc) (RenReindeer *r, RenReindeerBackData *back_data);

extern RenReindeerBackData*
ren_reindeer_back_data (RenReindeer *r);

extern RenMatrix*
ren_matrix_ref (RenMatrix *matrix);

extern void
ren_matrix_unref (RenMatrix *matrix);

extern void
ren_matrix_data (RenMatrix *matrix,
    const void **data_p, ren_size *width_p, ren_size *height_p,
    RenType *type_p, ren_bool *transposed_p);

typedef struct _RenMatrixBackData RenMatrixBackData;
typedef struct _RenMatrixBackDataKey RenMatrixBackDataKey;
typedef void (* RenMatrixBackDataInitFunc) (RenMatrix *matrix,
    RenMatrixBackData *back_data);
typedef void (* RenMatrixBackDataFiniFunc) (RenMatrix *matrix,
    RenMatrixBackData *back_data);
typedef void (* RenMatrixBackDataUpdateFunc) (RenMatrix *matrix,
    RenMatrixBackData *back_data);

extern RenMatrixBackDataKey*
ren_matrix_back_data_key_new (ren_size size, RenMatrixBackDataInitFunc init,
    RenMatrixBackDataFiniFunc fini, RenMatrixBackDataUpdateFunc update);

extern RenMatrixBackDataKey*
ren_matrix_back_data_key_ref (RenMatrixBackDataKey *key);

extern void
ren_matrix_back_data_key_unref (RenMatrixBackDataKey *key);

extern RenMatrixBackData*
ren_matrix_back_data (RenMatrix *matrix, RenMatrixBackDataKey *key);

extern RenVector*
ren_vector_ref (RenVector *vector);

extern void
ren_vector_unref (RenVector *vector);

extern void
ren_vector_data (RenVector *vector,
    const void **data_p, ren_size *length_p, RenType *type_p);

extern RenColor*
ren_color_ref (RenColor *color);

extern void
ren_color_unref (RenColor *color);

extern void
ren_color_data (RenColor *color,
    const void **data_p, RenColorFormat *format_p, RenType *type_p);

typedef struct _RenColorBackData RenColorBackData;
typedef struct _RenColorBackDataKey RenColorBackDataKey;
typedef void (* RenColorBackDataInitFunc) (RenColor *color,
    RenColorBackData *back_data);
typedef void (* RenColorBackDataFiniFunc) (RenColor *color,
    RenColorBackData *back_data);
typedef void (* RenColorBackDataUpdateFunc) (RenColor *color,
    RenColorBackData *back_data);

extern RenColorBackDataKey*
ren_color_back_data_key_new (ren_size size, RenColorBackDataInitFunc init,
    RenColorBackDataFiniFunc fini, RenColorBackDataUpdateFunc update);

extern RenColorBackDataKey*
ren_color_back_data_key_ref (RenColorBackDataKey *key);

extern void
ren_color_back_data_key_unref (RenColorBackDataKey *key);

extern RenColorBackData*
ren_color_back_data (RenColor *color, RenColorBackDataKey *key);

extern RenDataBlock*
ren_data_block_ref (RenDataBlock *data_block);

extern void
ren_data_block_unref (RenDataBlock *data_block);

extern void
ren_data_block_data (RenDataBlock *data_block, const void **data_p,
    ren_size *size_p, RenUsage *usage_p);

extern RenCoordArray*
ren_coord_array_ref (RenCoordArray *vx_array);

extern void
ren_coord_array_unref (RenCoordArray *vx_array);

extern void
ren_coord_array_data (RenCoordArray *vx_array, RenDataBlock **data_block_p,
    ren_size *start_p, ren_size *count_p, ren_size *stride_p);

extern void
ren_coord_array_type (RenCoordArray *vx_array,
    RenType *type_p, ren_uint08 *num_p);

extern RenColorArray*
ren_color_array_ref (RenColorArray *vx_array);

extern void
ren_color_array_unref (RenColorArray *vx_array);

extern void
ren_color_array_data (RenColorArray *vx_array, RenDataBlock **data_block_p,
    ren_size *start_p, ren_size *count_p, ren_size *stride_p);

extern void
ren_color_array_type (RenColorArray *vx_array,
    RenType *type_p, RenColorFormat *format_p);

extern RenNormalArray*
ren_normal_array_ref (RenNormalArray *vx_array);

extern void
ren_normal_array_unref (RenNormalArray *vx_array);

extern void
ren_normal_array_data (RenNormalArray *vx_array, RenDataBlock **data_block_p,
    ren_size *start_p, ren_size *count_p, ren_size *stride_p);

extern void
ren_normal_array_type (RenNormalArray *vx_array,
    RenType *type_p, ren_uint08 *num_p);

extern RenIndexArray*
ren_index_array_ref (RenIndexArray *ix_array);

extern void
ren_index_array_unref (RenIndexArray *ix_array);

extern void
ren_index_array_data (RenIndexArray *ix_array, RenType *type_p,
    RenDataBlock **data_block_p, ren_size *start_p, ren_size *count_p);

typedef struct _RenTemplatePrimitive RenTemplatePrimitive;
struct _RenTemplatePrimitive
{
    ren_uint32 mode;
    RenPrimitive prim;
    ren_size offset;
    ren_size count;
};

extern RenTemplate*
ren_template_ref (RenTemplate *template);

extern void
ren_template_unref (RenTemplate *template);

extern void
ren_template_data_primitives (RenTemplate *template,
    RenIndexArray **ix_array_p,
    ren_size *num_primitives_p, const RenTemplatePrimitive **primitives_p);

extern RenObject*
ren_object_ref (RenObject *object);

extern void
ren_object_unref (RenObject *object);

extern void
ren_object_data (RenObject *object, RenTemplate **template_p,
    RenCoordArray **coord_array_p, RenColorArray **color_array_p,
    RenNormalArray **normal_array_p/*, RenEdgeArray **edge_array_p*/);

extern void
ren_object_change_mode (RenReindeer *r, RenObject *object,
    ren_uint32 prev_mode, ren_uint32 next_mode, void *user_data);

extern void
ren_object_begin_mode (RenReindeer *r,
    RenTemplate *prev_template, ren_uint32 prev_mode,
    RenObject *object, ren_uint32 mode, void *user_data);

extern RenMaterial*
ren_material_ref (RenMaterial *material);

extern void
ren_material_unref (RenMaterial *material);

extern void
ren_material_data_light (RenMaterial *material,
    RenColor **ambient_p, RenColor **diffuse_p, RenColor **specular_p,
    RenColor **emission_p, ren_dfloat *shininess_p);

extern RenLight*
ren_light_ref (RenLight *light);

extern void
ren_light_unref (RenLight *light);

extern void
ren_light_data (RenLight *light,
    RenLightType *type_p,
    RenColor **ambient_p,
    RenColor **diffuse_p,
    RenColor **specular_p);

extern void
ren_light_data_point_light (RenLight *light,
    RenVector **attenuation_p);

extern void
ren_light_data_spot_light (RenLight *light,
    RenVector **attenuation_p,
    ren_dfloat *cutoff_p,
    ren_dfloat *exponent_p);

#endif /* REN_IMPL_H */
