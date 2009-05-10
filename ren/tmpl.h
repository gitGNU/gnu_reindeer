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

#ifndef _REN_TMPL_H
#define _REN_TMPL_H

#define _REN_TYPEN(T)   _REN_TYPEN_##T
#define _REN_TYPEN_s08  ren_sint08
#define _REN_TYPEN_u08  ren_uint08
#define _REN_TYPEN_s16  ren_sint16
#define _REN_TYPEN_u16  ren_uint16
#define _REN_TYPEN_s32  ren_sint32
#define _REN_TYPEN_u32  ren_uint32
#define _REN_TYPEN_s64  ren_sint64
#define _REN_TYPEN_u64  ren_uint64
#define _REN_TYPEN_sf   ren_sfloat
#define _REN_TYPEN_df   ren_dfloat

#define _REN_TYPEV(T)   _REN_TYPEV_##T
#define _REN_TYPEV_s08  REN_TYPE_SINT08
#define _REN_TYPEV_u08  REN_TYPE_UINT08
#define _REN_TYPEV_s16  REN_TYPE_SINT16
#define _REN_TYPEV_u16  REN_TYPE_UINT16
#define _REN_TYPEV_s32  REN_TYPE_SINT32
#define _REN_TYPEV_u32  REN_TYPE_UINT32
#define _REN_TYPEV_s64  REN_TYPE_SINT64
#define _REN_TYPEV_u64  REN_TYPE_UINT64
#define _REN_TYPEV_sf   REN_TYPE_SFLOAT
#define _REN_TYPEV_df   REN_TYPE_DFLOAT

#define _REN_ARG(F) _REN_ARG_##F

#define _REN_SYM(F) ren_##F
#define _REN_FNM(F) F
#define _REN_FTP(F) _ren_##F##_fn
#define _REN_RET(F) _REN_RET_##F
#define _REN_PRM(F) _REN_PRM_##F

#define _REN_SYM_T(F, T) ren_##F##_##T
#define _REN_FNM_T(F, T) F##_##T
#define _REN_FTP_T(F, T) _ren_##F##_##T##_fn
#define _REN_RET_T(F, T) _REN_RET_##F(T)
#define _REN_PRM_T(F, T) _REN_PRM_##F(T)

#define _REN_SYM_TN(F, T, N) ren_##F##_##T##N
#define _REN_FNM_TN(F, T, N) F##_##T##N
#define _REN_FTP_TN(F, T, N) _ren_##F##_##T##N##_fn
#define _REN_RET_TN(F, T, N) _REN_RET_##F(T, N)
#define _REN_PRM_TN(F, T, N) _REN_PRM_##F(T, N)

#define _REN_FUNC_POS(F)\
    _REN_FUNC_TN (F,s08,3)\
    _REN_FUNC_TN (F,s08,4)\
    _REN_FUNC_TN (F,s16,3)\
    _REN_FUNC_TN (F,s16,4)\
    _REN_FUNC_TN (F,s32,3)\
    _REN_FUNC_TN (F,s32,4)\
    _REN_FUNC_TN (F,sf,3)\
    _REN_FUNC_TN (F,sf,4)\
    _REN_FUNC_TN (F,df,3)\
    _REN_FUNC_TN (F,df,4)

#define _REN_FUNC_DIR(F)\
    _REN_FUNC_TN (F,s08,3)\
    _REN_FUNC_TN (F,s16,3)\
    _REN_FUNC_TN (F,s32,3)\
    _REN_FUNC_TN (F,sf,3)\
    _REN_FUNC_TN (F,df,3)

#define _REN_FUNC_CLAMP(F)\
    _REN_FUNC_T (F,u08)\
    _REN_FUNC_T (F,u16)\
    _REN_FUNC_T (F,u32)\
    _REN_FUNC_T (F,sf)\
    _REN_FUNC_T (F,df)

#define _REN_FUNC_NORM(F)\
    _REN_FUNC_T (F,s08)\
    _REN_FUNC_T (F,s16)\
    _REN_FUNC_T (F,s32)\
    _REN_FUNC_T (F,sf)\
    _REN_FUNC_T (F,df)

#define _REN_FUNC_INDEX(F)\
    _REN_FUNC_T (F,u08)\
    _REN_FUNC_T (F,u16)\
    _REN_FUNC_T (F,u32)

/* BEGIN */

#define _REN_RET_init ren_bool
#define _REN_PRM_init (RenReindeer *r)
#define _REN_ARG_init (r)

#define _REN_RET_flush void
#define _REN_PRM_flush (RenReindeer *r)
#define _REN_ARG_flush (r)

#define _REN_RET_finish void
#define _REN_PRM_finish (RenReindeer *r)
#define _REN_ARG_finish (r)

#define _REN_RET_clear void
#define _REN_PRM_clear (RenReindeer *r, RenBuffer buffers)
#define _REN_ARG_clear (r, buffers)

#define _REN_RET_clear_color(T) void
#define _REN_PRM_clear_color(T)\
    (RenReindeer *r, ren_uint08 components, const _REN_TYPEN(T) *color)
#define _REN_ARG_clear_color    (r, components, color)

#define _REN_RET_clear_depth(T) void
#define _REN_PRM_clear_depth(T) (RenReindeer *r, _REN_TYPEN(T) depth)
#define _REN_ARG_clear_depth    (r, depth)

#define _REN_RET_depth_test void
#define _REN_PRM_depth_test (RenReindeer *r, RenTest test)
#define _REN_ARG_depth_test (r, test)

#define _REN_RET_matrix_mode void
#define _REN_PRM_matrix_mode (RenReindeer *r, RenMatrixMode mode)
#define _REN_ARG_matrix_mode (r, mode)

#define _REN_RET_matrix_set(T)  void
#define _REN_PRM_matrix_set(T)  (RenReindeer *r, const _REN_TYPEN(T) m[16])
#define _REN_ARG_matrix_set     (r, m)

#define _REN_RET_matrix_set_transpose(T) void
#define _REN_PRM_matrix_set_transpose(T)\
    (RenReindeer *r, const _REN_TYPEN(T) m[16])
#define _REN_ARG_matrix_set_transpose    (r, m)

#define _REN_RET_matrix_mul(T)  void
#define _REN_PRM_matrix_mul(T)  (RenReindeer *r, const _REN_TYPEN(T) m[16])
#define _REN_ARG_matrix_mul     (r, m)

#define _REN_RET_matrix_mul_transpose(T) void
#define _REN_PRM_matrix_mul_transpose(T)\
    (RenReindeer *r, const _REN_TYPEN(T) m[16])
#define _REN_ARG_matrix_mul_transpose    (r, m)

#define _REN_RET_matrix_identity void
#define _REN_PRM_matrix_identity (RenReindeer *r)
#define _REN_ARG_matrix_identity (r)

#define _REN_RET_matrix_push void
#define _REN_PRM_matrix_push (RenReindeer *r)
#define _REN_ARG_matrix_push (r)

#define _REN_RET_matrix_pop void
#define _REN_PRM_matrix_pop (RenReindeer *r)
#define _REN_ARG_matrix_pop (r)

#define _REN_RET_viewport void
#define _REN_PRM_viewport (RenReindeer *r,\
    ren_uint32 x,     ren_uint32 y,\
    ren_uint32 width, ren_uint32 height)
#define _REN_ARG_viewport (r, x, y, width, height)

#define _REN_RET_coord_array_bind ren_bool
#define _REN_PRM_coord_array_bind (RenReindeer *r, RenCoordArray *vxarray)
#define _REN_ARG_coord_array_bind (r, vxarray)

#define _REN_RET_coord_array_enable void
#define _REN_PRM_coord_array_enable (RenReindeer *r)
#define _REN_ARG_coord_array_enable (r)

#define _REN_RET_coord_array_disable void
#define _REN_PRM_coord_array_disable (RenReindeer *r)
#define _REN_ARG_coord_array_disable (r)

#define _REN_RET_coord_array_unload void
#define _REN_PRM_coord_array_unload (RenReindeer *r, RenCoordArray *vxarray)
#define _REN_ARG_coord_array_unload (r, vxarray)
/* NOTE: Basically what the application aims to do with this function is telling
the backend that it is not going to bind this vxarray anymore for that reindeer,
or its sharers. While it does not make any sense to call it when it is still
bound by one or many in the reindeer sharegroup it might be difficult for the
application to keep track of that but it's probably easy for the backend to just
postpone the actual unload.
*/

#define _REN_RET_color_array_bind ren_bool
#define _REN_PRM_color_array_bind (RenReindeer *r, RenColorArray *vxarray)
#define _REN_ARG_color_array_bind (r, vxarray)

#define _REN_RET_color_array_enable void
#define _REN_PRM_color_array_enable (RenReindeer *r)
#define _REN_ARG_color_array_enable (r)

#define _REN_RET_color_array_disable void
#define _REN_PRM_color_array_disable (RenReindeer *r)
#define _REN_ARG_color_array_disable (r)

#define _REN_RET_color_array_unload void
#define _REN_PRM_color_array_unload (RenReindeer *r, RenColorArray *vxarray)
#define _REN_ARG_color_array_unload (r, vxarray)

#define _REN_RET_normal_array_bind ren_bool
#define _REN_PRM_normal_array_bind (RenReindeer *r, RenNormalArray *vxarray)
#define _REN_ARG_normal_array_bind (r, vxarray)

#define _REN_RET_normal_array_enable void
#define _REN_PRM_normal_array_enable (RenReindeer *r)
#define _REN_ARG_normal_array_enable (r)

#define _REN_RET_normal_array_disable void
#define _REN_PRM_normal_array_disable (RenReindeer *r)
#define _REN_ARG_normal_array_disable (r)

#define _REN_RET_normal_array_unload void
#define _REN_PRM_normal_array_unload (RenReindeer *r, RenNormalArray *vxarray)
#define _REN_ARG_normal_array_unload (r, vxarray)

#define _REN_RET_index_array_bind ren_bool
#define _REN_PRM_index_array_bind (RenReindeer *r, RenIndexArray *ixarray)
#define _REN_ARG_index_array_bind (r, ixarray)

#define _REN_RET_index_array_unload void
#define _REN_PRM_index_array_unload (RenReindeer *r, RenIndexArray *ixarray)
#define _REN_ARG_index_array_unload (r, ixarray)

#define _REN_RET_draw_begin void
#define _REN_PRM_draw_begin (RenReindeer *r)
#define _REN_ARG_draw_begin (r)
/* NOTE: These functions should be used before using ren_primitive_* functions
so that they are drawn to the screen (rather than defining an object).
It should only be necessary to call begin once, if we are never going to define
objects. So draw_end might not actually be needed, unless there is some backend
that needs it (but would that make any sense?)  */
#define _REN_RET_draw_end void
#define _REN_PRM_draw_end (RenReindeer *r)
#define _REN_ARG_draw_end (r)

#define _REN_RET_primitive(T)   void
#define _REN_PRM_primitive(T)   (RenReindeer *r,\
    RenPrimitive prim, _REN_TYPEN(T) offset, _REN_TYPEN(T) count)
#define _REN_ARG_primitive      (r, prim, offset, count)

#define _REN_RET_primitive_multi(T) void
#define _REN_PRM_primitive_multi(T)\
    (RenReindeer *r, ren_size count, RenPrimitive prim,\
    _REN_TYPEN(T) *offsets, _REN_TYPEN(T) *counts)
#define _REN_ARG_primitive_multi    (r, count, prim, offsets, counts)

#define _REN_RET_primitive_list(T) void
#define _REN_PRM_primitive_list(T)\
    (RenReindeer *r, ren_size count, RenPrimitive *prim,\
    _REN_TYPEN(T) *offsets, _REN_TYPEN(T) *counts)
#define _REN_ARG_primitive_list    (r, count, prim, offsets, counts)

#define _REN_RET_lighting_enable void
#define _REN_PRM_lighting_enable (RenReindeer *r)
#define _REN_ARG_lighting_enable (r)

#define _REN_RET_lighting_disable void
#define _REN_PRM_lighting_disable (RenReindeer *r)
#define _REN_ARG_lighting_disable (r)

#define _REN_RET_lighting_ambient(T)    void
#define _REN_PRM_lighting_ambient(T)\
    (RenReindeer *r, ren_uint08 components, const _REN_TYPEN(T) *color)
#define _REN_ARG_lighting_ambient       (r, components, color)

#define _REN_RET_light_bind  void
#define _REN_PRM_light_bind\
    (RenReindeer *r, RenLight *light, ren_uint32 index)
#define _REN_ARG_light_bind     (r, light, index)

#define _REN_RET_light_enable void
#define _REN_PRM_light_enable (RenReindeer *r, ren_uint32 index)
#define _REN_ARG_light_enable (r, index)

#define _REN_RET_light_disable void
#define _REN_PRM_light_disable (RenReindeer *r, ren_uint32 index)
#define _REN_ARG_light_disable (r, index)

#endif /* _REN_TMPL_H */
