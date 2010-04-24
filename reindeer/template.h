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

#ifndef _REN_REINDEER_TEMPLATE_H
#define _REN_REINDEER_TEMPLATE_H

struct _RenTemplate
{
	ren_uint32 ref_count;

	gboolean built;
	RenIndexArray *ix_array; /* If ix_array == NULL use direct index */
	ren_uint08 num_materials;

	union
	{
		struct /* When built */
		{
			ren_size num_primitives;
			union
			{
				struct _RenTemplatePrimitive *primitives;
				gpointer data;
			};
			ren_size num_modes;
			ren_uint08 **modes;
		};
		struct /* When !built */
		{
			GArray *primitives;
			GArray *modes;
			GArray *mode_data;
		} b;
	};
};

typedef enum
{
	REN_MODE_CMD_END = 0x0,
	REN_MODE_CMD_MATERIAL = 0x1,
} _RenModeCmd;

#endif /* _REN_REINDEER_TEMPLATE_H */
