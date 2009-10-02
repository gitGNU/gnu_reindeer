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
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenColorBackDataItem _RenColorBackDataItem;

struct _RenColor
{
    ren_uint32 ref_count;

    const void *data;
    RenColorFormat format;
    RenType type;

    _RenColorBackDataItem *bd_list;
};

struct _RenColorBackDataKey
{
    ren_uint32 ref_count;

    ren_size size;
    RenColorBackDataInitFunc init;
    RenColorBackDataFiniFunc fini;
    RenColorBackDataUpdateFunc update;

    _RenColorBackDataItem *bd_list;
};

struct _RenColorBackDataItem
{
    _RenBackDataItem base;
    ren_bool changed;
};

RenColor*
ren_color_new (const void *data, RenColorFormat format, RenType type)
{
    RenColor *color = g_new (RenColor, 1);

    color->ref_count = 1;

    color->data = data;
    color->format = format;
    color->type = type;
    /* TODO: Check if format and type are valid. */

    color->bd_list = NULL;

    return color;
}

void
ren_color_destroy (RenColor *color)
{
    ren_color_unref (color);
}

void
ren_color_changed (RenColor *color)
{
    _REN_RES_BACK_DATA_LIST_UPDATE (Color,color,color,
        if (key->update != NULL)
            item->changed = TRUE;
    );
}

void
ren_color_ref (RenColor *color)
{
    ++(color->ref_count);
}

void
ren_color_unref (RenColor *color)
{
    if (--(color->ref_count) > 0)
        return;

    _REN_RES_BACK_DATA_LIST_CLEAR (Color,color,color,
        if (key->fini != NULL)
            key->fini (color, data);
        g_free (data);
    );

    g_free (color);
}

void
ren_color_data (RenColor *color,
    const void **datap, RenColorFormat *formatp, RenType *typep)
{
    if (datap)
        (*datap) = color->data;
    if (formatp)
        (*formatp) = color->format;
    if (typep)
        (*typep) = color->type;
}

RenColorBackDataKey*
ren_color_back_data_key_new (ren_size size, RenColorBackDataInitFunc init,
    RenColorBackDataFiniFunc fini, RenColorBackDataUpdateFunc update)
{
    RenColorBackDataKey *key = g_new (RenColorBackDataKey, 1);

    key->ref_count = 1;

    key->size = size;
    key->init = init;
    key->fini = fini;
    key->update = update;

    key->bd_list = NULL;

    return key;
}

void
ren_color_back_data_key_ref (RenColorBackDataKey *key)
{
    ++(key->ref_count);
}

void
ren_color_back_data_key_unref (RenColorBackDataKey *key)
{
    if (--(key->ref_count) > 0)
        return;

    _REN_KEY_BACK_DATA_LIST_CLEAR (Color,color,key,color,
        if (key->fini != NULL)
            key->fini (color, data);
        g_free (data);
    );

    g_free (key);
}

RenColorBackData*
ren_color_back_data (RenColor *color, RenColorBackDataKey *key)
{
    _REN_BACK_DATA_GET_OR_NEW (Color,color,color,key,
        if (key->init != NULL)
            key->init (color, data);
        if (key->update != NULL)
            item->changed = TRUE;
    );
    if (key->update != NULL && item->changed)
    {
        key->update (color, data);
        item->changed = FALSE;
    }
    return data;
}
