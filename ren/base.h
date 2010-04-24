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

#ifndef REN_BASE_H
#define REN_BASE_H

#include <ren/types.h>

extern ren_bool
ren_library_init (RenError **error);

extern void
ren_library_exit (void);

extern ren_bool
ren_library_is_inited (void);

extern RenReindeer*
ren_reindeer_new (RenBackend *backend, RenError **error);

extern RenReindeer*
ren_reindeer_ref (RenReindeer *r);

extern void
ren_reindeer_unref (RenReindeer *r);

extern RenBackend*
ren_reindeer_backend (RenReindeer *r);

extern RenBackend*
ren_backend_lookup (const char *name, RenError **error);

extern ren_bool
ren_backend_use (RenBackend *backend, RenError **error);

extern void
ren_backend_unuse (RenBackend *backend);

static inline ren_size
ren_type_sizeof (RenType type)
{
	static const ren_size type_sizes[] =
	{
		0,			/* REN_TYPE_NONE */
		sizeof (ren_bool),	/* REN_TYPE_BOOL */
		sizeof (ren_sint08),	/* REN_TYPE_SINT08 */
		sizeof (ren_uint08),	/* REN_TYPE_UINT08 */
		sizeof (ren_sint16),	/* REN_TYPE_SINT16 */
		sizeof (ren_uint16),	/* REN_TYPE_UINT16 */
		sizeof (ren_sint32),	/* REN_TYPE_SINT32 */
		sizeof (ren_uint32),	/* REN_TYPE_UINT32 */
		sizeof (ren_sint64),	/* REN_TYPE_SINT64 */
		sizeof (ren_uint64),	/* REN_TYPE_UINT64 */
		sizeof (ren_sfloat),	/* REN_TYPE_SFLOAT */
		sizeof (ren_dfloat),	/* REN_TYPE_DFLOAT */
	};
	if (type < (sizeof (type_sizes)/sizeof (type_sizes[0])))
		return type_sizes[type];
	else
		return 0;
}

extern void
ren_error_free (RenError *error);

extern ren_size
ren_color_format_sizeof (RenColorFormat format, RenType type);

extern ren_size
ren_primitive_vertex_count (RenPrimitive prim, ren_size count);

#endif /* REN_BASE_H */
