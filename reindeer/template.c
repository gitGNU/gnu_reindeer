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
#include <stdio.h>
#include <string.h>

#include "template.h"

static void
ren_template_end_mode (RenTemplate *tmplt);

RenTemplate*
ren_template_new (RenIndexArray *index_array)
{
	RenTemplate *tmplt = g_new (RenTemplate, 1);

	tmplt->ref_count = 1;

	tmplt->built = FALSE;
	tmplt->num_materials = 0;

	tmplt->b.primitives = g_array_new (FALSE, FALSE,
		sizeof (RenTemplatePrimitive));
	tmplt->b.modes = g_array_new (FALSE, FALSE, sizeof (guint));
	tmplt->b.mode_data = g_array_new (FALSE, FALSE, sizeof (gchar));

	tmplt->info.index_array = (index_array != NULL) ?
		ren_index_array_ref (index_array) : NULL;
	tmplt->info.num_primitives = 0;
	tmplt->info.primitives = NULL;
	tmplt->info.num_modes = 0;
	tmplt->info.modes = NULL;

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

	if (tmplt->built)
	{
		_REN_RES_BACK_DATA_LIST_CLEAR (Template, template,
			tmplt, _REN_BACK_DATA_STATIC_FINI_FUNC);
		g_free (tmplt->data);
	}
	else
	{
		g_array_free (tmplt->b.primitives, TRUE);
		g_array_free (tmplt->b.modes, TRUE);
		g_array_free (tmplt->b.mode_data, TRUE);
	}

	if (tmplt->info.index_array != NULL)
		ren_index_array_unref (tmplt->info.index_array);

	g_free (tmplt);
}

static inline RenState calculate_mode_state (const ModeRecord *record)
{
	RenState state = 0;
	ren_bool done = FALSE;
	do
	{
		switch (record->meta.cmd)
		{
			case REN_TEMPLATE_MODE_CMD_END:
				done = TRUE;
				break;
			case REN_TEMPLATE_MODE_CMD_MATERIAL:
				state |= REN_STATE_MATERIALS;
				break;
			default:
				break;
		}
		record = ((void *) record) + record->meta.size;
	} while (!done);
	return state;
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
		num_primitives * sizeof (RenTemplatePrimitive);
	ren_size mode_data_size = tmplt->b.mode_data->len;
	ren_size num_modes = tmplt->b.modes->len;
	ren_size modes_array_size = num_modes * sizeof (RenTemplateMode);
	gpointer data = g_malloc (
		prim_array_size + modes_array_size + mode_data_size);
	RenTemplatePrimitive *primitives = data;
	RenTemplateMode *modes = (gpointer) primitives + prim_array_size;
	guint8 *mode_data = (gpointer) modes + modes_array_size;

	memcpy (primitives, tmplt->b.primitives->data, prim_array_size);
	memcpy (mode_data, tmplt->b.mode_data->data, mode_data_size);
	gint i;
	for (i = 0; i < num_modes; ++i)
	{
		modes[i].data = &mode_data[g_array_index (
			tmplt->b.modes, guint, i)];
		modes[i].state = calculate_mode_state (modes[i].data);
	}

	g_array_free (tmplt->b.primitives, TRUE);
	g_array_free (tmplt->b.modes, TRUE);
	g_array_free (tmplt->b.mode_data, TRUE);

	tmplt->built = TRUE;

	tmplt->data = data;
	tmplt->bd_list = NULL;

	tmplt->info.num_primitives = num_primitives;
	tmplt->info.primitives = primitives;
	tmplt->info.num_modes = num_modes;
	tmplt->info.modes = modes;
}

void
ren_template_debug (RenTemplate *tmplt)
{
	if (!tmplt->built) return;

	gint i;
	printf ("Number of materials: %d\n", (int) tmplt->num_materials);
	printf ("Primitives:\n");
	for (i = 0; i < tmplt->info.num_primitives; ++i)
	{
		const RenTemplatePrimitive *primitive =
			&tmplt->info.primitives[i];
		printf ("\t%d: M:%d P:%d O:%d C:%d\n",
			i, primitive->mode, primitive->prim,
			primitive->offset, primitive->count);
	}
	printf ("Modes:\n");
	for (i = 0; i < tmplt->info.num_modes; ++i)
	{
		printf ("\tMode %d:\n", i);
		const ModeRecord *record = tmplt->info.modes[i].data;
		while (TRUE)
		{
			RenTemplateModeCmd cmd = record->meta.cmd;
			if (cmd == REN_TEMPLATE_MODE_CMD_END)
				break;
			else
				printf ("\t\t");
			switch (cmd)
			{
				case REN_TEMPLATE_MODE_CMD_MATERIAL:
	printf ("Material: F:%u B:%u",
		(unsigned int) record->data.material.front,
		(unsigned int) record->data.material.back);
					break;
				default:
	printf ("Unknown mode! CMD=%u", cmd);
					break;
			}
			printf ("\n");
			record = ((void *) record) + record->meta.size;
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
	RenTemplatePrimitive *primitive = &g_array_index (
		tmplt->b.primitives, RenTemplatePrimitive, last);
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
	ModeRecordMeta record = {sizeof (record), REN_TEMPLATE_MODE_CMD_END};
	g_array_append_vals (tmplt->b.mode_data, &record, sizeof (record));
}

#define MODE_ADD_CHANGE_RECORD(ModeRecordType,MODE_CMD,ADD_CODE,CHANGE_CODE)\
	G_STMT_START {\
	if (tmplt->built) return;\
	if (tmplt->b.modes->len == 0) return;\
	ren_uint32 mode = tmplt->b.modes->len - 1;\
	guint offset = g_array_index (tmplt->b.modes, guint, mode);\
	while (TRUE)\
	{\
		if (offset > tmplt->b.mode_data->len)\
		{\
			g_critical ("Bug in Reindeer. Template corrupted!");\
			return;\
		}\
		ModeRecordMaterial *record = (ModeRecordMaterial *)\
			(tmplt->b.mode_data->data + offset);\
		if (offset == tmplt->b.mode_data->len ||\
			record->meta.cmd < REN_TEMPLATE_MODE_CMD_MATERIAL)\
		{\
			ADD_CODE();\
			break;\
		}\
		if (record->meta.cmd == REN_TEMPLATE_MODE_CMD_MATERIAL)\
		{\
			CHANGE_CODE();\
			break;\
		}\
		offset += record->meta.size;\
	}\
	} G_STMT_END

void
ren_template_material (RenTemplate *tmplt, RenFace face, ren_sint08 material)
{
	#define MATERIAL_ADD_RECORD()\
		G_STMT_START {\
		if (face == REN_FACE_BOTH && material == -1)\
			break;\
		ModeRecordMaterial new =\
		{\
			{\
			sizeof (new),\
			REN_TEMPLATE_MODE_CMD_MATERIAL,\
			},\
			{\
			(face & REN_FACE_FRONT) ? material : -1,\
			(face & REN_FACE_BACK) ? material : -1\
			}\
		};\
		g_array_insert_vals (tmplt->b.mode_data,\
			offset, &new, sizeof (new));\
		} G_STMT_END
	#define MATERIAL_CHANGE_RECORD()\
		G_STMT_START {\
		if (material == -1 && (\
			face == REN_FACE_BOTH ||\
			(face == REN_FACE_FRONT &&\
				record->data.back == -1) ||\
			(face == REN_FACE_BACK &&\
				record->data.front == -1)))\
		{\
			g_array_remove_range (tmplt->b.mode_data,\
				offset, record->meta.size);\
		}\
		else\
		{\
			if (face & REN_FACE_FRONT)\
				record->data.front = material;\
			if (face & REN_FACE_BACK)\
				record->data.back = material;\
		}\
		} G_STMT_END

	if (material < -1)
		material = -1;
	MODE_ADD_CHANGE_RECORD (ModeRecordMaterial,
		REN_TEMPLATE_MODE_CMD_MATERIAL,
		MATERIAL_ADD_RECORD,
		MATERIAL_CHANGE_RECORD);
	tmplt->num_materials = MAX (tmplt->num_materials, material + 1);
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

const RenTemplateInfo*
ren_template_info (RenTemplate *tmplt)
{
	return &(tmplt->info);
}

void
ren_template_mode_switch (void **next_p, void **prev_p,
	RenTemplateModeCmd *mc, const RenTemplateModeData **md)
{
	g_return_if_fail (next_p != NULL);
	g_return_if_fail (prev_p != NULL);
	g_return_if_fail (mc != NULL);
	g_return_if_fail (md != NULL);
	ModeRecord *next = *next_p;
	g_return_if_fail (next != NULL);
	ModeRecord *prev = *prev_p;
	RenTemplateModeCmd ncmd = next->meta.cmd;
	RenTemplateModeCmd pcmd = (prev != NULL) ?
		prev->meta.cmd : REN_TEMPLATE_MODE_CMD_END;
	if (	pcmd == REN_TEMPLATE_MODE_CMD_END &&
		ncmd == REN_TEMPLATE_MODE_CMD_END)
	{
		*mc = REN_TEMPLATE_MODE_CMD_END;
		*md = NULL;
		return;
	}
	else if (pcmd > ncmd)
	{
		*mc = pcmd;
		*md = NULL;
		*prev_p += prev->meta.size;
		return;
	}
	else
	{
		*mc = ncmd;
		*md = &(next->data);
		*next_p += next->meta.size;
		if (pcmd == ncmd)
			*prev_p += prev->meta.size;
		return;
	}
}

RenTemplateBackDataKey*
ren_template_back_data_key_new (ren_size data_size,
	RenTemplateBackDataInitFunc init,
	RenTemplateBackDataFiniFunc fini)
{
	RenTemplateBackDataKey *key = g_new (RenTemplateBackDataKey, 1);

	key->ref_count = 1;
	key->destroy_notify = NULL;

	key->bd_list = NULL;

	key->data_size = data_size;
	key->user_data = NULL;
	key->init = init;
	key->fini = fini;

	return key;
}

void
ren_template_back_data_key_user_data (RenTemplateBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_template_back_data_key_destroy_notify (RenTemplateBackDataKey *key,
	RenTemplateBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenTemplateBackDataKey*
ren_template_back_data_key_ref (RenTemplateBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_template_back_data_key_unref (RenTemplateBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (Template, template,
		key, _REN_BACK_DATA_STATIC_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenTemplateBackData*
ren_template_back_data (RenTemplate *tmplt, RenTemplateBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (Template, template,
		tmplt, key,
		_REN_BACK_DATA_STATIC_INIT_FUNC,
		_REN_BACK_DATA_STATIC_UPDATE_FUNC);
}
