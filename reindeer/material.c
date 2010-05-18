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

#include <ren/material.h>
#include <ren/color.h>
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenMaterialBackDataItem _RenMaterialBackDataItem;

struct _RenMaterial
{
	ren_uint32 ref_count;

	_RenMaterialBackDataItem *bd_list;
	ren_uint32 change;

	RenMaterialInfo info;
};

struct _RenMaterialBackDataKey
{
	ren_uint32 ref_count;
	RenMaterialBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenMaterialBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenMaterialBackDataInitFunc init;
	RenMaterialBackDataFiniFunc fini;
	RenMaterialBackDataUpdateFunc update;
};

struct _RenMaterialBackDataItem
{
	_RenBackDataItem base;
	ren_uint32 change;
};

RenMaterial*
ren_material_new ()
{
	RenMaterial *material = g_new (RenMaterial, 1);

	material->ref_count = 1;

	material->bd_list = NULL;
	material->change = 1;

	material->info.ambient = NULL;
	material->info.diffuse = NULL;
	material->info.specular = NULL;
	material->info.emission = NULL;
	material->info.shininess = 0.0;

	return material;
}

RenMaterial*
ren_material_ref (RenMaterial *material)
{
	++(material->ref_count);
	return material;
}

void
ren_material_unref (RenMaterial *material)
{
	if (--(material->ref_count) > 0)
		return;

	_REN_RES_BACK_DATA_LIST_CLEAR (Material, material,
		material, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

	if (material->info.ambient != NULL)
		ren_color_unref (material->info.ambient);
	if (material->info.diffuse != NULL)
		ren_color_unref (material->info.diffuse);
	if (material->info.specular != NULL)
		ren_color_unref (material->info.specular);
	if (material->info.emission != NULL)
		ren_color_unref (material->info.emission);
	g_free (material);
}

void
ren_material_begin_edit (RenMaterial *material)
{

}

void
ren_material_end_edit (RenMaterial *material)
{
	_REN_BACK_DATA_SIMPLE_CHANGED (Material, material, material);
}

void
ren_material_ambient (RenMaterial *material, RenColor *color)
{
	if (material->info.ambient != NULL)
		ren_color_unref (material->info.ambient);
	material->info.ambient = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_diffuse (RenMaterial *material, RenColor *color)
{
	if (material->info.diffuse != NULL)
		ren_color_unref (material->info.diffuse);
	material->info.diffuse = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_specular (RenMaterial *material, RenColor *color)
{
	if (material->info.specular != NULL)
		ren_color_unref (material->info.specular);
	material->info.specular = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_emission (RenMaterial *material, RenColor *color)
{
	if (material->info.emission != NULL)
		ren_color_unref (material->info.emission);
	material->info.emission = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_shininess (RenMaterial *material, ren_dfloat shininess)
{
	material->info.shininess = shininess;
}

const RenMaterialInfo*
ren_material_info (RenMaterial *material)
{
	return &(material->info);
}

RenMaterialBackDataKey*
ren_material_back_data_key_new (ren_size data_size,
	RenMaterialBackDataInitFunc init,
	RenMaterialBackDataFiniFunc fini,
	RenMaterialBackDataUpdateFunc update)
{
	RenMaterialBackDataKey *key = g_new (RenMaterialBackDataKey, 1);

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
ren_material_back_data_key_user_data (RenMaterialBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_material_back_data_key_destroy_notify (RenMaterialBackDataKey *key,
	RenMaterialBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenMaterialBackDataKey*
ren_material_back_data_key_ref (RenMaterialBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_material_back_data_key_unref (RenMaterialBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (Material, material,
		key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenMaterialBackData*
ren_material_back_data (RenMaterial *material, RenMaterialBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (Material, material,
		material, key,
		_REN_BACK_DATA_SIMPLE_INIT_FUNC,
		_REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}
