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

struct _RenDataBlock
{
    ren_uint32 ref_count;

    void *data;
    ren_size size;
    RenUsage usage;
};

RenDataBlock*
ren_data_block_new (ren_size size, RenUsage usage)
{
    RenDataBlock *data_block = g_new0 (RenDataBlock, 1);

    data_block->ref_count = 1;

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

    g_free (data_block->data);
    g_free (data_block);
}

void
ren_data_block_resize (RenDataBlock *data_block, ren_size size)
{

}

void*
ren_data_block_begin_edit (RenDataBlock *data_block)
{
    return data_block->data;
}

void
ren_data_block_end_edit (RenDataBlock *data_block)
{

}

void
ren_data_block_changed (RenDataBlock *data_block,
    ren_size from, ren_size count)
{

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
