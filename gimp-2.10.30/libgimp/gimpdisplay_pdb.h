/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpdisplay_pdb.h
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

/* NOTE: This file is auto-generated by pdbgen.pl */

#if !defined (__GIMP_H_INSIDE__) && !defined (GIMP_COMPILATION)
#error "Only <libgimp/gimp.h> can be included directly."
#endif

#ifndef __GIMP_DISPLAY_PDB_H__
#define __GIMP_DISPLAY_PDB_H__

G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


gboolean gimp_display_is_valid          (gint32 display_ID);
gint32   gimp_display_new               (gint32 image_ID);
gboolean gimp_display_delete            (gint32 display_ID);
gint     gimp_display_get_window_handle (gint32 display_ID);
gboolean gimp_displays_flush            (void);
gboolean gimp_displays_reconnect        (gint32 old_image_ID,
                                         gint32 new_image_ID);


G_END_DECLS

#endif /* __GIMP_DISPLAY_PDB_H__ */
