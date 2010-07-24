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

#ifndef REN_OBJECT_H
#define REN_OBJECT_H

#include <ren/types.h>

/* Public */

extern RenObject*
ren_object_new (RenTemplate *tmplt, RenVectorArray *coord_array);

extern RenObject*
ren_object_ref (RenObject *object);

extern void
ren_object_unref (RenObject *object);

extern void
ren_object_begin_edit (RenObject *object);

extern void
ren_object_end_edit (RenObject *object);

extern void
ren_object_coord_array (RenObject *object, RenVectorArray *coord_array);

extern void
ren_object_color_array (RenObject *object, RenColorArray *color_array);

extern void
ren_object_normal_array (RenObject *object, RenVectorArray *normal_array);

extern void
ren_object_material (RenObject *object, ren_sint08 id, RenMaterial *material);

/* Backend */

typedef struct RenObjectInfo RenObjectInfo;
struct RenObjectInfo
{
	RenTemplate *tmplt;
	RenState state;
	RenVectorArray *coord_array;
	RenColorArray *color_array;
	RenVectorArray *normal_array;
	RenMaterial **materials;
};

extern const RenObjectInfo*
ren_object_info (RenObject *object);

typedef struct _RenObjectBackData RenObjectBackData;
typedef struct _RenObjectBackDataKey RenObjectBackDataKey;

typedef void (* RenObjectBackDataKeyDestroyNotifyFunc) (
	RenObjectBackDataKey* key, void *user_data);

typedef void (* RenObjectBackDataInitFunc) (RenObject *object,
	RenObjectBackData *back_data, void *user_data,
	const RenObjectInfo *info);
typedef void (* RenObjectBackDataFiniFunc) (RenObject *object,
	RenObjectBackData *back_data, void *user_data);
typedef void (* RenObjectBackDataUpdateFunc) (RenObject *object,
	RenObjectBackData *back_data, void *user_data,
	const RenObjectInfo *info);

extern RenObjectBackDataKey*
ren_object_back_data_key_new (ren_size data_size,
	RenObjectBackDataInitFunc init,
	RenObjectBackDataFiniFunc fini,
	RenObjectBackDataUpdateFunc update);

extern void
ren_object_back_data_key_user_data (RenObjectBackDataKey *key,
	void *user_data);

extern void
ren_object_back_data_key_destroy_notify (RenObjectBackDataKey *key,
	RenObjectBackDataKeyDestroyNotifyFunc destroy_notify);

extern RenObjectBackDataKey*
ren_object_back_data_key_ref (RenObjectBackDataKey *key);

extern void
ren_object_back_data_key_unref (RenObjectBackDataKey *key);

extern RenObjectBackData*
ren_object_back_data (RenObject *object, RenObjectBackDataKey *key);

#endif /* REN_OBJECT_H */
