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

#include <ren/ren.h>
#include <ren/impl.h>
#include <glib.h>

#include "impl-macros.h"

struct _RenLight
{
    ren_uint32 ref_count;

    RenLightType type;

    _RenValue ambient;
    _RenValue diffuse;
    _RenValue specular;

    _RenValue attenuation;
    _RenValue cutoff;
    _RenValue exponent;
};

RenLight*
ren_light_new (RenLightType type)
{
    RenLight *light = g_new0 (RenLight, 1);

    light->ref_count = 1;

    light->type = type;

    return light;
}

void
ren_light_destroy (RenLight *light)
{
    _ren_light_unref (light);
}

void
_ren_light_ref (RenLight *light)
{
    ++(light->ref_count);
}

void
_ren_light_unref (RenLight *light)
{
    if (--(light->ref_count) > 0)
        return;

    g_free (light);
}

void
_ren_light_data (RenLight *light,
    RenLightType *typep,
    const _RenValue **ambientp,
    const _RenValue **diffusep,
    const _RenValue **specularp)
{
    if (typep)
        (*typep) = light->type;
    if (ambientp)
        (*ambientp) = &(light->ambient);
    if (diffusep)
        (*diffusep) = &(light->diffuse);
    if (specularp)
        (*specularp) = &(light->specular);
}

void
_ren_light_data_point_light (RenLight *light,
    const _RenValue **attenuationp)
{
    if (light->type != REN_LIGHT_TYPE_POINT_LIGHT)
        return;
    if (attenuationp)
        (*attenuationp) = &(light->attenuation);
}

void
_ren_light_data_spot_light (RenLight *light,
    const _RenValue **attenuationp,
    const _RenValue **cutoffp,
    const _RenValue **exponentp)
{
    if (light->type != REN_LIGHT_TYPE_SPOT_LIGHT)
        return;
    if (attenuationp)
        (*attenuationp) = &(light->attenuation);
    if (cutoffp)
        (*cutoffp) = &(light->cutoff);
    if (exponentp)
        (*exponentp) = &(light->exponent);
}

#define _REN_IMPL_CODE_T(F, T)\
    light->ambient.type = _REN_TYPEV(T);\
    light->ambient.num = components;\
    light->ambient.value = color;
_REN_FUNC_CLAMP (light_ambient)
#undef _REN_IMPL_CODE_T

#define _REN_IMPL_CODE_T(F, T)\
    light->diffuse.type = _REN_TYPEV(T);\
    light->diffuse.num = components;\
    light->diffuse.value = color;
_REN_FUNC_CLAMP (light_diffuse)
#undef _REN_IMPL_CODE_T

#define _REN_IMPL_CODE_T(F, T)\
    light->specular.type = _REN_TYPEV(T);\
    light->specular.num = components;\
    light->specular.value = color;
_REN_FUNC_CLAMP (light_specular)
#undef _REN_IMPL_CODE_T

#define _REN_IMPL_CODE_T(F, T)\
    light->attenuation.type = _REN_TYPEV(T);\
    light->attenuation.num = degree + 1;\
    light->attenuation.value = k;
_REN_FUNC_T (light_attenuation,sf)
_REN_FUNC_T (light_attenuation,df)
#undef _REN_IMPL_CODE_T
