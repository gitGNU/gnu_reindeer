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
ren_light_new (RenLightType light_type);

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

typedef struct RenLightInfo RenLightInfo;
struct RenLightInfo
{
	RenLightType light_type;

	RenColor *ambient;
	RenColor *diffuse;
	RenColor *specular;

	RenVector *attenuation;
	ren_dfloat cutoff;
	ren_dfloat exponent;
};

extern const RenLightInfo*
ren_light_info (RenLight *light);

typedef struct _RenLightBackData RenLightBackData;
typedef struct _RenLightBackDataKey RenLightBackDataKey;

typedef void (* RenLightBackDataKeyDestroyNotifyFunc) (
	RenLightBackDataKey* key, void *user_data);

typedef void (* RenLightBackDataInitFunc) (RenLight *light,
	RenLightBackData *back_data, void *user_data,
	const RenLightInfo *info);
typedef void (* RenLightBackDataFiniFunc) (RenLight *light,
	RenLightBackData *back_data, void *user_data);
typedef void (* RenLightBackDataUpdateFunc) (RenLight *light,
	RenLightBackData *back_data, void *user_data,
	const RenLightInfo *info);

extern RenLightBackDataKey*
ren_light_back_data_key_new (ren_size data_size,
	RenLightBackDataInitFunc init,
	RenLightBackDataFiniFunc fini,
	RenLightBackDataUpdateFunc update);

extern void
ren_light_back_data_key_user_data (RenLightBackDataKey *key,
	void *user_data);

extern void
ren_light_back_data_key_destroy_notify (RenLightBackDataKey *key,
	RenLightBackDataKeyDestroyNotifyFunc destroy_notify);

extern RenLightBackDataKey*
ren_light_back_data_key_ref (RenLightBackDataKey *key);

extern void
ren_light_back_data_key_unref (RenLightBackDataKey *key);

extern RenLightBackData*
ren_light_back_data (RenLight *light, RenLightBackDataKey *key);


#endif /* REN_LIGHT_H */
