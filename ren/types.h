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

#ifndef REN_TYPES_H
#define REN_TYPES_H

#include <ren/tmpl.h>
#include <glib.h>

/* Datatypes */

typedef struct _RenReindeer     RenReindeer;
typedef struct _RenBackend      RenBackend;

typedef gboolean    ren_bool;
typedef gint8       ren_sint08;
typedef guint8      ren_uint08;
typedef gint16      ren_sint16;
typedef guint16     ren_uint16;
typedef gint32      ren_sint32;
typedef guint32     ren_uint32;
typedef gint64      ren_sint64;
typedef guint64     ren_uint64;
typedef gfloat      ren_sfloat;
typedef gdouble     ren_dfloat;
typedef gsize       ren_size;

typedef struct _RenMatrix RenMatrix;
typedef struct _RenVector RenVector;
typedef struct _RenColor RenColor;
typedef struct _RenDataBlock RenDataBlock;
typedef struct _RenVectorArray RenVectorArray;
typedef struct _RenColorArray RenColorArray;
typedef struct _RenIndexArray RenIndexArray;
typedef struct _RenMaterial RenMaterial;
typedef struct _RenLight RenLight;
typedef struct _RenTemplate RenTemplate;
typedef struct _RenObject RenObject;

typedef enum
{
    REN_TYPE_NONE,
    REN_TYPE_BOOL,
    REN_TYPE_SINT08,
    REN_TYPE_UINT08,
    REN_TYPE_SINT16,
    REN_TYPE_UINT16,
    REN_TYPE_SINT32,
    REN_TYPE_UINT32,
    REN_TYPE_SINT64,
    REN_TYPE_UINT64,
    REN_TYPE_SFLOAT,
    REN_TYPE_DFLOAT,
} RenType;

typedef GError RenError;

enum RenErrorCode
{
    REN_ERROR_FAILED,
};

typedef enum
{
    REN_BUFFER_COLOR        = (1 << 0),
    REN_BUFFER_DEPTH        = (1 << 1),
    REN_BUFFER_STENCIL      = (1 << 2),
    REN_BUFFER_ACCUM        = (1 << 3),
} RenBuffer;

typedef enum
{
    REN_TEST_NONE,
    REN_TEST_ALWAYS,
    REN_TEST_NEVER,
    REN_TEST_EQUAL,
    REN_TEST_NOT_EQUAL,
    REN_TEST_LESS,
    REN_TEST_LESS_EQUAL,
    REN_TEST_GREATER,
    REN_TEST_GREATER_EQUAL,
} RenTest;

typedef enum
{
    REN_TRANSFORM_MODE_MODELVIEW,
    REN_TRANSFORM_MODE_PROJECTION,
    REN_TRANSFORM_MODE_COLOR,
    REN_TRANSFORM_MODE_TEXTURE,
} RenTransformMode;

typedef enum
{
    REN_COLOR_FORMAT_RGB,
    REN_COLOR_FORMAT_RGBA,
    REN_COLOR_FORMAT_DEPTH,
} RenColorFormat;

typedef enum
{
    REN_USAGE_DONT_CARE,
    REN_USAGE_IMMUTABLE,
    REN_USAGE_STATIC,
    REN_USAGE_DYNAMIC,
    REN_USAGE_STREAM,
} RenUsage;

typedef enum
{
    REN_PRIMITIVE_POINTS,
    REN_PRIMITIVE_LINES,
    REN_PRIMITIVE_LINE_STRIP,
    REN_PRIMITIVE_LINE_LOOP,
    REN_PRIMITIVE_TRIANGLES,
    REN_PRIMITIVE_TRIANGLE_FAN,
    REN_PRIMITIVE_TRIANGLE_STRIP,
    REN_PRIMITIVE_QUADS,
    REN_PRIMITIVE_QUAD_STRIP,
    REN_PRIMITIVE_CIRCLES,
    REN_PRIMITIVE_ELLIPSES,
    REN_PRIMITIVE_POLYGON,
    REN_PRIMITIVE_SPHERES,
    REN_PRIMITIVE_CYLINDERS,
} RenPrimitive;

typedef enum
{
    REN_FACE_FRONT  = (1 << 0),
    REN_FACE_BACK   = (1 << 1),
    REN_FACE_BOTH   = REN_FACE_FRONT | REN_FACE_BACK,
} RenFace;

typedef enum
{
    REN_LIGHT_TYPE_POINT_LIGHT,
    REN_LIGHT_TYPE_DIRECTIONAL,
    REN_LIGHT_TYPE_SPOT_LIGHT,
} RenLightType;

#define _REN_FUNC(F)\
    typedef _REN_RET(F) (* _REN_FTP(F)) _REN_PRM(F);
#include <ren/funcs.h>
#undef _REN_FUNC

#endif /* REN_TYPES_H */
