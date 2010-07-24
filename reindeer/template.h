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

#ifndef _REN_REINDEER_TEMPLATE_H
#define _REN_REINDEER_TEMPLATE_H

#include "backdata.h"

typedef struct _RenTemplateBackDataItem _RenTemplateBackDataItem;

struct _RenTemplate
{
	ren_uint32 ref_count;

	gboolean built;
	ren_uint08 num_materials;

	union
	{
		 /* When built */
		struct
		{
			gpointer data; /* Memory block used for info arrays */
			_RenTemplateBackDataItem *bd_list;
		};
		/* When !built */
		struct
		{
			GArray *primitives;
			GArray *modes; /* Offsets into mode_data */
			GArray *mode_data;
		} b;
	};

	RenTemplateInfo info;
};

struct _RenTemplateBackDataKey
{
	ren_uint32 ref_count;
	RenTemplateBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenTemplateBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenTemplateBackDataInitFunc init;
	RenTemplateBackDataFiniFunc fini;
};

struct _RenTemplateBackDataItem
{
	_RenBackDataItem base;
};

typedef struct ModeRecordMeta ModeRecordMeta;
struct ModeRecordMeta
{
	ren_uint08 size;
	RenTemplateModeCmd cmd;
};

typedef struct ModeRecord ModeRecord;
struct ModeRecord
{
	ModeRecordMeta meta;
	RenTemplateModeData data;
};

typedef struct ModeRecordMaterial ModeRecordMaterial;
struct ModeRecordMaterial
{
	ModeRecordMeta meta;
	RenTemplateModeDataMaterial data;
};

#endif /* _REN_REINDEER_TEMPLATE_H */
