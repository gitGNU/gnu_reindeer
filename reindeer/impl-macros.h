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

#ifndef _REN_FAST_IMPL_H
#define _REN_FAST_IMPL_H

#define _REN_IMPL(F)\
    _REN_RET(F)\
    _REN_SYM(F) _REN_PRM(F)\
    { _REN_IMPL_CODE(F) }
#define _REN_IMPL_T(F, T)\
    _REN_RET_T(F, T)\
    _REN_SYM_T(F, T) _REN_PRM_T(F, T)\
    { _REN_IMPL_CODE_T(F, T) }
#define _REN_IMPL_TN(F, T, N)\
    _REN_RET_TN(F, T, N)\
    _REN_SYM_TN(F, T, N) _REN_PRM_TN(F, T, N)\
    { _REN_IMPL_CODE_TN(F, T, N) }

#define _REN_FUNC       _REN_IMPL
#define _REN_FUNC_T     _REN_IMPL_T
#define _REN_FUNC_TN    _REN_IMPL_TN

#endif /* _REN_FAST_IMPL_H */
