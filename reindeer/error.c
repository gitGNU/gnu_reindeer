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
#include <ren/util.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static _RenSList *error_list = NULL;
static _RenSList *error_list_prev = NULL;
static _RenSList *error_list_last = NULL;

void
_ren_throw_error (const char *format, ...)
{
    va_list ap;

    _RenSList *newerror = calloc (1, sizeof (_RenSList));

    va_start (ap, format);
    vasprintf (&newerror->data, format, ap);
    va_end (ap);

    /* Just for debugging for now.  */
    if (getenv ("DEBUG"))
        fprintf (stderr, "Reindeer error: %s\n", newerror->data);

    if (error_list_last != NULL)
        error_list_last->next = newerror;
    error_list_last = newerror;
    if (error_list == NULL)
        error_list = newerror;
}

const char*
ren_error (void)
{
    if (error_list_prev)
    {
        free (error_list_prev->data);
        free (error_list_prev);
        error_list_prev = NULL;
    }
    if (error_list)
    {
        const char *ret = error_list->data;
        error_list_prev = error_list;
        error_list = error_list->next;
        return ret;
    }
    else
        return NULL;
}
