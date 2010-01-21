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

#include <ren/template.h>
#include <ren/base.h>
#include <ren/indexarray.h>
#include <glib.h>
#include <string.h>

#include "template.h"

static void
ren_template_end_mode (RenTemplate *tmplt);

RenTemplate*
ren_template_new (RenIndexArray *ix_array)
{
    RenTemplate *tmplt = g_new (RenTemplate, 1);
    
    tmplt->ref_count = 1;
    
    tmplt->built = FALSE;
    tmplt->ix_array = (ix_array != NULL) ? ren_index_array_ref (ix_array) : NULL;
    tmplt->num_materials = 0;
    
    tmplt->b.modes = g_array_new (FALSE, FALSE, sizeof (guint));
    tmplt->b.mode_data = g_array_new (FALSE, FALSE, sizeof (ren_uint08));
    tmplt->b.primitives = g_array_new (FALSE, FALSE,
        sizeof (struct _RenTemplatePrimitive));
    
    return tmplt;
}

RenTemplate*
ren_template_ref (RenTemplate *tmplt)
{
    ++(tmplt->ref_count);
    return tmplt;
}

void
ren_template_unref (RenTemplate *tmplt)
{
    if (--(tmplt->ref_count) > 0)
        return;

    if (tmplt->ix_array != NULL)
        ren_index_array_unref (tmplt->ix_array);

    if (tmplt->built)
        g_free (tmplt->data);
    else
    {
        g_array_free (tmplt->b.primitives, TRUE);
        g_array_free (tmplt->b.modes, TRUE);
        g_array_free (tmplt->b.mode_data, TRUE);
    }

    g_free (tmplt);
}

void
ren_template_build (RenTemplate *tmplt)
{
    if (tmplt->built) return;

    if (tmplt->b.modes->len == 0)
        ren_template_new_mode (tmplt);
    ren_template_end_mode (tmplt);

    ren_size num_primitives = tmplt->b.primitives->len;
    ren_size prim_array_size =
        num_primitives * sizeof (struct _RenTemplatePrimitive);
    ren_size num_modes = tmplt->b.modes->len;
    ren_size modes_array_size = num_modes * sizeof (ren_uint08*);
    gpointer data = g_malloc (
        prim_array_size + modes_array_size + tmplt->b.mode_data->len);
    ren_uint08 **modes = data + prim_array_size;
    ren_uint08 *mode_data = (gpointer) modes + modes_array_size;

    memcpy (data, tmplt->b.primitives->data, prim_array_size);
    gint i;
    for (i = 0; i < num_modes; ++i)
        modes[i] = &mode_data[g_array_index (tmplt->b.modes, guint, i)];
    memcpy (mode_data, tmplt->b.mode_data->data, tmplt->b.mode_data->len);

    g_array_free (tmplt->b.primitives, TRUE);
    g_array_free (tmplt->b.modes, TRUE);
    g_array_free (tmplt->b.mode_data, TRUE);
    
    tmplt->num_primitives = num_primitives;
    tmplt->primitives = data; /* Also sets tmplt->data.  */
    tmplt->num_modes = num_modes;
    tmplt->modes = modes;

    tmplt->built = TRUE;
}

#include <stdio.h>
void
ren_template_debug (RenTemplate *tmplt)
{
    if (!tmplt->built) return;

    gint i;
    printf ("Number of materials: %d\n", (int)tmplt->num_materials);
    printf ("Primitives:\n");
    for (i = 0; i < tmplt->num_primitives; ++i)
    {
        struct _RenTemplatePrimitive *primitive = &tmplt->primitives[i];
        printf ("\t%d: M:%d P:%d O:%d C:%d\n",
            i, primitive->mode, primitive->prim,
            primitive->offset, primitive->count);
    }
    printf ("Modes:\n");
    for (i = 0; i < tmplt->num_modes; ++i)
    {
        printf ("\tMode %d:\n", i);
        ren_uint08 *pos = tmplt->modes[i];
        while (TRUE)
        {
            _RenModeCmd cmd = pos[1];
            if (cmd == REN_MODE_CMD_END)
                break;
            else
                printf ("\t\t");
            switch (cmd)
            {
                case REN_MODE_CMD_MATERIAL:
                    printf ("Material: F:%u B:%u",
                        (unsigned int) pos[2], (unsigned int) pos[3]);
                    break;
                default:
                    printf ("Unknown mode! CMD=%u", cmd);
                    break;
            }
            printf ("\n");
            pos += pos[0];
        }
    }
}

void
ren_template_primitive (RenTemplate *tmplt,
    RenPrimitive prim, ren_size offset, ren_size count)
{
    if (tmplt->built) return;
    if (tmplt->b.modes->len == 0) return;
    ren_uint32 mode = tmplt->b.modes->len - 1;

    guint last = tmplt->b.primitives->len;
    g_array_set_size (tmplt->b.primitives, last + 1);
    struct _RenTemplatePrimitive *primitive = &g_array_index (
        tmplt->b.primitives, struct _RenTemplatePrimitive, last);
    primitive->mode = mode;
    primitive->prim = prim;
    primitive->offset = offset;
    primitive->count = count;
}

ren_uint32
ren_template_new_mode (RenTemplate *tmplt)
{
    if (tmplt->built) return 0;
    ren_uint32 mode = tmplt->b.modes->len;
    if (mode > 0) /* If this is not the first mode.  */
        ren_template_end_mode (tmplt);
    g_array_append_vals (tmplt->b.modes, &(tmplt->b.mode_data->len), 1);
    return mode;
}

static void
ren_template_end_mode (RenTemplate *tmplt)
{
    guint8 data[2] = {sizeof (data), REN_MODE_CMD_END};
    g_array_append_vals (tmplt->b.mode_data, data, sizeof (data));
}

void
ren_template_data_primitives (RenTemplate *tmplt,
    RenIndexArray **ix_array_p,
    ren_size *num_primitives_p, const RenTemplatePrimitive **primitives_p)
{
    if (ix_array_p)
        (*ix_array_p) = tmplt->ix_array;
    if (num_primitives_p)
        (*num_primitives_p) = tmplt->num_primitives;
    if (primitives_p)
        (*primitives_p) = tmplt->primitives;
}

ren_size
ren_primitive_vertex_count (RenPrimitive prim, ren_size count)
{
    if (count == 0)
        return 0;
    switch (prim)
    {
        case REN_PRIMITIVE_POINTS:
            return count;
        case REN_PRIMITIVE_LINES:
            return count * 2;
        case REN_PRIMITIVE_LINE_STRIP:
            return count + 1;
        case REN_PRIMITIVE_LINE_LOOP:
            return MAX (count, 2);
        case REN_PRIMITIVE_TRIANGLES:
            return count * 3;
        case REN_PRIMITIVE_TRIANGLE_FAN:
            return count + 2;
        case REN_PRIMITIVE_TRIANGLE_STRIP:
            return count + 2;
        case REN_PRIMITIVE_QUADS:
            return count * 4;
        case REN_PRIMITIVE_QUAD_STRIP:
            return count * 2 + 2;
        default:
            return count;
    }
};

#define GET_LAST_MODE_AND_OFFSET\
    if (tmplt->built) return;\
    if (tmplt->b.modes->len == 0) return;\
    ren_uint32 mode = tmplt->b.modes->len - 1;\
    ren_size offset = g_array_index (tmplt->b.modes, ren_size, mode);

void
ren_template_material (RenTemplate *tmplt, RenFace face, ren_uint08 material)
{
    GET_LAST_MODE_AND_OFFSET
    guint8 *mode_data = tmplt->b.mode_data->data;
    while (TRUE)
    {
        if (offset > tmplt->b.mode_data->len)
        {
            g_critical ("Bug in Reindeer. Template corrupted!");
            return;
        }
        if (offset == tmplt->b.mode_data->len ||
            mode_data[offset+1] < REN_MODE_CMD_MATERIAL)
        {
            guint8 data[4] = {sizeof (data), REN_MODE_CMD_MATERIAL,
                (face & REN_FACE_FRONT) ? material : 0,
                (face & REN_FACE_BACK) ? material : 0};
            g_array_insert_vals (tmplt->b.mode_data,
                offset, data, sizeof (data));
            break;
        }
        if (mode_data[offset+1] == REN_MODE_CMD_MATERIAL)
        {
            if (face & REN_FACE_FRONT)
                mode_data[offset+2] = material;
            if (face & REN_FACE_BACK)
                mode_data[offset+3] = material;
            break;
        }
        offset += mode_data[offset];
    }
    tmplt->num_materials = MAX (tmplt->num_materials, material);
}
