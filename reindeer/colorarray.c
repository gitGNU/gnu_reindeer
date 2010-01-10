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

#include <ren/colorarray.h>
#include <ren/datablock.h>
#include <glib.h>

struct _RenColorArray
{
    ren_uint32 ref_count;

    RenDataBlock *data_block;
    ren_size start;
    ren_size count;
    ren_size stride;

    RenType type;
    RenColorFormat format;
};

RenColorArray*
ren_color_array_new (RenType type, RenColorFormat format,
    RenDataBlock *data_block, ren_size start, ren_size count, ren_size stride)
{
    if (type == REN_TYPE_BOOL)
        return NULL;
    if (data_block == NULL)
        return NULL;

    RenColorArray *vx_array = g_new0 (RenColorArray, 1);

    vx_array->ref_count = 1;

    vx_array->data_block = ren_data_block_ref (data_block);
    vx_array->start = start;
    vx_array->count = count;
    vx_array->stride = stride;

    vx_array->type = type;
    vx_array->format = format;

    return vx_array;
}

RenColorArray*
ren_color_array_ref (RenColorArray *vx_array)
{
    ++(vx_array->ref_count);
    return vx_array;
}

void
ren_color_array_unref (RenColorArray *vx_array)
{
    if (--(vx_array->ref_count) > 0)
        return;

    ren_data_block_unref (vx_array->data_block);
    g_free (vx_array);
}

void
ren_color_array_set_size (RenColorArray *vx_array, ren_size count)
{
    vx_array->count = count;
}

void
ren_color_array_data (RenColorArray *vx_array, RenDataBlock **data_block_p,
    ren_size *start_p, ren_size *count_p, ren_size *stride_p)
{
    if (data_block_p)
        (*data_block_p) = vx_array->data_block;
    if (start_p)
        (*start_p) = vx_array->start;
    if (count_p)
        (*count_p) = vx_array->count;
    if (stride_p)
        (*stride_p) = vx_array->stride;
}

void
ren_color_array_type (RenColorArray *vx_array,
    RenType *type_p, RenColorFormat *format_p)
{
    if (type_p)
        (*type_p) = vx_array->type;
    if (format_p)
        (*format_p) = vx_array->format;
}
