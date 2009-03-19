/*
    This file is part of Reindeer.

    Copyright (C) 2008, 2009 - Patrik Olsson

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
#include <ren/impl.h>

#include <stdlib.h>

struct _RenDataBlock
{
    ren_uint ref;
#if 0
    ren_array__t *shares; /* of ren_pointer */
    ren_dlist__t *updates; /* of struct {ren_offset from, ren_offset to} */

    /*???*/ vxarrays; /* Arrays defined by this datablock. */
    /* TODO: Possibly other things defined by this datablock. */
#endif
    RenDataBlockCallback reload_func;
    RenDataBlockCallback unload_func;
    void *user_data;

    const void *data;
    ren_size size;
    RenUsage usage;
};

RenDataBlock*
ren_data_block_new (const void *data, ren_size size, RenUsage usage)
{
    RenDataBlock *datablock;

    datablock = (RenDataBlock *) calloc (1, sizeof (struct _RenDataBlock));
    if (!datablock)
        goto FAIL;

    datablock->ref = 1;
    datablock->data = data;
    datablock->size = size;
    datablock->usage = usage;

    return datablock;

    FAIL:
    if (datablock) free (datablock);
    return NULL;
}

void
ren_data_block_destroy (
    RenDataBlock *datablock,
    RenDataBlockCallback unload_func, void *user_data)
{
    datablock->unload_func = unload_func;
    datablock->user_data = user_data;
    _ren_data_block_unref (datablock);
}

void
ren_data_block_callback (
    RenDataBlock *datablock,
    RenDataBlockCallback reload_func,
    RenDataBlockCallback unload_func,
    void *user_data)
{
    datablock->reload_func = reload_func;
    datablock->unload_func = unload_func;
    datablock->user_data = user_data;
}

void
ren_data_block_relocated (RenDataBlock *datablock, const void *data)
{
    datablock->data = data;
    /* TODO: This might need to be notified to binders, even though it does
    not change the actual data... */
}

void
ren_data_block_resized (RenDataBlock *datablock, ren_size size)
{
    datablock->size = size;
    /* TODO: This might need to be notified to binders, even though it does
    not change the actual data if new size > old size... */
}

void
ren_data_block_changed (RenDataBlock *datablock,
    ren_size from, ren_size to)
{

}

void
_ren_data_block_ref (RenDataBlock *datablock)
{
    ++(datablock->ref);
}

void
_ren_data_block_unref (RenDataBlock *datablock)
{
    if (--(datablock->ref) == 0)
    {
        if (datablock->unload_func != NULL)
            datablock->unload_func (NULL, datablock->user_data);
        free (datablock);
    }
}

void
_ren_data_block_data (RenDataBlock *datablock, const void **datap,
    ren_size *sizep, RenUsage *usagep)
{
    if (datap)
        (*datap) = datablock->data;
    if (sizep)
        (*sizep) = datablock->size;
    if (usagep)
        (*usagep) = datablock->usage;
}
