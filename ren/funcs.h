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

/* FIXME: Remove */
#include <ren/tmpl.h>

_REN_FUNC               (init)
_REN_FUNC               (flush)
_REN_FUNC               (finish)
_REN_FUNC               (clear)
_REN_FUNC_CLAMP         (clear_color)
_REN_FUNC_CLAMP         (clear_depth)
_REN_FUNC               (depth_test)
_REN_FUNC               (matrix_mode)
_REN_FUNC_T             (matrix_set,sf)
_REN_FUNC_T             (matrix_set,df)
_REN_FUNC_T             (matrix_set_transpose,sf)
_REN_FUNC_T             (matrix_set_transpose,df)
_REN_FUNC_T             (matrix_mul,sf)
_REN_FUNC_T             (matrix_mul,df)
_REN_FUNC_T             (matrix_mul_transpose,sf)
_REN_FUNC_T             (matrix_mul_transpose,df)
_REN_FUNC               (matrix_identity)
_REN_FUNC               (matrix_push)
_REN_FUNC               (matrix_pop)
_REN_FUNC               (viewport)
_REN_FUNC               (coord_array_bind)
_REN_FUNC               (coord_array_enable)
_REN_FUNC               (coord_array_disable)
_REN_FUNC               (coord_array_unload)
_REN_FUNC               (color_array_bind)
_REN_FUNC               (color_array_enable)
_REN_FUNC               (color_array_disable)
_REN_FUNC               (color_array_unload)
_REN_FUNC               (normal_array_bind)
_REN_FUNC               (normal_array_enable)
_REN_FUNC               (normal_array_disable)
_REN_FUNC               (normal_array_unload)
_REN_FUNC               (draw_begin)
_REN_FUNC               (draw_end)
_REN_FUNC               (index_array_bind)
_REN_FUNC               (index_array_unload)
_REN_FUNC_INDEX         (primitive)
/*
_REN_FUNC_INDEX         (primitive_multi)
_REN_FUNC_INDEX         (primitive_list)
*/
_REN_FUNC               (lighting_enable)
_REN_FUNC               (lighting_disable)
_REN_FUNC_CLAMP         (lighting_ambient)
_REN_FUNC               (light_bind)
_REN_FUNC               (light_enable)
_REN_FUNC               (light_disable)

/* FIXME: Includers should do this manually. */
#undef _REN_FUNC
#undef _REN_FUNC_T
#undef _REN_FUNC_TN
