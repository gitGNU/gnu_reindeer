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

struct _RenMaterial
{
    ren_uint32 ref_count;

    RenColor *ambient;
    RenColor *diffuse;
    RenColor *specular;
    RenColor *emission;
    ren_dfloat shininess;
};

RenMaterial*
ren_material_new ()
{
    RenMaterial *material = g_new0 (RenMaterial, 1);

    material->ref_count = 1;

    return material;
}

void
ren_material_destroy (RenMaterial *material)
{
    ren_material_unref (material);
}

void
ren_material_ambient (RenMaterial *material, RenColor *color)
{
    if (material->ambient != NULL)
        ren_color_unref (material->ambient);
    material->ambient = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_diffuse (RenMaterial *material, RenColor *color)
{
    if (material->diffuse != NULL)
        ren_color_unref (material->diffuse);
    material->diffuse = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_specular (RenMaterial *material, RenColor *color)
{
    if (material->specular != NULL)
        ren_color_unref (material->specular);
    material->specular = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_emission (RenMaterial *material, RenColor *color)
{
    if (material->emission != NULL)
        ren_color_unref (material->emission);
    material->emission = (color != NULL) ? ren_color_ref (color) : NULL;
}

void
ren_material_shininess (RenMaterial *material, ren_dfloat shininess)
{
    material->shininess = shininess;
}

RenMaterial*
ren_material_ref (RenMaterial *material)
{
    ++(material->ref_count);
    return material;
}

void
ren_material_unref (RenMaterial *material)
{
    if (--(material->ref_count) > 0)
        return;

    if (material->ambient != NULL)
        ren_color_unref (material->ambient);
    if (material->diffuse != NULL)
        ren_color_unref (material->diffuse);
    if (material->specular != NULL)
        ren_color_unref (material->specular);
    if (material->emission != NULL)
        ren_color_unref (material->emission);
    g_free (material);
}

void
ren_material_data_light (RenMaterial *material,
    RenColor **ambient_p, RenColor **diffuse_p, RenColor **specular_p,
    RenColor **emission_p, ren_dfloat *shininess_p)
{
    if (ambient_p)
        (*ambient_p) = material->ambient;
    if (diffuse_p)
        (*diffuse_p) = material->diffuse;
    if (specular_p)
        (*specular_p) = material->specular;
    if (emission_p)
        (*emission_p) = material->emission;
    if (shininess_p)
        (*shininess_p) = material->shininess;
}
