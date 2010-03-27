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

    ren_size material_array_offset = sizeof (RenObject);
    ren_size material_array_size =
        tmplt->num_materials * sizeof (RenMaterial*);

    RenObject *object = g_malloc0 (
        sizeof (RenObject) +
        material_array_size);

    object->ref_count = 1;

    object->tmplt = ren_template_ref (tmplt);

    object->coord_array = ren_vector_array_ref (coord_array);
    object->color_array = NULL;
    object->normal_array = NULL;

    object->materials = (gpointer) object + material_array_offset;

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

    ren_template_unref (object->tmplt);
    ren_vector_array_unref (object->coord_array);
    if (object->color_array != NULL)
        ren_color_array_unref (object->color_array);
    if (object->normal_array != NULL)
        ren_vector_array_unref (object->normal_array);
    g_free (object);
}

void
ren_object_coord_array (RenObject *object, RenVectorArray *coord_array)
{
    if (coord_array == NULL)
        return;
    ren_vector_array_unref (object->coord_array);
    object->coord_array = ren_vector_array_ref (coord_array);
}

void
ren_object_color_array (RenObject *object, RenColorArray *color_array)
{
    if (object->color_array != NULL)
        ren_color_array_unref (object->color_array);
    object->color_array = (color_array != NULL) ?
        ren_color_array_ref (color_array) : NULL;
}

void
ren_object_normal_array (RenObject *object, RenVectorArray *normal_array)
{
    if (object->normal_array != NULL)
        ren_vector_array_unref (object->normal_array);
    object->normal_array = (normal_array != NULL) ?
        ren_vector_array_ref (normal_array) : NULL;
}

void
ren_object_material (RenObject *object, ren_uint08 id, RenMaterial *material)
{
    if (id == 0 || id > object->tmplt->num_materials)
        return;
    RenMaterial *prev_material = object->materials[id - 1];
    if (prev_material != NULL)
        ren_material_unref (prev_material);
    object->materials[id - 1] = (material != NULL) ?
        ren_material_ref (material) : NULL;
}

void
ren_object_data (RenObject *object, RenTemplate **tmplt_p,
    RenVectorArray **coord_array_p, RenColorArray **color_array_p,
    RenVectorArray **normal_array_p/*, RenEdgeArray **edge_array_p*/)
{
    if (tmplt_p)
        (*tmplt_p) = object->tmplt;
    if (coord_array_p)
        (*coord_array_p) = object->coord_array;
    if (color_array_p)
        (*color_array_p) = object->color_array;
    if (normal_array_p)
        (*normal_array_p) = object->normal_array;
/*    if (edge_array_p)
        (*edge_array_p) = object->edge_array;*/
}

void
ren_object_data_material (RenObject *object, const RenMaterial ***materials_p)
{

}

/*void
ren_object_data_texture ()
{

}*/

static inline void
ren_object_change_mode_common (RenReindeer *r, RenObject *object,
    ren_uint08 *prev, ren_uint08 *next, void *user_data)
{
    while (TRUE)
    {
        _RenModeCmd pcmd = prev ? prev[1] : REN_MODE_CMD_END;
        _RenModeCmd ncmd = next[1];
        if (pcmd == REN_MODE_CMD_END && ncmd == REN_MODE_CMD_END)
            return;
        if (pcmd > ncmd)
        {
            switch (pcmd)
            {
                case REN_MODE_CMD_MATERIAL:
                    r->backend_data.object.change_material (r, NULL, NULL, user_data);
                    break;
            }
            prev += prev[0];
        }
        else
        {
            switch (ncmd)
            {
                case REN_MODE_CMD_MATERIAL:
                {
                    RenMaterial *front = (next[2] > 0) ? object->materials[next[2] - 1] : NULL;
                    RenMaterial *back = (next[3] > 0) ? object->materials[next[3] - 1] : NULL;
                    r->backend_data.object.change_material (r, front, back, user_data);
                }   break;
            }
            next += next[0];
            if (pcmd == ncmd)
                prev += prev[0];
        }
    }
}

void
ren_object_change_mode (RenReindeer *r, RenObject *object,
    ren_uint32 prev_mode, ren_uint32 next_mode, void *user_data)
{
    RenTemplate *tmplt = object->tmplt;
    ren_uint08 *prev = tmplt->modes[prev_mode];
    ren_uint08 *next = tmplt->modes[next_mode];
    ren_object_change_mode_common (r, object, prev, next, user_data);
}

void
ren_object_begin_mode (RenReindeer *r,
    RenTemplate *prev_tmplt, ren_uint32 prev_mode,
    RenObject *object, ren_uint32 mode, void *user_data)
{
    ren_uint08 *prev = prev_tmplt ? prev_tmplt->modes[prev_mode] : NULL;
    ren_uint08 *next = object->tmplt->modes[mode];
    ren_object_change_mode_common (r, object, prev, next, user_data);
}

/*TODO: ren_object_read_mode (r, object, mode, user_data)*/
