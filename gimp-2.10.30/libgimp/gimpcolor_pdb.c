/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpcolor_pdb.c
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
 * SECTION: gimpcolor
 * @title: gimpcolor
 * @short_description: Functions for manipulating color.
 *
 * Functions for manipulating color, including curves and histograms.
 **/


/**
 * gimp_brightness_contrast:
 * @drawable_ID: The drawable.
 * @brightness: Brightness adjustment.
 * @contrast: Contrast adjustment.
 *
 * Deprecated: Use gimp_drawable_brightness_contrast() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_brightness_contrast (gint32 drawable_ID,
                          gint   brightness,
                          gint   contrast)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-brightness-contrast",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, brightness,
                                    GIMP_PDB_INT32, contrast,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_levels:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @low_input: Intensity of lowest input.
 * @high_input: Intensity of highest input.
 * @gamma: Gamma adjustment factor.
 * @low_output: Intensity of lowest output.
 * @high_output: Intensity of highest output.
 *
 * Deprecated: Use gimp_drawable_levels() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_levels (gint32               drawable_ID,
             GimpHistogramChannel channel,
             gint                 low_input,
             gint                 high_input,
             gdouble              gamma,
             gint                 low_output,
             gint                 high_output)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-levels",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, low_input,
                                    GIMP_PDB_INT32, high_input,
                                    GIMP_PDB_FLOAT, gamma,
                                    GIMP_PDB_INT32, low_output,
                                    GIMP_PDB_INT32, high_output,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_levels_auto:
 * @drawable_ID: The drawable.
 *
 * Deprecated: Use gimp_drawable_levels_stretch() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_levels_auto (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-levels-auto",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_levels_stretch:
 * @drawable_ID: The drawable.
 *
 * Deprecated: Use gimp_drawable_levels_stretch() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_levels_stretch (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-levels-stretch",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_posterize:
 * @drawable_ID: The drawable.
 * @levels: Levels of posterization.
 *
 * Deprecated: Use gimp_drawable_posterize() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_posterize (gint32 drawable_ID,
                gint   levels)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-posterize",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, levels,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_desaturate:
 * @drawable_ID: The drawable.
 *
 * Deprecated: Use gimp_drawable_desaturate() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_desaturate (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-desaturate",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_desaturate_full:
 * @drawable_ID: The drawable.
 * @desaturate_mode: The formula to use to desaturate.
 *
 * Deprecated: Use gimp_drawable_desaturate() instead.
 *
 * Returns: TRUE on success.
 *
 * Since: 2.4
 **/
gboolean
gimp_desaturate_full (gint32             drawable_ID,
                      GimpDesaturateMode desaturate_mode)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-desaturate-full",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, desaturate_mode,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_equalize:
 * @drawable_ID: The drawable.
 * @mask_only: Equalization option.
 *
 * Deprecated: Use gimp_drawable_equalize() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_equalize (gint32   drawable_ID,
               gboolean mask_only)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-equalize",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, mask_only,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_invert:
 * @drawable_ID: The drawable.
 *
 * Deprecated: Use gimp_drawable_invert() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_invert (gint32 drawable_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-invert",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_curves_spline:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @num_points: The number of values in the control point array.
 * @control_pts: The spline control points: { cp1.x, cp1.y, cp2.x, cp2.y, ... }.
 *
 * Deprecated: Use gimp_drawable_curves_spline() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_curves_spline (gint32                drawable_ID,
                    GimpHistogramChannel  channel,
                    gint                  num_points,
                    const guint8         *control_pts)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-curves-spline",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, num_points,
                                    GIMP_PDB_INT8ARRAY, control_pts,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_curves_explicit:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @num_bytes: The number of bytes in the new curve (always 256).
 * @curve: The explicit curve.
 *
 * Deprecated: Use gimp_drawable_curves_explicit() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_curves_explicit (gint32                drawable_ID,
                      GimpHistogramChannel  channel,
                      gint                  num_bytes,
                      const guint8         *curve)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-curves-explicit",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, num_bytes,
                                    GIMP_PDB_INT8ARRAY, curve,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_color_balance:
 * @drawable_ID: The drawable.
 * @transfer_mode: Transfer mode.
 * @preserve_lum: Preserve luminosity values at each pixel.
 * @cyan_red: Cyan-Red color balance.
 * @magenta_green: Magenta-Green color balance.
 * @yellow_blue: Yellow-Blue color balance.
 *
 * Modify the color balance of the specified drawable.
 *
 * Modify the color balance of the specified drawable. There are three
 * axis which can be modified: cyan-red, magenta-green, and
 * yellow-blue. Negative values increase the amount of the former,
 * positive values increase the amount of the latter. Color balance can
 * be controlled with the 'transfer_mode' setting, which allows
 * shadows, mid-tones, and highlights in an image to be affected
 * differently. The 'preserve-lum' parameter, if TRUE, ensures that the
 * luminosity of each pixel remains fixed.
 *
 * Deprecated: Use gimp_drawable_color_color_balance() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_color_balance (gint32           drawable_ID,
                    GimpTransferMode transfer_mode,
                    gboolean         preserve_lum,
                    gdouble          cyan_red,
                    gdouble          magenta_green,
                    gdouble          yellow_blue)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-color-balance",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, transfer_mode,
                                    GIMP_PDB_INT32, preserve_lum,
                                    GIMP_PDB_FLOAT, cyan_red,
                                    GIMP_PDB_FLOAT, magenta_green,
                                    GIMP_PDB_FLOAT, yellow_blue,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_colorize:
 * @drawable_ID: The drawable.
 * @hue: Hue in degrees.
 * @saturation: Saturation in percent.
 * @lightness: Lightness in percent.
 *
 * Deprecated: Use gimp_drawable_colorize_hsl() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_colorize (gint32  drawable_ID,
               gdouble hue,
               gdouble saturation,
               gdouble lightness)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-colorize",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_FLOAT, hue,
                                    GIMP_PDB_FLOAT, saturation,
                                    GIMP_PDB_FLOAT, lightness,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_histogram:
 * @drawable_ID: The drawable.
 * @channel: The channel to modify.
 * @start_range: Start of the intensity measurement range.
 * @end_range: End of the intensity measurement range.
 * @mean: Mean intensity value.
 * @std_dev: Standard deviation of intensity values.
 * @median: Median intensity value.
 * @pixels: Alpha-weighted pixel count for entire image.
 * @count: Alpha-weighted pixel count for range.
 * @percentile: Percentile that range falls under.
 *
 * Deprecated: Use gimp_drawable_histogram() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_histogram (gint32                drawable_ID,
                GimpHistogramChannel  channel,
                gint                  start_range,
                gint                  end_range,
                gdouble              *mean,
                gdouble              *std_dev,
                gdouble              *median,
                gdouble              *pixels,
                gdouble              *count,
                gdouble              *percentile)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-histogram",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, channel,
                                    GIMP_PDB_INT32, start_range,
                                    GIMP_PDB_INT32, end_range,
                                    GIMP_PDB_END);

  *mean = 0.0;
  *std_dev = 0.0;
  *median = 0.0;
  *pixels = 0.0;
  *count = 0.0;
  *percentile = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *mean = return_vals[1].data.d_float;
      *std_dev = return_vals[2].data.d_float;
      *median = return_vals[3].data.d_float;
      *pixels = return_vals[4].data.d_float;
      *count = return_vals[5].data.d_float;
      *percentile = return_vals[6].data.d_float;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_hue_saturation:
 * @drawable_ID: The drawable.
 * @hue_range: Range of affected hues.
 * @hue_offset: Hue offset in degrees.
 * @lightness: Lightness modification.
 * @saturation: Saturation modification.
 *
 * Deprecated: Use gimp_drawable_hue_saturation() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_hue_saturation (gint32       drawable_ID,
                     GimpHueRange hue_range,
                     gdouble      hue_offset,
                     gdouble      lightness,
                     gdouble      saturation)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-hue-saturation",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, hue_range,
                                    GIMP_PDB_FLOAT, hue_offset,
                                    GIMP_PDB_FLOAT, lightness,
                                    GIMP_PDB_FLOAT, saturation,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_threshold:
 * @drawable_ID: The drawable.
 * @low_threshold: The low threshold value.
 * @high_threshold: The high threshold value.
 *
 * Deprecated: Use gimp_drawable_threshold() instead.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_threshold (gint32 drawable_ID,
                gint   low_threshold,
                gint   high_threshold)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-threshold",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_INT32, low_threshold,
                                    GIMP_PDB_INT32, high_threshold,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}
