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

#ifndef REN_TEMPLATE_H
#define REN_TEMPLATE_H

#include <ren/types.h>

/* Public */

extern RenTemplate*
ren_template_new (RenIndexArray *ix_array);

extern RenTemplate*
ren_template_ref (RenTemplate *tmplt);

extern void
ren_template_unref (RenTemplate *tmplt);

extern void
ren_template_build (RenTemplate *tmplt);

extern void
ren_template_debug (RenTemplate *tmplt);

extern void
ren_template_primitive (RenTemplate *tmplt,
	RenPrimitive prim, ren_size offset, ren_size count);

extern ren_uint32
ren_template_new_mode (RenTemplate *tmplt);

extern void
ren_template_material (RenTemplate *tmplt, RenFace face, ren_sint08 material);

/* Backend */

typedef struct RenTemplatePrimitive RenTemplatePrimitive;
struct RenTemplatePrimitive
{
	ren_uint32 mode;
	RenPrimitive prim;
	ren_size offset;
	ren_size count;
};

typedef struct RenTemplateMode RenTemplateMode;
struct RenTemplateMode
{
	const void *data;
	RenState state;
};

typedef struct RenTemplateInfo RenTemplateInfo;
struct RenTemplateInfo
{
	RenIndexArray *index_array; /* If NULL use direct index */
	ren_size num_primitives;
	const RenTemplatePrimitive *primitives;
	ren_size num_modes;
	const RenTemplateMode *modes;
};

extern const RenTemplateInfo*
ren_template_info (RenTemplate *tmplt);

typedef struct _RenTemplateModeSwitch RenTemplateModeSwitch;

typedef ren_uint08 RenTemplateModeCmd;
enum
{
	REN_TEMPLATE_MODE_CMD_END = 0x0,
	REN_TEMPLATE_MODE_CMD_MATERIAL = 0x1,
};

typedef struct RenTemplateModeDataMaterial RenTemplateModeDataMaterial;
struct RenTemplateModeDataMaterial
{
	ren_sint08 front;
	ren_sint08 back;
};

typedef union RenTemplateModeData RenTemplateModeData;
union RenTemplateModeData
{
	RenTemplateModeDataMaterial material;
};

extern void
ren_template_mode_switch (const void **next_p, const void **prev_p,
	RenTemplateModeCmd *mc, const RenTemplateModeData **md);

typedef struct _RenTemplateBackData RenTemplateBackData;
typedef struct _RenTemplateBackDataKey RenTemplateBackDataKey;

typedef void (* RenTemplateBackDataKeyDestroyNotifyFunc) (
	RenTemplateBackDataKey* key, void *user_data);

typedef void (* RenTemplateBackDataInitFunc) (RenTemplate *tmplt,
	RenTemplateBackData *back_data, void *user_data,
	const RenTemplateInfo *info);
typedef void (* RenTemplateBackDataFiniFunc) (RenTemplate *tmplt,
	RenTemplateBackData *back_data, void *user_data);

extern RenTemplateBackDataKey*
ren_template_back_data_key_new (ren_size data_size,
	RenTemplateBackDataInitFunc init,
	RenTemplateBackDataFiniFunc fini);

extern void
ren_template_back_data_key_user_data (RenTemplateBackDataKey *key,
	void *user_data);

extern void
ren_template_back_data_key_destroy_notify (RenTemplateBackDataKey *key,
	RenTemplateBackDataKeyDestroyNotifyFunc destroy_notify);

extern RenTemplateBackDataKey*
ren_template_back_data_key_ref (RenTemplateBackDataKey *key);

extern void
ren_template_back_data_key_unref (RenTemplateBackDataKey *key);

extern RenTemplateBackData*
ren_template_back_data (RenTemplate *tmplt, RenTemplateBackDataKey *key);

#endif /* REN_TEMPLATE_H */
