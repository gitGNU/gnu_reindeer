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

#ifndef REN_DATABLOCK_H
#define REN_DATABLOCK_H

#include <ren/types.h>

/* Public */

extern RenDataBlock*
ren_data_block_new (ren_size size, RenUsage usage);

extern RenDataBlock*
ren_data_block_ref (RenDataBlock *data_block);

extern void
ren_data_block_unref (RenDataBlock *data_block);

extern void
ren_data_block_resize (RenDataBlock *data_block, ren_size size);

extern void*
ren_data_block_begin_edit (RenDataBlock *data_block);

extern void
ren_data_block_end_edit (RenDataBlock *data_block);

extern void
ren_data_block_changed (RenDataBlock *data_block,
    ren_size from, ren_size length);

/* Backend */

extern void
ren_data_block_data (RenDataBlock *data_block, const void **data_p,
    ren_size *size_p, RenUsage *usage_p);

typedef struct _RenDataBlockBackData RenDataBlockBackData;
typedef struct _RenDataBlockBackDataKey RenDataBlockBackDataKey;

typedef void (* RenDataBlockBackDataKeyDestroyNotifyFunc) (
    RenDataBlockBackDataKey* key, void *user_data);

typedef void (* RenDataBlockBackDataInitFunc) (RenDataBlock *data_block,
    RenDataBlockBackData *back_data, void* user_data);
typedef void (* RenDataBlockBackDataFiniFunc) (RenDataBlock *data_block,
    RenDataBlockBackData *back_data, void* user_data);
typedef void (* RenDataBlockBackDataUpdateFunc) (RenDataBlock *data_block,
    RenDataBlockBackData *back_data, void* user_data,
    ren_size from, ren_size length);
typedef void (* RenDataBlockBackDataRelocateFunc) (RenDataBlock *data_block,
    RenDataBlockBackData *back_data, void* user_data, void *new_data);
typedef void (* RenDataBlockBackDataResizeFunc) (RenDataBlock *data_block,
    RenDataBlockBackData *back_data, void* user_data, ren_size new_size);

extern RenDataBlockBackDataKey*
ren_data_block_back_data_key_new (ren_size data_size,
    RenDataBlockBackDataInitFunc init,
    RenDataBlockBackDataFiniFunc fini,
    RenDataBlockBackDataUpdateFunc update,
    RenDataBlockBackDataRelocateFunc relocate,
    RenDataBlockBackDataResizeFunc resize);

extern void
ren_data_block_back_data_key_user_data (RenDataBlockBackDataKey *key,
    void *user_data);

extern void
ren_data_block_back_data_key_destroy_notify (RenDataBlockBackDataKey *key,
    RenDataBlockBackDataKeyDestroyNotifyFunc destroy_notify);

extern RenDataBlockBackDataKey*
ren_data_block_back_data_key_ref (RenDataBlockBackDataKey *key);

extern void
ren_data_block_back_data_key_unref (RenDataBlockBackDataKey *key);

extern RenDataBlockBackData*
ren_data_block_back_data (RenDataBlock *data_block,
    RenDataBlockBackDataKey *key);

#endif /* REN_DATABLOCK_H */
