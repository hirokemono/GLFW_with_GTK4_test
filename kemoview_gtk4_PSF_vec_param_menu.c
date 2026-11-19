/***********************************************************************
 *  kemoview_gtk4_PSF_vec_param_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_vec_param_menu.h"

static struct psf_vector_param_views * allc_psf_vector_gtk_menu(void){
    struct psf_vector_param_views *psf_vec_vws;
    psf_vec_vws =  (struct psf_vector_param_views *) malloc(sizeof(struct psf_vector_param_views));
    if (psf_vec_vws == NULL) {
        printf("malloc error for psf_vector_param_views\n");
        exit(0);
    }
    return psf_vec_vws;
}

void deallc_psf_vector_param_views(struct psf_vector_param_views *psf_vec_vws){
    g_object_unref(psf_vec_vws->spin_ref_vect);
    g_object_unref(psf_vec_vws->spin_ref_digit);
    g_object_unref(psf_vec_vws->spin_vect_inc);
    g_object_unref(psf_vec_vws->spin_inc_digit);
    g_object_unref(psf_vec_vws->spin_vect_width);
    g_object_unref(psf_vec_vws->spin_width_digit);
    
    g_object_unref(psf_vec_vws->hbox_ref_vect);
    g_object_unref(psf_vec_vws->hbox_vect_inc);
    g_object_unref(psf_vec_vws->hbox_vect_width);
    free(psf_vec_vws);
    return;
}

static void set_ref_vector_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING, ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                       gtk_floatvalue, i_digit,
                                       SURFACE_RENDERING,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_ref_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_intvalue = (double) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                      current_value, gtk_intvalue,
                                      SURFACE_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_vect_increment_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_VECTOR_INC,
                                      gtk_floatvalue, i_digit,
                                      SURFACE_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_increment_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	int gtk_intvalue = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_VECTOR_INC,
                                      current_value, gtk_intvalue,
                                      SURFACE_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_vector_width_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_PSF_V_THICK,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_V_THICK,
                                      gtk_floatvalue, i_digit,
                                      SURFACE_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}
static void set_width_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	int gtk_intvalue = (double) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_PSF_V_THICK,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_V_THICK,
                                      current_value, gtk_intvalue,
                                      SURFACE_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

void set_psf_vector_param_menus(struct kemoviewer_gl_type *kemo_gl,
                                struct psf_vector_param_views *psf_vec_vws){
    int i_digit;
    double current_value;
    
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_vec_vws->spin_ref_vect), current_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_vec_vws->spin_ref_digit), i_digit);
    
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_vec_vws->spin_vect_inc), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_vec_vws->spin_inc_digit), i_digit);
	
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING,
                                  ISET_PSF_V_THICK,
                                  &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_vec_vws->spin_vect_width), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_vec_vws->spin_width_digit), i_digit);
	return;
};

struct psf_vector_param_views * init_psf_vector_param_menus(struct kemoviewer_gl_type *kemo_gl){
    struct psf_vector_param_views *psf_vec_vws = allc_psf_vector_gtk_menu();
    
    GtkAdjustment *adj_ref_vect = gtk_adjustment_new(1, 1, 9, 1, 1, 0);
    psf_vec_vws->spin_ref_vect = gtk_spin_button_new(GTK_ADJUSTMENT(adj_ref_vect), 0, 0);
    g_signal_connect(psf_vec_vws->spin_ref_vect, "value-changed", 
                     G_CALLBACK(set_ref_vector_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_ref_digit = gtk_adjustment_new(0, -10, 10, 1, 1, 0);
    psf_vec_vws->spin_ref_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_ref_digit), 0, 0);
    g_signal_connect(psf_vec_vws->spin_ref_digit, "value-changed",
                     G_CALLBACK(set_ref_digit_CB), (gpointer) kemo_gl);
    
    
    GtkAdjustment *adj_vect_inc = gtk_adjustment_new(1, 1, 9, 1, 1, 0);
    psf_vec_vws->spin_vect_inc = gtk_spin_button_new(GTK_ADJUSTMENT(adj_vect_inc), 0, 0);
    g_signal_connect(psf_vec_vws->spin_vect_inc, "value-changed",
                     G_CALLBACK(set_vect_increment_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_inc_digit = gtk_adjustment_new(0, 0, 10, 1, 1, 0);
    psf_vec_vws->spin_inc_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_inc_digit), 0, 0);
    g_signal_connect(psf_vec_vws->spin_inc_digit, "value-changed",
                     G_CALLBACK(set_increment_digit_CB), (gpointer) kemo_gl);
    
    
    GtkAdjustment *adj_vect_width = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    psf_vec_vws->spin_vect_width = gtk_spin_button_new(GTK_ADJUSTMENT(adj_vect_width), 0, 0);
    g_signal_connect(psf_vec_vws->spin_vect_width, "value-changed",
                     G_CALLBACK(set_vector_width_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_width_digit = gtk_adjustment_new(-3, -10, 10, 1, 1, 0);
    psf_vec_vws->spin_width_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_width_digit), 0, 0);
    g_signal_connect(psf_vec_vws->spin_width_digit, "value-changed",
                     G_CALLBACK(set_width_digit_CB), (gpointer) kemo_gl);
    
    psf_vec_vws->hbox_ref_vect = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_ref_vect), gtk_label_new("Reference: "));
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_ref_vect), psf_vec_vws->spin_ref_vect);
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_ref_vect), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_ref_vect), psf_vec_vws->spin_ref_digit);
    
    psf_vec_vws->hbox_vect_width = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_width), gtk_label_new("Arrow width: "));
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_width), psf_vec_vws->spin_vect_width);
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_width), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_width), psf_vec_vws->spin_width_digit);
    
    psf_vec_vws->hbox_vect_inc = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_inc), gtk_label_new("Increment: "));
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_inc), psf_vec_vws->spin_vect_inc);
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_inc), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(psf_vec_vws->hbox_vect_inc), psf_vec_vws->spin_inc_digit);
    
    return psf_vec_vws;
}
