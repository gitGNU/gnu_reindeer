/*
	This file is part of Reindeer.

	Copyright (C) 2008, 2009, 2010 - Patrik Olsson

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

#define _REN_SYM(F) ren_##F
#define _REN_FNM(F) F
#define _REN_FTP(F) _REN_FTP_##F
#define _REN_RET(F) _REN_RET_##F
#define _REN_PRM(F) _REN_PRM_##F
#define _REN_ARG(F) _REN_ARG_##F
#define _REN_DRT(F) _REN_DRT_##F

/* BEGIN */

#define _REN_FTP_init RenInitFunc
#define _REN_RET_init ren_bool
#define _REN_PRM_init (RenReindeer *r)
#define _REN_ARG_init (r)
#define _REN_DRT_init TRUE

#define _REN_FTP_flush RenFlushFunc
#define _REN_RET_flush void
#define _REN_PRM_flush (RenReindeer *r)
#define _REN_ARG_flush (r)
#define _REN_DRT_flush NULL

#define _REN_FTP_finish RenFinishFunc
#define _REN_RET_finish void
#define _REN_PRM_finish (RenReindeer *r)
#define _REN_ARG_finish (r)
#define _REN_DRT_finish NULL

#define _REN_FTP_clear RenClearFunc
#define _REN_RET_clear void
#define _REN_PRM_clear (RenReindeer *r, RenBuffer buffers)
#define _REN_ARG_clear (r, buffers)
#define _REN_DRT_clear NULL

#define _REN_FTP_clear_color RenClearColorFunc
#define _REN_RET_clear_color void
#define _REN_PRM_clear_color (RenReindeer *r, RenColor *color)
#define _REN_ARG_clear_color (r, color)
#define _REN_DRT_clear_color NULL

#define _REN_FTP_clear_depth RenClearDepthFunc
#define _REN_RET_clear_depth void
#define _REN_PRM_clear_depth (RenReindeer *r, ren_dfloat depth)
#define _REN_ARG_clear_depth (r, depth)
#define _REN_DRT_clear_depth NULL

#define _REN_FTP_depth_test RenDepthTestFunc
#define _REN_RET_depth_test void
#define _REN_PRM_depth_test (RenReindeer *r, RenTest test)
#define _REN_ARG_depth_test (r, test)
#define _REN_DRT_depth_test NULL

#define _REN_FTP_transform_mode RenTransformModeFunc
#define _REN_RET_transform_mode void
#define _REN_PRM_transform_mode (RenReindeer *r, RenTransformMode mode)
#define _REN_ARG_transform_mode (r, mode)
#define _REN_DRT_transform_mode NULL

#define _REN_FTP_transform_set RenTransformSetFunc
#define _REN_RET_transform_set void
#define _REN_PRM_transform_set (RenReindeer *r, RenMatrix *matrix)
#define _REN_ARG_transform_set (r, matrix)
#define _REN_DRT_transform_set NULL

#define _REN_FTP_transform_mul RenTransformMulFunc
#define _REN_RET_transform_mul void
#define _REN_PRM_transform_mul (RenReindeer *r, RenMatrix *matrix)
#define _REN_ARG_transform_mul (r, matrix)
#define _REN_DRT_transform_mul NULL

#define _REN_FTP_transform_identity RenTransformIdentityFunc
#define _REN_RET_transform_identity void
#define _REN_PRM_transform_identity (RenReindeer *r)
#define _REN_ARG_transform_identity (r)
#define _REN_DRT_transform_identity NULL

#define _REN_FTP_transform_push RenTransformPushFunc
#define _REN_RET_transform_push void
#define _REN_PRM_transform_push (RenReindeer *r)
#define _REN_ARG_transform_push (r)
#define _REN_DRT_transform_push NULL

#define _REN_FTP_transform_pop RenTransformPopFunc
#define _REN_RET_transform_pop void
#define _REN_PRM_transform_pop (RenReindeer *r)
#define _REN_ARG_transform_pop (r)
#define _REN_DRT_transform_pop NULL

#define _REN_FTP_viewport RenViewportFunc
#define _REN_RET_viewport void
#define _REN_PRM_viewport (RenReindeer *r,\
	ren_uint32 x, ren_uint32 y,\
	ren_uint32 width, ren_uint32 height)
#define _REN_ARG_viewport (r, x, y, width, height)
#define _REN_DRT_viewport NULL

#define _REN_FTP_coord_array_bind RenCoordArrayBindFunc
#define _REN_RET_coord_array_bind ren_bool
#define _REN_PRM_coord_array_bind (RenReindeer *r, RenVectorArray *array)
#define _REN_ARG_coord_array_bind (r, array)
#define _REN_DRT_coord_array_bind TRUE

#define _REN_FTP_color_array_bind RenColorArrayBindFunc
#define _REN_RET_color_array_bind ren_bool
#define _REN_PRM_color_array_bind (RenReindeer *r, RenColorArray *array)
#define _REN_ARG_color_array_bind (r, array)
#define _REN_DRT_color_array_bind TRUE

#define _REN_FTP_normal_array_bind RenNormalArrayBindFunc
#define _REN_RET_normal_array_bind ren_bool
#define _REN_PRM_normal_array_bind (RenReindeer *r, RenVectorArray *array)
#define _REN_ARG_normal_array_bind (r, array)
#define _REN_DRT_normal_array_bind TRUE

#define _REN_FTP_index_array_bind RenIndexArrayBindFunc
#define _REN_RET_index_array_bind ren_bool
#define _REN_PRM_index_array_bind (RenReindeer *r, RenIndexArray *ixarray)
#define _REN_ARG_index_array_bind (r, ixarray)
#define _REN_DRT_index_array_bind TRUE

#define _REN_FTP_state_keep RenStateKeepFunc
#define _REN_RET_state_keep void
#define _REN_PRM_state_keep (RenReindeer *r, RenState state)
#define _REN_ARG_state_keep (r, state)
#define _REN_DRT_state_keep NULL

#define _REN_FTP_primitive RenPrimitiveFunc
#define _REN_RET_primitive void
#define _REN_PRM_primitive (RenReindeer *r,\
	RenPrimitive prim, ren_size offset, ren_size count)
#define _REN_ARG_primitive (r, prim, offset, count)
#define _REN_DRT_primitive NULL

#define _REN_FTP_object_render RenObjectRenderFunc
#define _REN_RET_object_render void
#define _REN_PRM_object_render (RenReindeer *r, RenObject *object)
#define _REN_ARG_object_render (r, object)
#define _REN_DRT_object_render NULL

#define _REN_FTP_lighting_enable RenLightingEnableFunc
#define _REN_RET_lighting_enable void
#define _REN_PRM_lighting_enable (RenReindeer *r)
#define _REN_ARG_lighting_enable (r)
#define _REN_DRT_lighting_enable NULL

#define _REN_FTP_lighting_disable RenLightingDisableFunc
#define _REN_RET_lighting_disable void
#define _REN_PRM_lighting_disable (RenReindeer *r)
#define _REN_ARG_lighting_disable (r)
#define _REN_DRT_lighting_disable NULL

#define _REN_FTP_lighting_ambient RenLightingAmbientFunc
#define _REN_RET_lighting_ambient void
#define _REN_PRM_lighting_ambient (RenReindeer *r, RenColor *color)
#define _REN_ARG_lighting_ambient (r, color)
#define _REN_DRT_lighting_ambient NULL

#define _REN_FTP_light_bind RenLightBindFunc
#define _REN_RET_light_bind ren_size
#define _REN_PRM_light_bind (RenReindeer *r, ren_size n, RenLight **lights)
#define _REN_ARG_light_bind (r, n, lights)
#define _REN_DRT_light_bind n

#define _REN_FTP_light_enable RenLightEnableFunc
#define _REN_RET_light_enable ren_bool
#define _REN_PRM_light_enable (RenReindeer *r, RenLight *light)
#define _REN_ARG_light_enable (r, light)
#define _REN_DRT_light_enable TRUE

#define _REN_FTP_light_disable RenLightDisableFunc
#define _REN_RET_light_disable void
#define _REN_PRM_light_disable (RenReindeer *r, RenLight *light)
#define _REN_ARG_light_disable (r, light)
#define _REN_DRT_light_disable NULL

#define _REN_FTP_material_bind RenMaterialBindFunc
#define _REN_RET_material_bind void
#define _REN_PRM_material_bind (RenReindeer *r,\
	RenMaterial *front, RenMaterial *back)
#define _REN_ARG_material_bind (r, front, back)
#define _REN_DRT_material_bind NULL

#define _REN_FTP_material_front RenMaterialFrontFunc
#define _REN_RET_material_front void
#define _REN_PRM_material_front (RenReindeer *r, RenMaterial *material)
#define _REN_ARG_material_front (r, material)
#define _REN_DRT_material_front NULL

#define _REN_FTP_material_back RenMaterialBackFunc
#define _REN_RET_material_back void
#define _REN_PRM_material_back (RenReindeer *r, RenMaterial *material)
#define _REN_ARG_material_back (r, material)
#define _REN_DRT_material_back NULL

#endif /* _REN_TMPL_H */
