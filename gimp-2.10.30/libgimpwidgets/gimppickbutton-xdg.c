/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimppickbutton-xdg.c
 * Copyright (C) 2021 Niels De Graef <nielsdegraef@gmail.com>
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gegl.h>
#include <gtk/gtk.h>

#include "libgimpcolor/gimpcolor.h"

#include "gimpwidgetstypes.h"
#include "gimppickbutton.h"
#include "gimppickbutton-default.h"
#include "gimppickbutton-xdg.h"

#include "libgimp/libgimp-intl.h"

gboolean
_gimp_pick_button_xdg_available (void)
{
  GDBusProxy *proxy = NULL;

  proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                         G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START,
                                         NULL,
                                         "org.freedesktop.portal.Desktop",
                                         "/org/freedesktop/portal/desktop",
                                         "org.freedesktop.portal.Screenshot",
                                         NULL, NULL);

  if (proxy)
    {
      GError *error = NULL;

      g_dbus_proxy_call_sync (proxy, "org.freedesktop.DBus.Peer.Ping",
                              NULL,
                              G_DBUS_CALL_FLAGS_NONE,
                              -1, NULL, &error);
      if (! error)
        return TRUE;

      g_clear_error (&error);

      g_object_unref (proxy);
      proxy = NULL;
    }

  return FALSE;
}

static void
pick_color_xdg_dbus_signal (GDBusProxy      *proxy,
                            gchar           *sender_name,
                            gchar           *signal_name,
                            GVariant        *parameters,
                            GimpPickButton  *button)
{
  if (g_strcmp0 (signal_name, "Response") == 0)
    {
      GVariant *results;
      guint32   response;

      g_variant_get (parameters, "(u@a{sv})",
                     &response,
                     &results);

      /* Possible values:
       * 0: Success, the request is carried out
       * 1: The user cancelled the interaction
       * 2: The user interaction was ended in some other way
       * Cf. https://github.com/flatpak/xdg-desktop-portal/blob/master/data/org.freedesktop.portal.Request.xml
       */
      if (response == 0)
        {
          GimpRGB color;

          if (g_variant_lookup (results, "color", "(ddd)", &color.r, &color.g, &color.b))
            {
              g_signal_emit_by_name (button, "color-picked", &color);
            }
        }

      g_variant_unref (results);
      /* Quit anyway. */
      gtk_main_quit ();
    }
}

/* entry point to this file, called from gimppickbutton.c */
void
_gimp_pick_button_xdg_pick (GimpPickButton *button)
{
  GDBusProxy *proxy         = NULL;
  GVariant   *retval        = NULL;
  gchar      *opath         = NULL;

  proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                         G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START,
                                         NULL,
                                         "org.freedesktop.portal.Desktop",
                                         "/org/freedesktop/portal/desktop",
                                         "org.freedesktop.portal.Screenshot",
                                         NULL, NULL);
  if (!proxy)
    {
      return;
    }

  retval = g_dbus_proxy_call_sync (proxy, "PickColor",
                                   g_variant_new ("(sa{sv})", "", NULL),
                                   G_DBUS_CALL_FLAGS_NONE,
                                   -1, NULL, NULL);
  g_clear_object (&proxy);
  if (retval)
    {
      g_variant_get (retval, "(o)", &opath);
      g_variant_unref (retval);
    }

  if (opath)
    {
      GDBusProxy *proxy2 = NULL;

      proxy2 = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                              G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START,
                                              NULL,
                                              "org.freedesktop.portal.Desktop",
                                              opath,
                                              "org.freedesktop.portal.Request",
                                              NULL, NULL);
      g_signal_connect (proxy2, "g-signal",
                        G_CALLBACK (pick_color_xdg_dbus_signal),
                        button);

      gtk_main ();
      g_object_unref (proxy2);
      g_free (opath);
    }
}
