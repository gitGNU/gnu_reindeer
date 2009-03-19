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
    #define _REN_IMPL(F)\
        _REN_RET(F)\
        _REN_IMPL_MANGLE(_REN_FNM(F)) _REN_PRM(F)\
        { _REN_IMPL_CODE(F) }
    #define _REN_IMPL_T(F, T)\
        _REN_RET_T(F, T)\
        _REN_IMPL_MANGLE(_REN_FNM_T(F, T)) _REN_PRM_T(F, T)\
        { _REN_IMPL_CODE_T(F, T) }
    #define _REN_IMPL_TN(F, T, N)\
        _REN_RET_TN(F, T, N)\
        _REN_IMPL_MANGLE(_REN_FNM_TN(F, T, N)) _REN_PRM_TN(F, T, N)\
        { _REN_IMPL_CODE_TN(F, T, N) }

    #define _REN_IMPL_MANGLE3(NAME, F) ren__##NAME##__##F
    #define _REN_IMPL_MANGLE2(NAME, F) _REN_IMPL_MANGLE3(NAME, F)
    #define _REN_IMPL_MANGLE(F) _REN_IMPL_MANGLE2(_REN_IMPL_NAME, F)
    /* I hate C preprocessor.  */

    #define _REN_FUNC       _REN_IMPL
    #define _REN_FUNC_T     _REN_IMPL_T
    #define _REN_FUNC_TN    _REN_IMPL_TN
#endif

typedef ren_bool RenBackendInitFunc (RenBackend *backend);
typedef ren_bool RenBackendFiniFunc (void);

typedef struct _RenValue _RenValue;
struct _RenValue
{
    RenType type;
    ren_ubyte num;
    const void *value;
};

extern void
_ren_throw_error (const char *format, ...);

extern void*
_ren_get_backend_data (RenReindeer *r/*, RenBackend *backend*/);

extern void
_ren_set_backend_data (RenReindeer *r/*, RenBackend *backend*/, void *data);

/*
extern ren_bool
reindeer_set_ren_function_ (RenReindeer *r, RenBackend *backend,
    ren_function_t f, const char *symbol);
*/

extern void
_ren_data_block_ref (RenDataBlock *datablock);

extern void
_ren_data_block_unref (RenDataBlock *datablock);

extern void
_ren_data_block_data (RenDataBlock *datablock, const void **datap,
    ren_size *sizep, RenUsage *usagep);

extern ren_bool
_ren_data_block_changes (RenDataBlock *datablock, const RenReindeer *r,
    ren_offset *fromp, ren_offset *top);
/* NOTE: Backends should use this in a while loop until it returns REN_FALSE.
By setting fromp = top = NULL, they can force the update list to exhaust. This
is useful for times when backends don't need to receive these events, but they
still need to clear the list to prevent it from filling up.  */

extern void
_ren_vertex_array_data (RenVertexArray *vxarray, RenDataBlock **datablockp,
    ren_offset *startp, ren_size *countp, ren_size *stridep);

#if 0
extern void*
_ren_vertex_array_get_backend_data (RenVertexArray *vxarray,
    const RenReindeer *r);

extern void
_ren_vertex_array_set_backend_data (RenVertexArray *vxarray,
    const RenReindeer *r, void *data);
#endif

extern void
_ren_coord_array_ref (RenCoordArray *vxarray);

extern void
_ren_coord_array_unref (RenCoordArray *vxarray);

extern void
_ren_coord_array_type (RenCoordArray *vxarray, RenType *typep, ren_uint *nump);

extern void
_ren_color_array_ref (RenColorArray *vxarray);

extern void
_ren_color_array_unref (RenColorArray *vxarray);

extern void
_ren_color_array_type (RenColorArray *vxarray, RenType *typep, ren_uint *nump);

extern void
_ren_normal_array_ref (RenNormalArray *vxarray);

extern void
_ren_normal_array_unref (RenNormalArray *vxarray);

extern void
_ren_normal_array_type (RenNormalArray *vxarray, RenType *typep, ren_uint *nump);


extern void
_ren_index_array_ref (RenIndexArray *ixarray);

extern void
_ren_index_array_unref (RenIndexArray *ixarray);

extern void
_ren_index_array_data (RenIndexArray *ixarray, RenType *typep,
    RenDataBlock **datablockp, ren_offset *startp, ren_size *countp);


extern void
_ren_light_ref (RenLight *light);

extern void
_ren_light_unref (RenLight *light);

extern void
_ren_light_data (RenLight *light,
    RenLightType *typep,
    const _RenValue **ambientp,
    const _RenValue **diffusep,
    const _RenValue **specularp);

extern void
_ren_light_data_point_light (RenLight *light,
    const _RenValue **attenuationp);

extern void
_ren_light_data_spot_light (RenLight *light,
    const _RenValue **attenuationp,
    const _RenValue **cutoffp,
    const _RenValue **exponentp);

#endif /* REN_IMPL_H */
