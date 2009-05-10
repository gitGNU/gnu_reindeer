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

#ifndef _REN_REINDEER_H
#define _REN_REINDEER_H

#include <ren/types.h>
#include <ren/impl.h>
#include <ltdl.h>

#include "functable.h"

struct _RenReindeer
{
    RenBackend *backend;
    _RenBackendData *backend_data;
    struct _RenFuncTable ft;
};

struct _RenBackend
{
    ren_uint32 ref_count;
    char *name;
    char *version;
    lt_dlhandle libhandle;
    ren_size backend_data_size;
    _RenBackendDataDestroyFunc backend_data_destroy;
    struct _RenFuncTable *ft;
};

#endif /* _REN_REINDEER_H */
