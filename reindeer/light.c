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

struct _RenLight
{
    ren_uint32 ref_count;

    RenLightType type;

    RenColor *ambient;
    RenColor *diffuse;
    RenColor *specular;

    RenVector *attenuation;
    ren_dfloat cutoff;
    ren_dfloat exponent;
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
    ren_light_unref (light);
}

void
ren_light_ambient (RenLight *light, RenColor *color)
{
    light->ambient = color;
}

void
ren_light_diffuse (RenLight *light, RenColor *color)
{
    light->diffuse = color;
}

void
ren_light_specular (RenLight *light, RenColor *color)
{
    light->specular = color;
}

void
ren_light_attenuation (RenLight *light, RenVector *k)
{
    light->attenuation = k;
}

void
ren_light_ref (RenLight *light)
{
    ++(light->ref_count);
}

void
ren_light_unref (RenLight *light)
{
    if (--(light->ref_count) > 0)
        return;

    g_free (light);
}

void
ren_light_data (RenLight *light,
    RenLightType *typep,
    RenColor **ambientp,
    RenColor **diffusep,
    RenColor **specularp)
{
    if (typep)
        (*typep) = light->type;
    if (ambientp)
        (*ambientp) = light->ambient;
    if (diffusep)
        (*diffusep) = light->diffuse;
    if (specularp)
        (*specularp) = light->specular;
}

void
ren_light_data_point_light (RenLight *light,
    RenVector **attenuationp)
{
    if (light->type != REN_LIGHT_TYPE_POINT_LIGHT)
        return;
    if (attenuationp)
        (*attenuationp) = light->attenuation;
}

void
ren_light_data_spot_light (RenLight *light,
    RenVector **attenuationp,
    ren_dfloat *cutoffp,
    ren_dfloat *exponentp)
{
    if (light->type != REN_LIGHT_TYPE_SPOT_LIGHT)
        return;
    if (attenuationp)
        (*attenuationp) = light->attenuation;
    if (cutoffp)
        (*cutoffp) = light->cutoff;
    if (exponentp)
        (*exponentp) = light->exponent;
}
