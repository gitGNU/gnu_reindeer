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
    if (light->ambient != NULL)
        ren_color_unref (light->ambient);
    light->ambient = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_light_diffuse (RenLight *light, RenColor *color)
{
    if (light->diffuse != NULL)
        ren_color_unref (light->diffuse);
    light->diffuse = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_light_specular (RenLight *light, RenColor *color)
{
    if (light->specular != NULL)
        ren_color_unref (light->specular);
    light->specular = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_light_attenuation (RenLight *light, RenVector *k)
{
    if (light->attenuation != NULL)
        ren_vector_unref (light->attenuation);
    light->attenuation = (k != NULL) ? ren_vector_ref (k) : NULL;
}

RenLight*
ren_light_ref (RenLight *light)
{
    ++(light->ref_count);
    return light;
}

void
ren_light_unref (RenLight *light)
{
    if (--(light->ref_count) > 0)
        return;

    if (light->ambient != NULL)
        ren_color_unref (light->ambient);
    if (light->diffuse != NULL)
        ren_color_unref (light->diffuse);
    if (light->specular != NULL)
        ren_color_unref (light->specular);
    if (light->attenuation != NULL)
        ren_vector_unref (light->attenuation);
    g_free (light);
}

void
ren_light_data (RenLight *light,
    RenLightType *type_p,
    RenColor **ambient_p,
    RenColor **diffuse_p,
    RenColor **specular_p)
{
    if (type_p)
        (*type_p) = light->type;
    if (ambient_p)
        (*ambient_p) = light->ambient;
    if (diffuse_p)
        (*diffuse_p) = light->diffuse;
    if (specular_p)
        (*specular_p) = light->specular;
}

void
ren_light_data_point_light (RenLight *light,
    RenVector **attenuation_p)
{
    if (light->type != REN_LIGHT_TYPE_POINT_LIGHT)
        return;
    if (attenuation_p)
        (*attenuation_p) = light->attenuation;
}

void
ren_light_data_spot_light (RenLight *light,
    RenVector **attenuation_p,
    ren_dfloat *cutoff_p,
    ren_dfloat *exponent_p)
{
    if (light->type != REN_LIGHT_TYPE_SPOT_LIGHT)
        return;
    if (attenuation_p)
        (*attenuation_p) = light->attenuation;
    if (cutoff_p)
        (*cutoff_p) = light->cutoff;
    if (exponent_p)
        (*exponent_p) = light->exponent;
}
