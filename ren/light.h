/*
    This file is part of Reindeer.

    Copyright (C) 2010 - Patrik Olsson

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

#ifndef REN_LIGHT_H
#define REN_LIGHT_H

#include <ren/types.h>

/* Public */

extern RenLight*
ren_light_new (RenLightType type);

extern RenLight*
ren_light_ref (RenLight *light);

extern void
ren_light_unref (RenLight *light);

extern void
ren_light_ambient (RenLight *light, RenColor *color);

extern void
ren_light_diffuse (RenLight *light, RenColor *color);

extern void
ren_light_specular (RenLight *light, RenColor *color);

extern void
ren_light_attenuation (RenLight *light, RenVector *k);

/* Backend */

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

#endif /* REN_LIGHT_H */
