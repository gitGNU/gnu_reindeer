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

#undef ren_real
#undef REN_TYPE_REAL
#if defined(REN_DFLOAT_AS_REAL)
#define ren_real ren_dfloat
#define REN_TYPE_REAL REN_TYPE_DFLOAT
#elif defined(REN_SFLOAT_AS_REAL)
#define ren_real ren_sfloat
#define REN_TYPE_REAL REN_TYPE_SFLOAT
#endif

#undef ren_clear_color
#undef ren_clear_depth
#undef ren_matrix_set
#undef ren_matrix_set_transpose
#undef ren_matrix_mul
#undef ren_matrix_mul_transpose
#undef ren_lighting_ambient
#undef ren_light_ambient
#undef ren_light_diffuse
#undef ren_light_specular
#if defined(REN_DFLOAT_AS_REAL)
#define ren_clear_color             ren_clear_color_df
#define ren_clear_depth             ren_clear_depth_df
#define ren_matrix_set              ren_matrix_set_df
#define ren_matrix_set_transpose    ren_matrix_set_transpose_df
#define ren_matrix_mul              ren_matrix_mul_df
#define ren_matrix_mul_transpose    ren_matrix_mul_transpose_df
#define ren_lighting_ambient        ren_lighting_ambient_df
#define ren_light_ambient           ren_light_ambient_df
#define ren_light_diffuse           ren_light_diffuse_df
#define ren_light_specular          ren_light_specular_df
#elif defined(REN_SFLOAT_AS_REAL)
#define ren_clear_color             ren_clear_color_sf
#define ren_clear_depth             ren_clear_depth_sf
#define ren_matrix_set              ren_matrix_set_sf
#define ren_matrix_set_transpose    ren_matrix_set_transpose_sf
#define ren_matrix_mul              ren_matrix_mul_sf
#define ren_matrix_mul_transpose    ren_matrix_mul_transpose_sf
#define ren_lighting_ambient        ren_lighting_ambient_sf
#define ren_light_ambient           ren_light_ambient_sf
#define ren_light_diffuse           ren_light_diffuse_sf
#define ren_light_specular          ren_light_specular_sf
#endif
