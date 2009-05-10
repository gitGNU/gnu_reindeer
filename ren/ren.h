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

#define _REN_FUNC(F)\
    extern _REN_RET(F) _REN_SYM(F) _REN_PRM(F);
#define _REN_FUNC_T(F, T)\
    extern _REN_RET_T(F, T) _REN_SYM_T(F, T) _REN_PRM_T(F, T);
#define _REN_FUNC_TN(F, T, N)\
    extern _REN_RET_TN(F, T, N) _REN_SYM_TN(F, T, N) _REN_PRM_TN(F, T, N);

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

extern RenDataBlock*
ren_data_block_new (const void *data, ren_size size, RenUsage usage);

extern void
ren_data_block_destroy (RenDataBlock *datablock);
/* Applications should not free the original data after this. They should wait
for the unload callback.  */

extern void
ren_data_block_callback (
    RenDataBlock *datablock,
    RenDataBlockCallback reload_func,
    RenDataBlockCallback unload_func,
    void *user_data);
/* NOTE: A mechanism to free the initial data when it's not actually used
by backends after the first bind. This call should only be done after having
used the datablock for all known reindeers, because otherwise it might keep
unloading and reloading for each first use.

The unload callback is expected to free the data, and in any case the library
will set its pointer to NULL.
On reload the callback is expected to call ren_datablock_relocated. If this is
not done, the behaviour is undefined.
*/

extern void
ren_data_block_relocated (RenDataBlock *datablock, const void *data);
/* This function should still assume that the data has not changed, but only
been moved. */

extern void
ren_data_block_resized (RenDataBlock *datablock, ren_size size);

extern void
ren_data_block_changed (RenDataBlock *datablock, ren_size from, ren_size to);
/* Signal the data changed in the range from 'from' to, but exclusive, 'to'.
If 'from' is 0 and 'to' is -1, the whole is updated.

It's a good idea to try to collect updates before calling this function. Neither
the main library nor backends are required to optimize the handling of updated
ranges. Also it is recommended to call this function for different ranges in
the order they come. So 1-4,6-10,20-count should be called in that order.
*/

#if 0
extern RenVertexArray*
ren_vertex_array_new (
    RenVertexArrayType vxtype, RenType type, ren_uint08 num,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);
/*  num = number of elements of type (e.g. float[3]),
    count = number of elements in vxarray.
    start = offset in basic machine units.  */

extern void
ren_vertex_array_destroy (RenVertexArray *vxarray);
#endif

extern RenCoordArray*
ren_coord_array_new (RenType type, ren_uint08 num,
    RenDataBlock *datablock, ren_size start, ren_size count, ren_size stride);

extern void
ren_coord_array_destroy (RenCoordArray *vxarray);

extern void
ren_coord_array_set_size (RenCoordArray *vxarray, ren_size count);

extern RenColorArray*
ren_color_array_new (RenType type, ren_uint08 num,
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

#define _REN_RET_light_color(T) void
#define _REN_PRM_light_color(T)\
    (RenLight *light, ren_uint08 components, const _REN_TYPEN(T) *color)
#define _REN_ARG_light_color    (light, components, color)

#define _REN_RET_light_ambient _REN_RET_light_color
#define _REN_PRM_light_ambient _REN_PRM_light_color
#define _REN_ARG_light_ambient _REN_ARG_light_color
_REN_FUNC_CLAMP (light_ambient)

#define _REN_RET_light_diffuse _REN_RET_light_color
#define _REN_PRM_light_diffuse _REN_PRM_light_color
#define _REN_ARG_light_diffuse _REN_ARG_light_color
_REN_FUNC_CLAMP (light_diffuse)

#define _REN_RET_light_specular _REN_RET_light_color
#define _REN_PRM_light_specular _REN_PRM_light_color
#define _REN_ARG_light_specular _REN_ARG_light_color
_REN_FUNC_CLAMP (light_specular)

#if 0
/*XXX: Position and Direction are relative current model-view matrix anyway. */
#define _REN_RET_light_position(T, N)   void
#define _REN_PRM_light_position(T, N)\
    (RenLight *light, const _REN_TYPE(T) pos[N])
#define _REN_ARG_light_position         (light, pos)
_REN_FUNC_POS (light_position)

#define _REN_RET_light_direction(T, N)  void
#define _REN_PRM_light_direction(T, N)\
    (RenLight *light, const _REN_TYPE(T) dir[N])
#define _REN_ARG_light_direction        (light, pos)
_REN_FUNC_DIR (light_direction)
#endif

#define _REN_RET_light_attenuation(T)   void
#define _REN_PRM_light_attenuation(T)\
    (RenLight *light, ren_uint08 degree, const _REN_TYPEN(T) *k)
#define _REN_ARG_light_attenuation      (light, degree, pos)
_REN_FUNC_T (light_attenuation,sf)
_REN_FUNC_T (light_attenuation,df)

/*
extern void
ren_light_position_s4 (RenLight *light, ren_sfloat position[4]);
*/

#include <ren/funcs.h>

#endif /* REN_REN_H */
