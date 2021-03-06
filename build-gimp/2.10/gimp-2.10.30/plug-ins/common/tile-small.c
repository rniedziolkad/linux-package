/*
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This is a plug-in for GIMP.
 *
 * Tileit - This plugin take an image and makes repeated copies of it.
 *
 * Copyright (C) 1997 Andy Thomas  alt@picnic.demon.co.uk
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
 *
 * A fair proprotion of this code was taken from the Whirl plug-in
 * which was copyrighted by Federico Mena Quintero (as below).
 *
 * Whirl plug-in --- distort an image into a whirlpool
 * Copyright (C) 1997 Federico Mena Quintero
 *
 */

/* Change log:-
 * 0.2  Added new functions to allow "editing" of the tile patten.
 *
 * 0.1 First version released.
 */

#include "config.h"

#include <string.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "libgimp/stdplugins-intl.h"

#define PLUG_IN_PROC   "plug-in-small-tiles"
#define PLUG_IN_BINARY "tile-small"
#define PLUG_IN_ROLE   "gimp-tile-small"

/***** Magic numbers *****/

#define PREVIEW_SIZE 128
#define SCALE_WIDTH   80

#define MAX_SEGS       6

#define PREVIEW_MASK   GDK_EXPOSURE_MASK | \
                       GDK_BUTTON_PRESS_MASK | \
                       GDK_BUTTON_MOTION_MASK

/* Variables set in dialog box */
typedef struct data
{
  gint numtiles;
} TileItVals;

typedef struct
{
  GtkWidget *preview;
  guchar     preview_row[PREVIEW_SIZE * 4];
  gint       img_bpp;
  guchar    *pv_cache;
} TileItInterface;

static TileItInterface tint =
{
  NULL,  /* Preview */
  {
    '4',
    'u'
  },     /* Preview_row */
  4,     /* bpp of drawable */
  NULL
};


static void      query  (void);
static void      run    (const gchar      *name,
                         gint              nparams,
                         const GimpParam  *param,
                         gint             *nreturn_vals,
                         GimpParam       **return_vals);

static gboolean  tileit_dialog          (gint32         drawable_ID);

static void      tileit_scale_update    (GtkAdjustment *adjustment,
                                         gpointer       data);

static void      tileit_exp_update      (GtkWidget     *widget,
                                         gpointer       value);
static void      tileit_exp_update_f    (GtkWidget     *widget,
                                         gpointer       value);

static void      tileit_reset           (GtkWidget     *widget,
                                         gpointer       value);
static void      tileit_radio_update    (GtkWidget     *widget,
                                         gpointer       data);
static void      tileit_hvtoggle_update (GtkWidget     *widget,
                                         gpointer       data);

static void      do_tiles               (gint32         drawable_ID);
static gint      tiles_xy               (gint           width,
                                         gint           height,
                                         gint           x,
                                         gint           y,
                                         gint          *nx,
                                         gint          *ny);
static void      all_update             (void);
static void      alt_update             (void);
static void      explicit_update        (gboolean);

static void      dialog_update_preview  (void);
static void      cache_preview          (gint32         drawable_ID);
static gboolean  tileit_preview_expose  (GtkWidget     *widget,
                                         GdkEvent      *event);
static gboolean  tileit_preview_events  (GtkWidget     *widget,
                                         GdkEvent      *event);


const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

/* Values when first invoked */
static TileItVals itvals =
{
  2
};

/* Structures for call backs... */
/* The "explicit tile" & family */
typedef enum
{
  ALL,
  ALT,
  EXPLICIT
} AppliedTo;

typedef struct
{
  AppliedTo  type;

  gint       x;        /* X - pos of tile   */
  gint       y;        /* Y - pos of tile   */
  GtkObject *r_adj;    /* row adjustment    */
  GtkObject *c_adj;    /* column adjustment */
  GtkWidget *applybut; /* The apply button  */
} Exp_Call;

static Exp_Call exp_call =
{
  ALL,
  -1,
  -1,
  NULL,
  NULL,
  NULL,
};

/* The reset button needs to know some toggle widgets.. */

typedef struct
{
  GtkWidget *htoggle;
  GtkWidget *vtoggle;
} Reset_Call;

static Reset_Call res_call =
{
  NULL,
  NULL,
};

/* 2D - Array that holds the actions for each tile */
/* Action type on cell */
#define HORIZONTAL 0x1
#define VERTICAL   0x2

static gint tileactions[MAX_SEGS][MAX_SEGS];

/* What actions buttons toggled */
static gint   do_horz = FALSE;
static gint   do_vert = FALSE;
static gint   opacity = 100;

/* Stuff for the preview bit */
static gint     sel_x1, sel_y1, sel_x2, sel_y2;
static gint     sel_width, sel_height;
static gint     preview_width, preview_height;
static gboolean has_alpha;

MAIN ()

static void
query (void)
{
  static const GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,    "run-mode",  "The run mode { RUN-INTERACTIVE (0), RUN-NONINTERACTIVE (1) }" },
    { GIMP_PDB_IMAGE,    "image",     "Input image (unused)"         },
    { GIMP_PDB_DRAWABLE, "drawable",  "Input drawable"               },
    { GIMP_PDB_INT32,    "num-tiles", "Number of tiles to make"      }
  };

  gimp_install_procedure (PLUG_IN_PROC,
                          N_("Tile image into smaller versions of the original"),
                          "More here later",
                          "Andy Thomas",
                          "Andy Thomas",
                          "1997",
                          N_("_Small Tiles..."),
                          "RGB*, GRAY*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args), 0,
                          args, NULL);
}

static void
run (const gchar      *name,
     gint              nparams,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  static GimpParam   values[1];
  GimpRunMode        run_mode;
  gint32             drawable_ID;
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;
  gint               pwidth;
  gint               pheight;

  INIT_I18N ();
  gegl_init (NULL, NULL);

  *nreturn_vals = 1;
  *return_vals  = values;

  values[0].type          = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  run_mode    = param[0].data.d_int32;
  drawable_ID = param[2].data.d_drawable;

  has_alpha = gimp_drawable_has_alpha (drawable_ID);

  if (! gimp_drawable_mask_intersect (drawable_ID,
                                      &sel_x1,    &sel_y1,
                                      &sel_width, &sel_height))
    {
      g_message (_("Region selected for filter is empty."));
      return;
    }

  sel_x2 = sel_x1 + sel_width;
  sel_y2 = sel_y1 + sel_height;

  /* Calculate preview size */

  if (sel_width > sel_height)
    {
      pwidth  = MIN (sel_width, PREVIEW_SIZE);
      pheight = sel_height * pwidth / sel_width;
    }
  else
    {
      pheight = MIN (sel_height, PREVIEW_SIZE);
      pwidth  = sel_width * pheight / sel_height;
    }

  preview_width  = MAX (pwidth, 2);  /* Min size is 2 */
  preview_height = MAX (pheight, 2);

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
      gimp_get_data (PLUG_IN_PROC, &itvals);
      if (! tileit_dialog (drawable_ID))
        return;
      break;

    case GIMP_RUN_NONINTERACTIVE:
      if (nparams != 4)
        {
          status = GIMP_PDB_CALLING_ERROR;
        }
      else
        {
          itvals.numtiles = param[3].data.d_int32;
        }
      break;

    case GIMP_RUN_WITH_LAST_VALS:
      gimp_get_data (PLUG_IN_PROC, &itvals);
      break;

    default:
      break;
    }

  if (gimp_drawable_is_rgb (drawable_ID) ||
      gimp_drawable_is_gray (drawable_ID))
    {
      /* Set the tile cache size */

      gimp_progress_init (_("Tiling"));

      do_tiles (drawable_ID);

      if (run_mode != GIMP_RUN_NONINTERACTIVE)
        gimp_displays_flush ();

      if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_set_data (PLUG_IN_PROC, &itvals, sizeof (TileItVals));
    }
  else
    {
      status = GIMP_PDB_EXECUTION_ERROR;
    }

  values[0].data.d_status = status;
}

static gboolean
tileit_dialog (gint drawable_ID)
{
  GtkWidget *dlg;
  GtkWidget *main_vbox;
  GtkWidget *hbox;
  GtkWidget *vbox;
  GtkWidget *frame;
  GtkWidget *table;
  GtkWidget *table2;
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *spinbutton;
  GtkObject *adj;
  GtkObject *scale;
  GtkWidget *toggle;
  GSList    *orientation_group = NULL;
  gboolean   run;

  gimp_ui_init (PLUG_IN_BINARY, TRUE);

  cache_preview (drawable_ID); /* Get the preview image */

  dlg = gimp_dialog_new (_("Small Tiles"), PLUG_IN_ROLE,
                         NULL, 0,
                         gimp_standard_help_func, PLUG_IN_PROC,

                         _("_Cancel"), GTK_RESPONSE_CANCEL,
                         _("_OK"),     GTK_RESPONSE_OK,

                         NULL);

  gtk_dialog_set_alternative_button_order (GTK_DIALOG (dlg),
                                           GTK_RESPONSE_OK,
                                           GTK_RESPONSE_CANCEL,
                                           -1);

  gimp_window_set_transient (GTK_WINDOW (dlg));

  main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 12);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dlg))),
                      main_vbox, TRUE, TRUE, 0);
  gtk_widget_show (main_vbox);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_box_pack_start (GTK_BOX (main_vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show (vbox);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  tint.preview = gimp_preview_area_new ();
  gtk_widget_set_size_request (tint.preview, preview_width, preview_height);
  gtk_widget_set_events (GTK_WIDGET (tint.preview), PREVIEW_MASK);
  gtk_container_add (GTK_CONTAINER (frame), tint.preview);
  gtk_widget_show (tint.preview);

  g_signal_connect_after (tint.preview, "expose-event",
                          G_CALLBACK (tileit_preview_expose),
                          NULL);
  g_signal_connect (tint.preview, "event",
                    G_CALLBACK (tileit_preview_events),
                    NULL);

  /* Area for buttons etc */

  frame = gimp_frame_new (_("Flip"));
  gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_set_homogeneous (GTK_BOX (hbox), TRUE);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  toggle = gtk_check_button_new_with_mnemonic (_("_Horizontal"));
  gtk_box_pack_start (GTK_BOX (hbox), toggle, TRUE, TRUE, 0);
  gtk_widget_show (toggle);

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (tileit_hvtoggle_update),
                    &do_horz);

  res_call.htoggle = toggle;

  toggle = gtk_check_button_new_with_mnemonic (_("_Vertical"));
  gtk_box_pack_start (GTK_BOX (hbox), toggle, TRUE, TRUE, 0);
  gtk_widget_show (toggle);

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (tileit_hvtoggle_update),
                    &do_vert);

  res_call.vtoggle = toggle;

  button = gtk_button_new_with_mnemonic (_("_Reset"));
  gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
  gtk_widget_show (button);

  g_signal_connect (button, "clicked",
                    G_CALLBACK (tileit_reset),
                    &res_call);

  /* Table for the inner widgets..*/
  table = gtk_table_new (4, 4, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  toggle = gtk_radio_button_new_with_mnemonic (orientation_group,
                                               _("A_ll tiles"));
  orientation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (toggle));
  gtk_table_attach (GTK_TABLE (table), toggle, 0, 4, 0, 1,
                    GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
  gtk_widget_show (toggle);

  g_object_set_data (G_OBJECT (toggle), "gimp-item-data",
                     GINT_TO_POINTER (ALL));

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (tileit_radio_update),
                    &exp_call.type);

  toggle = gtk_radio_button_new_with_mnemonic (orientation_group,
                                               _("Al_ternate tiles"));
  orientation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (toggle));
  gtk_table_attach (GTK_TABLE (table), toggle, 0, 4, 1, 2,
                    GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
  gtk_widget_show (toggle);

  g_object_set_data (G_OBJECT (toggle), "gimp-item-data",
                     GINT_TO_POINTER (ALT));

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (tileit_radio_update),
                    &exp_call.type);

  toggle = gtk_radio_button_new_with_mnemonic (orientation_group,
                                               _("_Explicit tile"));
  orientation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (toggle));
  gtk_table_attach (GTK_TABLE (table), toggle, 0, 1, 2, 4,
                    GTK_FILL | GTK_SHRINK, GTK_FILL, 0, 0);
  gtk_widget_show (toggle);

  label = gtk_label_new_with_mnemonic (_("Ro_w:"));
  gtk_label_set_xalign (GTK_LABEL (label), 1.0);
  gtk_table_attach (GTK_TABLE (table), label, 1, 2, 2, 3,
                    GTK_FILL | GTK_SHRINK , GTK_FILL, 0, 0);
  gtk_widget_show (label);

  g_object_bind_property (toggle, "active",
                          label,  "sensitive",
                          G_BINDING_SYNC_CREATE);

  spinbutton = gimp_spin_button_new (&adj, 2, 1, 6, 1, 1, 0, 1, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);
  gtk_table_attach (GTK_TABLE (table), spinbutton, 2, 3, 2, 3,
                    GTK_FILL | GTK_SHRINK, GTK_FILL, 0, 0);
  gtk_widget_show (spinbutton);

  g_signal_connect (adj, "value-changed",
                    G_CALLBACK (tileit_exp_update_f),
                    &exp_call);

  exp_call.r_adj = adj;

  g_object_bind_property (toggle,     "active",
                          spinbutton, "sensitive",
                          G_BINDING_SYNC_CREATE);

  label = gtk_label_new_with_mnemonic (_("Col_umn:"));
  gtk_label_set_xalign (GTK_LABEL (label), 1.0);
  gtk_widget_show (label);
  gtk_table_attach (GTK_TABLE (table), label, 1, 2, 3, 4,
                    GTK_FILL , GTK_FILL, 0, 0);

  g_object_bind_property (toggle, "active",
                          label,  "sensitive",
                          G_BINDING_SYNC_CREATE);

  spinbutton = gimp_spin_button_new (&adj, 2, 1, 6, 1, 1, 0, 1, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);
  gtk_table_attach (GTK_TABLE (table), spinbutton, 2, 3, 3, 4,
                    GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
  gtk_widget_show (spinbutton);

  g_signal_connect (adj, "value-changed",
                    G_CALLBACK (tileit_exp_update_f),
                    &exp_call);

  exp_call.c_adj = adj;

  g_object_bind_property (toggle,     "active",
                          spinbutton, "sensitive",
                          G_BINDING_SYNC_CREATE);

  g_object_set_data (G_OBJECT (toggle), "gimp-item-data",
                     GINT_TO_POINTER (EXPLICIT));

  g_signal_connect (toggle, "toggled",
                    G_CALLBACK (tileit_radio_update),
                    &exp_call.type);

  button = gtk_button_new_with_mnemonic (_("_Apply"));
  gtk_table_attach (GTK_TABLE (table), button, 3, 4, 2, 4, 0, 0, 0, 0);
  gtk_widget_show (button);

  g_signal_connect (button, "clicked",
                    G_CALLBACK (tileit_exp_update),
                    &exp_call);

  exp_call.applybut = button;

  g_object_bind_property (toggle,     "active",
                          spinbutton, "sensitive",
                          G_BINDING_SYNC_CREATE);

  /* Widget for selecting the Opacity */

  table2 = gtk_table_new (1, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table2), 6);
  gtk_box_pack_start (GTK_BOX (vbox), table2, FALSE, FALSE, 0);
  gtk_widget_show (table2);

  scale = gimp_scale_entry_new (GTK_TABLE (table2), 0, 0,
                                _("O_pacity:"), SCALE_WIDTH, -1,
                                opacity, 0, 100, 1, 10, 0,
                                TRUE, 0, 0,
                                NULL, NULL);
  g_signal_connect (scale, "value-changed",
                    G_CALLBACK (tileit_scale_update),
                    &opacity);

  /* Lower frame saying howmany segments */
  frame = gimp_frame_new (_("Number of Segments"));
  gtk_box_pack_start (GTK_BOX (main_vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  table = gtk_table_new (1, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  gtk_widget_set_sensitive (table2, gimp_drawable_has_alpha (drawable_ID));

  scale = gimp_scale_entry_new (GTK_TABLE (table), 0, 0,
                                "_n??", SCALE_WIDTH, -1,
                                itvals.numtiles, 2, MAX_SEGS, 1, 1, 0,
                                TRUE, 0, 0,
                                NULL, NULL);
  g_signal_connect (scale, "value-changed",
                    G_CALLBACK (tileit_scale_update),
                    &itvals.numtiles);

  gtk_widget_show (dlg);
  dialog_update_preview ();

  run = (gimp_dialog_run (GIMP_DIALOG (dlg)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dlg);

  return run;
}

static void
tileit_hvtoggle_update (GtkWidget *widget,
                        gpointer   data)
{
  gimp_toggle_button_update (widget, data);

  switch (exp_call.type)
    {
    case ALL:
      /* Clear current settings */
      memset (tileactions, 0, sizeof (tileactions));
      all_update ();
      break;

    case ALT:
      /* Clear current settings */
      memset (tileactions, 0, sizeof (tileactions));
      alt_update ();
      break;

    case EXPLICIT:
      break;
    }

  dialog_update_preview ();
}

static gboolean
tileit_preview_expose (GtkWidget *widget,
                       GdkEvent  *event)
{
  if (exp_call.type == EXPLICIT)
    {
      cairo_t  *cr     = gdk_cairo_create (gtk_widget_get_window (tint.preview));
      gdouble   width  = (gdouble) preview_width / (gdouble) itvals.numtiles;
      gdouble   height = (gdouble) preview_height / (gdouble) itvals.numtiles;
      gdouble   x , y;

      x = width  * (exp_call.x - 1);
      y = height * (exp_call.y - 1);

      cairo_rectangle (cr, x + 1.5, y + 1.5, width - 2, height - 2);

      cairo_set_line_width (cr, 3.0);
      cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.6);
      cairo_stroke_preserve (cr);

      cairo_set_line_width (cr, 1.0);
      cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.8);
      cairo_stroke_preserve (cr);

      cairo_destroy (cr);
    }

  return FALSE;
}

static void
exp_need_update (gint nx,
                 gint ny)
{
  if (nx <= 0 || nx > itvals.numtiles || ny <= 0 || ny > itvals.numtiles)
    return;

  if (nx != exp_call.x || ny != exp_call.y)
    {
      exp_call.x = nx;
      exp_call.y = ny;
      gtk_widget_queue_draw (tint.preview);

      g_signal_handlers_block_by_func (exp_call.c_adj,
                                       tileit_exp_update_f,
                                       &exp_call);
      g_signal_handlers_block_by_func (exp_call.r_adj,
                                       tileit_exp_update_f,
                                       &exp_call);

      gtk_adjustment_set_value (GTK_ADJUSTMENT (exp_call.c_adj), nx);
      gtk_adjustment_set_value (GTK_ADJUSTMENT (exp_call.r_adj), ny);

      g_signal_handlers_unblock_by_func (exp_call.c_adj,
                                         tileit_exp_update_f,
                                         &exp_call);
      g_signal_handlers_unblock_by_func (exp_call.r_adj,
                                         tileit_exp_update_f,
                                         &exp_call);
    }
}

static gboolean
tileit_preview_events (GtkWidget *widget,
                       GdkEvent  *event)
{
  GdkEventButton *bevent;
  GdkEventMotion *mevent;
  gint nx, ny;
  gint twidth  = preview_width / itvals.numtiles;
  gint theight = preview_height / itvals.numtiles;

  switch (event->type)
    {
    case GDK_EXPOSE:
      break;

    case GDK_BUTTON_PRESS:
      bevent = (GdkEventButton *) event;
      nx = bevent->x/twidth + 1;
      ny = bevent->y/theight + 1;
      exp_need_update (nx, ny);
      break;

    case GDK_MOTION_NOTIFY:
      mevent = (GdkEventMotion *) event;
      if ( !mevent->state )
        break;
      if (mevent->x < 0 || mevent->y < 0)
        break;
      nx = mevent->x/twidth + 1;
      ny = mevent->y/theight + 1;
      exp_need_update (nx, ny);
      break;

    default:
      break;
    }

  return FALSE;
}

static void
explicit_update (gboolean settile)
{
  gint x,y;

  /* Make sure bounds are OK */
  y = ROUND (gtk_adjustment_get_value (GTK_ADJUSTMENT (exp_call.r_adj)));
  if (y > itvals.numtiles || y <= 0)
    {
      y = itvals.numtiles;
    }
  x = ROUND (gtk_adjustment_get_value (GTK_ADJUSTMENT (exp_call.c_adj)));
  if (x > itvals.numtiles || x <= 0)
    {
      x = itvals.numtiles;
    }

  /* Set it */
  if (settile)
    tileactions[x-1][y-1] = (((do_horz) ? HORIZONTAL : 0) |
                             ((do_vert) ? VERTICAL : 0));

  exp_call.x = x;
  exp_call.y = y;
}

static void
all_update (void)
{
  gint x,y;

  for (x = 0 ; x < MAX_SEGS; x++)
    for (y = 0 ; y < MAX_SEGS; y++)
      tileactions[x][y] |= (((do_horz) ? HORIZONTAL : 0) |
                            ((do_vert) ? VERTICAL : 0));
}

static void
alt_update (void)
{
  gint x,y;

  for (x = 0 ; x < MAX_SEGS; x++)
    for (y = 0 ; y < MAX_SEGS; y++)
      if (!((x + y) % 2))
        tileactions[x][y] |= (((do_horz) ? HORIZONTAL : 0) |
                              ((do_vert) ? VERTICAL : 0));
}

static void
tileit_radio_update (GtkWidget *widget,
                     gpointer   data)
{
  gimp_radio_button_update (widget, data);

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      switch (exp_call.type)
        {
        case ALL:
          /* Clear current settings */
          memset (tileactions, 0, sizeof (tileactions));
          all_update ();
          break;

        case ALT:
          /* Clear current settings */
          memset (tileactions, 0, sizeof (tileactions));
          alt_update ();
          break;

        case EXPLICIT:
          explicit_update (FALSE);
          break;
        }

      dialog_update_preview ();
    }
}


static void
tileit_scale_update (GtkAdjustment *adjustment,
                     gpointer       data)
{
  gimp_int_adjustment_update (adjustment, data);

  dialog_update_preview ();
}

static void
tileit_reset (GtkWidget *widget,
              gpointer   data)
{
  Reset_Call *r = (Reset_Call *) data;

  memset (tileactions, 0, sizeof (tileactions));

  g_signal_handlers_block_by_func (r->htoggle,
                                   tileit_hvtoggle_update,
                                   &do_horz);
  g_signal_handlers_block_by_func (r->vtoggle,
                                   tileit_hvtoggle_update,
                                   &do_vert);

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r->htoggle), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r->vtoggle), FALSE);

  g_signal_handlers_unblock_by_func (r->htoggle,
                                     tileit_hvtoggle_update,
                                     &do_horz);
  g_signal_handlers_unblock_by_func (r->vtoggle,
                                     tileit_hvtoggle_update,
                                     &do_vert);

  do_horz = do_vert = FALSE;

  dialog_update_preview ();
}


/* Could avoid almost dup. functions by using a field in the data
 * passed.  Must still pass the data since used in sig blocking func.
 */

static void
tileit_exp_update (GtkWidget *widget,
                   gpointer   applied)
{
  explicit_update (TRUE);
  dialog_update_preview ();
}

static void
tileit_exp_update_f (GtkWidget *widget,
                     gpointer   applied)
{
  explicit_update (FALSE);
  dialog_update_preview ();
}

/* Cache the preview image - updates are a lot faster. */
/* The preview_cache will contain the small image */

static void
cache_preview (gint32 drawable_ID)
{
  GeglBuffer *buffer = gimp_drawable_get_buffer (drawable_ID);
  const Babl *format;
  gdouble     scale;

  if (gimp_drawable_has_alpha (drawable_ID))
    format = babl_format ("R'G'B'A u8");
  else
    format = babl_format ("R'G'B' u8");

  tint.img_bpp = babl_format_get_bytes_per_pixel (format);

  tint.pv_cache = g_new (guchar, preview_width * preview_height * 4);

  scale = (gdouble) preview_width / (gdouble) sel_width;

  gegl_buffer_get (buffer, GEGL_RECTANGLE (scale * sel_x1, scale * sel_y1,
                                           preview_width, preview_height),
                   scale,
                   format, tint.pv_cache,
                   GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);

  g_object_unref (buffer);
}

static void
do_tiles (gint32 drawable_ID)
{
  GeglBuffer         *src_buffer;
  GeglBuffer         *dest_buffer;
  GeglBufferIterator *iter;
  const Babl         *format;
  gboolean            has_alpha;
  gint                progress, max_progress;
  gint                bpp;
  guchar              pixel[4];
  gint                nc, nr;
  gint                i;

  src_buffer  = gimp_drawable_get_buffer (drawable_ID);
  dest_buffer = gimp_drawable_get_shadow_buffer (drawable_ID);

  has_alpha = gimp_drawable_has_alpha (drawable_ID);

  if (has_alpha)
    format = babl_format ("R'G'B'A u8");
  else
    format = babl_format ("R'G'B' u8");

  bpp = babl_format_get_bytes_per_pixel (format);

  progress     = 0;
  max_progress = sel_width * sel_height;

  iter = gegl_buffer_iterator_new (dest_buffer,
                                   GEGL_RECTANGLE (sel_x1, sel_y1,
                                                   sel_width, sel_height), 0,
                                   format,
                                   GEGL_ACCESS_WRITE, GEGL_ABYSS_NONE, 1);


  while (gegl_buffer_iterator_next (iter))
    {
      GeglRectangle  dest_roi = iter->items[0].roi;
      guchar        *dest_row = iter->items[0].data;
      gint           row;

      for (row = dest_roi.y; row < (dest_roi.y + dest_roi.height); row++)
        {
          guchar *dest = dest_row;
          gint    col;

          for (col = dest_roi.x; col < (dest_roi.x + dest_roi.width); col++)
            {
              tiles_xy (sel_width,
                        sel_height,
                        col - sel_x1,
                        row - sel_y1,
                        &nc, &nr);

              gegl_buffer_sample (src_buffer, nc + sel_x1, nr + sel_y1, NULL,
                                  pixel, format,
                                  GEGL_SAMPLER_NEAREST, GEGL_ABYSS_NONE);

              for (i = 0; i < bpp; i++)
                dest[i] = pixel[i];

              if (has_alpha)
                dest[bpp - 1] = (pixel[bpp - 1] * opacity) / 100;

              dest += bpp;
            }

          dest_row += dest_roi.width * bpp;
        }

      progress += dest_roi.width * dest_roi.height;
      gimp_progress_update ((double) progress / max_progress);
    }

  gimp_progress_update (1.0);

  g_object_unref (src_buffer);
  g_object_unref (dest_buffer);

  gimp_drawable_merge_shadow (drawable_ID, TRUE);
  gimp_drawable_update (drawable_ID,
                        sel_x1, sel_y1, sel_width, sel_height);
}


/* Get the xy pos and any action */
static gint
tiles_xy (gint  width,
          gint  height,
          gint  x,
          gint  y,
          gint *nx,
          gint *ny)
{
  gint    px,py;
  gint    rnum,cnum;
  gint    actiontype;
  gdouble rnd = 1 - (1.0 / (gdouble) itvals.numtiles) + 0.01;

  rnum = y * itvals.numtiles / height;

  py   = (y * itvals.numtiles) % height;
  px   = (x * itvals.numtiles) % width;
  cnum = x * itvals.numtiles / width;

  if ((actiontype = tileactions[cnum][rnum]))
    {
      if (actiontype & VERTICAL)
        {
          gdouble pyr;

          pyr =  height - y - 1 + rnd;
          py = ((gint) (pyr * (gdouble) itvals.numtiles)) % height;
        }

      if (actiontype & HORIZONTAL)
        {
          gdouble pxr;

          pxr = width - x - 1 + rnd;
          px = ((gint) (pxr * (gdouble) itvals.numtiles)) % width;
        }
    }

  *nx = px;
  *ny = py;

  return(actiontype);
}


/* Given a row then shrink it down a bit */
static void
do_tiles_preview (guchar *dest_row,
                  guchar *src_rows,
                  gint    width,
                  gint    dh,
                  gint    height,
                  gint    bpp)
{
  gint    x;
  gint    i;
  gint    px, py;
  gint    rnum,cnum;
  gint    actiontype;
  gdouble rnd = 1 - (1.0 / (gdouble) itvals.numtiles) + 0.01;

  rnum = dh * itvals.numtiles / height;

  for (x = 0; x < width; x ++)
    {

      py = (dh*itvals.numtiles)%height;

      px = (x*itvals.numtiles)%width;
      cnum = x*itvals.numtiles/width;

      if ((actiontype = tileactions[cnum][rnum]))
        {
          if (actiontype & VERTICAL)
            {
              gdouble pyr;
              pyr =  height - dh - 1 + rnd;
              py = ((int)(pyr*(gdouble)itvals.numtiles))%height;
            }

          if (actiontype & HORIZONTAL)
            {
              gdouble pxr;
              pxr = width - x - 1 + rnd;
              px = ((int)(pxr*(gdouble)itvals.numtiles))%width;
            }
        }

      for (i = 0 ; i < bpp; i++ )
        dest_row[x*tint.img_bpp+i] =
          src_rows[(px + (py*width))*bpp+i];

      if (has_alpha)
        dest_row[x*tint.img_bpp + (bpp - 1)] =
          (dest_row[x*tint.img_bpp + (bpp - 1)]*opacity)/100;

    }
}

static void
dialog_update_preview (void)
{
  gint    y;
  guchar *buffer;

  buffer = g_new (guchar, preview_width * preview_height * tint.img_bpp);

  for (y = 0; y < preview_height; y++)
    {
      do_tiles_preview (tint.preview_row,
                        tint.pv_cache,
                        preview_width,
                        y,
                        preview_height,
                        tint.img_bpp);

      memcpy (buffer + y* (preview_width * tint.img_bpp),
              tint.preview_row,
              preview_width * tint.img_bpp);
    }

  gimp_preview_area_draw (GIMP_PREVIEW_AREA (tint.preview),
                          0, 0, preview_width, preview_height,
                          (tint.img_bpp>3)?GIMP_RGBA_IMAGE:GIMP_RGB_IMAGE,
                          buffer,
                          preview_width * tint.img_bpp);

  g_free (buffer);

  gtk_widget_queue_draw (tint.preview);
}
