/*
	This file is part of Reindeer.

	Copyright (C) 2010 - Patrik Olsson

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

[CCode (cprefix = "ren_", cheader_filename = "ren/ren.h")]
namespace Ren
{
	[CCode (cname = "ren_library_init")]
	public Ren.bool init (out Error? error);
	[CCode (cname = "ren_library_exit")]
	public void exit ();
	[CCode (cname = "ren_library_is_inited")]
	public Ren.bool is_inited ();

	[Compact]
	[CCode (cname = "RenReindeer", cprefix = "ren_",
		ref_function = "ren_reindeer_ref", unref_function = "ren_reindeer_unref")]
	public class Reindeer
	{
		[CCode (cname = "ren_reindeer_new")]
		public Reindeer (Ren.Backend backend, out Error? error);
		[CCode (cname = "ren_reindeer_backend")]
		public Backend get_backend ();

		public Ren.bool init ();
		public void flush ();
		public void finish ();
		public void clear (Ren.Buffer buffers);
		public void clear_color (Ren.Color color);
		public void clear_depth (Ren.dfloat depth);
		public void depth_test (Ren.Test test);
		public void transform_mode (Ren.TransformMode mode);
		public void transform_set (Ren.Matrix matrix);
		public void transform_mul (Ren.Matrix matrix);
		public void transform_identity ();
		public void transform_push ();
		public void transform_pop ();
		public void viewport (Ren.uint32 x, Ren.uint32 y, Ren.uint32 width, Ren.uint32 height);
		public Ren.bool coord_array_bind (Ren.VectorArray array);
		public Ren.bool color_array_bind (Ren.ColorArray array);
		public Ren.bool normal_array_bind (Ren.VectorArray array);
		public Ren.bool index_array_bind (Ren.IndexArray ixarray);
		public void state_keep (Ren.State state);
		public void primitive (Ren.Primitive prim, Ren.size offset, Ren.size count);
		public void object_render (Ren.Object object);
		public void lighting_enable ();
		public void lighting_disable ();
		public void lighting_ambient (Ren.Color color);
		public void light_bind ([CCode (array_length_pos = 0.9)] Ren.Light[] lights);
		public Ren.bool light_enable (Ren.Light light);
		public void light_disable (Ren.Light light);
		public void material_bind (Ren.Material front, Ren.Material back);
		public void material_front (Ren.Material material);
		public void material_back (Ren.Material material);
	}

	[CCode (cname = "RenBackend*", cprefix = "ren_backend_")]
	[SimpleType]
	public struct Backend
	{
		public static Ren.Backend lookup (string name, out Error? error);

		public Ren.bool use (out Error? error);
		public void unuse ();

		[CCode (cname = "_vala_ren_backend_is_valid")]
		public inline Ren.bool is_valid ()
		{
			return (Ren.bool)((void*) this != null);
		}
	}

	[SimpleType]
	[CCode (cname = "ren_bool")]
	[BooleanType]
	public struct bool : global::bool {}

	[SimpleType]
	[CCode (cname = "ren_sint08")]
	[IntegerType (rank = 1)]
	public struct sint08 : global::int8 {}

	[SimpleType]
	[CCode (cname = "ren_uint08")]
	[IntegerType (rank = 3)]
	public struct uint08 : global::uint8 {}

	[SimpleType]
	[CCode (cname = "ren_sint16")]
	[IntegerType (rank = 4)]
	public struct sint16 : global::int16 {}

	[SimpleType]
	[CCode (cname = "ren_uint16")]
	[IntegerType (rank = 5)]
	public struct uint16 : global::uint16 {}

	[SimpleType]
	[CCode (cname = "ren_sint32")]
	[IntegerType (rank = 6)]
	public struct sint32 : global::int32 {}

	[SimpleType]
	[CCode (cname = "ren_uint32")]
	[IntegerType (rank = 7)]
	public struct uint32 : global::uint32 {}

	[SimpleType]
	[CCode (cname = "ren_sint64")]
	[IntegerType (rank = 10)]
	public struct sint64 : global::int64 {}

	[SimpleType]
	[CCode (cname = "ren_uint64")]
	[IntegerType (rank = 11)]
	public struct uint64 : global::uint64 {}

	[CCode (cname = "ren_sfloat")]
	[SimpleType]
	[FloatingType (rank = 1)]
	public struct sfloat : global::float {}

	[CCode (cname = "ren_dfloat")]
	[SimpleType]
	[FloatingType (rank = 2)]
	public struct dfloat : global::double {}

	[SimpleType]
	[CCode (cname = "ren_size")]
	[IntegerType (rank = 9)]
	public struct size : global::size_t {}

	[CCode (cname = "RenType", cprefix = "REN_TYPE_")]
	public enum Type
	{
		NONE, BOOL, SINT08, UINT08, SINT16, UINT16, SINT32, UINT32,
		SINT64, UINT64, SFLOAT, DFLOAT
	}
	public Ren.size type_sizeof (Ren.Type type);

	[Compact]
	[CCode (cname = "RenError", free_function = "ren_error_free")]
	public class Error
	{
		public int code;
		public string message;

		[CCode (cname = "REN_ERROR_FAILED")]
		public static const int FAILED;
	}

	[CCode (cname = "RenBuffer", cprefix = "REN_BUFFER_")]
	public enum Buffer
	{
		COLOR, DEPTH, STENCIL, ACCUM
	}

	[CCode (cname = "RenTest", cprefix = "REN_TEST_")]
	public enum Test
	{
		NONE, ALWAYS, NEVER,
		EQUAL, NOT_EQUAL,
		LESS, LESS_EQUAL,
		GREATER, GREATER_EQUAL
	}

	[CCode (cname = "RenTransformMode", cprefix = "REN_TRANSFORM_MODE_")]
	public enum TransformMode
	{
		MODELVIEW, PROJECTION, COLOR, TEXTURE
	}

	[CCode (cname = "RenColorFormat", cprefix = "REN_COLOR_FORMAT_")]
	public enum ColorFormat
	{
		RGB, RGBA, DEPTH
	}
	public Ren.size color_format_sizeof (Ren.ColorFormat format, Ren.Type type);

	[CCode (cname = "RenUsage", cprefix = "REN_USAGE_")]
	public enum Usage
	{
		DONT_CARE, IMMUTABLE, STATIC, DYNAMIC, STREAM
	}

	[CCode (cname = "RenState", cprefix = "REN_STATE_")]
	public enum State
	{
		INDEX_ARRAY, COORD_ARRAY, COLOR_ARRAY, NORMAL_ARRAY,
		LIGHTS, MATERIALS
	}

	[CCode (cname = "RenPrimitive", cprefix = "REN_PRIMITIVE_")]
	public enum Primitive
	{
		POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, TRIANGLE_FAN,
		TRIANGLE_STRIP, QUADS, QUAD_STRIP, CIRCLES, ELLIPSES, POLYGON,
		SPHERES, CYLINDERS
	}
	public Ren.size primitive_vertex_count (Ren.Primitive prim, Ren.size count);

	[CCode (cname = "RenFace", cprefix = "REN_FACE_")]
	public enum Face
	{
		FRONT, BACK, BOTH
	}

	[CCode (cname = "RenLightType", cprefix = "REN_LIGHT_TYPE_")]
	public enum LightType
	{
		POINT_LIGHT, DIRECTIONAL, SPOT_LIGHT
	}

	[Compact]
	[CCode (cname = "RenMatrix", cprefix = "ren_matrix_",
		ref_function = "ren_matrix_ref", unref_function = "ren_matrix_unref")]
	public class Matrix
	{
		public Matrix (Ren.size width, Ren.size height,
			Ren.Type type, Ren.bool transposed);
		public void* begin_edit ();
		public void end_edit ();
	}

	[Compact]
	[CCode (cname = "RenVector", cprefix = "ren_vector_",
		ref_function = "ren_vector_ref", unref_function = "ren_vector_unref")]
	public class Vector
	{
		public Vector (Ren.size length, Ren.Type type);
		public void* begin_edit ();
		public void end_edit ();
	}

	[Compact]
	[CCode (cname = "RenColor", cprefix = "ren_color_",
		ref_function = "ren_color_ref", unref_function = "ren_color_unref")]
	public class Color
	{
		public Color (Ren.ColorFormat format, Ren.Type type);
		public void* begin_edit ();
		public void end_edit ();
	}

	[Compact]
	[CCode (cname = "RenDataBlock", cprefix = "ren_data_block_",
		ref_function = "ren_data_block_ref", unref_function = "ren_data_block_unref")]
	public class DataBlock
	{
		public DataBlock (Ren.size size, Ren.Usage usage);
		public void resize (Ren.size size);
		public void* begin_edit ();
		public void end_edit ();
		public void changed (Ren.size from, Ren.size length);
	}

	[Compact]
	[CCode (cname = "RenVectorArray", cprefix = "ren_vector_array_",
		ref_function = "ren_vector_array_ref", unref_function = "ren_vector_array_unref")]
	public class VectorArray
	{
		public VectorArray (Ren.Type type, Ren.uint08 num,
			Ren.DataBlock datablock, Ren.size start, Ren.size count, Ren.size stride);
		public void set_size (Ren.size count);
	}

	[Compact]
	[CCode (cname = "RenColorArray", cprefix = "ren_color_array_",
		ref_function = "ren_color_array_ref", unref_function = "ren_color_array_unref")]
	public class ColorArray
	{
		public ColorArray (Ren.ColorFormat format, Ren.Type type,
			Ren.DataBlock datablock, Ren.size start, Ren.size count, Ren.size stride);
		public void set_size (Ren.size count);
	}

	[Compact]
	[CCode (cname = "RenIndexArray", cprefix = "ren_index_array_",
		ref_function = "ren_index_array_ref", unref_function = "ren_index_array_unref")]
	public class IndexArray
	{
		public IndexArray (Ren.Type type, Ren.DataBlock datablock,
			Ren.size start, Ren.size count);
		public void set_size (Ren.size count);
	}

	[Compact]
	[CCode (cname = "RenMaterial", cprefix = "ren_material_",
		ref_function = "ren_material_ref", unref_function = "ren_material_unref")]
	public class Material
	{
		public Material ();
		public void ambient (Ren.Color color);
		public void diffuse (Ren.Color color);
		public void specular (Ren.Color color);
		public void emission (Ren.Color color);
		public void shininess (Ren.dfloat shininess);
	}

	[Compact]
	[CCode (cname = "RenLight", cprefix = "ren_light_",
		ref_function = "ren_light_ref", unref_function = "ren_light_unref")]
	public class Light
	{
		public Light (Ren.LightType type);
		public void ambient (Ren.Color color);
		public void diffuse (Ren.Color color);
		public void specular (Ren.Color color);
		public void attenuation (Ren.Vector k);
	}

	[Compact]
	[CCode (cname = "RenTemplate", cprefix = "ren_template_",
		ref_function = "ren_template_ref", unref_function = "ren_template_unref")]
	public class Template
	{
		public Template (Ren.IndexArray? ix_array);
		public void build ();
		public void debug ();
		public void primitive (Ren.Primitive prim,
			Ren.size offset, Ren.size count);
		public Ren.uint32 new_mode ();
		public void material (Ren.Face face, Ren.sint08 material);
	}

	[Compact]
	[CCode (cname = "RenObject", cprefix = "ren_object_",
		ref_function = "ren_object_ref", unref_function = "ren_object_unref")]
	public class Object
	{
		public Object (Ren.Template template, Ren.VectorArray coord_array);
		public void begin_edit ();
		public void end_edit ();
		public void coord_array (Ren.VectorArray coord_array);
		public void color_array (Ren.ColorArray color_array);
		public void normal_array (Ren.VectorArray normal_array);
		public void material (Ren.sint08 id, Ren.Material material);
	}
}
