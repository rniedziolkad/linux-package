/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpimagesamplepoints_pdb.c
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
 * SECTION: gimpimagesamplepoints
 * @title: gimpimagesamplepoints
 * @short_description: Functions for manipulating an image's sample points.
 *
 * Functions for manipulating an image's sample points.
 **/


/**
 * gimp_image_add_sample_point:
 * @image_ID: The image.
 * @position_x: The guide'sample points x-offset from left of image.
 * @position_y: The guide'sample points y-offset from top of image.
 *
 * Add a sample point to an image.
 *
 * This procedure adds a sample point to an image. It takes the input
 * image and the position of the new sample points as parameters. It
 * returns the sample point ID of the new sample point.
 *
 * Returns: The new sample point.
 *
 * Since: 2.10
 **/
gint32
gimp_image_add_sample_point (gint32 image_ID,
                             gint   position_x,
                             gint   position_y)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 sample_point_ID = -1;

  return_vals = gimp_run_procedure ("gimp-image-add-sample-point",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, position_x,
                                    GIMP_PDB_INT32, position_y,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    sample_point_ID = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return sample_point_ID;
}

/**
 * gimp_image_delete_sample_point:
 * @image_ID: The image.
 * @sample_point_ID: The ID of the sample point to be removed.
 *
 * Deletes a sample point from an image.
 *
 * This procedure takes an image and a sample point ID as input and
 * removes the specified sample point from the specified image.
 *
 * Returns: TRUE on success.
 *
 * Since: 2.10
 **/
gboolean
gimp_image_delete_sample_point (gint32 image_ID,
                                gint32 sample_point_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-image-delete-sample-point",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, sample_point_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_image_find_next_sample_point:
 * @image_ID: The image.
 * @sample_point_ID: The ID of the current sample point (0 if first invocation).
 *
 * Find next sample point on an image.
 *
 * This procedure takes an image and a sample point ID as input and
 * finds the sample point ID of the successor of the given sample point
 * ID in the image's sample point list. If the supplied sample point ID
 * is 0, the procedure will return the first sample point. The
 * procedure will return 0 if given the final sample point ID as an
 * argument or the image has no sample points.
 *
 * Returns: The next sample point's ID.
 *
 * Since: 2.10
 **/
gint32
gimp_image_find_next_sample_point (gint32 image_ID,
                                   gint32 sample_point_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 next_sample_point_ID = -1;

  return_vals = gimp_run_procedure ("gimp-image-find-next-sample-point",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, sample_point_ID,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    next_sample_point_ID = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return next_sample_point_ID;
}

/**
 * gimp_image_get_sample_point_position:
 * @image_ID: The image.
 * @sample_point_ID: The guide.
 * @position_y: The sample points's position relative to top of image.
 *
 * Get position of a sample point on an image.
 *
 * This procedure takes an image and a sample point ID as input and
 * returns the position of the sample point relative to the top and
 * left of the image.
 *
 * Returns: The sample points's position relative to top of image.
 *
 * Since: 2.10
 **/
gint
gimp_image_get_sample_point_position (gint32  image_ID,
                                      gint32  sample_point_ID,
                                      gint   *position_y)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint position_x = G_MININT;

  return_vals = gimp_run_procedure ("gimp-image-get-sample-point-position",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, sample_point_ID,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    {
      position_x = return_vals[1].data.d_int32;
      *position_y = return_vals[2].data.d_int32;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return position_x;
}
