/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpwidgets-utils.c
 * Copyright (C) 1999-2003 Michael Natterer <mitch@gimp.org>
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

#include "config.h"

#include <string.h>

#include <gegl.h>
#include <gtk/gtk.h>

#ifdef GDK_WINDOWING_WIN32
#include <gdk/gdkwin32.h>
#endif

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#ifdef PLATFORM_OSX
#include <ApplicationServices/ApplicationServices.h>
#endif

#include "libgimpbase/gimpbase.h"
#include "libgimpconfig/gimpconfig.h"
#include "libgimpmath/gimpmath.h"
#include "libgimpcolor/gimpcolor.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "widgets-types.h"

#include "gegl/gimp-babl.h"

#include "gimpaction.h"
#include "gimpdialogfactory.h"
#include "gimpdock.h"
#include "gimpdockcontainer.h"
#include "gimpdockwindow.h"
#include "gimperrordialog.h"
#include "gimpsessioninfo.h"
#include "gimpwidgets-utils.h"

#include "gimp-intl.h"


#define GIMP_TOOL_OPTIONS_GUI_KEY      "gimp-tool-options-gui"
#define GIMP_TOOL_OPTIONS_GUI_FUNC_KEY "gimp-tool-options-gui-func"


/**
 * gimp_menu_position:
 * @menu: a #GtkMenu widget
 * @x: pointer to horizontal position
 * @y: pointer to vertical position
 *
 * Positions a #GtkMenu so that it pops up on screen.  This function
 * takes care of the preferred popup direction (taken from the widget
 * render direction) and it handles multiple monitors representing a
 * single #GdkScreen (Xinerama).
 *
 * You should call this function with @x and @y initialized to the
 * origin of the menu. This is typically the center of the widget the
 * menu is popped up from. gimp_menu_position() will then decide if
 * and how these initial values need to be changed.
 **/
void
gimp_menu_position (GtkMenu *menu,
                    gint    *x,
                    gint    *y)
{
  GtkWidget      *widget;
  GdkScreen      *screen;
  GtkRequisition  requisition;
  GdkRectangle    rect;
  gint            monitor;

  g_return_if_fail (GTK_IS_MENU (menu));
  g_return_if_fail (x != NULL);
  g_return_if_fail (y != NULL);

  widget = GTK_WIDGET (menu);

  screen = gtk_widget_get_screen (widget);

  monitor = gdk_screen_get_monitor_at_point (screen, *x, *y);
  gdk_screen_get_monitor_workarea (screen, monitor, &rect);

  gtk_menu_set_screen (menu, screen);

  gtk_widget_size_request (widget, &requisition);

  if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
    {
      *x -= requisition.width;
      if (*x < rect.x)
        *x += requisition.width;
    }
  else
    {
      if (*x + requisition.width > rect.x + rect.width)
        *x -= requisition.width;
    }

  if (*x < rect.x)
    *x = rect.x;

  if (*y + requisition.height > rect.y + rect.height)
    *y -= requisition.height;

  if (*y < rect.y)
    *y = rect.y;
}

/**
 * gimp_button_menu_position:
 * @button: a button widget to popup the menu from
 * @menu: the menu to position
 * @position: the preferred popup direction for the menu (left or right)
 * @x: return location for x coordinate
 * @y: return location for y coordinate
 *
 * Utility function to position a menu that pops up from a button.
 **/
void
gimp_button_menu_position (GtkWidget       *button,
                           GtkMenu         *menu,
                           GtkPositionType  position,
                           gint            *x,
                           gint            *y)
{
  GdkScreen      *screen;
  GtkAllocation   button_allocation;
  GtkRequisition  menu_requisition;
  GdkRectangle    rect;
  gint            monitor;

  g_return_if_fail (GTK_IS_WIDGET (button));
  g_return_if_fail (gtk_widget_get_realized (button));
  g_return_if_fail (GTK_IS_MENU (menu));
  g_return_if_fail (x != NULL);
  g_return_if_fail (y != NULL);

  gtk_widget_get_allocation (button, &button_allocation);

  if (gtk_widget_get_direction (button) == GTK_TEXT_DIR_RTL)
    {
      switch (position)
        {
        case GTK_POS_LEFT:   position = GTK_POS_RIGHT;  break;
        case GTK_POS_RIGHT:  position = GTK_POS_LEFT;   break;
        default:
          break;
        }
    }

  *x = 0;
  *y = 0;

  if (! gtk_widget_get_has_window (button))
    {
      *x += button_allocation.x;
      *y += button_allocation.y;
    }

  gdk_window_get_root_coords (gtk_widget_get_window (button), *x, *y, x, y);

  gtk_widget_size_request (GTK_WIDGET (menu), &menu_requisition);

  screen = gtk_widget_get_screen (button);

  monitor = gdk_screen_get_monitor_at_point (screen, *x, *y);
  gdk_screen_get_monitor_workarea (screen, monitor, &rect);

  gtk_menu_set_screen (menu, screen);

  switch (position)
    {
    case GTK_POS_LEFT:
      *x -= menu_requisition.width;
      if (*x < rect.x)
        *x += menu_requisition.width + button_allocation.width;
      break;

    case GTK_POS_RIGHT:
      *x += button_allocation.width;
      if (*x + menu_requisition.width > rect.x + rect.width)
        *x -= button_allocation.width + menu_requisition.width;
      break;

    default:
      g_warning ("%s: unhandled position (%d)", G_STRFUNC, position);
      break;
    }

  if (*y + menu_requisition.height > rect.y + rect.height)
    *y -= menu_requisition.height - button_allocation.height;

  if (*y < rect.y)
    *y = rect.y;
}

void
gimp_table_attach_icon (GtkTable    *table,
                        gint         row,
                        const gchar *icon_name,
                        GtkWidget   *widget,
                        gint         colspan,
                        gboolean     left_align)
{
  GtkWidget *image;

  g_return_if_fail (GTK_IS_TABLE (table));
  g_return_if_fail (icon_name != NULL);
  g_return_if_fail (GTK_IS_WIDGET (widget));

  image = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_BUTTON);
  gtk_misc_set_alignment (GTK_MISC (image), 1.0, 0.5);
  gtk_table_attach (table, image, 0, 1, row, row + 1,
                    GTK_FILL, GTK_FILL, 0, 0);
  gtk_widget_show (image);

  if (left_align)
    {
      GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

      gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 0);
      gtk_widget_show (widget);

      widget = hbox;
    }

  gtk_table_attach (table, widget, 1, 1 + colspan, row, row + 1,
                    GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_widget_show (widget);
}

void
gimp_enum_radio_box_add (GtkBox    *box,
                         GtkWidget *widget,
                         gint       enum_value,
                         gboolean   below)
{
  GList *children;
  GList *list;
  gint   pos;

  g_return_if_fail (GTK_IS_BOX (box));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  children = gtk_container_get_children (GTK_CONTAINER (box));

  for (list = children, pos = 1;
       list;
       list = g_list_next (list), pos++)
    {
      if (GTK_IS_RADIO_BUTTON (list->data) &&
          GPOINTER_TO_INT (g_object_get_data (list->data, "gimp-item-data")) ==
          enum_value)
        {
          GtkWidget *radio = list->data;
          GtkWidget *hbox;

          hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
          gtk_box_pack_start (GTK_BOX (box), hbox, FALSE, FALSE, 0);
          gtk_box_reorder_child (GTK_BOX (box), hbox, pos);

          if (below)
            {
              GtkWidget *spacer;
              gint       indicator_size;
              gint       indicator_spacing;
              gint       focus_width;
              gint       focus_padding;
              gint       border_width;

              gtk_widget_style_get (radio,
                                    "indicator-size",    &indicator_size,
                                    "indicator-spacing", &indicator_spacing,
                                    "focus-line-width",  &focus_width,
                                    "focus-padding",     &focus_padding,
                                    NULL);

              border_width = gtk_container_get_border_width (GTK_CONTAINER (radio));

              spacer = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
              gtk_widget_set_size_request (spacer,
                                           indicator_size +
                                           3 * indicator_spacing +
                                           focus_width +
                                           focus_padding +
                                           border_width,
                                           -1);
              gtk_box_pack_start (GTK_BOX (hbox), spacer, FALSE, FALSE, 0);
              gtk_widget_show (spacer);
            }
          else
            {
              GtkSizeGroup *size_group;

              size_group = g_object_get_data (G_OBJECT (box), "size-group");

              if (! size_group)
                {
                  size_group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
                  g_object_set_data (G_OBJECT (box), "size-group", size_group);

                  gtk_size_group_add_widget (size_group, radio);
                  g_object_unref (size_group);
                }
              else
                {
                  gtk_size_group_add_widget (size_group, radio);
                }

              gtk_box_set_spacing (GTK_BOX (hbox), 4);

              g_object_ref (radio);
              gtk_container_remove (GTK_CONTAINER (box), radio);
              gtk_box_pack_start (GTK_BOX (hbox), radio, FALSE, FALSE, 0);
              g_object_unref (radio);
            }

          gtk_box_pack_start (GTK_BOX (hbox), widget, TRUE, TRUE, 0);
          gtk_widget_show (widget);

          g_object_bind_property (radio,  "active",
                                  widget, "sensitive",
                                  G_BINDING_SYNC_CREATE);

          gtk_widget_show (hbox);

          break;
        }
    }

  g_list_free (children);
}

void
gimp_enum_radio_frame_add (GtkFrame  *frame,
                           GtkWidget *widget,
                           gint       enum_value,
                           gboolean   below)
{
  GtkWidget *box;

  g_return_if_fail (GTK_IS_FRAME (frame));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  box = gtk_bin_get_child (GTK_BIN (frame));

  g_return_if_fail (GTK_IS_BOX (box));

  gimp_enum_radio_box_add (GTK_BOX (box), widget, enum_value, below);
}

/**
 * gimp_widget_load_icon:
 * @widget:                  parent widget (to determine icon theme and
 *                           style)
 * @icon_name:               icon name
 * @size:                    requested pixel size
 *
 * Loads an icon into a pixbuf with size as close as possible to @size.
 * If icon does not exist or fail to load, the function will fallback to
 * "gimp-wilber-eek" instead to prevent NULL pixbuf. As a last resort,
 * if even the fallback failed to load, a magenta @size square will be
 * returned, so this function is guaranteed to always return a
 * #GdkPixbuf.
 *
 * Return value: a newly allocated #GdkPixbuf containing @icon_name at
 * size @size or a fallback icon/size.
 **/
GdkPixbuf *
gimp_widget_load_icon (GtkWidget   *widget,
                       const gchar *icon_name,
                       gint         size)
{
  GdkPixbuf    *pixbuf;
  GtkIconTheme *icon_theme;
  gint         *icon_sizes;
  gint          closest_size = -1;
  gint          min_diff     = G_MAXINT;
  gint          i;

  g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);
  g_return_val_if_fail (icon_name != NULL, NULL);

  icon_theme = gtk_icon_theme_get_for_screen (gtk_widget_get_screen (widget));

  if (! gtk_icon_theme_has_icon (icon_theme, icon_name))
    {
      g_printerr ("WARNING: icon theme has no icon '%s'.\n",
                  icon_name);

      return gtk_icon_theme_load_icon (icon_theme, GIMP_ICON_WILBER_EEK,
                                       size, 0, NULL);
    }

  icon_sizes = gtk_icon_theme_get_icon_sizes (icon_theme, icon_name);

  for (i = 0; icon_sizes[i]; i++)
    {
      if (icon_sizes[i] > 0 &&
          icon_sizes[i] <= size)
        {
          if (size - icon_sizes[i] < min_diff)
            {
              min_diff     = size - icon_sizes[i];
              closest_size = icon_sizes[i];
            }
        }
    }

  g_free (icon_sizes);

  if (closest_size != -1)
    size = closest_size;

  pixbuf = gtk_icon_theme_load_icon (icon_theme, icon_name, size,
                                     GTK_ICON_LOOKUP_USE_BUILTIN, NULL);

  if (! pixbuf)
    {
      /* The icon was seemingly present in the current icon theme, yet
       * it failed to load. Maybe the file is broken?
       * As last resort, try to load "gimp-wilber-eek" as fallback.
       * Note that we are not making more checks, so if the fallback
       * icon fails to load as well, the function may still return NULL.
       */
      g_printerr ("WARNING: icon '%s' failed to load. Check the files "
                  "in your icon theme.\n", icon_name);

      pixbuf = gtk_icon_theme_load_icon (icon_theme,
                                         GIMP_ICON_WILBER_EEK,
                                         size, 0, NULL);
      if (! pixbuf)
        {
          /* As last resort, just draw an ugly magenta square. */
          guchar *data;
          gint    rowstride = 3 * size;
          gint    i, j;

          g_printerr ("WARNING: icon '%s' failed to load. Check the files "
                      "in your icon theme.\n", GIMP_ICON_WILBER_EEK);

          data = g_new (guchar, rowstride * size);
          for (i = 0; i < size; i++)
            {
              for (j = 0; j < size; j++)
                {
                  data[i * rowstride + j * 3] = 255;
                  data[i * rowstride + j * 3 + 1] = 0;
                  data[i * rowstride + j * 3 + 2] = 255;
                }
            }
          pixbuf = gdk_pixbuf_new_from_data (data, GDK_COLORSPACE_RGB, FALSE,
                                             8, size, size, rowstride,
                                             (GdkPixbufDestroyNotify) g_free,
                                             NULL);
        }
    }

  return pixbuf;
}

GtkIconSize
gimp_get_icon_size (GtkWidget   *widget,
                    const gchar *icon_name,
                    GtkIconSize  max_size,
                    gint         width,
                    gint         height)
{
  GtkIconSet   *icon_set;
  GtkIconSize  *sizes;
  gint          n_sizes;
  gint          i;
  gint          width_diff  = 1024;
  gint          height_diff = 1024;
  gint          max_width;
  gint          max_height;
  GtkIconSize   icon_size = GTK_ICON_SIZE_MENU;
  GtkSettings  *settings;

  g_return_val_if_fail (GTK_IS_WIDGET (widget), icon_size);
  g_return_val_if_fail (icon_name != NULL, icon_size);
  g_return_val_if_fail (width > 0, icon_size);
  g_return_val_if_fail (height > 0, icon_size);

  icon_set = gtk_style_lookup_icon_set (gtk_widget_get_style (widget),
                                        icon_name);

  if (! icon_set)
    return GTK_ICON_SIZE_INVALID;

  settings = gtk_widget_get_settings (widget);

  if (! gtk_icon_size_lookup_for_settings (settings, max_size,
                                           &max_width, &max_height))
    {
      max_width  = 1024;
      max_height = 1024;
    }

  gtk_icon_set_get_sizes (icon_set, &sizes, &n_sizes);

  for (i = 0; i < n_sizes; i++)
    {
      gint icon_width;
      gint icon_height;

      if (gtk_icon_size_lookup_for_settings (settings, sizes[i],
                                             &icon_width, &icon_height))
        {
          if (icon_width  <= width      &&
              icon_height <= height     &&
              icon_width  <= max_width  &&
              icon_height <= max_height &&
              ((width  - icon_width)  < width_diff ||
               (height - icon_height) < height_diff))
            {
              width_diff  = width  - icon_width;
              height_diff = height - icon_height;

              icon_size = sizes[i];
            }
        }
    }

  g_free (sizes);

  return icon_size;
}

GimpTabStyle
gimp_preview_tab_style_to_icon (GimpTabStyle tab_style)
{
  switch (tab_style)
    {
    case GIMP_TAB_STYLE_PREVIEW:
      tab_style = GIMP_TAB_STYLE_ICON;
      break;

    case GIMP_TAB_STYLE_PREVIEW_NAME:
      tab_style = GIMP_TAB_STYLE_ICON_NAME;
      break;

    case GIMP_TAB_STYLE_PREVIEW_BLURB:
      tab_style = GIMP_TAB_STYLE_ICON_BLURB;
      break;

    default:
      break;
    }

  return tab_style;
}

const gchar *
gimp_get_mod_string (GdkModifierType modifiers)
{
  static GHashTable *mod_labels;
  gchar             *label;

  if (! modifiers)
    return NULL;

  if (G_UNLIKELY (! mod_labels))
    mod_labels = g_hash_table_new (g_int_hash, g_int_equal);

  modifiers = gimp_replace_virtual_modifiers (modifiers);

  label = g_hash_table_lookup (mod_labels, &modifiers);

  if (! label)
    {
      GtkAccelLabelClass *accel_label_class;

      label = gtk_accelerator_get_label (0, modifiers);

      accel_label_class = g_type_class_ref (GTK_TYPE_ACCEL_LABEL);

      if (accel_label_class->mod_separator &&
          *accel_label_class->mod_separator)
        {
          gchar *sep = g_strrstr (label, accel_label_class->mod_separator);

          if (sep - label ==
              strlen (label) - strlen (accel_label_class->mod_separator))
            *sep = '\0';
        }

      g_type_class_unref (accel_label_class);

      g_hash_table_insert (mod_labels,
                           g_memdup (&modifiers, sizeof (GdkModifierType)),
                           label);
    }

  return label;
}

#define BUF_SIZE 100
/**
 * gimp_suggest_modifiers:
 * @message:                 initial text for the message
 * @modifiers:               bit mask of modifiers that should be suggested
 * @extend_selection_format: optional format string for the
 *                           "Extend selection" modifier
 * @toggle_behavior_format:  optional format string for the
 *                           "Toggle behavior" modifier
 * @alt_format:              optional format string for the Alt modifier
 *
 * Utility function to build a message suggesting to use some
 * modifiers for performing different actions (only Shift, Ctrl and
 * Alt are currently supported).  If some of these modifiers are
 * already active, they will not be suggested.  The optional format
 * strings #extend_selection_format, #toggle_behavior_format and
 * #alt_format may be used to describe what the modifier will do.
 * They must contain a single '%%s' which will be replaced by the name
 * of the modifier.  They can also be %NULL if the modifier name
 * should be left alone.
 *
 * Return value: a newly allocated string containing the message.
 **/
gchar *
gimp_suggest_modifiers (const gchar     *message,
                        GdkModifierType  modifiers,
                        const gchar     *extend_selection_format,
                        const gchar     *toggle_behavior_format,
                        const gchar     *alt_format)
{
  GdkModifierType  extend_mask = gimp_get_extend_selection_mask ();
  GdkModifierType  toggle_mask = gimp_get_toggle_behavior_mask ();
  gchar            msg_buf[3][BUF_SIZE];
  gint             num_msgs = 0;
  gboolean         try      = FALSE;

  if (modifiers & extend_mask)
    {
      if (extend_selection_format && *extend_selection_format)
        {
          g_snprintf (msg_buf[num_msgs], BUF_SIZE, extend_selection_format,
                      gimp_get_mod_string (extend_mask));
        }
      else
        {
          g_strlcpy (msg_buf[num_msgs],
                     gimp_get_mod_string (extend_mask), BUF_SIZE);
          try = TRUE;
        }

      num_msgs++;
    }

  if (modifiers & toggle_mask)
    {
      if (toggle_behavior_format && *toggle_behavior_format)
        {
          g_snprintf (msg_buf[num_msgs], BUF_SIZE, toggle_behavior_format,
                      gimp_get_mod_string (toggle_mask));
        }
      else
        {
          g_strlcpy (msg_buf[num_msgs],
                     gimp_get_mod_string (toggle_mask), BUF_SIZE);
          try = TRUE;
        }

      num_msgs++;
    }

  if (modifiers & GDK_MOD1_MASK)
    {
      if (alt_format && *alt_format)
        {
          g_snprintf (msg_buf[num_msgs], BUF_SIZE, alt_format,
                      gimp_get_mod_string (GDK_MOD1_MASK));
        }
      else
        {
          g_strlcpy (msg_buf[num_msgs],
                     gimp_get_mod_string (GDK_MOD1_MASK), BUF_SIZE);
          try = TRUE;
        }

      num_msgs++;
    }

  /* This convoluted way to build the message using multiple format strings
   * tries to make the messages easier to translate to other languages.
   */

  switch (num_msgs)
    {
    case 1:
      return g_strdup_printf (try ? _("%s (try %s)") : _("%s (%s)"),
                              message, msg_buf[0]);

    case 2:
      return g_strdup_printf (_("%s (try %s, %s)"),
                              message, msg_buf[0], msg_buf[1]);

    case 3:
      return g_strdup_printf (_("%s (try %s, %s, %s)"),
                              message, msg_buf[0], msg_buf[1], msg_buf[2]);
    }

  return g_strdup (message);
}
#undef BUF_SIZE

GimpChannelOps
gimp_modifiers_to_channel_op (GdkModifierType  modifiers)
{
  GdkModifierType extend_mask = gimp_get_extend_selection_mask ();
  GdkModifierType modify_mask = gimp_get_modify_selection_mask ();

  if (modifiers & extend_mask)
    {
      if (modifiers & modify_mask)
        {
          return GIMP_CHANNEL_OP_INTERSECT;
        }
      else
        {
          return GIMP_CHANNEL_OP_ADD;
        }
    }
  else if (modifiers & modify_mask)
    {
      return GIMP_CHANNEL_OP_SUBTRACT;
    }

  return GIMP_CHANNEL_OP_REPLACE;
}

GdkModifierType
gimp_replace_virtual_modifiers (GdkModifierType modifiers)
{
  GdkDisplay      *display = gdk_display_get_default ();
  GdkModifierType  result  = 0;
  gint             i;

  for (i = 0; i < 8; i++)
    {
      GdkModifierType real = 1 << i;

      if (modifiers & real)
        {
          GdkModifierType virtual = real;

          gdk_keymap_add_virtual_modifiers (gdk_keymap_get_for_display (display),
                                            &virtual);

          if (virtual == real)
            result |= virtual;
          else
            result |= virtual & ~real;
        }
    }

  return result;
}

GdkModifierType
gimp_get_primary_accelerator_mask (void)
{
  GdkDisplay *display = gdk_display_get_default ();

  return gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                       GDK_MODIFIER_INTENT_PRIMARY_ACCELERATOR);
}

GdkModifierType
gimp_get_extend_selection_mask (void)
{
  GdkDisplay *display = gdk_display_get_default ();

  return gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                       GDK_MODIFIER_INTENT_EXTEND_SELECTION);
}

GdkModifierType
gimp_get_modify_selection_mask (void)
{
  GdkDisplay *display = gdk_display_get_default ();

  return gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                       GDK_MODIFIER_INTENT_MODIFY_SELECTION);
}

GdkModifierType
gimp_get_toggle_behavior_mask (void)
{
  GdkDisplay *display = gdk_display_get_default ();

  /* use the modify selection modifier */
  return gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                       GDK_MODIFIER_INTENT_MODIFY_SELECTION);
}

GdkModifierType
gimp_get_constrain_behavior_mask (void)
{
  GdkDisplay *display = gdk_display_get_default ();

  /* use the modify selection modifier */
  return gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                       GDK_MODIFIER_INTENT_MODIFY_SELECTION);
}

GdkModifierType
gimp_get_all_modifiers_mask (void)
{
  GdkDisplay *display = gdk_display_get_default ();

  return (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK |
          gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                        GDK_MODIFIER_INTENT_PRIMARY_ACCELERATOR) |
          gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                        GDK_MODIFIER_INTENT_EXTEND_SELECTION) |
          gdk_keymap_get_modifier_mask (gdk_keymap_get_for_display (display),
                                        GDK_MODIFIER_INTENT_MODIFY_SELECTION));
}

/**
 * gimp_get_monitor_resolution:
 * @screen: a #GdkScreen
 * @monitor: a monitor number
 * @xres: returns the horizontal monitor resolution (in dpi)
 * @yres: returns the vertical monitor resolution (in dpi)
 *
 * Retrieves the monitor's resolution from GDK.
 **/
void
gimp_get_monitor_resolution (GdkScreen *screen,
                             gint       monitor,
                             gdouble   *xres,
                             gdouble   *yres)
{
  GdkRectangle size_pixels;
  gint         width_mm, height_mm;
  gdouble      x = 0.0;
  gdouble      y = 0.0;
#ifdef PLATFORM_OSX
  CGSize       size;
#endif

  g_return_if_fail (GDK_IS_SCREEN (screen));
  g_return_if_fail (xres != NULL);
  g_return_if_fail (yres != NULL);

#ifndef PLATFORM_OSX
  gdk_screen_get_monitor_geometry (screen, monitor, &size_pixels);

  width_mm  = gdk_screen_get_monitor_width_mm  (screen, monitor);
  height_mm = gdk_screen_get_monitor_height_mm (screen, monitor);
#else
  width_mm  = 0;
  height_mm = 0;
  size = CGDisplayScreenSize (kCGDirectMainDisplay);
  if (!CGSizeEqualToSize (size, CGSizeZero))
    {
      width_mm  = size.width;
      height_mm = size.height;
    }
  size_pixels.width  = CGDisplayPixelsWide (kCGDirectMainDisplay);
  size_pixels.height = CGDisplayPixelsHigh (kCGDirectMainDisplay);
#endif
  /*
   * From xdpyinfo.c:
   *
   * there are 2.54 centimeters to an inch; so there are 25.4 millimeters.
   *
   *     dpi = N pixels / (M millimeters / (25.4 millimeters / 1 inch))
   *         = N pixels / (M inch / 25.4)
   *         = N * 25.4 pixels / M inch
   */

  if (width_mm > 0 && height_mm > 0)
    {
      x = (size_pixels.width  * 25.4) / (gdouble) width_mm;
      y = (size_pixels.height * 25.4) / (gdouble) height_mm;
    }

  if (x < GIMP_MIN_RESOLUTION || x > GIMP_MAX_RESOLUTION ||
      y < GIMP_MIN_RESOLUTION || y > GIMP_MAX_RESOLUTION)
    {
      g_printerr ("gimp_get_monitor_resolution(): GDK returned bogus "
                  "values for the monitor resolution, using 96 dpi instead.\n");

      x = 96.0;
      y = 96.0;
    }

  /*  round the value to full integers to give more pleasant results  */
  *xres = ROUND (x);
  *yres = ROUND (y);
}


/**
 * gimp_rgb_get_gdk_color:
 * @rgb: the source color as #GimpRGB
 * @gdk_color: pointer to a #GdkColor
 *
 * Initializes @gdk_color from a #GimpRGB. This function does not
 * allocate the color for you. Depending on how you want to use it,
 * you may have to call gdk_colormap_alloc_color().
 **/
void
gimp_rgb_get_gdk_color (const GimpRGB *rgb,
                        GdkColor      *gdk_color)
{
  guchar r, g, b;

  g_return_if_fail (rgb != NULL);
  g_return_if_fail (gdk_color != NULL);

  gimp_rgb_get_uchar (rgb, &r, &g, &b);

  gdk_color->red   = (r << 8) | r;
  gdk_color->green = (g << 8) | g;
  gdk_color->blue  = (b << 8) | b;
}

/**
 * gimp_rgb_set_gdk_color:
 * @rgb: a #GimpRGB that is to be set
 * @gdk_color: pointer to the source #GdkColor
 *
 * Initializes @rgb from a #GdkColor. This function does not touch
 * the alpha value of @rgb.
 **/
void
gimp_rgb_set_gdk_color (GimpRGB        *rgb,
                        const GdkColor *gdk_color)
{
  guchar r, g, b;

  g_return_if_fail (rgb != NULL);
  g_return_if_fail (gdk_color != NULL);

  r = gdk_color->red   >> 8;
  g = gdk_color->green >> 8;
  b = gdk_color->blue  >> 8;

  gimp_rgb_set_uchar (rgb, r, g, b);
}

void
gimp_window_set_hint (GtkWindow      *window,
                      GimpWindowHint  hint)
{
  g_return_if_fail (GTK_IS_WINDOW (window));

  switch (hint)
    {
    case GIMP_WINDOW_HINT_NORMAL:
      gtk_window_set_type_hint (window, GDK_WINDOW_TYPE_HINT_NORMAL);
      break;

    case GIMP_WINDOW_HINT_UTILITY:
      gtk_window_set_type_hint (window, GDK_WINDOW_TYPE_HINT_UTILITY);
      break;

    case GIMP_WINDOW_HINT_KEEP_ABOVE:
      gtk_window_set_keep_above (window, TRUE);
      break;
    }
}

/**
 * gimp_window_get_native_id:
 * @window: a #GtkWindow
 *
 * This function is used to pass a window handle to plug-ins so that
 * they can set their dialog windows transient to the parent window.
 *
 * Return value: a native window ID of the window's #GdkWindow or 0
 *               if the window isn't realized yet
 */
guint32
gimp_window_get_native_id (GtkWindow *window)
{
  g_return_val_if_fail (GTK_IS_WINDOW (window), 0);

#ifdef GDK_NATIVE_WINDOW_POINTER
#ifdef __GNUC__
#warning gimp_window_get_native() unimplementable for the target windowing system
#endif
  return 0;
#endif

#ifdef GDK_WINDOWING_WIN32
  if (window && gtk_widget_get_realized (GTK_WIDGET (window)))
    return GDK_WINDOW_HWND (gtk_widget_get_window (GTK_WIDGET (window)));
#endif

#ifdef GDK_WINDOWING_X11
  if (window && gtk_widget_get_realized (GTK_WIDGET (window)))
    return GDK_WINDOW_XID (gtk_widget_get_window (GTK_WIDGET (window)));
#endif

  return 0;
}

static void
gimp_window_transient_realized (GtkWidget *window,
                                GdkWindow *parent)
{
  if (gtk_widget_get_realized (window))
    gdk_window_set_transient_for (gtk_widget_get_window (window), parent);
}

/* similar to what we have in libgimp/gimpui.c */
static GdkWindow *
gimp_get_foreign_window (guint32 window)
{
#ifdef GDK_WINDOWING_X11
  return gdk_x11_window_foreign_new_for_display (gdk_display_get_default (),
                                                 window);
#endif

#ifdef GDK_WINDOWING_WIN32
  return gdk_win32_window_foreign_new_for_display (gdk_display_get_default (),
                                                   window);
#endif

  return NULL;
}

void
gimp_window_set_transient_for (GtkWindow *window,
                               guint32    parent_ID)
{
  /* Cross-process transient-for is broken in gdk/win32 <= 2.10.6. It
   * causes hangs, at least when used as by the gimp and script-fu
   * processes. In some newer GTK+ version it will be fixed to be a
   * no-op. If it eventually is fixed to actually work, change this to
   * a run-time check of GTK+ version. Remember to change also the
   * function with the same name in libgimp/gimpui.c
   */
#ifndef GDK_WINDOWING_WIN32
  GdkWindow *parent;

  parent = gimp_get_foreign_window (parent_ID);
  if (! parent)
    return;

  if (gtk_widget_get_realized (GTK_WIDGET (window)))
    gdk_window_set_transient_for (gtk_widget_get_window (GTK_WIDGET (window)),
                                  parent);

  g_signal_connect_object (window, "realize",
                           G_CALLBACK (gimp_window_transient_realized),
                           parent, 0);

  g_object_unref (parent);
#endif
}

static gboolean
gimp_widget_accel_find_func (GtkAccelKey *key,
                             GClosure    *closure,
                             gpointer     data)
{
  return (GClosure *) data == closure;
}

static void
gimp_widget_accel_changed (GtkAccelGroup   *accel_group,
                           guint            unused1,
                           GdkModifierType  unused2,
                           GClosure        *accel_closure,
                           GtkWidget       *widget)
{
  GClosure *widget_closure;

  widget_closure = g_object_get_data (G_OBJECT (widget), "gimp-accel-closure");

  if (accel_closure == widget_closure)
    {
      GimpAction  *action;
      GtkAccelKey *accel_key;
      const gchar *tooltip;
      const gchar *help_id;

      action = g_object_get_data (G_OBJECT (widget), "gimp-accel-action");

      tooltip = gimp_action_get_tooltip (action);
      help_id = gimp_action_get_help_id (action);

      accel_key = gtk_accel_group_find (accel_group,
                                        gimp_widget_accel_find_func,
                                        accel_closure);

      if (accel_key            &&
          accel_key->accel_key &&
          (accel_key->accel_flags & GTK_ACCEL_VISIBLE))
        {
          gchar *escaped = g_markup_escape_text (tooltip, -1);
          gchar *accel   = gtk_accelerator_get_label (accel_key->accel_key,
                                                      accel_key->accel_mods);
          gchar *tmp     = g_strdup_printf ("%s  <b>%s</b>", escaped, accel);

          g_free (accel);
          g_free (escaped);

          gimp_help_set_help_data_with_markup (widget, tmp, help_id);
          g_free (tmp);
        }
      else
        {
          gimp_help_set_help_data (widget, tooltip, help_id);
        }
    }
}

static void   gimp_accel_help_widget_weak_notify (gpointer  accel_group,
                                                  GObject  *where_widget_was);

static void
gimp_accel_help_accel_group_weak_notify (gpointer  widget,
                                         GObject  *where_accel_group_was)
{
  g_object_weak_unref (widget,
                       gimp_accel_help_widget_weak_notify,
                       where_accel_group_was);

  g_object_set_data (widget, "gimp-accel-group", NULL);
}

static void
gimp_accel_help_widget_weak_notify (gpointer  accel_group,
                                    GObject  *where_widget_was)
{
  g_object_weak_unref (accel_group,
                       gimp_accel_help_accel_group_weak_notify,
                       where_widget_was);
}

void
gimp_widget_set_accel_help (GtkWidget  *widget,
                            GimpAction *action)
{
  GtkAccelGroup *accel_group;
  GClosure      *accel_closure;

  accel_group = g_object_get_data (G_OBJECT (widget), "gimp-accel-group");

  if (accel_group)
    {
      g_signal_handlers_disconnect_by_func (accel_group,
                                            gimp_widget_accel_changed,
                                            widget);
      g_object_weak_unref (G_OBJECT (accel_group),
                           gimp_accel_help_accel_group_weak_notify,
                           widget);
      g_object_weak_unref (G_OBJECT (widget),
                           gimp_accel_help_widget_weak_notify,
                           accel_group);
      g_object_set_data (G_OBJECT (widget), "gimp-accel-group", NULL);
    }

  accel_closure = gimp_action_get_accel_closure (action);

  if (accel_closure)
    {
      accel_group = gtk_accel_group_from_accel_closure (accel_closure);

      g_object_set_data (G_OBJECT (widget), "gimp-accel-group",
                         accel_group);
      g_object_weak_ref (G_OBJECT (accel_group),
                         gimp_accel_help_accel_group_weak_notify,
                         widget);
      g_object_weak_ref (G_OBJECT (widget),
                         gimp_accel_help_widget_weak_notify,
                         accel_group);

      g_object_set_data (G_OBJECT (widget), "gimp-accel-closure",
                         accel_closure);
      g_object_set_data (G_OBJECT (widget), "gimp-accel-action",
                         action);

      g_signal_connect_object (accel_group, "accel-changed",
                               G_CALLBACK (gimp_widget_accel_changed),
                               widget, 0);

      gimp_widget_accel_changed (accel_group,
                                 0, 0,
                                 accel_closure,
                                 widget);
    }
  else
    {
      gimp_help_set_help_data (widget,
                               gimp_action_get_tooltip (action),
                               gimp_action_get_help_id (action));

    }
}

const gchar *
gimp_get_message_icon_name (GimpMessageSeverity severity)
{
  switch (severity)
    {
    case GIMP_MESSAGE_INFO:
      return GIMP_ICON_DIALOG_INFORMATION;

    case GIMP_MESSAGE_WARNING:
      return GIMP_ICON_DIALOG_WARNING;

    case GIMP_MESSAGE_ERROR:
      return GIMP_ICON_DIALOG_ERROR;

    case GIMP_MESSAGE_BUG_WARNING:
    case GIMP_MESSAGE_BUG_CRITICAL:
      return GIMP_ICON_WILBER_EEK;
    }

  g_return_val_if_reached (GIMP_ICON_DIALOG_WARNING);
}

gboolean
gimp_get_color_tag_color (GimpColorTag  color_tag,
                          GimpRGB      *color,
                          gboolean      inherited)
{
  static const struct
  {
    guchar r;
    guchar g;
    guchar b;
  }
  colors[] =
  {
    {    0,   0,   0  }, /* none   */
    {   84, 102, 159  }, /* blue   */
    {  111, 143,  48  }, /* green  */
    {  210, 182,  45  }, /* yellow */
    {  217, 122,  38  }, /* orange */
    {   87,  53,  25  }, /* brown  */
    {  170,  42,  47  }, /* red    */
    {   99,  66, 174  }, /* violet */
    {   87,  87,  87  }  /* gray   */
  };

  g_return_val_if_fail (color != NULL, FALSE);
  g_return_val_if_fail (color_tag < G_N_ELEMENTS (colors), FALSE);

  if (color_tag > GIMP_COLOR_TAG_NONE)
    {
      gimp_rgba_set_uchar (color,
                           colors[color_tag].r,
                           colors[color_tag].g,
                           colors[color_tag].b,
                           255);

      if (inherited)
        {
          gimp_rgb_composite (color, &(GimpRGB) {1.0, 1.0, 1.0, 0.2},
                              GIMP_RGB_COMPOSITE_NORMAL);
        }

      return TRUE;
    }

  return FALSE;
}

void
gimp_pango_layout_set_scale (PangoLayout *layout,
                             gdouble      scale)
{
  PangoAttrList  *attrs;
  PangoAttribute *attr;

  g_return_if_fail (PANGO_IS_LAYOUT (layout));

  attrs = pango_attr_list_new ();

  attr = pango_attr_scale_new (scale);
  attr->start_index = 0;
  attr->end_index   = -1;
  pango_attr_list_insert (attrs, attr);

  pango_layout_set_attributes (layout, attrs);
  pango_attr_list_unref (attrs);
}

void
gimp_pango_layout_set_weight (PangoLayout *layout,
                              PangoWeight  weight)
{
  PangoAttrList  *attrs;
  PangoAttribute *attr;

  g_return_if_fail (PANGO_IS_LAYOUT (layout));

  attrs = pango_attr_list_new ();

  attr = pango_attr_weight_new (weight);
  attr->start_index = 0;
  attr->end_index   = -1;
  pango_attr_list_insert (attrs, attr);

  pango_layout_set_attributes (layout, attrs);
  pango_attr_list_unref (attrs);
}

static gboolean
gimp_highlight_widget_expose (GtkWidget      *widget,
                              GdkEventExpose *event,
                              gpointer        data)
{
  /* this code is a modified version of gtk+'s gtk_drag_highlight_expose(),
   * changing the highlight color from black to the widget's text color, which
   * improves its visibility when using a dark theme.
   */

  gint x, y, width, height;

  if (gtk_widget_is_drawable (widget))
    {
      GdkWindow      *window;
      GtkStyle       *style;
      const GdkColor *color;
      cairo_t        *cr;

      window = gtk_widget_get_window (widget);
      style  = gtk_widget_get_style (widget);

      if (!gtk_widget_get_has_window (widget))
        {
          GtkAllocation allocation;

          gtk_widget_get_allocation (widget, &allocation);

          x = allocation.x;
          y = allocation.y;
          width = allocation.width;
          height = allocation.height;
        }
      else
        {
          x = 0;
          y = 0;
          width = gdk_window_get_width (window);
          height = gdk_window_get_height (window);
        }

      gtk_paint_shadow (style, window,
                        GTK_STATE_NORMAL, GTK_SHADOW_OUT,
                        &event->area, widget, "dnd",
                        x, y, width, height);

      color = &style->text[GTK_STATE_NORMAL];

      cr = gdk_cairo_create (gtk_widget_get_window (widget));
      cairo_set_source_rgb (cr,
                            (gdouble) color->red   / 0xffff,
                            (gdouble) color->green / 0xffff,
                            (gdouble) color->blue  / 0xffff);
      cairo_set_line_width (cr, 1.0);
      cairo_rectangle (cr,
                       x + 0.5, y + 0.5,
                       width - 1, height - 1);
      cairo_stroke (cr);
      cairo_destroy (cr);
    }

  return FALSE;
}

/**
 * gimp_highlight_widget:
 * @widget:
 * @highlight:
 *
 * Turns highlighting for @widget on or off according to
 * @highlight, in a similar fashion to gtk_drag_highlight()
 * and gtk_drag_unhighlight().
 **/
void
gimp_highlight_widget (GtkWidget *widget,
                       gboolean   highlight)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));

  if (highlight)
    {
      g_signal_connect_after (widget, "expose-event",
                              G_CALLBACK (gimp_highlight_widget_expose),
                              NULL);
    }
  else
    {
      g_signal_handlers_disconnect_by_func (widget,
                                            gimp_highlight_widget_expose,
                                            NULL);
    }

  gtk_widget_queue_draw (widget);
}

typedef struct
{
  gint timeout_id;
  gint counter;
} WidgetBlink;

static WidgetBlink *
widget_blink_new (void)
{
  WidgetBlink *blink;

  blink = g_slice_new (WidgetBlink);

  blink->timeout_id = 0;
  blink->counter    = 0;

  return blink;
}

static void
widget_blink_free (WidgetBlink *blink)
{
  if (blink->timeout_id)
    {
      g_source_remove (blink->timeout_id);
      blink->timeout_id = 0;
    }

  g_slice_free (WidgetBlink, blink);
}

static gboolean
gimp_widget_blink_timeout (GtkWidget *widget)
{
  WidgetBlink *blink;

  blink = g_object_get_data (G_OBJECT (widget), "gimp-widget-blink");

  gimp_highlight_widget (widget, blink->counter % 2 == 1);
  blink->counter++;

  if (blink->counter == 3)
    {
      blink->timeout_id = 0;

      g_object_set_data (G_OBJECT (widget), "gimp-widget-blink", NULL);

      return G_SOURCE_REMOVE;
    }

  return G_SOURCE_CONTINUE;
}

void
gimp_widget_blink (GtkWidget *widget)
{
  WidgetBlink *blink;

  g_return_if_fail (GTK_IS_WIDGET (widget));

  blink = widget_blink_new ();

  g_object_set_data_full (G_OBJECT (widget), "gimp-widget-blink", blink,
                          (GDestroyNotify) widget_blink_free);

  blink->timeout_id = g_timeout_add (150,
                                     (GSourceFunc) gimp_widget_blink_timeout,
                                     widget);

  gimp_highlight_widget (widget, TRUE);

  while ((widget = gtk_widget_get_parent (widget)))
    gimp_widget_blink_cancel (widget);
}

void gimp_widget_blink_cancel (GtkWidget *widget)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));

  if (g_object_get_data (G_OBJECT (widget), "gimp-widget-blink"))
    {
      gimp_highlight_widget (widget, FALSE);

      g_object_set_data (G_OBJECT (widget), "gimp-widget-blink", NULL);
    }
}

/**
 * gimp_dock_with_window_new:
 * @factory: a #GimpDialogFacotry
 * @screen:  the #GdkScreen the dock window should appear on
 * @toolbox: if %TRUE; gives a "gimp-toolbox-window" with a
 *           "gimp-toolbox", "gimp-dock-window"+"gimp-dock"
 *           otherwise
 *
 * Returns: the newly created #GimpDock with the #GimpDockWindow
 **/
GtkWidget *
gimp_dock_with_window_new (GimpDialogFactory *factory,
                           GdkScreen         *screen,
                           gint               monitor,
                           gboolean           toolbox)
{
  GtkWidget         *dock_window;
  GimpDockContainer *dock_container;
  GtkWidget         *dock;
  GimpUIManager     *ui_manager;

  g_return_val_if_fail (GIMP_IS_DIALOG_FACTORY (factory), NULL);
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);

  /* Create a dock window to put the dock in. We need to create the
   * dock window before the dock because the dock has a dependency to
   * the ui manager in the dock window
   */
  dock_window = gimp_dialog_factory_dialog_new (factory, screen, monitor,
                                                NULL /*ui_manager*/,
                                                (toolbox ?
                                                 "gimp-toolbox-window" :
                                                 "gimp-dock-window"),
                                                -1 /*view_size*/,
                                                FALSE /*present*/);

  dock_container = GIMP_DOCK_CONTAINER (dock_window);
  ui_manager     = gimp_dock_container_get_ui_manager (dock_container);
  dock           = gimp_dialog_factory_dialog_new (factory,
                                                   screen,
                                                   monitor,
                                                   ui_manager,
                                                   (toolbox ?
                                                    "gimp-toolbox" :
                                                    "gimp-dock"),
                                                   -1 /*view_size*/,
                                                   FALSE /*present*/);

  if (dock)
    gimp_dock_window_add_dock (GIMP_DOCK_WINDOW (dock_window),
                               GIMP_DOCK (dock),
                               -1);

  return dock;
}

GtkWidget *
gimp_tools_get_tool_options_gui (GimpToolOptions *tool_options)
{
  GtkWidget *widget;

  widget = g_object_get_data (G_OBJECT (tool_options),
                              GIMP_TOOL_OPTIONS_GUI_KEY);

  if (! widget)
    {
      GimpToolOptionsGUIFunc func;

      func = g_object_get_data (G_OBJECT (tool_options),
                                GIMP_TOOL_OPTIONS_GUI_FUNC_KEY);

      if (func)
        {
          widget = func (tool_options);

          gimp_tools_set_tool_options_gui (tool_options, widget);
        }
    }

  return widget;
}

void
gimp_tools_set_tool_options_gui (GimpToolOptions *tool_options,
                                 GtkWidget       *widget)
{
  GtkWidget *prev_widget;

  prev_widget = g_object_get_data (G_OBJECT (tool_options),
                                   GIMP_TOOL_OPTIONS_GUI_KEY);

  if (widget == prev_widget)
    return;

  if (prev_widget)
    gtk_widget_destroy (prev_widget);

  g_object_set_data_full (G_OBJECT (tool_options),
                          GIMP_TOOL_OPTIONS_GUI_KEY,
                          widget ? g_object_ref_sink (widget)      : NULL,
                          widget ? (GDestroyNotify) g_object_unref : NULL);
}

void
gimp_tools_set_tool_options_gui_func (GimpToolOptions        *tool_options,
                                      GimpToolOptionsGUIFunc  func)
{
  g_object_set_data (G_OBJECT (tool_options),
                     GIMP_TOOL_OPTIONS_GUI_FUNC_KEY,
                     func);
}

void
gimp_widget_flush_expose (GtkWidget *widget)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));

  if (! gtk_widget_is_drawable (widget))
    return;

  gdk_window_process_updates (gtk_widget_get_window (widget), FALSE);
  gdk_flush ();
}

gboolean
gimp_widget_get_fully_opaque (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), FALSE);

  return g_object_get_data (G_OBJECT (widget),
                            "gimp-widget-fully-opaque") != NULL;
}

void
gimp_widget_set_fully_opaque (GtkWidget *widget,
                              gboolean   fully_opaque)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));

  return g_object_set_data (G_OBJECT (widget),
                            "gimp-widget-fully-opaque",
                            GINT_TO_POINTER (fully_opaque));
}

static void
gimp_gtk_container_clear_callback (GtkWidget    *widget,
                                   GtkContainer *container)
{
  gtk_container_remove (container, widget);
}

void
gimp_gtk_container_clear (GtkContainer *container)
{
  gtk_container_foreach (container,
                         (GtkCallback) gimp_gtk_container_clear_callback,
                         container);
}

void
gimp_gtk_adjustment_chain (GtkAdjustment *adjustment1,
                           GtkAdjustment *adjustment2)
{
  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment1));
  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment2));

  g_object_bind_property (adjustment1, "value",
                          adjustment2, "lower",
                          G_BINDING_SYNC_CREATE);
  g_object_bind_property (adjustment2, "value",
                          adjustment1, "upper",
                          G_BINDING_SYNC_CREATE);
}

static gboolean
gimp_print_event_free (gpointer data)
{
  g_free (data);

  return FALSE;
}

const gchar *
gimp_print_event (const GdkEvent *event)
{
  gchar *str;

  switch (event->type)
    {
    case GDK_ENTER_NOTIFY:
      str = g_strdup ("ENTER_NOTIFY");
      break;

    case GDK_LEAVE_NOTIFY:
      str = g_strdup ("LEAVE_NOTIFY");
      break;

    case GDK_PROXIMITY_IN:
      str = g_strdup ("PROXIMITY_IN");
      break;

    case GDK_PROXIMITY_OUT:
      str = g_strdup ("PROXIMITY_OUT");
      break;

    case GDK_FOCUS_CHANGE:
      if (event->focus_change.in)
        str = g_strdup ("FOCUS_IN");
      else
        str = g_strdup ("FOCUS_OUT");
      break;

    case GDK_BUTTON_PRESS:
      str = g_strdup_printf ("BUTTON_PRESS (%d @ %0.0f:%0.0f)",
                             event->button.button,
                             event->button.x,
                             event->button.y);
      break;

    case GDK_2BUTTON_PRESS:
      str = g_strdup_printf ("2BUTTON_PRESS (%d @ %0.0f:%0.0f)",
                             event->button.button,
                             event->button.x,
                             event->button.y);
      break;

    case GDK_3BUTTON_PRESS:
      str = g_strdup_printf ("3BUTTON_PRESS (%d @ %0.0f:%0.0f)",
                             event->button.button,
                             event->button.x,
                             event->button.y);
      break;

    case GDK_BUTTON_RELEASE:
      str = g_strdup_printf ("BUTTON_RELEASE (%d @ %0.0f:%0.0f)",
                             event->button.button,
                             event->button.x,
                             event->button.y);
      break;

    case GDK_SCROLL:
      str = g_strdup_printf ("SCROLL (%d)",
                             event->scroll.direction);
      break;

    case GDK_MOTION_NOTIFY:
      str = g_strdup_printf ("MOTION_NOTIFY (%0.0f:%0.0f %d)",
                             event->motion.x,
                             event->motion.y,
                             event->motion.time);
      break;

    case GDK_KEY_PRESS:
      str = g_strdup_printf ("KEY_PRESS (%d, %s)",
                             event->key.keyval,
                             gdk_keyval_name (event->key.keyval) ?
                             gdk_keyval_name (event->key.keyval) : "<none>");
      break;

    case GDK_KEY_RELEASE:
      str = g_strdup_printf ("KEY_RELEASE (%d, %s)",
                             event->key.keyval,
                             gdk_keyval_name (event->key.keyval) ?
                             gdk_keyval_name (event->key.keyval) : "<none>");
      break;

    default:
      str = g_strdup_printf ("UNHANDLED (type %d)",
                             event->type);
      break;
    }

  g_idle_add (gimp_print_event_free, str);

  return str;
}

gboolean
gimp_color_profile_store_add_defaults (GimpColorProfileStore  *store,
                                       GimpColorConfig        *config,
                                       GimpImageBaseType       base_type,
                                       GimpPrecision           precision,
                                       GError                **error)
{
  GimpColorProfile *profile;
  const Babl       *format;
  gchar            *label;
  GError           *my_error = NULL;

  g_return_val_if_fail (GIMP_IS_COLOR_PROFILE_STORE (store), FALSE);
  g_return_val_if_fail (GIMP_IS_COLOR_CONFIG (config), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  format  = gimp_babl_format (base_type, precision, TRUE);
  profile = gimp_babl_format_get_color_profile (format);

  if (base_type == GIMP_GRAY)
    {
      label = g_strdup_printf (_("Built-in grayscale (%s)"),
                               gimp_color_profile_get_label (profile));

      profile = gimp_color_config_get_gray_color_profile (config, &my_error);
    }
  else
    {
      label = g_strdup_printf (_("Built-in RGB (%s)"),
                               gimp_color_profile_get_label (profile));

      profile = gimp_color_config_get_rgb_color_profile (config, &my_error);
    }

  gimp_color_profile_store_add_file (store, NULL, label);
  g_free (label);

  if (profile)
    {
      GFile *file;

      if (base_type == GIMP_GRAY)
        {
          file = gimp_file_new_for_config_path (config->gray_profile, NULL);

          label = g_strdup_printf (_("Preferred grayscale (%s)"),
                                   gimp_color_profile_get_label (profile));
        }
      else
        {
          file = gimp_file_new_for_config_path (config->rgb_profile, NULL);

          label = g_strdup_printf (_("Preferred RGB (%s)"),
                                   gimp_color_profile_get_label (profile));
        }

      g_object_unref (profile);

      gimp_color_profile_store_add_file (store, file, label);

      g_object_unref (file);
      g_free (label);

      return TRUE;
    }
  else if (my_error)
    {
      g_propagate_error (error, my_error);

      return FALSE;
    }

  return TRUE;
}

static void
connect_path_show (GimpColorProfileChooserDialog *dialog)
{
  GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
  GFile          *file    = gtk_file_chooser_get_file (chooser);

  if (file)
    {
      /*  if something is already selected in this dialog,
       *  leave it alone
       */
      g_object_unref (file);
    }
  else
    {
      GObject     *config;
      const gchar *property;
      gchar       *path = NULL;

      config   = g_object_get_data (G_OBJECT (dialog), "profile-path-config");
      property = g_object_get_data (G_OBJECT (dialog), "profile-path-property");

      g_object_get (config, property, &path, NULL);

      if (path)
        {
          GFile *folder = gimp_file_new_for_config_path (path, NULL);

          if (folder)
            {
              gtk_file_chooser_set_current_folder_file (chooser, folder, NULL);
              g_object_unref (folder);
            }

          g_free (path);
        }
    }
}

static void
connect_path_response (GimpColorProfileChooserDialog *dialog,
                       gint                           response)
{
  if (response == GTK_RESPONSE_ACCEPT)
    {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      GFile          *file    = gtk_file_chooser_get_file (chooser);

      if (file)
        {
          GFile *folder = gtk_file_chooser_get_current_folder_file (chooser);

          if (folder)
            {
              GObject     *config;
              const gchar *property;
              gchar       *path = NULL;

              config   = g_object_get_data (G_OBJECT (dialog),
                                            "profile-path-config");
              property = g_object_get_data (G_OBJECT (dialog),
                                            "profile-path-property");

              path = gimp_file_get_config_path (folder, NULL);

              g_object_set (config, property, path, NULL);

              if (path)
                g_free (path);

              g_object_unref (folder);
            }

          g_object_unref (file);
        }
    }
}

void
gimp_color_profile_chooser_dialog_connect_path (GtkWidget   *dialog,
                                                GObject     *config,
                                                const gchar *property_name)
{
  g_return_if_fail (GIMP_IS_COLOR_PROFILE_CHOOSER_DIALOG (dialog));
  g_return_if_fail (G_IS_OBJECT (config));
  g_return_if_fail (property_name != NULL);

  g_object_set_data_full (G_OBJECT (dialog), "profile-path-config",
                          g_object_ref (config),
                          (GDestroyNotify) g_object_unref);
  g_object_set_data_full (G_OBJECT (dialog), "profile-path-property",
                          g_strdup (property_name),
                          (GDestroyNotify) g_free);

  g_signal_connect (dialog, "show",
                    G_CALLBACK (connect_path_show),
                    NULL);
  g_signal_connect (dialog, "response",
                    G_CALLBACK (connect_path_response),
                    NULL);
}
