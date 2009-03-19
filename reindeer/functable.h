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

#ifndef _REN_FUNCTABLE_H
#define _REN_FUNCTABLE_H

#include <ren/funcdefs.h>

struct _RenFuncTable
{
    #define _REN_FUNC(F)\
        _REN_FTP(F) *_REN_FNM(F);
    #define _REN_FUNC_T(F, T)\
        _REN_FTP_T(F, T) *_REN_FNM_T(F, T);
    #define _REN_FUNC_TN(F, T, N)\
        _REN_FTP_TN(F, T, N) *_REN_FNM_TN(F, T, N);
    #include <ren/funcs.h>
};

#endif /* _REN_FUNCTABLE_H */
