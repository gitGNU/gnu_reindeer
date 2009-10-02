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

extern void
ren_matrix_ref (RenMatrix *matrix);

extern void
ren_matrix_unref (RenMatrix *matrix);

extern void
ren_matrix_data (RenMatrix *matrix,
    const void **datap, ren_size *widthp, ren_size *heightp,
    RenType *typep, ren_bool *transposedp);

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

extern void
ren_matrix_back_data_key_ref (RenMatrixBackDataKey *key);

extern void
ren_matrix_back_data_key_unref (RenMatrixBackDataKey *key);

extern RenMatrixBackData*
ren_matrix_back_data (RenMatrix *matrix, RenMatrixBackDataKey *key);

extern void
ren_vector_ref (RenVector *vector);

extern void
ren_vector_unref (RenVector *vector);

extern void
ren_color_ref (RenColor *color);

extern void
ren_color_unref (RenColor *color);

extern void
ren_color_data (RenColor *color,
    const void **datap, RenColorFormat *formatp, RenType *typep);

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

extern void
ren_color_back_data_key_ref (RenColorBackDataKey *key);

extern void
ren_color_back_data_key_unref (RenColorBackDataKey *key);

extern RenColorBackData*
ren_color_back_data (RenColor *color, RenColorBackDataKey *key);

extern void
ren_data_block_ref (RenDataBlock *datablock);

extern void
ren_data_block_unref (RenDataBlock *datablock);

extern void
ren_data_block_data (RenDataBlock *datablock, const void **datap,
    ren_size *sizep, RenUsage *usagep);

extern void
ren_coord_array_ref (RenCoordArray *vxarray);

extern void
ren_coord_array_unref (RenCoordArray *vxarray);

extern void
ren_coord_array_data (RenCoordArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep);

extern void
ren_coord_array_type (RenCoordArray *vxarray,
    RenType *typep, ren_uint08 *nump);

extern void
ren_color_array_ref (RenColorArray *vxarray);

extern void
ren_color_array_unref (RenColorArray *vxarray);

extern void
ren_color_array_data (RenColorArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep);

extern void
ren_color_array_type (RenColorArray *vxarray,
    RenType *typep, RenColorFormat *formatp);

extern void
ren_normal_array_ref (RenNormalArray *vxarray);

extern void
ren_normal_array_unref (RenNormalArray *vxarray);

extern void
ren_normal_array_data (RenNormalArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep);

extern void
ren_normal_array_type (RenNormalArray *vxarray,
    RenType *typep, ren_uint08 *nump);

extern void
ren_index_array_ref (RenIndexArray *ixarray);

extern void
ren_index_array_unref (RenIndexArray *ixarray);

extern void
ren_index_array_data (RenIndexArray *ixarray, RenType *typep,
    RenDataBlock **datablockp, ren_size *startp, ren_size *countp);

typedef struct _RenTemplatePrimitive RenTemplatePrimitive;
struct _RenTemplatePrimitive
{
    ren_uint32 mode;
    RenPrimitive prim;
    ren_size offset;
    ren_size count;
};

extern void
ren_template_ref (RenTemplate *template);

extern void
ren_template_unref (RenTemplate *template);

extern void
ren_template_data_primitives (RenTemplate *template,
    RenIndexArray **ix_array_p,
    ren_size *num_primitives_p, const RenTemplatePrimitive **primitives_p);

extern void
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

extern void
ren_material_ref (RenMaterial *material);

extern void
ren_material_unref (RenMaterial *material);

extern void
ren_material_data_light (RenMaterial *material,
    RenColor **ambient_p, RenColor **diffuse_p, RenColor **specular_p,
    RenColor **emission_p, ren_dfloat *shininess_p);

extern void
ren_light_ref (RenLight *light);

extern void
ren_light_unref (RenLight *light);

extern void
ren_light_data (RenLight *light,
    RenLightType *typep,
    RenColor **ambientp,
    RenColor **diffusep,
    RenColor **specularp);

extern void
ren_light_data_point_light (RenLight *light,
    RenVector **attenuationp);

extern void
ren_light_data_spot_light (RenLight *light,
    RenVector **attenuationp,
    ren_dfloat *cutoffp,
    ren_dfloat *exponentp);

#endif /* REN_IMPL_H */
