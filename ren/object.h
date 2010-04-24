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
ren_object_coord_array (RenObject *object, RenVectorArray *coord_array);

extern void
ren_object_color_array (RenObject *object, RenColorArray *color_array);

extern void
ren_object_normal_array (RenObject *object, RenVectorArray *normal_array);

extern void
ren_object_material (RenObject *object, ren_uint08 id, RenMaterial *material);

/* Backend */

extern void
ren_object_data (RenObject *object, RenTemplate **tmplt_p,
	RenVectorArray **coord_array_p, RenColorArray **color_array_p,
	RenVectorArray **normal_array_p/*, RenEdgeArray **edge_array_p*/);

extern void
ren_object_change_mode (RenReindeer *r, RenObject *object,
	ren_uint32 prev_mode, ren_uint32 next_mode, void *user_data);

extern void
ren_object_begin_mode (RenReindeer *r,
	RenTemplate *prev_template, ren_uint32 prev_mode,
	RenObject *object, ren_uint32 mode, void *user_data);

#endif /* REN_OBJECT_H */
