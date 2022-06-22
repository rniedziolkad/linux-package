/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is auto-generated by pdbgen.pl. */

#include "config.h"

#include <gegl.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"

#include "core/gimp.h"
#include "core/gimpdatafactory.h"
#include "core/gimpparamspecs.h"

#include "gimppdb.h"
#include "gimpprocedure.h"
#include "internal-procs.h"


static GimpValueArray *
fonts_popup_invoker (GimpProcedure         *procedure,
                     Gimp                  *gimp,
                     GimpContext           *context,
                     GimpProgress          *progress,
                     const GimpValueArray  *args,
                     GError               **error)
{
  gboolean success = TRUE;
  const gchar *font_callback;
  const gchar *popup_title;
  const gchar *initial_font;

  font_callback = g_value_get_string (gimp_value_array_index (args, 0));
  popup_title = g_value_get_string (gimp_value_array_index (args, 1));
  initial_font = g_value_get_string (gimp_value_array_index (args, 2));

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup_procedure (gimp->pdb, font_callback) ||
          ! gimp_data_factory_data_wait (gimp->font_factory)     ||
          ! gimp_pdb_dialog_new (gimp, context, progress,
                                 gimp_data_factory_get_container (gimp->font_factory),
                                 popup_title, font_callback, initial_font,
                                 NULL))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

static GimpValueArray *
fonts_close_popup_invoker (GimpProcedure         *procedure,
                           Gimp                  *gimp,
                           GimpContext           *context,
                           GimpProgress          *progress,
                           const GimpValueArray  *args,
                           GError               **error)
{
  gboolean success = TRUE;
  const gchar *font_callback;

  font_callback = g_value_get_string (gimp_value_array_index (args, 0));

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup_procedure (gimp->pdb, font_callback) ||
          ! gimp_pdb_dialog_close (gimp,
                                   gimp_data_factory_get_container (gimp->font_factory),
                                   font_callback))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

static GimpValueArray *
fonts_set_popup_invoker (GimpProcedure         *procedure,
                         Gimp                  *gimp,
                         GimpContext           *context,
                         GimpProgress          *progress,
                         const GimpValueArray  *args,
                         GError               **error)
{
  gboolean success = TRUE;
  const gchar *font_callback;
  const gchar *font_name;

  font_callback = g_value_get_string (gimp_value_array_index (args, 0));
  font_name = g_value_get_string (gimp_value_array_index (args, 1));

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup_procedure (gimp->pdb, font_callback) ||
          ! gimp_data_factory_data_wait (gimp->font_factory)     ||
          ! gimp_pdb_dialog_set (gimp,
                                 gimp_data_factory_get_container (gimp->font_factory),
                                 font_callback, font_name,
                                 NULL))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

void
register_font_select_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-fonts-popup
   */
  procedure = gimp_procedure_new (fonts_popup_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-fonts-popup");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-fonts-popup",
                                     "Invokes the Gimp font selection.",
                                     "This procedure opens the font selection dialog.",
                                     "Sven Neumann <sven@gimp.org>",
                                     "Sven Neumann",
                                     "2003",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("font-callback",
                                                       "font callback",
                                                       "The callback PDB proc to call when font selection is made",
                                                       FALSE, FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("popup-title",
                                                       "popup title",
                                                       "Title of the font selection dialog",
                                                       FALSE, FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("initial-font",
                                                       "initial font",
                                                       "The name of the font to set as the first selected",
                                                       FALSE, TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-fonts-close-popup
   */
  procedure = gimp_procedure_new (fonts_close_popup_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-fonts-close-popup");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-fonts-close-popup",
                                     "Close the font selection dialog.",
                                     "This procedure closes an opened font selection dialog.",
                                     "Sven Neumann <sven@gimp.org>",
                                     "Sven Neumann",
                                     "2003",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("font-callback",
                                                       "font callback",
                                                       "The name of the callback registered for this pop-up",
                                                       FALSE, FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-fonts-set-popup
   */
  procedure = gimp_procedure_new (fonts_set_popup_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-fonts-set-popup");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-fonts-set-popup",
                                     "Sets the current font in a font selection dialog.",
                                     "Sets the current font in a font selection dialog.",
                                     "Sven Neumann <sven@gimp.org>",
                                     "Sven Neumann",
                                     "2003",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("font-callback",
                                                       "font callback",
                                                       "The name of the callback registered for this pop-up",
                                                       FALSE, FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("font-name",
                                                       "font name",
                                                       "The name of the font to set as selected",
                                                       FALSE, FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
