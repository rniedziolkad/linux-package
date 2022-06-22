
/* Generated data (by gimp-mkenums) */

#include "config.h"
#include <gio/gio.h>
#include <gegl.h>
#undef GIMP_DISABLE_DEPRECATED
#include "libgimpbase/gimpbase.h"
#include "libgimpbase/gimpbase-private.h"
#include "libgimpconfig/gimpconfigenums.h"
#include "gimpenums.h"

/* enumerations from "../../libgimp/gimpenums.h" */
GType
gimp_brush_application_mode_get_type (void)
{
  static const GEnumValue values[] =
  {
    { GIMP_BRUSH_HARD, "GIMP_BRUSH_HARD", "hard" },
    { GIMP_BRUSH_SOFT, "GIMP_BRUSH_SOFT", "soft" },
    { 0, NULL, NULL }
  };

  static const GimpEnumDesc descs[] =
  {
    { GIMP_BRUSH_HARD, "GIMP_BRUSH_HARD", NULL },
    { GIMP_BRUSH_SOFT, "GIMP_BRUSH_SOFT", NULL },
    { 0, NULL, NULL }
  };

  static GType type = 0;

  if (G_UNLIKELY (! type))
    {
      type = g_enum_register_static ("GimpBrushApplicationMode", values);
      gimp_type_set_translation_domain (type, GETTEXT_PACKAGE "-libgimp");
      gimp_type_set_translation_context (type, "brush-application-mode");
      gimp_enum_set_value_descriptions (type, descs);
    }

  return type;
}

GType
gimp_convert_dither_type_get_type (void)
{
  static const GEnumValue values[] =
  {
    { GIMP_CONVERT_DITHER_NONE, "GIMP_CONVERT_DITHER_NONE", "none" },
    { GIMP_CONVERT_DITHER_FS, "GIMP_CONVERT_DITHER_FS", "fs" },
    { GIMP_CONVERT_DITHER_FS_LOWBLEED, "GIMP_CONVERT_DITHER_FS_LOWBLEED", "fs-lowbleed" },
    { GIMP_CONVERT_DITHER_FIXED, "GIMP_CONVERT_DITHER_FIXED", "fixed" },
    { 0, NULL, NULL }
  };

  static const GimpEnumDesc descs[] =
  {
    { GIMP_CONVERT_DITHER_NONE, "GIMP_CONVERT_DITHER_NONE", NULL },
    { GIMP_CONVERT_DITHER_FS, "GIMP_CONVERT_DITHER_FS", NULL },
    { GIMP_CONVERT_DITHER_FS_LOWBLEED, "GIMP_CONVERT_DITHER_FS_LOWBLEED", NULL },
    { GIMP_CONVERT_DITHER_FIXED, "GIMP_CONVERT_DITHER_FIXED", NULL },
    { 0, NULL, NULL }
  };

  static GType type = 0;

  if (G_UNLIKELY (! type))
    {
      type = g_enum_register_static ("GimpConvertDitherType", values);
      gimp_type_set_translation_domain (type, GETTEXT_PACKAGE "-libgimp");
      gimp_type_set_translation_context (type, "convert-dither-type");
      gimp_enum_set_value_descriptions (type, descs);
    }

  return type;
}

GType
gimp_histogram_channel_get_type (void)
{
  static const GEnumValue values[] =
  {
    { GIMP_HISTOGRAM_VALUE, "GIMP_HISTOGRAM_VALUE", "value" },
    { GIMP_HISTOGRAM_RED, "GIMP_HISTOGRAM_RED", "red" },
    { GIMP_HISTOGRAM_GREEN, "GIMP_HISTOGRAM_GREEN", "green" },
    { GIMP_HISTOGRAM_BLUE, "GIMP_HISTOGRAM_BLUE", "blue" },
    { GIMP_HISTOGRAM_ALPHA, "GIMP_HISTOGRAM_ALPHA", "alpha" },
    { GIMP_HISTOGRAM_LUMINANCE, "GIMP_HISTOGRAM_LUMINANCE", "luminance" },
    { 0, NULL, NULL }
  };

  static const GimpEnumDesc descs[] =
  {
    { GIMP_HISTOGRAM_VALUE, "GIMP_HISTOGRAM_VALUE", NULL },
    { GIMP_HISTOGRAM_RED, "GIMP_HISTOGRAM_RED", NULL },
    { GIMP_HISTOGRAM_GREEN, "GIMP_HISTOGRAM_GREEN", NULL },
    { GIMP_HISTOGRAM_BLUE, "GIMP_HISTOGRAM_BLUE", NULL },
    { GIMP_HISTOGRAM_ALPHA, "GIMP_HISTOGRAM_ALPHA", NULL },
    { GIMP_HISTOGRAM_LUMINANCE, "GIMP_HISTOGRAM_LUMINANCE", NULL },
    { 0, NULL, NULL }
  };

  static GType type = 0;

  if (G_UNLIKELY (! type))
    {
      type = g_enum_register_static ("GimpHistogramChannel", values);
      gimp_type_set_translation_domain (type, GETTEXT_PACKAGE "-libgimp");
      gimp_type_set_translation_context (type, "histogram-channel");
      gimp_enum_set_value_descriptions (type, descs);
    }

  return type;
}

GType
gimp_layer_color_space_get_type (void)
{
  static const GEnumValue values[] =
  {
    { GIMP_LAYER_COLOR_SPACE_AUTO, "GIMP_LAYER_COLOR_SPACE_AUTO", "auto" },
    { GIMP_LAYER_COLOR_SPACE_RGB_LINEAR, "GIMP_LAYER_COLOR_SPACE_RGB_LINEAR", "rgb-linear" },
    { GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL, "GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL", "rgb-perceptual" },
    { 0, NULL, NULL }
  };

  static const GimpEnumDesc descs[] =
  {
    { GIMP_LAYER_COLOR_SPACE_AUTO, "GIMP_LAYER_COLOR_SPACE_AUTO", NULL },
    { GIMP_LAYER_COLOR_SPACE_RGB_LINEAR, "GIMP_LAYER_COLOR_SPACE_RGB_LINEAR", NULL },
    { GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL, "GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL", NULL },
    { 0, NULL, NULL }
  };

  static GType type = 0;

  if (G_UNLIKELY (! type))
    {
      type = g_enum_register_static ("GimpLayerColorSpace", values);
      gimp_type_set_translation_domain (type, GETTEXT_PACKAGE "-libgimp");
      gimp_type_set_translation_context (type, "layer-color-space");
      gimp_enum_set_value_descriptions (type, descs);
    }

  return type;
}

GType
gimp_layer_composite_mode_get_type (void)
{
  static const GEnumValue values[] =
  {
    { GIMP_LAYER_COMPOSITE_AUTO, "GIMP_LAYER_COMPOSITE_AUTO", "auto" },
    { GIMP_LAYER_COMPOSITE_UNION, "GIMP_LAYER_COMPOSITE_UNION", "union" },
    { GIMP_LAYER_COMPOSITE_CLIP_TO_BACKDROP, "GIMP_LAYER_COMPOSITE_CLIP_TO_BACKDROP", "clip-to-backdrop" },
    { GIMP_LAYER_COMPOSITE_CLIP_TO_LAYER, "GIMP_LAYER_COMPOSITE_CLIP_TO_LAYER", "clip-to-layer" },
    { GIMP_LAYER_COMPOSITE_INTERSECTION, "GIMP_LAYER_COMPOSITE_INTERSECTION", "intersection" },
    { 0, NULL, NULL }
  };

  static const GimpEnumDesc descs[] =
  {
    { GIMP_LAYER_COMPOSITE_AUTO, "GIMP_LAYER_COMPOSITE_AUTO", NULL },
    { GIMP_LAYER_COMPOSITE_UNION, "GIMP_LAYER_COMPOSITE_UNION", NULL },
    { GIMP_LAYER_COMPOSITE_CLIP_TO_BACKDROP, "GIMP_LAYER_COMPOSITE_CLIP_TO_BACKDROP", NULL },
    { GIMP_LAYER_COMPOSITE_CLIP_TO_LAYER, "GIMP_LAYER_COMPOSITE_CLIP_TO_LAYER", NULL },
    { GIMP_LAYER_COMPOSITE_INTERSECTION, "GIMP_LAYER_COMPOSITE_INTERSECTION", NULL },
    { 0, NULL, NULL }
  };

  static GType type = 0;

  if (G_UNLIKELY (! type))
    {
      type = g_enum_register_static ("GimpLayerCompositeMode", values);
      gimp_type_set_translation_domain (type, GETTEXT_PACKAGE "-libgimp");
      gimp_type_set_translation_context (type, "layer-composite-mode");
      gimp_enum_set_value_descriptions (type, descs);
    }

  return type;
}

GType
gimp_layer_mode_get_type (void)
{
  static const GEnumValue values[] =
  {
    { GIMP_LAYER_MODE_NORMAL_LEGACY, "GIMP_LAYER_MODE_NORMAL_LEGACY", "normal-legacy" },
    { GIMP_LAYER_MODE_DISSOLVE, "GIMP_LAYER_MODE_DISSOLVE", "dissolve" },
    { GIMP_LAYER_MODE_BEHIND_LEGACY, "GIMP_LAYER_MODE_BEHIND_LEGACY", "behind-legacy" },
    { GIMP_LAYER_MODE_MULTIPLY_LEGACY, "GIMP_LAYER_MODE_MULTIPLY_LEGACY", "multiply-legacy" },
    { GIMP_LAYER_MODE_SCREEN_LEGACY, "GIMP_LAYER_MODE_SCREEN_LEGACY", "screen-legacy" },
    { GIMP_LAYER_MODE_OVERLAY_LEGACY, "GIMP_LAYER_MODE_OVERLAY_LEGACY", "overlay-legacy" },
    { GIMP_LAYER_MODE_DIFFERENCE_LEGACY, "GIMP_LAYER_MODE_DIFFERENCE_LEGACY", "difference-legacy" },
    { GIMP_LAYER_MODE_ADDITION_LEGACY, "GIMP_LAYER_MODE_ADDITION_LEGACY", "addition-legacy" },
    { GIMP_LAYER_MODE_SUBTRACT_LEGACY, "GIMP_LAYER_MODE_SUBTRACT_LEGACY", "subtract-legacy" },
    { GIMP_LAYER_MODE_DARKEN_ONLY_LEGACY, "GIMP_LAYER_MODE_DARKEN_ONLY_LEGACY", "darken-only-legacy" },
    { GIMP_LAYER_MODE_LIGHTEN_ONLY_LEGACY, "GIMP_LAYER_MODE_LIGHTEN_ONLY_LEGACY", "lighten-only-legacy" },
    { GIMP_LAYER_MODE_HSV_HUE_LEGACY, "GIMP_LAYER_MODE_HSV_HUE_LEGACY", "hsv-hue-legacy" },
    { GIMP_LAYER_MODE_HSV_SATURATION_LEGACY, "GIMP_LAYER_MODE_HSV_SATURATION_LEGACY", "hsv-saturation-legacy" },
    { GIMP_LAYER_MODE_HSL_COLOR_LEGACY, "GIMP_LAYER_MODE_HSL_COLOR_LEGACY", "hsl-color-legacy" },
    { GIMP_LAYER_MODE_HSV_VALUE_LEGACY, "GIMP_LAYER_MODE_HSV_VALUE_LEGACY", "hsv-value-legacy" },
    { GIMP_LAYER_MODE_DIVIDE_LEGACY, "GIMP_LAYER_MODE_DIVIDE_LEGACY", "divide-legacy" },
    { GIMP_LAYER_MODE_DODGE_LEGACY, "GIMP_LAYER_MODE_DODGE_LEGACY", "dodge-legacy" },
    { GIMP_LAYER_MODE_BURN_LEGACY, "GIMP_LAYER_MODE_BURN_LEGACY", "burn-legacy" },
    { GIMP_LAYER_MODE_HARDLIGHT_LEGACY, "GIMP_LAYER_MODE_HARDLIGHT_LEGACY", "hardlight-legacy" },
    { GIMP_LAYER_MODE_SOFTLIGHT_LEGACY, "GIMP_LAYER_MODE_SOFTLIGHT_LEGACY", "softlight-legacy" },
    { GIMP_LAYER_MODE_GRAIN_EXTRACT_LEGACY, "GIMP_LAYER_MODE_GRAIN_EXTRACT_LEGACY", "grain-extract-legacy" },
    { GIMP_LAYER_MODE_GRAIN_MERGE_LEGACY, "GIMP_LAYER_MODE_GRAIN_MERGE_LEGACY", "grain-merge-legacy" },
    { GIMP_LAYER_MODE_COLOR_ERASE_LEGACY, "GIMP_LAYER_MODE_COLOR_ERASE_LEGACY", "color-erase-legacy" },
    { GIMP_LAYER_MODE_OVERLAY, "GIMP_LAYER_MODE_OVERLAY", "overlay" },
    { GIMP_LAYER_MODE_LCH_HUE, "GIMP_LAYER_MODE_LCH_HUE", "lch-hue" },
    { GIMP_LAYER_MODE_LCH_CHROMA, "GIMP_LAYER_MODE_LCH_CHROMA", "lch-chroma" },
    { GIMP_LAYER_MODE_LCH_COLOR, "GIMP_LAYER_MODE_LCH_COLOR", "lch-color" },
    { GIMP_LAYER_MODE_LCH_LIGHTNESS, "GIMP_LAYER_MODE_LCH_LIGHTNESS", "lch-lightness" },
    { GIMP_LAYER_MODE_NORMAL, "GIMP_LAYER_MODE_NORMAL", "normal" },
    { GIMP_LAYER_MODE_BEHIND, "GIMP_LAYER_MODE_BEHIND", "behind" },
    { GIMP_LAYER_MODE_MULTIPLY, "GIMP_LAYER_MODE_MULTIPLY", "multiply" },
    { GIMP_LAYER_MODE_SCREEN, "GIMP_LAYER_MODE_SCREEN", "screen" },
    { GIMP_LAYER_MODE_DIFFERENCE, "GIMP_LAYER_MODE_DIFFERENCE", "difference" },
    { GIMP_LAYER_MODE_ADDITION, "GIMP_LAYER_MODE_ADDITION", "addition" },
    { GIMP_LAYER_MODE_SUBTRACT, "GIMP_LAYER_MODE_SUBTRACT", "subtract" },
    { GIMP_LAYER_MODE_DARKEN_ONLY, "GIMP_LAYER_MODE_DARKEN_ONLY", "darken-only" },
    { GIMP_LAYER_MODE_LIGHTEN_ONLY, "GIMP_LAYER_MODE_LIGHTEN_ONLY", "lighten-only" },
    { GIMP_LAYER_MODE_HSV_HUE, "GIMP_LAYER_MODE_HSV_HUE", "hsv-hue" },
    { GIMP_LAYER_MODE_HSV_SATURATION, "GIMP_LAYER_MODE_HSV_SATURATION", "hsv-saturation" },
    { GIMP_LAYER_MODE_HSL_COLOR, "GIMP_LAYER_MODE_HSL_COLOR", "hsl-color" },
    { GIMP_LAYER_MODE_HSV_VALUE, "GIMP_LAYER_MODE_HSV_VALUE", "hsv-value" },
    { GIMP_LAYER_MODE_DIVIDE, "GIMP_LAYER_MODE_DIVIDE", "divide" },
    { GIMP_LAYER_MODE_DODGE, "GIMP_LAYER_MODE_DODGE", "dodge" },
    { GIMP_LAYER_MODE_BURN, "GIMP_LAYER_MODE_BURN", "burn" },
    { GIMP_LAYER_MODE_HARDLIGHT, "GIMP_LAYER_MODE_HARDLIGHT", "hardlight" },
    { GIMP_LAYER_MODE_SOFTLIGHT, "GIMP_LAYER_MODE_SOFTLIGHT", "softlight" },
    { GIMP_LAYER_MODE_GRAIN_EXTRACT, "GIMP_LAYER_MODE_GRAIN_EXTRACT", "grain-extract" },
    { GIMP_LAYER_MODE_GRAIN_MERGE, "GIMP_LAYER_MODE_GRAIN_MERGE", "grain-merge" },
    { GIMP_LAYER_MODE_VIVID_LIGHT, "GIMP_LAYER_MODE_VIVID_LIGHT", "vivid-light" },
    { GIMP_LAYER_MODE_PIN_LIGHT, "GIMP_LAYER_MODE_PIN_LIGHT", "pin-light" },
    { GIMP_LAYER_MODE_LINEAR_LIGHT, "GIMP_LAYER_MODE_LINEAR_LIGHT", "linear-light" },
    { GIMP_LAYER_MODE_HARD_MIX, "GIMP_LAYER_MODE_HARD_MIX", "hard-mix" },
    { GIMP_LAYER_MODE_EXCLUSION, "GIMP_LAYER_MODE_EXCLUSION", "exclusion" },
    { GIMP_LAYER_MODE_LINEAR_BURN, "GIMP_LAYER_MODE_LINEAR_BURN", "linear-burn" },
    { GIMP_LAYER_MODE_LUMA_DARKEN_ONLY, "GIMP_LAYER_MODE_LUMA_DARKEN_ONLY", "luma-darken-only" },
    { GIMP_LAYER_MODE_LUMA_LIGHTEN_ONLY, "GIMP_LAYER_MODE_LUMA_LIGHTEN_ONLY", "luma-lighten-only" },
    { GIMP_LAYER_MODE_LUMINANCE, "GIMP_LAYER_MODE_LUMINANCE", "luminance" },
    { GIMP_LAYER_MODE_COLOR_ERASE, "GIMP_LAYER_MODE_COLOR_ERASE", "color-erase" },
    { GIMP_LAYER_MODE_ERASE, "GIMP_LAYER_MODE_ERASE", "erase" },
    { GIMP_LAYER_MODE_MERGE, "GIMP_LAYER_MODE_MERGE", "merge" },
    { GIMP_LAYER_MODE_SPLIT, "GIMP_LAYER_MODE_SPLIT", "split" },
    { GIMP_LAYER_MODE_PASS_THROUGH, "GIMP_LAYER_MODE_PASS_THROUGH", "pass-through" },
    { 0, NULL, NULL }
  };

  static const GimpEnumDesc descs[] =
  {
    { GIMP_LAYER_MODE_NORMAL_LEGACY, "GIMP_LAYER_MODE_NORMAL_LEGACY", NULL },
    { GIMP_LAYER_MODE_DISSOLVE, "GIMP_LAYER_MODE_DISSOLVE", NULL },
    { GIMP_LAYER_MODE_BEHIND_LEGACY, "GIMP_LAYER_MODE_BEHIND_LEGACY", NULL },
    { GIMP_LAYER_MODE_MULTIPLY_LEGACY, "GIMP_LAYER_MODE_MULTIPLY_LEGACY", NULL },
    { GIMP_LAYER_MODE_SCREEN_LEGACY, "GIMP_LAYER_MODE_SCREEN_LEGACY", NULL },
    { GIMP_LAYER_MODE_OVERLAY_LEGACY, "GIMP_LAYER_MODE_OVERLAY_LEGACY", NULL },
    { GIMP_LAYER_MODE_DIFFERENCE_LEGACY, "GIMP_LAYER_MODE_DIFFERENCE_LEGACY", NULL },
    { GIMP_LAYER_MODE_ADDITION_LEGACY, "GIMP_LAYER_MODE_ADDITION_LEGACY", NULL },
    { GIMP_LAYER_MODE_SUBTRACT_LEGACY, "GIMP_LAYER_MODE_SUBTRACT_LEGACY", NULL },
    { GIMP_LAYER_MODE_DARKEN_ONLY_LEGACY, "GIMP_LAYER_MODE_DARKEN_ONLY_LEGACY", NULL },
    { GIMP_LAYER_MODE_LIGHTEN_ONLY_LEGACY, "GIMP_LAYER_MODE_LIGHTEN_ONLY_LEGACY", NULL },
    { GIMP_LAYER_MODE_HSV_HUE_LEGACY, "GIMP_LAYER_MODE_HSV_HUE_LEGACY", NULL },
    { GIMP_LAYER_MODE_HSV_SATURATION_LEGACY, "GIMP_LAYER_MODE_HSV_SATURATION_LEGACY", NULL },
    { GIMP_LAYER_MODE_HSL_COLOR_LEGACY, "GIMP_LAYER_MODE_HSL_COLOR_LEGACY", NULL },
    { GIMP_LAYER_MODE_HSV_VALUE_LEGACY, "GIMP_LAYER_MODE_HSV_VALUE_LEGACY", NULL },
    { GIMP_LAYER_MODE_DIVIDE_LEGACY, "GIMP_LAYER_MODE_DIVIDE_LEGACY", NULL },
    { GIMP_LAYER_MODE_DODGE_LEGACY, "GIMP_LAYER_MODE_DODGE_LEGACY", NULL },
    { GIMP_LAYER_MODE_BURN_LEGACY, "GIMP_LAYER_MODE_BURN_LEGACY", NULL },
    { GIMP_LAYER_MODE_HARDLIGHT_LEGACY, "GIMP_LAYER_MODE_HARDLIGHT_LEGACY", NULL },
    { GIMP_LAYER_MODE_SOFTLIGHT_LEGACY, "GIMP_LAYER_MODE_SOFTLIGHT_LEGACY", NULL },
    { GIMP_LAYER_MODE_GRAIN_EXTRACT_LEGACY, "GIMP_LAYER_MODE_GRAIN_EXTRACT_LEGACY", NULL },
    { GIMP_LAYER_MODE_GRAIN_MERGE_LEGACY, "GIMP_LAYER_MODE_GRAIN_MERGE_LEGACY", NULL },
    { GIMP_LAYER_MODE_COLOR_ERASE_LEGACY, "GIMP_LAYER_MODE_COLOR_ERASE_LEGACY", NULL },
    { GIMP_LAYER_MODE_OVERLAY, "GIMP_LAYER_MODE_OVERLAY", NULL },
    { GIMP_LAYER_MODE_LCH_HUE, "GIMP_LAYER_MODE_LCH_HUE", NULL },
    { GIMP_LAYER_MODE_LCH_CHROMA, "GIMP_LAYER_MODE_LCH_CHROMA", NULL },
    { GIMP_LAYER_MODE_LCH_COLOR, "GIMP_LAYER_MODE_LCH_COLOR", NULL },
    { GIMP_LAYER_MODE_LCH_LIGHTNESS, "GIMP_LAYER_MODE_LCH_LIGHTNESS", NULL },
    { GIMP_LAYER_MODE_NORMAL, "GIMP_LAYER_MODE_NORMAL", NULL },
    { GIMP_LAYER_MODE_BEHIND, "GIMP_LAYER_MODE_BEHIND", NULL },
    { GIMP_LAYER_MODE_MULTIPLY, "GIMP_LAYER_MODE_MULTIPLY", NULL },
    { GIMP_LAYER_MODE_SCREEN, "GIMP_LAYER_MODE_SCREEN", NULL },
    { GIMP_LAYER_MODE_DIFFERENCE, "GIMP_LAYER_MODE_DIFFERENCE", NULL },
    { GIMP_LAYER_MODE_ADDITION, "GIMP_LAYER_MODE_ADDITION", NULL },
    { GIMP_LAYER_MODE_SUBTRACT, "GIMP_LAYER_MODE_SUBTRACT", NULL },
    { GIMP_LAYER_MODE_DARKEN_ONLY, "GIMP_LAYER_MODE_DARKEN_ONLY", NULL },
    { GIMP_LAYER_MODE_LIGHTEN_ONLY, "GIMP_LAYER_MODE_LIGHTEN_ONLY", NULL },
    { GIMP_LAYER_MODE_HSV_HUE, "GIMP_LAYER_MODE_HSV_HUE", NULL },
    { GIMP_LAYER_MODE_HSV_SATURATION, "GIMP_LAYER_MODE_HSV_SATURATION", NULL },
    { GIMP_LAYER_MODE_HSL_COLOR, "GIMP_LAYER_MODE_HSL_COLOR", NULL },
    { GIMP_LAYER_MODE_HSV_VALUE, "GIMP_LAYER_MODE_HSV_VALUE", NULL },
    { GIMP_LAYER_MODE_DIVIDE, "GIMP_LAYER_MODE_DIVIDE", NULL },
    { GIMP_LAYER_MODE_DODGE, "GIMP_LAYER_MODE_DODGE", NULL },
    { GIMP_LAYER_MODE_BURN, "GIMP_LAYER_MODE_BURN", NULL },
    { GIMP_LAYER_MODE_HARDLIGHT, "GIMP_LAYER_MODE_HARDLIGHT", NULL },
    { GIMP_LAYER_MODE_SOFTLIGHT, "GIMP_LAYER_MODE_SOFTLIGHT", NULL },
    { GIMP_LAYER_MODE_GRAIN_EXTRACT, "GIMP_LAYER_MODE_GRAIN_EXTRACT", NULL },
    { GIMP_LAYER_MODE_GRAIN_MERGE, "GIMP_LAYER_MODE_GRAIN_MERGE", NULL },
    { GIMP_LAYER_MODE_VIVID_LIGHT, "GIMP_LAYER_MODE_VIVID_LIGHT", NULL },
    { GIMP_LAYER_MODE_PIN_LIGHT, "GIMP_LAYER_MODE_PIN_LIGHT", NULL },
    { GIMP_LAYER_MODE_LINEAR_LIGHT, "GIMP_LAYER_MODE_LINEAR_LIGHT", NULL },
    { GIMP_LAYER_MODE_HARD_MIX, "GIMP_LAYER_MODE_HARD_MIX", NULL },
    { GIMP_LAYER_MODE_EXCLUSION, "GIMP_LAYER_MODE_EXCLUSION", NULL },
    { GIMP_LAYER_MODE_LINEAR_BURN, "GIMP_LAYER_MODE_LINEAR_BURN", NULL },
    { GIMP_LAYER_MODE_LUMA_DARKEN_ONLY, "GIMP_LAYER_MODE_LUMA_DARKEN_ONLY", NULL },
    { GIMP_LAYER_MODE_LUMA_LIGHTEN_ONLY, "GIMP_LAYER_MODE_LUMA_LIGHTEN_ONLY", NULL },
    { GIMP_LAYER_MODE_LUMINANCE, "GIMP_LAYER_MODE_LUMINANCE", NULL },
    { GIMP_LAYER_MODE_COLOR_ERASE, "GIMP_LAYER_MODE_COLOR_ERASE", NULL },
    { GIMP_LAYER_MODE_ERASE, "GIMP_LAYER_MODE_ERASE", NULL },
    { GIMP_LAYER_MODE_MERGE, "GIMP_LAYER_MODE_MERGE", NULL },
    { GIMP_LAYER_MODE_SPLIT, "GIMP_LAYER_MODE_SPLIT", NULL },
    { GIMP_LAYER_MODE_PASS_THROUGH, "GIMP_LAYER_MODE_PASS_THROUGH", NULL },
    { 0, NULL, NULL }
  };

  static GType type = 0;

  if (G_UNLIKELY (! type))
    {
      type = g_enum_register_static ("GimpLayerMode", values);
      gimp_type_set_translation_domain (type, GETTEXT_PACKAGE "-libgimp");
      gimp_type_set_translation_context (type, "layer-mode");
      gimp_enum_set_value_descriptions (type, descs);
    }

  return type;
}


/* Generated data ends here */


typedef GType (* GimpGetTypeFunc) (void);

static const GimpGetTypeFunc get_type_funcs[] =
{
  gegl_distance_metric_get_type,
  gimp_add_mask_type_get_type,
  gimp_blend_mode_get_type,
  gimp_brush_application_mode_get_type,
  gimp_brush_generated_shape_get_type,
  gimp_bucket_fill_mode_get_type,
  gimp_cap_style_get_type,
  gimp_channel_ops_get_type,
  gimp_channel_type_get_type,
  gimp_clone_type_get_type,
  gimp_color_management_mode_get_type,
  gimp_color_rendering_intent_get_type,
  gimp_color_tag_get_type,
  gimp_component_type_get_type,
  gimp_convert_dither_type_get_type,
  gimp_convert_palette_type_get_type,
  gimp_convolve_type_get_type,
  gimp_desaturate_mode_get_type,
  gimp_dodge_burn_type_get_type,
  gimp_fill_type_get_type,
  gimp_foreground_extract_mode_get_type,
  gimp_gradient_blend_color_space_get_type,
  gimp_gradient_segment_color_get_type,
  gimp_gradient_segment_type_get_type,
  gimp_gradient_type_get_type,
  gimp_grid_style_get_type,
  gimp_histogram_channel_get_type,
  gimp_hue_range_get_type,
  gimp_icon_type_get_type,
  gimp_image_base_type_get_type,
  gimp_image_type_get_type,
  gimp_ink_blob_type_get_type,
  gimp_interpolation_type_get_type,
  gimp_join_style_get_type,
  gimp_layer_color_space_get_type,
  gimp_layer_composite_mode_get_type,
  gimp_layer_mode_get_type,
  gimp_mask_apply_mode_get_type,
  gimp_merge_type_get_type,
  gimp_message_handler_type_get_type,
  gimp_offset_type_get_type,
  gimp_orientation_type_get_type,
  gimp_pdb_arg_type_get_type,
  gimp_pdb_error_handler_get_type,
  gimp_pdb_proc_type_get_type,
  gimp_pdb_status_type_get_type,
  gimp_paint_application_mode_get_type,
  gimp_precision_get_type,
  gimp_progress_command_get_type,
  gimp_repeat_mode_get_type,
  gimp_rotation_type_get_type,
  gimp_run_mode_get_type,
  gimp_select_criterion_get_type,
  gimp_size_type_get_type,
  gimp_stack_trace_mode_get_type,
  gimp_stroke_method_get_type,
  gimp_text_direction_get_type,
  gimp_text_hint_style_get_type,
  gimp_text_justification_get_type,
  gimp_transfer_mode_get_type,
  gimp_transform_direction_get_type,
  gimp_transform_resize_get_type,
  gimp_user_directory_get_type,
  gimp_vectors_stroke_type_get_type
};

static const gchar * const type_names[] =
{
  "GeglDistanceMetric",
  "GimpAddMaskType",
  "GimpBlendMode",
  "GimpBrushApplicationMode",
  "GimpBrushGeneratedShape",
  "GimpBucketFillMode",
  "GimpCapStyle",
  "GimpChannelOps",
  "GimpChannelType",
  "GimpCloneType",
  "GimpColorManagementMode",
  "GimpColorRenderingIntent",
  "GimpColorTag",
  "GimpComponentType",
  "GimpConvertDitherType",
  "GimpConvertPaletteType",
  "GimpConvolveType",
  "GimpDesaturateMode",
  "GimpDodgeBurnType",
  "GimpFillType",
  "GimpForegroundExtractMode",
  "GimpGradientBlendColorSpace",
  "GimpGradientSegmentColor",
  "GimpGradientSegmentType",
  "GimpGradientType",
  "GimpGridStyle",
  "GimpHistogramChannel",
  "GimpHueRange",
  "GimpIconType",
  "GimpImageBaseType",
  "GimpImageType",
  "GimpInkBlobType",
  "GimpInterpolationType",
  "GimpJoinStyle",
  "GimpLayerColorSpace",
  "GimpLayerCompositeMode",
  "GimpLayerMode",
  "GimpMaskApplyMode",
  "GimpMergeType",
  "GimpMessageHandlerType",
  "GimpOffsetType",
  "GimpOrientationType",
  "GimpPDBArgType",
  "GimpPDBErrorHandler",
  "GimpPDBProcType",
  "GimpPDBStatusType",
  "GimpPaintApplicationMode",
  "GimpPrecision",
  "GimpProgressCommand",
  "GimpRepeatMode",
  "GimpRotationType",
  "GimpRunMode",
  "GimpSelectCriterion",
  "GimpSizeType",
  "GimpStackTraceMode",
  "GimpStrokeMethod",
  "GimpTextDirection",
  "GimpTextHintStyle",
  "GimpTextJustification",
  "GimpTransferMode",
  "GimpTransformDirection",
  "GimpTransformResize",
  "GimpUserDirectory",
  "GimpVectorsStrokeType"
};

static gboolean enums_initialized = FALSE;

GType gimp_convert_dither_type_compat_get_type (void);
GType gimp_layer_mode_effects_get_type         (void);

/**
 * gimp_enums_init:
 *
 * This function makes sure all the enum types are registered
 * with the #GType system. This is intended for use by language
 * bindings that need the symbols early, before gimp_main is run.
 * It's not necessary for plug-ins to call this directly, because
 * the normal plug-in initialization code will handle it implicitly.
 *
 * Since: 2.4
 **/
void
gimp_enums_init (void)
{
  const GimpGetTypeFunc *funcs = get_type_funcs;
  GQuark                 quark;
  gint                   i;

  if (enums_initialized)
    return;

  for (i = 0; i < G_N_ELEMENTS (get_type_funcs); i++, funcs++)
    {
      GType type = (*funcs) ();

      g_type_class_ref (type);
    }

  /*  keep compat enum code in sync with app/app.c (app_libs_init)  */
  quark = g_quark_from_static_string ("gimp-compat-enum");

  g_type_set_qdata (GIMP_TYPE_CONVERT_DITHER_TYPE, quark,
		    (gpointer) gimp_convert_dither_type_compat_get_type ());
  g_type_set_qdata (GIMP_TYPE_LAYER_MODE, quark,
		    (gpointer) gimp_layer_mode_effects_get_type ());

  gimp_base_compat_enums_init ();

  enums_initialized = TRUE;
}

/**
 * gimp_enums_get_type_names:
 * @n_type_names: return location for the number of names
 *
 * This function gives access to the list of enums registered by libgimp.
 * The returned array is static and must not be modified.
 *
 * Return value: an array with type names
 *
 * Since: 2.2
 **/
const gchar **
gimp_enums_get_type_names (gint *n_type_names)
{
  g_return_val_if_fail (n_type_names != NULL, NULL);

  *n_type_names = G_N_ELEMENTS (type_names);

  return (const gchar **) type_names;
}
