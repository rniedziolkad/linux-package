/* This file is generated by glib-genmarshal, do not modify it. This code is licensed under the same license as the containing project. Note that it links to GLib, so must comply with the LGPL linking clauses. */
#ifndef ___GIMP_WIDGETS_MARSHAL_MARSHAL_H__
#define ___GIMP_WIDGETS_MARSHAL_MARSHAL_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* VOID: ENUM (../../libgimpwidgets/gimpwidgetsmarshal.list:25) */
#define _gimp_widgets_marshal_VOID__ENUM	g_cclosure_marshal_VOID__ENUM

/* VOID: ENUM, BOOLEAN (../../libgimpwidgets/gimpwidgetsmarshal.list:26) */
extern
void _gimp_widgets_marshal_VOID__ENUM_BOOLEAN (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);

/* VOID: INT, INT (../../libgimpwidgets/gimpwidgetsmarshal.list:27) */
extern
void _gimp_widgets_marshal_VOID__INT_INT (GClosure     *closure,
                                          GValue       *return_value,
                                          guint         n_param_values,
                                          const GValue *param_values,
                                          gpointer      invocation_hint,
                                          gpointer      marshal_data);

/* VOID: OBJECT (../../libgimpwidgets/gimpwidgetsmarshal.list:28) */
#define _gimp_widgets_marshal_VOID__OBJECT	g_cclosure_marshal_VOID__OBJECT

/* VOID: OBJECT, INT (../../libgimpwidgets/gimpwidgetsmarshal.list:29) */
extern
void _gimp_widgets_marshal_VOID__OBJECT_INT (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);

/* VOID: POINTER, POINTER (../../libgimpwidgets/gimpwidgetsmarshal.list:30) */
extern
void _gimp_widgets_marshal_VOID__POINTER_POINTER (GClosure     *closure,
                                                  GValue       *return_value,
                                                  guint         n_param_values,
                                                  const GValue *param_values,
                                                  gpointer      invocation_hint,
                                                  gpointer      marshal_data);

/* VOID: STRING, FLAGS (../../libgimpwidgets/gimpwidgetsmarshal.list:31) */
extern
void _gimp_widgets_marshal_VOID__STRING_FLAGS (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);

/* VOID: STRING, INT (../../libgimpwidgets/gimpwidgetsmarshal.list:32) */
extern
void _gimp_widgets_marshal_VOID__STRING_INT (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);

/* VOID: DOUBLE, DOUBLE (../../libgimpwidgets/gimpwidgetsmarshal.list:33) */
extern
void _gimp_widgets_marshal_VOID__DOUBLE_DOUBLE (GClosure     *closure,
                                                GValue       *return_value,
                                                guint         n_param_values,
                                                const GValue *param_values,
                                                gpointer      invocation_hint,
                                                gpointer      marshal_data);

/* BOOLEAN: POINTER (../../libgimpwidgets/gimpwidgetsmarshal.list:35) */
extern
void _gimp_widgets_marshal_BOOLEAN__POINTER (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);


G_END_DECLS

#endif /* ___GIMP_WIDGETS_MARSHAL_MARSHAL_H__ */