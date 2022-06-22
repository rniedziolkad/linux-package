/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __GIMP_PLUG_IN_ERROR_H__
#define __GIMP_PLUG_IN_ERROR_H__


typedef enum
{
  GIMP_PLUG_IN_FAILED,  /* generic error condition */
  GIMP_PLUG_IN_EXECUTION_FAILED,
  GIMP_PLUG_IN_NOT_FOUND
} GimpPlugInErrorCode;


#define GIMP_PLUG_IN_ERROR (gimp_plug_in_error_quark ())

GQuark  gimp_plug_in_error_quark (void) G_GNUC_CONST;


#endif /* __GIMP_PLUG_IN_ERROR_H__ */
