/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Decompose plug-in (C) 1997 Peter Kirchgessner
 * e-mail: peter@kirchgessner.net, WWW: http://www.kirchgessner.net
 *
 * Copyright 2013 Martijn van Beers <mail_dev@martijn.at>
 * Copyright 2013 Téo Mazars        <teo.mazars@ensimag.fr>
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

/*  Lab colorspace support originally written by Alexey Dyachenko,
 *  merged into the officical plug-in by Sven Neumann.
 */

#include "config.h"

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "libgimp/stdplugins-intl.h"

#define PLUG_IN_PROC      "plug-in-decompose"
#define PLUG_IN_PROC_REG  "plug-in-decompose-registered"
#define PLUG_IN_BINARY    "decompose"
#define PLUG_IN_ROLE      "gimp-decompose"


/* Description of a component */
typedef struct
{
  const gchar    *babl_name;           /* channel's  babl_component name    */
  const gchar    *channel_name;        /* name of channel to extract        */

  const gdouble   range_min;           /* min and max                       */
  const gdouble   range_max;
  const gboolean  perceptual_channel;  /* "correct" the channel in Y' space */

} Component;


/* Maximum number of images/layers generated by an extraction */
#define MAX_EXTRACT_IMAGES 4

/* Description of an extraction */
typedef struct
{
  const gchar     *type;        /* What to extract */
  const gchar     *model;       /* the babl_model string to use */
  const gboolean   dialog;      /* Set to TRUE if you want
                                 * this extract function in the dialog */
  const gint       num_images;  /* Number of images to create */

  const gboolean   clamp;       /* clamping values in [0.0, 1.0] */

                                /* the babl_component names of the channels */
  const Component  component[MAX_EXTRACT_IMAGES];

} Extract;

typedef struct
{
  gchar     extract_type[32];
  gboolean  as_layers;
  gboolean  use_registration;
} DecomposeVals;


/* Declare local functions
 */
static void      query                       (void);
static void      run                         (const gchar         *name,
                                              gint                 nparams,
                                              const GimpParam     *param,
                                              gint                *nreturn_vals,
                                              GimpParam          **return_vals);
static gint32    decompose                   (gint32               image_id,
                                              gint32               drawable_ID,
                                              const gchar         *extract_type,
                                              gint32              *image_ID_dst,
                                              gint32              *num_layers,
                                              gint32              *layer_ID_dst);
static gint32    create_new_image            (const gchar         *filename,
                                              const gchar         *layername,
                                              guint                width,
                                              guint                height,
                                              GimpImageBaseType    type,
                                              GimpPrecision        precision,
                                              gdouble              xres,
                                              gdouble              yres,
                                              gint32              *layer_ID);
static gint32    create_new_layer            (gint32               image_ID,
                                              gint                 position,
                                              const gchar         *layername,
                                              guint                width,
                                              guint                height,
                                              GimpImageBaseType    type);
static void      transfer_registration_color (GeglBuffer          *src,
                                              GeglBuffer         **dst,
                                              gint                 count);
static void      cpn_affine_transform_clamp  (GeglBuffer          *buffer,
                                              gdouble              min,
                                              gdouble              max,
                                              gboolean             clamp);
static void      copy_n_components           (GeglBuffer          *src,
                                              GeglBuffer         **dst,
                                              Extract              ext);
static void      copy_one_component          (GeglBuffer          *src,
                                              GeglBuffer          *dst,
                                              const char          *model,
                                              const Component      component,
                                              gboolean             clamp);
static gboolean  decompose_dialog            (void);
static gchar   * generate_filename           (guint32              image_ID,
                                              guint                colorspace,
                                              guint                channel);


#define CPN_RGBA_R      { "R",          N_("red"),           0.0, 1.0, FALSE }
#define CPN_RGBA_G      { "G",          N_("green"),         0.0, 1.0, FALSE }
#define CPN_RGBA_B      { "B",          N_("blue"),          0.0, 1.0, FALSE }
#define CPN_RGBA_A      { "A",          N_("alpha"),         0.0, 1.0, TRUE  }

#define CPN_HSV_H       { "hue",        N_("hue"),           0.0, 1.0, TRUE }
#define CPN_HSV_S       { "saturation", N_("saturation"),    0.0, 1.0, TRUE }
#define CPN_HSV_V       { "value",      N_("value"),         0.0, 1.0, TRUE }

#define CPN_HSL_H       { "hue",        N_("hue"),           0.0, 1.0, TRUE }
#define CPN_HSL_S       { "saturation", N_("saturation"),    0.0, 1.0, TRUE }
#define CPN_HSL_L       { "lightness",  N_("lightness"),     0.0, 1.0, TRUE }

#define CPN_CMYK_C      { "Cyan",       N_("cyan"),          0.0, 1.0, TRUE }
#define CPN_CMYK_M      { "Magenta",    N_("magenta"),       0.0, 1.0, TRUE }
#define CPN_CMYK_Y      { "Yellow",     N_("yellow"),        0.0, 1.0, TRUE }
#define CPN_CMYK_K      { "Key",        N_("black"),         0.0, 1.0, TRUE }

#define CPN_LAB_L       { "CIE L",      N_("L"),             0.0, 100.0, TRUE }
#define CPN_LAB_A       { "CIE a",      N_("A"),          -127.5, 127.5, TRUE }
#define CPN_LAB_B       { "CIE b",      N_("B"),          -127.5, 127.5, TRUE }

#define CPN_LCH_L       { "CIE L",      N_("L"),             0.0, 100.0, TRUE }
#define CPN_LCH_C       { "CIE C(ab)",  N_("C"),             0.0, 200.0, TRUE }
#define CPN_LCH_H       { "CIE H(ab)",  N_("H"),             0.0, 360.0, TRUE }

#define CPN_YCBCR_Y     { "Y'",         N_("luma-y470"),       0.0, 1.0, TRUE }
#define CPN_YCBCR_CB    { "Cb",         N_("blueness-cb470"), -0.5, 0.5, TRUE }
#define CPN_YCBCR_CR    { "Cr",         N_("redness-cr470"),  -0.5, 0.5, TRUE }

#define CPN_YCBCR709_Y  { "Y'",         N_("luma-y709"),       0.0, 1.0, TRUE }
#define CPN_YCBCR709_CB { "Cb",         N_("blueness-cb709"), -0.5, 0.5, TRUE }
#define CPN_YCBCR709_CR { "Cr",         N_("redness-cr709"),  -0.5, 0.5, TRUE }


static const Extract extract[] =
{
  { N_("RGB"),   "RGB",  TRUE,  3, FALSE, { CPN_RGBA_R, CPN_RGBA_G, CPN_RGBA_B } },
  { N_("RGBA"),  "RGBA", TRUE,  4, FALSE, { CPN_RGBA_R, CPN_RGBA_G, CPN_RGBA_B, CPN_RGBA_A } },

  { N_("Red"),   "RGB",  FALSE, 1, FALSE, { CPN_RGBA_R } },
  { N_("Green"), "RGB",  FALSE, 1, FALSE, { CPN_RGBA_G } },
  { N_("Blue"),  "RGB",  FALSE, 1, FALSE, { CPN_RGBA_B } },
  { N_("Alpha"), "RGBA", TRUE , 1, FALSE, { CPN_RGBA_A } },

  { N_("HSV"),        "HSV",  TRUE,  3, FALSE, { CPN_HSV_H, CPN_HSV_S, CPN_HSV_V } },
  { N_("Hue"),        "HSV",  FALSE, 1, FALSE, { CPN_HSV_H } },
  { N_("Saturation"), "HSV",  FALSE, 1, FALSE, { CPN_HSV_S } },
  { N_("Value"),      "HSV",  FALSE, 1, FALSE, { CPN_HSV_V } },

  { N_("HSL"),              "HSL", TRUE,  3, FALSE, { CPN_HSL_H, CPN_HSL_S, CPN_HSL_L } },
  { N_("Hue (HSL)"),        "HSL", FALSE, 1, FALSE, { CPN_HSL_H } },
  { N_("Saturation (HSL)"), "HSL", FALSE, 1, FALSE, { CPN_HSL_S } },
  { N_("Lightness"),        "HSL", FALSE, 1, FALSE, { CPN_HSL_L } },

  { N_("CMYK"),      "CMYK", TRUE,  4, FALSE, { CPN_CMYK_C, CPN_CMYK_M, CPN_CMYK_Y, CPN_CMYK_K } },
  { N_("Cyan"),      "CMYK", FALSE, 1, FALSE, { CPN_CMYK_C } },
  { N_("Magenta"),   "CMYK", FALSE, 1, FALSE, { CPN_CMYK_M } },
  { N_("Yellow"),    "CMYK", FALSE, 1, FALSE, { CPN_CMYK_Y } },
  { N_("Black"),     "CMYK", FALSE, 1, FALSE, { CPN_CMYK_K } },

  { N_("LAB"), "CIE Lab",     TRUE, 3, FALSE, { CPN_LAB_L, CPN_LAB_A, CPN_LAB_B } },

  { N_("LCH"), "CIE LCH(ab)", TRUE, 3, FALSE, { CPN_LCH_L, CPN_LCH_C, CPN_LCH_H } },

  { N_("YCbCr_ITU_R470"),     "Y'CbCr", TRUE, 3, FALSE, { CPN_YCBCR_Y, CPN_YCBCR_CB, CPN_YCBCR_CR} },
  { N_("YCbCr_ITU_R470_256"), "Y'CbCr", TRUE, 3, TRUE,  { CPN_YCBCR_Y, CPN_YCBCR_CB, CPN_YCBCR_CR} },

  { N_("YCbCr_ITU_R709"),     "Y'CbCr709", TRUE, 3, FALSE, { CPN_YCBCR709_Y, CPN_YCBCR709_CB, CPN_YCBCR709_CR} },
  { N_("YCbCr_ITU_R709_256"), "Y'CbCr709", TRUE, 3, TRUE,  { CPN_YCBCR709_Y, CPN_YCBCR709_CB, CPN_YCBCR709_CR} }
};

const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

static DecomposeVals decovals =
{
  "rgb",    /* Decompose type      */
  TRUE,     /* Decompose to Layers */
  FALSE     /* use registration color */
};


MAIN ()


static void
query (void)
{
  static GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,    "run-mode",       "The run mode { RUN-INTERACTIVE (0), RUN-NONINTERACTIVE (1) }" },
    { GIMP_PDB_IMAGE,    "image",          "Input image (unused)"         },
    { GIMP_PDB_DRAWABLE, "drawable",       "Input drawable"               },
    { GIMP_PDB_STRING,   "decompose-type", NULL                           },
    { GIMP_PDB_INT32,    "layers-mode",    "Create channels as layers in a single image" }
  };
  static const GimpParamDef return_vals[] =
  {
    { GIMP_PDB_IMAGE, "new-image", "Output gray image" },
    { GIMP_PDB_IMAGE, "new-image", "Output gray image (N/A for single channel extract)" },
    { GIMP_PDB_IMAGE, "new-image", "Output gray image (N/A for single channel extract)" },
    { GIMP_PDB_IMAGE, "new-image", "Output gray image (N/A for single channel extract)" }
  };

  GString *type_desc;
  gint     i;

  type_desc = g_string_new ("What to decompose: ");
  g_string_append_c (type_desc, '"');
  g_string_append (type_desc, extract[0].type);
  g_string_append_c (type_desc, '"');

  for (i = 1; i < G_N_ELEMENTS (extract); i++)
    {
      g_string_append (type_desc, ", ");
      g_string_append_c (type_desc, '"');
      g_string_append (type_desc, extract[i].type);
      g_string_append_c (type_desc, '"');
    }

  args[3].description = type_desc->str;

  gimp_install_procedure (PLUG_IN_PROC,
                          N_("Decompose an image into separate colorspace components"),
                          "This function creates new gray images with "
                          "different channel information in each of them",
                          "Peter Kirchgessner",
                          "Peter Kirchgessner",
                          "1997",
                          N_("_Decompose..."),
                          "RGB*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args),
                          G_N_ELEMENTS (return_vals),
                          args, return_vals);

  gimp_install_procedure (PLUG_IN_PROC_REG,
                          N_("Decompose an image into separate colorspace components"),
                          "This function creates new gray images with "
                          "different channel information in each of them. "
                          "Pixels in the foreground color will appear black "
                          "in all output images.  This can be used for "
                          "things like crop marks that have to show up on "
                          "all channels.",
                          "Peter Kirchgessner",
                          "Peter Kirchgessner, Clarence Risher",
                          "1997",
                          N_("_Decompose..."),
                          "RGB*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args),
                          G_N_ELEMENTS (return_vals),
                          args, return_vals);

  gimp_plugin_menu_register (PLUG_IN_PROC_REG, "<Image>/Colors/Components");

  g_string_free (type_desc, TRUE);
}

static void
run (const gchar      *name,
     gint              nparams,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  static GimpParam  values[MAX_EXTRACT_IMAGES + 1];
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;
  GimpRunMode       run_mode;
  gint32            num_images;
  gint32            image_ID_extract[MAX_EXTRACT_IMAGES];
  gint32            layer_ID_extract[MAX_EXTRACT_IMAGES];
  gint              j;
  gint32            layer;
  gint32            num_layers;
  gint32            image_ID;

  INIT_I18N ();
  gegl_init (NULL, NULL);

  run_mode = param[0].data.d_int32;
  image_ID = param[1].data.d_image;
  layer    = param[2].data.d_drawable;

  *nreturn_vals = MAX_EXTRACT_IMAGES + 1;
  *return_vals  = values;

  values[0].type          = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  for (j = 0; j < MAX_EXTRACT_IMAGES; j++)
    {
      values[j+1].type         = GIMP_PDB_IMAGE;
      values[j+1].data.d_int32 = -1;
    }

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
      /*  Possibly retrieve data  */
      gimp_get_data (PLUG_IN_PROC, &decovals);

      /*  First acquire information with a dialog  */
      if (! decompose_dialog ())
        return;
      break;

    case GIMP_RUN_NONINTERACTIVE:
      /*  Make sure all the arguments are there!  */
      if (nparams != 4 && nparams != 5 && nparams != 6)
        {
          status = GIMP_PDB_CALLING_ERROR;
        }
      else
        {
          strncpy (decovals.extract_type, param[3].data.d_string,
                   sizeof (decovals.extract_type));
          decovals.extract_type[sizeof (decovals.extract_type) - 1] = '\0';

          decovals.as_layers = nparams > 4 ? param[4].data.d_int32 : FALSE;
          decovals.use_registration = (strcmp (name, PLUG_IN_PROC_REG) == 0);
        }
      break;

    case GIMP_RUN_WITH_LAST_VALS:
      /*  Possibly retrieve data  */
      gimp_get_data (PLUG_IN_PROC, &decovals);
      break;

    default:
      break;
    }

  if (status == GIMP_PDB_SUCCESS)
    {
      gimp_progress_init (_("Decomposing"));

      num_images = decompose (image_ID, layer,
                              decovals.extract_type,
                              image_ID_extract,
                              &num_layers,
                              layer_ID_extract);

      if (num_images <= 0)
        {
          status = GIMP_PDB_EXECUTION_ERROR;
        }
      else
        {
          /* create decompose-data parasite */
          GString *data = g_string_new ("");

          g_string_printf (data, "source=%d type=%s ",
                           layer, decovals.extract_type);

          for (j = 0; j < num_layers; j++)
            g_string_append_printf (data, "%d ", layer_ID_extract[j]);

          for (j = 0; j < num_images; j++)
            {
              GimpParasite *parasite;

              values[j+1].data.d_int32 = image_ID_extract[j];

              gimp_image_undo_enable (image_ID_extract[j]);
              gimp_image_clean_all (image_ID_extract[j]);

              parasite = gimp_parasite_new ("decompose-data",
                                            0, data->len + 1, data->str);
              gimp_image_attach_parasite (image_ID_extract[j], parasite);
              gimp_parasite_free (parasite);

              if (run_mode != GIMP_RUN_NONINTERACTIVE)
                gimp_display_new (image_ID_extract[j]);
            }

          /*  Store data  */
          if (run_mode == GIMP_RUN_INTERACTIVE)
            gimp_set_data (PLUG_IN_PROC, &decovals, sizeof (DecomposeVals));
        }

      gimp_progress_end ();
    }

  values[0].data.d_status = status;
}


/* Decompose an image. It returns the number of new (gray) images.
 * The image IDs for the new images are returned in image_ID_dst.
 * On failure, -1 is returned.
 */
static gint32
decompose (gint32       image_ID,
           gint32       drawable_ID,
           const gchar *extract_type,
           gint32      *image_ID_dst,
           gint32      *nlayers,
           gint32      *layer_ID_dst)
{
  const gchar   *layername;
  gint           j, extract_idx;
  gint           height, width, num_layers;
  GeglBuffer    *src_buffer;
  GeglBuffer    *dst_buffer[MAX_EXTRACT_IMAGES];
  GimpPrecision  precision;
  gboolean       requirements      = FALSE;
  gboolean       decomp_has_alpha = FALSE;

  extract_idx = -1;   /* Search extract type */
  for (j = 0; j < G_N_ELEMENTS (extract); j++)
    {
      if (g_ascii_strcasecmp (extract_type, extract[j].type) == 0)
        {
          extract_idx = j;
          break;
        }
    }
  if (extract_idx < 0)
    return -1;

  num_layers = extract[extract_idx].num_images;

  /* Sanity checks */
  src_buffer = gimp_drawable_get_buffer (drawable_ID);
  precision  = gimp_image_get_precision (image_ID);

  for (j = 0; j < num_layers; j++)
    {
      /* FIXME: Not 100% reliable */
      decomp_has_alpha |= ! g_strcmp0 ("alpha", extract[extract_idx].component[j].babl_name);
      decomp_has_alpha |= ! g_strcmp0 ("A",     extract[extract_idx].component[j].babl_name);
    }

  requirements |= (gimp_drawable_is_rgb (drawable_ID));
  requirements |= (gimp_drawable_is_indexed (drawable_ID));
  requirements |= (gimp_drawable_is_gray (drawable_ID)
                  && gimp_drawable_has_alpha (drawable_ID)
                  && (num_layers <= 2)
                  && decomp_has_alpha);
  requirements &= (!decomp_has_alpha || gimp_drawable_has_alpha (drawable_ID));

  if (!requirements)
    {
      g_message (_("Image not suitable for this decomposition"));
      return -1;
    }

  width  = gegl_buffer_get_width  (src_buffer);
  height = gegl_buffer_get_height (src_buffer);

  /* Create all new gray images */
  for (j = 0; j < num_layers; j++)
    {
      gchar   *filename;
      gdouble  xres, yres;

      filename = generate_filename (image_ID, extract_idx, j);
      gimp_image_get_resolution (image_ID, &xres, &yres);

      if (decovals.as_layers)
        {
          layername = gettext (extract[extract_idx].component[j].channel_name);

          if (j == 0)
            image_ID_dst[j] = create_new_image (filename, layername,
                                                width, height, GIMP_GRAY, precision,
                                                xres, yres,
                                                layer_ID_dst + j);
          else
            layer_ID_dst[j] = create_new_layer (image_ID_dst[0], j, layername,
                                                width, height, GIMP_GRAY);
        }
      else
        {
          image_ID_dst[j] = create_new_image (filename, NULL,
                                              width, height, GIMP_GRAY, precision,
                                              xres, yres,
                                              layer_ID_dst + j);
        }

      g_free (filename);

      dst_buffer[j] = gimp_drawable_get_buffer (layer_ID_dst[j]);
    }

  copy_n_components (src_buffer, dst_buffer,
                     extract[extract_idx]);

  if (decovals.use_registration)
    transfer_registration_color (src_buffer, dst_buffer, num_layers);

  gimp_progress_update (1.0);

  g_object_unref (src_buffer);

  for (j = 0; j < num_layers; j++)
    {
      g_object_unref (dst_buffer[j]);
    }

  *nlayers = num_layers;

  return (decovals.as_layers ? 1 : num_layers);
}


/* Create an image. Returns layer_ID and image_ID */
static gint32
create_new_image (const gchar       *filename,
                  const gchar       *layername,
                  guint              width,
                  guint              height,
                  GimpImageBaseType  type,
                  GimpPrecision      precision,
                  gdouble            xres,
                  gdouble            yres,
                  gint32            *layer_ID)
{
  gint32 image_ID;

  image_ID = gimp_image_new_with_precision (width, height, type, precision);

  gimp_image_undo_disable (image_ID);
  gimp_image_set_filename (image_ID, filename);
  gimp_image_set_resolution (image_ID, xres, yres);

  *layer_ID = create_new_layer (image_ID, 0,
                                layername, width, height, type);

  return image_ID;
}


static gint32
create_new_layer (gint32             image_ID,
                  gint               position,
                  const gchar       *layername,
                  guint              width,
                  guint              height,
                  GimpImageBaseType  type)
{
  gint32        layer_ID;
  GimpImageType gdtype = GIMP_RGB_IMAGE;

  switch (type)
    {
    case GIMP_RGB:
      gdtype = GIMP_RGB_IMAGE;
      break;
    case GIMP_GRAY:
      gdtype = GIMP_GRAY_IMAGE;
      break;
    case GIMP_INDEXED:
      gdtype = GIMP_INDEXED_IMAGE;
      break;
    }

  if (! layername)
    layername = _("Background");

  layer_ID = gimp_layer_new (image_ID, layername, width, height,
                             gdtype,
                             100,
                             gimp_image_get_default_new_layer_mode (image_ID));
  gimp_image_insert_layer (image_ID, layer_ID, -1, position);

  return layer_ID;
}

/* Registration Color function */

static void
transfer_registration_color (GeglBuffer  *src,
                             GeglBuffer **dst,
                             gint         count)
{
  GimpRGB             color, test;
  GeglBufferIterator *gi;
  const Babl         *src_format;
  const Babl         *dst_format;
  gint                src_bpp;
  gint                dst_bpp;
  gint                i;
  gdouble             white;

  gimp_context_get_foreground (&color);
  white = 1.0;

  src_format = gegl_buffer_get_format (src);
  src_bpp = babl_format_get_bytes_per_pixel (src_format);

  dst_format = gegl_buffer_get_format (dst[0]);
  dst_bpp = babl_format_get_bytes_per_pixel (dst_format);

  gi = gegl_buffer_iterator_new (src, NULL, 0, NULL,
                                 GEGL_ACCESS_READ, GEGL_ABYSS_NONE, 10);

  for (i = 0; i < count; i++)
    {
      gegl_buffer_iterator_add (gi, dst[i], NULL, 0, NULL,
                                GEGL_ACCESS_READWRITE, GEGL_ABYSS_NONE);
    }

  while (gegl_buffer_iterator_next (gi))
    {
      gpointer src_data;
      gpointer dst_data[MAX_EXTRACT_IMAGES];
      gint     j, k;

      src_data = gi->items[0].data;
      for (j = 0; j < count; j++)
        dst_data[j] = gi->items[j + 1].data;

      for (k = 0; k < gi->length; k++)
        {
          gulong pos = k * src_bpp;

          gimp_rgba_set_pixel (&test, src_format, ((guchar *)src_data) + pos);

          if (gimp_rgb_distance (&test, &color) < 1e-6)
            {
              for (j = 0; j < count; j++)
                {
                  gpointer data = dst_data[j];

                  babl_process (babl_fish (babl_format ("Y double"), dst_format),
                                &white, (guchar *)data + (k * dst_bpp), 1);
                }
            }
        }
    }
}

static void
cpn_affine_transform_clamp (GeglBuffer *buffer,
                            gdouble     min,
                            gdouble     max,
                            gboolean    clamp)
{
  GeglBufferIterator *gi;
  gdouble             scale  = 1.0 / (max - min);
  gdouble             offset = - min;

  /* We want to scale values linearly, regardless of the format of the buffer */
  gegl_buffer_set_format (buffer, babl_format ("Y double"));

  gi = gegl_buffer_iterator_new (buffer, NULL, 0, NULL,
                                 GEGL_ACCESS_READWRITE, GEGL_ABYSS_NONE, 1);

  while (gegl_buffer_iterator_next (gi))
    {
      guint k;
      double *data;

      data = (double*) gi->items[0].data;

      if (clamp)
        {
          for (k = 0; k < gi->length; k++)
            {
              data[k] = CLAMP ((data[k] + offset) * scale, 0.0, 1.0);
            }
        }
      else
        {
          for (k = 0; k < gi->length; k++)
            {
              data[k] = (data[k] + offset) * scale;
            }
        }
    }
}

static void
copy_n_components (GeglBuffer  *src,
                   GeglBuffer **dst,
                   Extract      ext)
{
  gint i;

  for (i = 0; i < ext.num_images; i++)
    {
      gimp_progress_update ((gdouble) i / (gdouble) ext.num_images);

      copy_one_component (src, dst[i], ext.model, ext.component[i], ext.clamp);
    }
}

static void
copy_one_component (GeglBuffer      *src,
                    GeglBuffer      *dst,
                    const gchar     *model,
                    const Component  component,
                    gboolean         clamp)
{
  const Babl          *component_format;
  const Babl          *dst_format;
  GeglBuffer          *temp;
  const GeglRectangle *extent;

  /* We are working in linear double precision */
  component_format = babl_format_new (babl_model (model),
                                      babl_type ("double"),
                                      babl_component (component.babl_name),
                                      NULL);

  /* We need to enforce linearity here
   * If the output is "Y'", the output of temp is already ok
   * If the output is "Y" , it will enforce gamma-decoding.
   * A bit tricky and suboptimal...
   */
  if (component.perceptual_channel)
    dst_format = babl_format ("Y' double");
  else
    dst_format = babl_format ("Y double");

  extent = gegl_buffer_get_extent (src);
  temp = gegl_buffer_new (extent, dst_format);

  /* we want to copy the component as is */
  gegl_buffer_set_format (temp, component_format);
  gegl_buffer_copy (src, NULL, GEGL_ABYSS_NONE, temp, NULL);

  if (component.range_min != 0.0 ||
      component.range_max != 1.0 ||
      clamp)
    {
      cpn_affine_transform_clamp (temp,
                                  component.range_min, component.range_max,
                                  clamp);
    }

  /* This is our new "Y(') double" component buffer */
  gegl_buffer_set_format (temp, NULL);

  /* Now we let babl convert it back to the format that dst needs */
  gegl_buffer_copy (temp, NULL, GEGL_ABYSS_NONE, dst, NULL);

  g_object_unref (temp);
}

static gboolean
decompose_dialog (void)
{
  GtkWidget *dialog;
  GtkWidget *main_vbox;
  GtkWidget *frame;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *combo;
  GtkWidget *toggle;
  gint       j;
  gint       extract_idx;
  gboolean   run;

  extract_idx = 0;
  for (j = 0; j < G_N_ELEMENTS (extract); j++)
    {
      if (extract[j].dialog &&
          g_ascii_strcasecmp (decovals.extract_type, extract[j].type) == 0)
        {
          extract_idx = j;
          break;
        }
    }

  gimp_ui_init (PLUG_IN_BINARY, FALSE);

  dialog = gimp_dialog_new (_("Decompose"), PLUG_IN_ROLE,
                            NULL, 0,
                            gimp_standard_help_func, PLUG_IN_PROC,

                            _("_Cancel"), GTK_RESPONSE_CANCEL,
                            _("_OK"),     GTK_RESPONSE_OK,

                            NULL);

  gtk_dialog_set_alternative_button_order (GTK_DIALOG (dialog),
                                           GTK_RESPONSE_OK,
                                           GTK_RESPONSE_CANCEL,
                                           -1);

  gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
  gimp_window_set_transient (GTK_WINDOW (dialog));

  main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 12);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))),
                      main_vbox, TRUE, TRUE, 0);
  gtk_widget_show (main_vbox);

  frame = gimp_frame_new (_("Extract Channels"));
  gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  label = gtk_label_new_with_mnemonic (_("Color _model:"));
  gtk_label_set_xalign (GTK_LABEL (label), 0.0);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  combo = g_object_new (GIMP_TYPE_INT_COMBO_BOX, NULL);
  for (j = 0; j < G_N_ELEMENTS (extract); j++)
    {
      if (extract[j].dialog)
        {
          gchar *label = g_strdup (gettext (extract[j].type));
          gchar *l;

          for (l = label; *l; l++)
            if (*l == '-' || *l == '_')
              *l = ' ';

          gimp_int_combo_box_append (GIMP_INT_COMBO_BOX (combo),
                                     GIMP_INT_STORE_LABEL, label,
                                     GIMP_INT_STORE_VALUE, j,
                                     -1);
          g_free (label);
        }
    }

  gtk_box_pack_start (GTK_BOX (hbox), combo, TRUE, TRUE, 0);
  gtk_widget_show (combo);

  gtk_label_set_mnemonic_widget (GTK_LABEL (label), combo);

  gimp_int_combo_box_connect (GIMP_INT_COMBO_BOX (combo),
                              extract_idx,
                              G_CALLBACK (gimp_int_combo_box_get_active),
                              &extract_idx);

  toggle = gtk_check_button_new_with_mnemonic (_("_Decompose to layers"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                decovals.as_layers);
  gtk_box_pack_start (GTK_BOX (main_vbox), toggle, FALSE, FALSE, 0);
  gtk_widget_show (toggle);

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &decovals.as_layers);

  toggle =
    gtk_check_button_new_with_mnemonic (_("_Foreground as registration color"));
  gimp_help_set_help_data (toggle, _("Pixels in the foreground color will "
                                     "appear black in all output images.  "
                                     "This can be used for things like crop "
                                     "marks that have to show up on all "
                                     "channels."), PLUG_IN_PROC);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                decovals.use_registration);
  gtk_box_pack_start (GTK_BOX (main_vbox), toggle, FALSE, FALSE, 0);
  gtk_widget_show (toggle);

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (gimp_toggle_button_update),
                    &decovals.use_registration);

  gtk_widget_show (dialog);

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  if (run)
    strncpy (decovals.extract_type, extract[extract_idx].type,
             sizeof decovals.extract_type - 1);

  return run;
}

/* Build a filename like <imagename>-<channel>.<extension> */
gchar *
generate_filename (guint32 image_ID,
                   guint   colorspace,
                   guint   channel)
{
  /* Build a filename like <imagename>-<channel>.<extension> */
  gchar   *fname;
  gchar   *filename;
  gchar   *extension;

  fname = gimp_image_get_filename (image_ID);

  if (fname)
    {
      extension = fname + strlen (fname) - 1;

      while (extension >= fname)
        {
          if (*extension == '.')
            break;
          extension--;
        }

      if (extension >= fname)
        {
          *(extension++) = '\0';

          if (decovals.as_layers)
            filename = g_strdup_printf ("%s-%s.%s", fname,
                                        gettext (extract[colorspace].type),
                                        extension);
          else
            filename = g_strdup_printf ("%s-%s.%s", fname,
                                        gettext (extract[colorspace].component[channel].channel_name),
                                        extension);
        }
      else
        {
          if (decovals.as_layers)
            filename = g_strdup_printf ("%s-%s", fname,
                                        gettext (extract[colorspace].type));
          else
            filename = g_strdup_printf ("%s-%s", fname,
                                        gettext (extract[colorspace].component[channel].channel_name));
        }
    }
  else
    {
      filename = g_strdup (gettext (extract[colorspace].component[channel].channel_name));
    }

  g_free (fname);

  return filename;
}
