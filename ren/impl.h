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

#ifdef _REN_IMPL_NAME
    #define _REN_IMPL_MANGLE3(NAME, F) ren__##NAME##__##F
    #define _REN_IMPL_MANGLE2(NAME, F) _REN_IMPL_MANGLE3(NAME, F)
    #define _REN_IMPL_MANGLE(F) _REN_IMPL_MANGLE2(_REN_IMPL_NAME, F)
    /* I hate C preprocessor.  */
#endif

typedef ren_bool (* _RenBackendInitFunc) (RenBackend *backend);
typedef ren_bool (* _RenBackendFiniFunc) (void);

typedef struct _RenContextData _RenContextData;
typedef void (* _RenContextDataInitFunc) (_RenContextData *context_data);
typedef void (* _RenContextDataFiniFunc) (_RenContextData *context_data);

extern _RenContextData*
_ren_context_data (RenReindeer *r);

extern void
_ren_matrix_ref (RenMatrix *matrix);

extern void
_ren_matrix_unref (RenMatrix *matrix);

extern void
_ren_matrix_data (RenMatrix *matrix,
    const void **datap, ren_size *widthp, ren_size *heightp,
    RenType *typep, ren_bool *transposedp);

typedef struct _RenMatrixContextData _RenMatrixContextData;
typedef void (* _RenMatrixContextDataInitFunc) (RenMatrix *matrix,
    _RenMatrixContextData *context_data);
typedef void (* _RenMatrixContextDataFiniFunc) (RenMatrix *matrix,
    _RenMatrixContextData *context_data);
typedef void (* _RenMatrixContextDataUpdateFunc) (RenMatrix *matrix,
    _RenMatrixContextData *context_data);

extern _RenMatrixContextData*
_ren_matrix_context_data (RenMatrix *matrix, RenReindeer *r);

typedef struct _RenMatrixBackendData _RenMatrixBackendData;
typedef void (* _RenMatrixBackendDataInitFunc) (RenMatrix *matrix,
    _RenMatrixBackendData *backend_data);
typedef void (* _RenMatrixBackendDataFiniFunc) (RenMatrix *matrix,
    _RenMatrixBackendData *backend_data);
typedef void (* _RenMatrixBackendDataUpdateFunc) (RenMatrix *matrix,
    _RenMatrixBackendData *backend_data);

extern _RenMatrixBackendData*
_ren_matrix_backend_data (RenMatrix *matrix, RenReindeer *r);

extern void
_ren_vector_ref (RenVector *vector);

extern void
_ren_vector_unref (RenVector *vector);

extern void
_ren_color_ref (RenColor *color);

extern void
_ren_color_unref (RenColor *color);

extern void
_ren_color_data (RenColor *color,
    const void **datap, RenColorFormat *formatp, RenType *typep);

typedef struct _RenColorContextData _RenColorContextData;
typedef void (* _RenColorContextDataInitFunc) (RenColor *color,
    _RenColorContextData *context_data);
typedef void (* _RenColorContextDataFiniFunc) (RenColor *color,
    _RenColorContextData *context_data);
typedef void (* _RenColorContextDataUpdateFunc) (RenColor *color,
    _RenColorContextData *context_data);

extern _RenColorContextData*
_ren_color_context_data (RenColor *color, RenReindeer *r);

typedef struct _RenColorBackendData _RenColorBackendData;
typedef void (* _RenColorBackendDataInitFunc) (RenColor *color,
    _RenColorBackendData *backend_data);
typedef void (* _RenColorBackendDataFiniFunc) (RenColor *color,
    _RenColorBackendData *backend_data);
typedef void (* _RenColorBackendDataUpdateFunc) (RenColor *color,
    _RenColorBackendData *backend_data);

extern _RenColorBackendData*
_ren_color_backend_data (RenColor *color, RenReindeer *r);

extern void
_ren_data_block_ref (RenDataBlock *datablock);

extern void
_ren_data_block_unref (RenDataBlock *datablock);

extern void
_ren_data_block_data (RenDataBlock *datablock, const void **datap,
    ren_size *sizep, RenUsage *usagep);

extern void
_ren_coord_array_ref (RenCoordArray *vxarray);

extern void
_ren_coord_array_unref (RenCoordArray *vxarray);

extern void
_ren_coord_array_data (RenCoordArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep);

extern void
_ren_coord_array_type (RenCoordArray *vxarray,
    RenType *typep, ren_uint08 *nump);

extern void
_ren_color_array_ref (RenColorArray *vxarray);

extern void
_ren_color_array_unref (RenColorArray *vxarray);

extern void
_ren_color_array_data (RenColorArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep);

extern void
_ren_color_array_type (RenColorArray *vxarray,
    RenType *typep, RenColorFormat *formatp);

extern void
_ren_normal_array_ref (RenNormalArray *vxarray);

extern void
_ren_normal_array_unref (RenNormalArray *vxarray);

extern void
_ren_normal_array_data (RenNormalArray *vxarray, RenDataBlock **datablockp,
    ren_size *startp, ren_size *countp, ren_size *stridep);

extern void
_ren_normal_array_type (RenNormalArray *vxarray,
    RenType *typep, ren_uint08 *nump);


extern void
_ren_index_array_ref (RenIndexArray *ixarray);

extern void
_ren_index_array_unref (RenIndexArray *ixarray);

extern void
_ren_index_array_data (RenIndexArray *ixarray, RenType *typep,
    RenDataBlock **datablockp, ren_size *startp, ren_size *countp);


extern void
_ren_light_ref (RenLight *light);

extern void
_ren_light_unref (RenLight *light);

extern void
_ren_light_data (RenLight *light,
    RenLightType *typep,
    RenColor **ambientp,
    RenColor **diffusep,
    RenColor **specularp);

extern void
_ren_light_data_point_light (RenLight *light,
    RenVector **attenuationp);

extern void
_ren_light_data_spot_light (RenLight *light,
    RenVector **attenuationp,
    ren_dfloat *cutoffp,
    ren_dfloat *exponentp);

#endif /* REN_IMPL_H */
