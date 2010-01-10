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

#ifndef REN_TEMPLATE_H
#define REN_TEMPLATE_H

#include <ren/types.h>

/* Public */

extern RenTemplate*
ren_template_new (RenIndexArray *ix_array);

extern RenTemplate*
ren_template_ref (RenTemplate *tmplt);

extern void
ren_template_unref (RenTemplate *tmplt);

extern void
ren_template_build (RenTemplate *tmplt);

extern void
ren_template_debug (RenTemplate *tmplt);

extern void
ren_template_primitive (RenTemplate *tmplt,
    RenPrimitive prim, ren_size offset, ren_size count);

extern ren_uint32
ren_template_new_mode (RenTemplate *tmplt);

extern void
ren_template_material (RenTemplate *tmplt,
    RenFace face, ren_uint08 material);

/* Backend */

typedef struct _RenTemplatePrimitive RenTemplatePrimitive;
struct _RenTemplatePrimitive
{
    ren_uint32 mode;
    RenPrimitive prim;
    ren_size offset;
    ren_size count;
};

extern void
ren_template_data_primitives (RenTemplate *tmplt,
    RenIndexArray **ix_array_p,
    ren_size *num_primitives_p, const RenTemplatePrimitive **primitives_p);

#endif /* REN_TEMPLATE_H */
