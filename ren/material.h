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
ren_material_begin_edit (RenMaterial *material);

extern void
ren_material_end_edit (RenMaterial *material);

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

typedef struct RenMaterialInfo RenMaterialInfo;
struct RenMaterialInfo
{
	RenColor *ambient;
	RenColor *diffuse;
	RenColor *specular;
	RenColor *emission;
	ren_dfloat shininess;
};

extern const RenMaterialInfo*
ren_material_info (RenMaterial *material);

typedef struct _RenMaterialBackData RenMaterialBackData;
typedef struct _RenMaterialBackDataKey RenMaterialBackDataKey;

typedef void (* RenMaterialBackDataKeyDestroyNotifyFunc) (
	RenMaterialBackDataKey* key, void *user_data);

typedef void (* RenMaterialBackDataInitFunc) (RenMaterial *material,
	RenMaterialBackData *back_data, void *user_data,
	const RenMaterialInfo *info);
typedef void (* RenMaterialBackDataFiniFunc) (RenMaterial *material,
	RenMaterialBackData *back_data, void *user_data);
typedef void (* RenMaterialBackDataUpdateFunc) (RenMaterial *material,
	RenMaterialBackData *back_data, void *user_data,
	const RenMaterialInfo *info);

extern RenMaterialBackDataKey*
ren_material_back_data_key_new (ren_size data_size,
	RenMaterialBackDataInitFunc init,
	RenMaterialBackDataFiniFunc fini,
	RenMaterialBackDataUpdateFunc update);

extern void
ren_material_back_data_key_user_data (RenMaterialBackDataKey *key,
	void *user_data);

extern void
ren_material_back_data_key_destroy_notify (RenMaterialBackDataKey *key,
	RenMaterialBackDataKeyDestroyNotifyFunc destroy_notify);

extern RenMaterialBackDataKey*
ren_material_back_data_key_ref (RenMaterialBackDataKey *key);

extern void
ren_material_back_data_key_unref (RenMaterialBackDataKey *key);

extern RenMaterialBackData*
ren_material_back_data (RenMaterial *material, RenMaterialBackDataKey *key);

#endif /* REN_MATERIAL_H */
