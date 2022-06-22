/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimppaletteselect_pdb.c
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

#include "config.h"

#include "gimp.h"


/**
 * SECTION: gimppaletteselect
 * @title: gimppaletteselect
 * @short_description: Functions providing a palette selection dialog.
 *
 * Functions providing a palette selection dialog.
 **/


/**
 * gimp_palettes_popup:
 * @palette_callback: The callback PDB proc to call when palette selection is made.
 * @popup_title: Title of the palette selection dialog.
 * @initial_palette: The name of the palette to set as the first selected.
 *
 * Invokes the Gimp palette selection.
 *
 * This procedure opens the palette selection dialog.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_palettes_popup (const gchar *palette_callback,
                     const gchar *popup_title,
                     const gchar *initial_palette)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-palettes-popup",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, palette_callback,
                                    GIMP_PDB_STRING, popup_title,
                                    GIMP_PDB_STRING, initial_palette,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_palettes_close_popup:
 * @palette_callback: The name of the callback registered for this pop-up.
 *
 * Close the palette selection dialog.
 *
 * This procedure closes an opened palette selection dialog.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_palettes_close_popup (const gchar *palette_callback)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-palettes-close-popup",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, palette_callback,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_palettes_set_popup:
 * @palette_callback: The name of the callback registered for this pop-up.
 * @palette_name: The name of the palette to set as selected.
 *
 * Sets the current palette in a palette selection dialog.
 *
 * Sets the current palette in a palette selection dialog.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_palettes_set_popup (const gchar *palette_callback,
                         const gchar *palette_name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-palettes-set-popup",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, palette_callback,
                                    GIMP_PDB_STRING, palette_name,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}
