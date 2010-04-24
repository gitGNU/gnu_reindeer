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

#ifndef REN_REN_H
#define REN_REN_H

#include <ren/tmpl.h>
#include <ren/types.h>
#include <ren/version.h>
#include <ren/base.h>
#include <ren/matrix.h>
#include <ren/vector.h>
#include <ren/color.h>
#include <ren/datablock.h>
#include <ren/vectorarray.h>
#include <ren/colorarray.h>
#include <ren/indexarray.h>
#include <ren/material.h>
#include <ren/light.h>
#include <ren/template.h>
#include <ren/object.h>

#define _REN_FUNC(F)\
	extern _REN_RET(F) _REN_SYM(F) _REN_PRM(F);
#include <ren/funcs.h>
#undef _REN_FUNC

#include <ren/impl.h>

#endif /* REN_REN_H */
