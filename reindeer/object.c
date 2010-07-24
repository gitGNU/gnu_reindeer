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

#include <ren/ren.h>
#include <glib.h>

#include "reindeer.h"
#include "object.h"

RenObject*
ren_object_new (RenTemplate *tmplt, RenVectorArray *coord_array)
{
	if (tmplt == NULL || !tmplt->built)
		return NULL;
	if (coord_array == NULL)
		return NULL;

	RenState state = REN_STATE_COORD_ARRAY;
	if (tmplt->info.index_array != NULL)
		state |= REN_STATE_INDEX_ARRAY;

	RenObject *object = g_new (RenObject, 1);

	object->ref_count = 1;

	object->bd_list = NULL;
	object->change = 1;

	object->info.tmplt = ren_template_ref (tmplt);
	object->info.state = state;
	object->info.coord_array = ren_vector_array_ref (coord_array);
	object->info.color_array = NULL;
	object->info.normal_array = NULL;
	object->info.materials = g_new0 (RenMaterial*, tmplt->num_materials);

	return object;
}

RenObject*
ren_object_ref (RenObject *object)
{
	++(object->ref_count);
	return object;
}

void
ren_object_unref (RenObject *object)
{
	if (--(object->ref_count) > 0)
		return;

	_REN_RES_BACK_DATA_LIST_CLEAR (Object, object,
		object, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

	ren_template_unref (object->info.tmplt);
	ren_vector_array_unref (object->info.coord_array);
	if (object->info.color_array != NULL)
		ren_color_array_unref (object->info.color_array);
	if (object->info.normal_array != NULL)
		ren_vector_array_unref (object->info.normal_array);
	g_free (object);
}

void
ren_object_begin_edit (RenObject *object)
{

}

void
ren_object_end_edit (RenObject *object)
{
	_REN_BACK_DATA_SIMPLE_CHANGED (Object, object, object);
}

void
ren_object_coord_array (RenObject *object, RenVectorArray *coord_array)
{
	if (coord_array == NULL)
		return;
	ren_vector_array_unref (object->info.coord_array);
	object->info.coord_array = ren_vector_array_ref (coord_array);
}

void
ren_object_color_array (RenObject *object, RenColorArray *color_array)
{
	if (object->info.color_array != NULL)
		ren_color_array_unref (object->info.color_array);
	if (color_array != NULL)
	{
		object->info.color_array = ren_color_array_ref (color_array);
		object->info.state |= REN_STATE_COLOR_ARRAY;
	}
	else
	{
		object->info.color_array = NULL;
		object->info.state &= ~REN_STATE_COLOR_ARRAY;
	}
}

void
ren_object_normal_array (RenObject *object, RenVectorArray *normal_array)
{
	if (object->info.normal_array != NULL)
		ren_vector_array_unref (object->info.normal_array);
	if (normal_array != NULL)
	{
		object->info.normal_array = ren_vector_array_ref (normal_array);
		object->info.state |= REN_STATE_NORMAL_ARRAY;
	}
	else
	{
		object->info.normal_array = NULL;
		object->info.state &= ~REN_STATE_NORMAL_ARRAY;
	}
}

void
ren_object_material (RenObject *object, ren_sint08 id, RenMaterial *material)
{
	if (id < 0 || id > object->info.tmplt->num_materials - 1)
		return;
	RenMaterial *prev_material = object->info.materials[id];
	if (prev_material != NULL)
		ren_material_unref (prev_material);
	object->info.materials[id] = (material != NULL) ?
		ren_material_ref (material) : NULL;
}

const RenObjectInfo*
ren_object_info (RenObject *object)
{
	return &(object->info);
}

RenObjectBackDataKey*
ren_object_back_data_key_new (ren_size data_size,
	RenObjectBackDataInitFunc init,
	RenObjectBackDataFiniFunc fini,
	RenObjectBackDataUpdateFunc update)
{
	RenObjectBackDataKey *key = g_new (RenObjectBackDataKey, 1);

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
ren_object_back_data_key_user_data (RenObjectBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_object_back_data_key_destroy_notify (RenObjectBackDataKey *key,
	RenObjectBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenObjectBackDataKey*
ren_object_back_data_key_ref (RenObjectBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_object_back_data_key_unref (RenObjectBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (Object, object,
		key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenObjectBackData*
ren_object_back_data (RenObject *object, RenObjectBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (Object, object,
		object, key,
		_REN_BACK_DATA_SIMPLE_INIT_FUNC,
		_REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}
