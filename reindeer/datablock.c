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

#include <ren/datablock.h>
#include <glib.h>

#include "backdata.h"

typedef struct _RenDataBlockBackDataItem _RenDataBlockBackDataItem;

typedef struct ChangeSet ChangeSet;
typedef struct ChangeInterval ChangeInterval;

struct _RenDataBlock
{
	ren_uint32 ref_count;

	_RenDataBlockBackDataItem *bd_list;
	ChangeSet *changes;
	ChangeInterval *recent_changes;

	void *data;
	ren_size size;
	RenUsage usage;
};

struct _RenDataBlockBackDataKey
{
	ren_uint32 ref_count;
	RenDataBlockBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenDataBlockBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenDataBlockBackDataInitFunc init;
	RenDataBlockBackDataFiniFunc fini;
	RenDataBlockBackDataUpdateFunc update;
	RenDataBlockBackDataRelocateFunc relocate;
	RenDataBlockBackDataResizeFunc resize;
};

struct _RenDataBlockBackDataItem
{
	_RenBackDataItem base;
	ChangeSet *change_set;
	void* last_data;
	ren_size last_size;
};

struct ChangeSet
{
	ren_uint32 ref_count;
	ChangeSet *prev;
	ChangeSet *next;
	ChangeInterval *intervals;
};

struct ChangeInterval
{
	ChangeInterval *next;
	ren_size from;
	ren_size length;
};

static inline ChangeInterval*
new_change (ChangeInterval *next, ren_size from, ren_size length);

static inline ChangeInterval**
insert_change (ChangeInterval **previntv_next, ren_size from, ren_size length);

static inline void
try_merge_changes (ChangeInterval *intv);

static inline ChangeSet*
new_change_set (ChangeSet *next);

static inline ChangeSet*
push_change_set (RenDataBlock *data_block);

static inline void
unref_change_set (RenDataBlock *data_block, ChangeSet *set);

#define BACK_DATA_INIT_FUNC(res, key, item, data)\
	G_STMT_START {\
		item->last_data = res->data;\
		item->last_size = res->size;\
		if (key->init != NULL)\
			key->init (res, data, key->user_data);\
		if (key->update != NULL)\
		{\
			item->change_set = push_change_set (res);\
			key->update (res, data, key->user_data, 0, res->size);\
		}\
		else\
			item->change_set = NULL;\
	} G_STMT_END

#define BACK_DATA_FINI_FUNC(res, key, item, data)\
	G_STMT_START {\
		unref_change_set (res, item->change_set);\
		if (key->fini != NULL)\
			key->fini (res, data, key->user_data);\
	} G_STMT_END

#define BACK_DATA_UPDATE_FUNC(res, key, item, data)\
	G_STMT_START {\
		if (key->relocate != NULL && item->last_data != res->data)\
		{\
			key->relocate (res, data, key->user_data, res->data);\
			item->last_data = res->data;\
		}\
		ren_size db_size = res->size;\
		if (key->resize != NULL && item->last_size != db_size)\
		{\
			key->resize (res, data, key->user_data, db_size);\
			item->last_size = db_size;\
		}\
		if (item->change_set != NULL)\
		{\
			ChangeInterval *intv = item->change_set->intervals;\
			if (intv != NULL)\
			{\
				do\
				{\
					ren_size from = intv->from;\
					if (from >= db_size)\
						break;\
					ren_size length = intv->length;\
					ren_size end_length = db_size - from;\
					if (length < end_length)\
					{\
						key->update (res, data, key->user_data,\
							from, length);\
						intv = intv->next;\
					}\
					else\
					{\
						key->update (res, data, key->user_data,\
							from, end_length);\
						break;\
					}\
				} while (intv != NULL);\
				unref_change_set (res, item->change_set);\
				item->change_set = push_change_set (res);\
			}\
		}\
	} G_STMT_END

RenDataBlock*
ren_data_block_new (ren_size size, RenUsage usage)
{
	RenDataBlock *data_block = g_new (RenDataBlock, 1);

	data_block->ref_count = 1;

	data_block->bd_list = NULL;
	data_block->changes = new_change_set (NULL);
	data_block->recent_changes = NULL;

	data_block->data = g_malloc (size);
	data_block->size = size;
	data_block->usage = usage;

	return data_block;
}

RenDataBlock*
ren_data_block_ref (RenDataBlock *data_block)
{
	++(data_block->ref_count);
	return data_block;
}

void
ren_data_block_unref (RenDataBlock *data_block)
{
	if (--(data_block->ref_count) > 0)
		return;

	_REN_RES_BACK_DATA_LIST_CLEAR (DataBlock, data_block,
		data_block, BACK_DATA_FINI_FUNC);

	g_free (data_block->data);
	g_free (data_block);
}

void
ren_data_block_resize (RenDataBlock *data_block, ren_size size)
{
	data_block->data = g_realloc (data_block->data, size);
	data_block->size = size;
}

void*
ren_data_block_begin_edit (RenDataBlock *data_block)
{
	return data_block->data;
}

void
ren_data_block_end_edit (RenDataBlock *data_block)
{
	ChangeSet *set;
	ChangeInterval **previntv_next;
	ChangeInterval *intv;
	if (data_block->recent_changes == NULL)
	{
		data_block->recent_changes = new_change (NULL, 0, data_block->size);
	}
	for (set = data_block->changes; set != NULL; set = set->next)
	{
		previntv_next = &(set->intervals);
		for (intv = data_block->recent_changes; intv != NULL; intv = intv->next)
		{
			previntv_next = insert_change (previntv_next,
				intv->from, intv->length);
		}
	}
	ChangeInterval *nextintv;
	intv = data_block->recent_changes;
	data_block->recent_changes = NULL;
	while (intv != NULL)
	{
		nextintv = intv->next;
		g_free (intv);
		intv = nextintv;
	}
}

void
ren_data_block_changed (RenDataBlock *data_block,
	ren_size from, ren_size length)
{
	ren_size db_size = data_block->size;
	if (from >= db_size)
		return;
	ren_size end_length = db_size - from;
	if (length == 0 || end_length < length)
		length = end_length;
	insert_change (&(data_block->recent_changes), from, length);
}

void
ren_data_block_data (RenDataBlock *data_block, const void **data_p,
	ren_size *size_p, RenUsage *usage_p)
{
	if (data_p)
		(*data_p) = data_block->data;
	if (size_p)
		(*size_p) = data_block->size;
	if (usage_p)
		(*usage_p) = data_block->usage;
}

RenDataBlockBackDataKey*
ren_data_block_back_data_key_new (ren_size data_size,
	RenDataBlockBackDataInitFunc init,
	RenDataBlockBackDataFiniFunc fini,
	RenDataBlockBackDataUpdateFunc update,
	RenDataBlockBackDataRelocateFunc relocate,
	RenDataBlockBackDataResizeFunc resize)
{
	RenDataBlockBackDataKey *key = g_new (RenDataBlockBackDataKey, 1);

	key->ref_count = 1;
	key->destroy_notify = NULL;

	key->bd_list = NULL;

	key->data_size = data_size;
	key->user_data = NULL;
	key->init = init;
	key->fini = fini;
	key->update = update;
	key->relocate = relocate;
	key->resize = resize;

	return key;
}

void
ren_data_block_back_data_key_user_data (RenDataBlockBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_data_block_back_data_key_destroy_notify (RenDataBlockBackDataKey *key,
	RenDataBlockBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenDataBlockBackDataKey*
ren_data_block_back_data_key_ref (RenDataBlockBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_data_block_back_data_key_unref (RenDataBlockBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (DataBlock, data_block,
		key, BACK_DATA_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenDataBlockBackData*
ren_data_block_back_data (RenDataBlock *data_block,
	RenDataBlockBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (DataBlock, data_block,
		data_block, key, BACK_DATA_INIT_FUNC, BACK_DATA_UPDATE_FUNC);
}

static inline ChangeInterval*
new_change (ChangeInterval *next, ren_size from, ren_size length)
{
	ChangeInterval *intv = g_new (ChangeInterval, 1);
	intv->next = next;
	intv->from = from;
	intv->length = length;
	return intv;
}

static inline ChangeInterval**
insert_change (ChangeInterval **previntv_next, ren_size from, ren_size length)
{
	ChangeInterval *intv = *previntv_next;
	while (intv != NULL)
	{
		if (from + length < intv->from)
		{
			*previntv_next = new_change (intv, from, length);
			return previntv_next;
		}
		else if (from < intv->from)
		{
			intv->from = from;
			intv->length = MAX (length, intv->length + (intv->from - from));
			try_merge_changes (intv);
			return previntv_next;
		}
		else if (from <= intv->from + intv->length)
		{
			intv->length = MAX (intv->length, length + (from - intv->from));
			try_merge_changes (intv);
			return previntv_next;
		}
		previntv_next = &(intv->next);
		intv = intv->next;
	}
	*previntv_next = new_change (NULL, from, length);
	return previntv_next;
}

static inline void
try_merge_changes (ChangeInterval *intv)
{
	ChangeInterval *ointv;
	while ((ointv = intv->next) != NULL)
	{
		if (ointv->from <= intv->from + intv->length)
		{
			intv->next = ointv->next;
			intv->length = MAX (
				intv->length, ointv->length + (ointv->from - intv->from));
			g_free (ointv);
		}
		else
			break;
	}
}

static inline ChangeSet*
new_change_set (ChangeSet *next)
{
	ChangeSet *set = g_new (ChangeSet, 1);
	set->ref_count = 0;
	set->prev = NULL;
	set->next = next;
	if (next != NULL)
		next->prev = set;
	set->intervals = NULL;
	return set;
}

static inline ChangeSet*
push_change_set (RenDataBlock *data_block)
{
	if (data_block->changes->intervals != NULL)
		data_block->changes = new_change_set (data_block->changes);
	++(data_block->changes->ref_count);
	return data_block->changes;
}

static inline void
unref_change_set (RenDataBlock *data_block, ChangeSet *set)
{
	if (--(set->ref_count) > 0)
		return;

	ChangeInterval *intv, *nextintv;
	intv = set->intervals;
	while (intv != NULL)
	{
		nextintv = intv->next;
		g_free (intv);
		intv = nextintv;
	}

	if (data_block->changes == set)
		data_block->changes = set->next;
	if (set->prev != NULL)
		set->prev->next = set->next;
	if (set->next != NULL)
		set->next->prev = set->prev;

	g_free (set);
}
