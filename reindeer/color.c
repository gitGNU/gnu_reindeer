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

#include <ren/color.h>
#include <ren/base.h>
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenColorBackDataItem _RenColorBackDataItem;

struct _RenColor
{
    ren_uint32 ref_count;

    _RenColorBackDataItem *bd_list;
    ren_uint32 change;

    void *data;
    RenColorFormat format;
    RenType type;
};

struct _RenColorBackDataKey
{
    ren_uint32 ref_count;
    RenColorBackDataKeyDestroyNotifyFunc destroy_notify;

    _RenColorBackDataItem *bd_list;

    ren_size data_size;
    void *user_data;
    RenColorBackDataInitFunc init;
    RenColorBackDataFiniFunc fini;
    RenColorBackDataUpdateFunc update;
};

struct _RenColorBackDataItem
{
    _RenBackDataItem base;
    ren_uint32 change;
};

RenColor*
ren_color_new (RenColorFormat format, RenType type)
{
    if (type == REN_TYPE_BOOL)
        return NULL;

    RenColor *color = g_new (RenColor, 1);

    color->ref_count = 1;

    color->bd_list = NULL;
    color->change = 1;

    color->data = g_malloc (ren_color_format_sizeof (format, type));
    color->format = format;
    color->type = type;

    return color;
}

RenColor*
ren_color_ref (RenColor *color)
{
    ++(color->ref_count);
    return color;
}

void
ren_color_unref (RenColor *color)
{
    if (--(color->ref_count) > 0)
        return;

    _REN_RES_BACK_DATA_LIST_CLEAR (Color, color,
        color, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

    g_free (color->data);
    g_free (color);
}

void*
ren_color_begin_edit (RenColor *color)
{
    return color->data;
}

void
ren_color_end_edit (RenColor *color)
{
    _REN_BACK_DATA_SIMPLE_CHANGED (Color, color, color);
}

void
ren_color_data (RenColor *color,
    const void **data_p, RenColorFormat *format_p, RenType *type_p)
{
    if (data_p)
        (*data_p) = color->data;
    if (format_p)
        (*format_p) = color->format;
    if (type_p)
        (*type_p) = color->type;
}

RenColorBackDataKey*
ren_color_back_data_key_new (ren_size data_size,
    RenColorBackDataInitFunc init,
    RenColorBackDataFiniFunc fini,
    RenColorBackDataUpdateFunc update)
{
    RenColorBackDataKey *key = g_new (RenColorBackDataKey, 1);

    key->ref_count = 1;
    key->destroy_notify = NULL;

    key->bd_list = NULL;

    key->data_size = data_size;
    key->user_data = NULL;
    key->init = init;
    key->fini = fini;
    key->update = update;

    return key;
}

void
ren_color_back_data_key_user_data (RenColorBackDataKey *key,
    void *user_data)
{
    key->user_data = user_data;
}

void
ren_color_back_data_key_destroy_notify (RenColorBackDataKey *key,
    RenColorBackDataKeyDestroyNotifyFunc destroy_notify)
{
    key->destroy_notify = destroy_notify;
}

RenColorBackDataKey*
ren_color_back_data_key_ref (RenColorBackDataKey *key)
{
    ++(key->ref_count);
    return key;
}

void
ren_color_back_data_key_unref (RenColorBackDataKey *key)
{
    if (--(key->ref_count) > 0)
        return;

    _REN_KEY_BACK_DATA_LIST_CLEAR (Color, color,
        key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);
    if (key->destroy_notify != NULL)
        key->destroy_notify (key, key->user_data);

    g_free (key);
}

RenColorBackData*
ren_color_back_data (RenColor *color, RenColorBackDataKey *key)
{
    _REN_BACK_DATA_RETURN (Color, color,
        color, key,
        _REN_BACK_DATA_SIMPLE_INIT_FUNC,
        _REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}

ren_size
ren_color_format_sizeof (RenColorFormat format, RenType type)
{
    switch (format)
    {
        case REN_COLOR_FORMAT_RGB:      return 3 * ren_type_sizeof (type);
        case REN_COLOR_FORMAT_RGBA:     return 4 * ren_type_sizeof (type);
        case REN_COLOR_FORMAT_DEPTH:    return 1 * ren_type_sizeof (type);
        default: return 0;
    }
}
