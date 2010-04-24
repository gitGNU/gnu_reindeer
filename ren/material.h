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

#ifndef REN_MATERIAL_H
#define REN_MATERIAL_H

#include <ren/types.h>

/* Public */

extern RenMaterial*
ren_material_new ();

extern RenMaterial*
ren_material_ref (RenMaterial *material);

extern void
ren_material_unref (RenMaterial *material);

extern void
ren_material_ambient (RenMaterial *material, RenColor *color);

extern void
ren_material_diffuse (RenMaterial *material, RenColor *color);

extern void
ren_material_specular (RenMaterial *material, RenColor *color);

extern void
ren_material_emission (RenMaterial *material, RenColor *color);

extern void
ren_material_shininess (RenMaterial *material, ren_dfloat shininess);

/* Backend */

extern void
ren_material_data_light (RenMaterial *material,
	RenColor **ambient_p, RenColor **diffuse_p, RenColor **specular_p,
	RenColor **emission_p, ren_dfloat *shininess_p);

#endif /* REN_MATERIAL_H */
