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

#ifndef _REN_REINDEER_H
#define _REN_REINDEER_H

#include <ren/types.h>
#include <ren/impl.h>
#include <ltdl.h>

typedef struct _RenBackendData _RenBackendData;
struct _RenBackendData
{
    struct
    {
        #define _REN_FUNC(F)\
            _REN_FTP(F) _REN_FNM(F);
        #include <ren/funcs.h>
        #undef _REN_FUNC
    } ren;

    struct
    {
        void (* change_material) (RenReindeer *r,
            RenMaterial *front, RenMaterial *back, void *user_data);
    } object;

    RenReindeerInitFunc reindeer_init;
    RenReindeerFiniFunc reindeer_fini;
    ren_size reindeer_back_data_size;
};

struct _RenReindeer
{
    ren_uint32 ref_count;
    RenBackend *backend;
    _RenBackendData backend_data;
    RenReindeerBackData *back_data;
};

struct _RenBackend
{
    ren_uint32 ref_count;
    char *name;
    char *version;
    lt_dlhandle libhandle;

    _RenBackendData *data;
};

#define REN_ERROR ren_error_quark_ ()
GQuark
ren_error_quark_ (void);

#endif /* _REN_REINDEER_H */
