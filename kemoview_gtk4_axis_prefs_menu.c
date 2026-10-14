/***********************************************************************
 *  kemoview_gtk4_axis_prefs_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "kemoview_gtk4_axis_prefs_menu.h"


static void axis_thickness_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double current_thick;
    int current_digit;
    
    double thick_in = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    if(thick_in < 0) return;
    
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                 &current_thick, &current_digit);
    kemoview_set_axis_thickness_w_exp(thick_in, current_digit,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}
static void axis_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double current_thick;
    int current_digit;
    
    int in_digit = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                      &current_thick, &current_digit);
    kemoview_set_axis_thickness_w_exp(current_thick, in_digit,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

GtkWidget * init_axis_position_menu(struct kemoviewer_gl_type *kemo_gl){
    int iflag_lowerleft = kemoview_get_object_property_flags(kemo_gl->kemoview_data, AXIS_POSITION);
    GtkWidget *combobox_axispos = init_axis_position_button(kemo_gl, 
                                                            &iflag_lowerleft);
    
    int int_thick, current_digit;
    double current_thick;
    GtkAdjustment *adj_thick = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    GtkWidget *spin_axis_thick = gtk_spin_button_new(GTK_ADJUSTMENT(adj_thick), 0, 0);
    gtk_editable_set_width_chars(GTK_EDITABLE(spin_axis_thick), 1);
    g_signal_connect(spin_axis_thick, "value-changed",
                     G_CALLBACK(axis_thickness_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_digit = gtk_adjustment_new(-3, -30, 30, 1, 1, 0.0);
    GtkWidget *spin_axis_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
    gtk_editable_set_width_chars(GTK_EDITABLE(spin_axis_digit), 3);
    g_signal_connect(spin_axis_digit, "value-changed",
                     G_CALLBACK(axis_digit_CB), (gpointer) kemo_gl);
 
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                      &current_thick, &current_digit);
    int_thick = (int) current_thick;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_axis_thick), (double) int_thick);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_axis_digit), (double) current_digit);
    
    GtkWidget *grid_axis_pref = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid_axis_pref), gtk_label_new("Axis position: "), 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), combobox_axispos,                 3, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid_axis_pref), gtk_label_new("Thickness: "), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), spin_axis_thick,              1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), gtk_label_new("X 10^"),       2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), spin_axis_digit,              3, 1, 1, 1);
    
    return wrap_into_frame_gtk4("Axis parameters", grid_axis_pref);
}

