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

#ifndef _REN_REINDEER_OBJECT_H
#define _REN_REINDEER_OBJECT_H

#include "template.h"
#include "backdata.h"

typedef struct _RenObjectBackDataItem _RenObjectBackDataItem;

struct _RenObject
{
	ren_uint32 ref_count;

	_RenObjectBackDataItem *bd_list;
	ren_uint32 change;

	RenObjectInfo info;
};

struct _RenObjectBackDataKey
{
	ren_uint32 ref_count;
	RenObjectBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenObjectBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenObjectBackDataInitFunc init;
	RenObjectBackDataFiniFunc fini;
	RenObjectBackDataUpdateFunc update;
};

struct _RenObjectBackDataItem
{
	_RenBackDataItem base;
	ren_uint32 change;
};

#endif /* _REN_REINDEER_OBJECT_H */
