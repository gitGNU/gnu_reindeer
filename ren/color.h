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

#ifndef REN_COLOR_H
#define REN_COLOR_H

#include <ren/types.h>

/* Public */

extern RenColor*
ren_color_new (RenColorFormat format, RenType type);

extern RenColor*
ren_color_ref (RenColor *color);

extern void
ren_color_unref (RenColor *color);

extern void*
ren_color_begin_edit (RenColor *color);

extern void
ren_color_end_edit (RenColor *color);

/* Backend */

extern void
ren_color_data (RenColor *color,
    const void **data_p, RenColorFormat *format_p, RenType *type_p);

typedef struct _RenColorBackData RenColorBackData;
typedef struct _RenColorBackDataKey RenColorBackDataKey;
typedef void (* RenColorBackDataInitFunc) (RenColor *color,
    RenColorBackData *back_data, void* user_data);
typedef void (* RenColorBackDataFiniFunc) (RenColor *color,
    RenColorBackData *back_data, void* user_data);
typedef void (* RenColorBackDataUpdateFunc) (RenColor *color,
    RenColorBackData *back_data, void* user_data);

extern RenColorBackDataKey*
ren_color_back_data_key_new (ren_size data_size,
    RenColorBackDataInitFunc init,
    RenColorBackDataFiniFunc fini,
    RenColorBackDataUpdateFunc update);

extern void
ren_color_back_data_key_user_data (RenColorBackDataKey *key,
    void *user_data);

extern RenColorBackDataKey*
ren_color_back_data_key_ref (RenColorBackDataKey *key);

extern void
ren_color_back_data_key_unref (RenColorBackDataKey *key);

extern RenColorBackData*
ren_color_back_data (RenColor *color, RenColorBackDataKey *key);

#endif /* REN_COLOR_H */
