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
#include "core/gimpgradient.h"
#include "core/gimpparamspecs.h"

#include "gimppdb.h"
#include "gimpprocedure.h"
#include "internal-procs.h"


static GimpValueArray *
gradients_popup_invoker (GimpProcedure         *procedure,
                         Gimp                  *gimp,
                         GimpContext           *context,
                         GimpProgress          *progress,
                         const GimpValueArray  *args,
                         GError               **error)
{
  gboolean success = TRUE;
  const gchar *gradient_callback;
  const gchar *popup_title;
  const gchar *initial_gradient;
  gint32 sample_size;

  gradient_callback = g_value_get_string (gimp_value_array_index (args, 0));
  popup_title = g_value_get_string (gimp_value_array_index (args, 1));
  initial_gradient = g_value_get_string (gimp_value_array_index (args, 2));
  sample_size = g_value_get_int (gimp_value_array_index (args, 3));

  if (success)
    {
      if (sample_size < 1 || sample_size > 10000)
        sample_size = GIMP_GRADIENT_DEFAULT_SAMPLE_SIZE;

      if (gimp->no_interface ||
          ! gimp_pdb_lookup_procedure (gimp->pdb, gradient_callback) ||
          ! gimp_pdb_dialog_new (gimp, context, progress,
                                 gimp_data_factory_get_container (gimp->gradient_factory),
                                 popup_title, gradient_callback, initial_gradient,
                                 "sample-size", sample_size,
                                 NULL))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

static GimpValueArray *
gradients_close_popup_invoker (GimpProcedure         *procedure,
                               Gimp                  *gimp,
                               GimpContext           *context,
                               GimpProgress          *progress,
                               const GimpValueArray  *args,
                               GError               **error)
{
  gboolean success = TRUE;
  const gchar *gradient_callback;

  gradient_callback = g_value_get_string (gimp_value_array_index (args, 0));

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup_procedure (gimp->pdb, gradient_callback) ||
          ! gimp_pdb_dialog_close (gimp, gimp_data_factory_get_container (gimp->gradient_factory),
                                   gradient_callback))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

static GimpValueArray *
gradients_set_popup_invoker (GimpProcedure         *procedure,
                             Gimp                  *gimp,
                             GimpContext           *context,
                             GimpProgress          *progress,
                             const GimpValueArray  *args,
                             GError               **error)
{
  gboolean success = TRUE;
  const gchar *gradient_callback;
  const gchar *gradient_name;

  gradient_callback = g_value_get_string (gimp_value_array_index (args, 0));
  gradient_name = g_value_get_string (gimp_value_array_index (args, 1));

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup_procedure (gimp->pdb, gradient_callback) ||
          ! gimp_pdb_dialog_set (gimp, gimp_data_factory_get_container (gimp->gradient_factory),
                                 gradient_callback, gradient_name,
                                 NULL))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

void
register_gradient_select_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-gradients-popup
   */
  procedure = gimp_procedure_new (gradients_popup_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-gradients-popup");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-gradients-popup",
                                     "Invokes the Gimp gradients selection.",
                                     "This procedure opens the gradient selection dialog.",
                                     "Andy Thomas",
                                     "Andy Thomas",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("gradient-callback",
                                                       "gradient callback",
                                                       "The callback PDB proc to call when gradient selection is made",
                                                       FALSE, FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("popup-title",
                                                       "popup title",
                                                       "Title of the gradient selection dialog",
                                                       FALSE, FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("initial-gradient",
                                                       "initial gradient",
                                                       "The name of the gradient to set as the first selected",
                                                       FALSE, TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("sample-size",
                                                      "sample size",
                                                      "Size of the sample to return when the gradient is changed",
                                                      1, 10000, 1,
                                                      GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-gradients-close-popup
   */
  procedure = gimp_procedure_new (gradients_close_popup_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-gradients-close-popup");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-gradients-close-popup",
                                     "Close the gradient selection dialog.",
                                     "This procedure closes an opened gradient selection dialog.",
                                     "Andy Thomas",
                                     "Andy Thomas",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("gradient-callback",
                                                       "gradient callback",
                                                       "The name of the callback registered for this pop-up",
                                                       FALSE, FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-gradients-set-popup
   */
  procedure = gimp_procedure_new (gradients_set_popup_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-gradients-set-popup");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-gradients-set-popup",
                                     "Sets the current gradient in a gradient selection dialog.",
                                     "Sets the current gradient in a gradient selection dialog.",
                                     "Andy Thomas",
                                     "Andy Thomas",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("gradient-callback",
                                                       "gradient callback",
                                                       "The name of the callback registered for this pop-up",
                                                       FALSE, FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("gradient-name",
                                                       "gradient name",
                                                       "The name of the gradient to set as selected",
                                                       FALSE, FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
