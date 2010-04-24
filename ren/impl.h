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

#ifndef REN_IMPL_H
#define REN_IMPL_H

#include <ren/tmpl.h>
#include <ren/types.h>

#ifdef REN_IMPL_NAME
	#define _REN_IMPL_MANGLE3(NAME, F) ren__##NAME##__##F
	#define _REN_IMPL_MANGLE2(NAME, F) _REN_IMPL_MANGLE3(NAME, F)
	#define REN_IMPL_MANGLE(F) _REN_IMPL_MANGLE2(REN_IMPL_NAME, F)
	#define _REN_IMPL_PRIV_MANGLE3(NAME, F) _ren__##NAME##__##F
	#define _REN_IMPL_PRIV_MANGLE2(NAME, F) _REN_IMPL_PRIV_MANGLE3(NAME, F)
	#define REN_IMPL_PRIV_MANGLE(F) _REN_IMPL_PRIV_MANGLE2(REN_IMPL_NAME, F)
	/* I hate C preprocessor.  */
#endif

typedef ren_bool (* RenBackendInitFunc) (RenBackend *backend);
typedef ren_bool (* RenBackendFiniFunc) (void);

typedef struct _RenReindeerBackData RenReindeerBackData;
typedef void (* RenReindeerInitFunc) (RenReindeer *r, RenReindeerBackData *back_data);
typedef void (* RenReindeerFiniFunc) (RenReindeer *r, RenReindeerBackData *back_data);

extern RenReindeerBackData*
ren_reindeer_back_data (RenReindeer *r);

#endif /* REN_IMPL_H */
