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

#ifndef REN_TYPES_H
#define REN_TYPES_H

/* Datatypes */

typedef struct _RenReindeer     RenReindeer;
typedef struct _RenBackend      RenBackend;

typedef unsigned char           ren_bool;
typedef signed char             ren_sbyte;
typedef unsigned char           ren_ubyte;
typedef signed short            ren_sshort;
typedef unsigned short          ren_ushort;
typedef signed int              ren_sint;
typedef unsigned int            ren_uint;
typedef signed long long        ren_slong; /* 64 bit or more. */
typedef unsigned long long      ren_ulong; /* 64 bit or more. */
typedef float                   ren_hfloat; /* 16 bit or more. */
typedef float                   ren_sfloat; /* 32 bit or more. */
typedef double                  ren_dfloat; /* 64 bit or more. */
typedef signed short            ren_hfixed; /* 08.08 bit or more. */
typedef signed int              ren_sfixed; /* 16.16 bit or more. */
typedef signed long long        ren_dfixed; /* 32.32 bit or more. */

typedef signed int              ren_size;
typedef signed int              ren_offset;

typedef enum
{
    REN_TYPE_NONE,
    REN_TYPE_BOOL,
    REN_TYPE_SBYTE,
    REN_TYPE_UBYTE,
    REN_TYPE_SSHORT,
    REN_TYPE_USHORT,
    REN_TYPE_SINT,
    REN_TYPE_UINT,
    REN_TYPE_SLONG,
    REN_TYPE_ULONG,
    REN_TYPE_HFLOAT,
    REN_TYPE_SFLOAT,
    REN_TYPE_DFLOAT,
    REN_TYPE_HFIXED,
    REN_TYPE_SFIXED,
    REN_TYPE_DFIXED,
} RenType;

#if defined(REN_DFLOAT_AS_REAL)
typedef ren_dfloat ren_real;
#define REN_TYPE_REAL REN_TYPE_DFLOAT
#elif defined(REN_SFLOAT_AS_REAL)
typedef ren_sfloat ren_real;
#define REN_TYPE_REAL REN_TYPE_SFLOAT
#endif

#define REN_FALSE   0x0
#define REN_TRUE    0x1

typedef enum
{
    REN_LIMIT_MAX_LIGHTS,
} RenLimit;

typedef enum
{
    REN_BUFFER_COLOR        = 0x01,
    REN_BUFFER_DEPTH        = 0x02,
    REN_BUFFER_STENCIL      = 0x04,
    REN_BUFFER_ACCUM        = 0x08,
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
    REN_MATRIX_MODE_MODELVIEW     = 0x01,
    REN_MATRIX_MODE_PROJECTION    = 0x02,
    REN_MATRIX_MODE_COLOR         = 0x04,
    REN_MATRIX_MODE_TEXTURE       = 0x08,
} RenMatrixMode;

typedef struct _RenDataBlock RenDataBlock;

typedef void (*RenDataBlockCallback) (RenDataBlock *datablock, void *user_data);

typedef enum
{
    REN_USAGE_DONT_CARE     = 0x0000,
    REN_USAGE_STREAM        = 0x0001,
    REN_USAGE_STATIC        = 0x0002,
    REN_USAGE_DYNAMIC       = 0x0003,
    /*
    REN_USAGE_CLIENT        = 0x0000,
    REN_USAGE_SERVER        = 0x0000,
    */
} RenUsage;


typedef struct _RenVertexArray RenVertexArray;

#if 0
typedef enum
{
    REN_VERTEX_ARRAY_TYPE_COORD     = 0x0001,
    REN_VERTEX_ARRAY_TYPE_COLOR     = 0x0002,
    REN_VERTEX_ARRAY_TYPE_INDEX     = 0x0004,
    REN_VERTEX_ARRAY_TYPE_NORMAL    = 0x0008,
    REN_VERTEX_ARRAY_TYPE_FOGCOORD  = 0x0010,
    REN_VERTEX_ARRAY_TYPE_TEXCOORD  = 0x0020,
    REN_VERTEX_ARRAY_TYPE_EDGEFLAG  = 0x0040,
    REN_VERTEX_ARRAY_TYPE_ATTRIB    = 0x8000,
    REN_VERTEX_ARRAY_TYPE_ALL       = 0xFFFF,
} RenVertexArrayType;
#endif

typedef struct _RenCoordArray RenCoordArray;
typedef struct _RenColorArray RenColorArray;
typedef struct _RenNormalArray RenNormalArray;

typedef struct _RenIndexArray RenIndexArray;

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

typedef struct _RenLight RenLight;

typedef enum
{
    REN_LIGHT_TYPE_POINT_LIGHT,
    REN_LIGHT_TYPE_DIRECTIONAL,
    REN_LIGHT_TYPE_SPOT_LIGHT,
} RenLightType;

#endif /* REN_TYPES_H */
