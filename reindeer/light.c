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

#include <ren/light.h>
#include <ren/color.h>
#include <ren/vector.h>
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenLightBackDataItem _RenLightBackDataItem;

struct _RenLight
{
	ren_uint32 ref_count;

	_RenLightBackDataItem *bd_list;
	ren_uint32 change;

	RenLightInfo info;
};

struct _RenLightBackDataKey
{
	ren_uint32 ref_count;
	RenLightBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenLightBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenLightBackDataInitFunc init;
	RenLightBackDataFiniFunc fini;
	RenLightBackDataUpdateFunc update;
};

struct _RenLightBackDataItem
{
	_RenBackDataItem base;
	ren_uint32 change;
};

RenLight*
ren_light_new (RenLightType light_type)
{
	RenLight *light = g_new (RenLight, 1);

	light->ref_count = 1;

	light->bd_list = NULL;
	light->change = 1;

	light->info.light_type = light_type;
	light->info.ambient = NULL;
	light->info.diffuse = NULL;
	light->info.specular = NULL;
	light->info.attenuation = NULL;
	light->info.cutoff = 45.0;
	light->info.exponent = 0.0;

	return light;
}

RenLight*
ren_light_ref (RenLight *light)
{
	++(light->ref_count);
	return light;
}

void
ren_light_unref (RenLight *light)
{
	if (--(light->ref_count) > 0)
		return;

	_REN_RES_BACK_DATA_LIST_CLEAR (Light, light,
		light, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

	if (light->info.ambient != NULL)
		ren_color_unref (light->info.ambient);
	if (light->info.diffuse != NULL)
		ren_color_unref (light->info.diffuse);
	if (light->info.specular != NULL)
		ren_color_unref (light->info.specular);
	if (light->info.attenuation != NULL)
		ren_vector_unref (light->info.attenuation);
	g_free (light);
}

void
ren_light_begin_edit (RenLight *light)
{

}

void
ren_light_end_edit (RenLight *light)
{
	_REN_BACK_DATA_SIMPLE_CHANGED (Light, light, light);
}

void
ren_light_ambient (RenLight *light, RenColor *color)
{
	if (light->info.ambient != NULL)
		ren_color_unref (light->info.ambient);
	light->info.ambient = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_light_diffuse (RenLight *light, RenColor *color)
{
	if (light->info.diffuse != NULL)
		ren_color_unref (light->info.diffuse);
	light->info.diffuse = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_light_specular (RenLight *light, RenColor *color)
{
	if (light->info.specular != NULL)
		ren_color_unref (light->info.specular);
	light->info.specular = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_light_attenuation (RenLight *light, RenVector *k)
{
	if (light->info.attenuation != NULL)
		ren_vector_unref (light->info.attenuation);
	light->info.attenuation = (k != NULL) ? ren_vector_ref (k) : NULL;
}

extern const RenLightInfo*
ren_light_info (RenLight *light)
{
	return &(light->info);
}

RenLightBackDataKey*
ren_light_back_data_key_new (ren_size data_size,
	RenLightBackDataInitFunc init,
	RenLightBackDataFiniFunc fini,
	RenLightBackDataUpdateFunc update)
{
	RenLightBackDataKey *key = g_new (RenLightBackDataKey, 1);

	key->ref_count = 1;
	key->destroy_notify = NULL;

	key->bd_list = NULL;

	key->data_size = data_size;
	key->user_data = NULL;
	key->init = init;
	key->fini = fini;
	key->update = update;

	return key;
}

void
ren_light_back_data_key_user_data (RenLightBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_light_back_data_key_destroy_notify (RenLightBackDataKey *key,
	RenLightBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenLightBackDataKey*
ren_light_back_data_key_ref (RenLightBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_light_back_data_key_unref (RenLightBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (Light, light,
		key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenLightBackData*
ren_light_back_data (RenLight *light, RenLightBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (Light, light,
		light, key,
		_REN_BACK_DATA_SIMPLE_INIT_FUNC,
		_REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}
