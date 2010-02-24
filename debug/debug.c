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

#ifndef REN_IMPL_NAME
#define REN_IMPL_NAME debug
#endif

#include <ren/types.h>
#include <ren/impl.h>
#include <ren/tmpl.h>

struct _RenReindeerBackData
{
};

ren_size REN_IMPL_MANGLE(reindeer_back_data_size) = sizeof (RenReindeerBackData);

void
REN_IMPL_MANGLE(reindeer_init) (RenReindeer *r, RenReindeerBackData *back_data)
{
}

void
REN_IMPL_MANGLE(reindeer_fini) (RenReindeer *r, RenReindeerBackData *back_data)
{
}

#define _REN_FUNC(F)\
    _REN_RET(F) REN_IMPL_MANGLE(F) _REN_PRM(F) {\
        return (_REN_RET(F)) _REN_DRT(F);\
    }
#include <ren/funcs.h>
#undef _REN_FUNC
