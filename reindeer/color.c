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

struct _RenColor
{
    ren_uint32 ref_count;

    const void *data;
    RenColorFormat format;
    RenType type;

    GList *context_updater_list;
    GList *backend_updater_list;
};

typedef struct _RenColorContextUpdater _RenColorContextUpdater;
struct _RenColorContextUpdater
{
    _RenContextUpdater base;
    RenColorContextData *context_data;
    ren_bool changed;
};

typedef struct _RenColorBackendUpdater _RenColorBackendUpdater;
struct _RenColorBackendUpdater
{
    _RenBackendUpdater base;
    RenColorBackendData *backend_data;
    ren_bool changed;
};

static void
remove_context_updater (RenColor *color, GList *updater_item);
static void
remove_backend_updater (RenColor *color, GList *updater_item);

RenColor*
ren_color_new (const void *data, RenColorFormat format, RenType type)
{
    RenColor *color = g_new (RenColor, 1);

    color->ref_count = 1;

    color->data = data;
    color->format = format;
    color->type = type;
    /* TODO: Check if format and type are valid. */

    color->context_updater_list = NULL;
    color->backend_updater_list = NULL;

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

    CLEAR_UPDATER_LISTS (Color,color,color);

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

RenColorContextData*
ren_color_context_data (RenColor *color, RenReindeer *r)
{
    RETURN_CONTEXT_DATA(Color,color,color,
        context_updater->changed = TRUE;
        ,
        if (context_updater->changed)
        {
            context_data_update (color, context_data);
            context_updater->changed = FALSE;
        }
    );
}

RenColorBackendData*
ren_color_backend_data (RenColor *color, RenReindeer *r)
{
    RETURN_BACKEND_DATA(Color,color,color,
        backend_updater->changed = TRUE;
        ,
        if (backend_updater->changed)
        {
            backend_data_update (color, backend_data);
            backend_updater->changed = FALSE;
        }
    );
}

static void
remove_context_updater (RenColor *color, GList *updater_item)
{
    REMOVE_CONTEXT_UPDATER(Color,color,color,{});
}

static void
remove_backend_updater (RenColor *color, GList *updater_item)
{
    REMOVE_BACKEND_UPDATER(Color,color,color,{});
}
