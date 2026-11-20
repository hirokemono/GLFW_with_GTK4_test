/***********************************************************************
 *  kemoview_gtk4_PSF_range_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_range_menu.h"

static struct psf_surface_range_view * alloc_psf_surface_range_view(void){
    struct psf_surface_range_view *psf_range_vws;
    psf_range_vws = (struct psf_surface_range_view *) malloc(sizeof(struct psf_surface_range_view));
    if(psf_range_vws == NULL) {
        printf("malloc error for psf_surface_range_view\n");
        exit(0);
    }
    return psf_range_vws;
}

static void MinRangeValueChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(gtk_floatvalue, i_min_digit,
                                 value_max, i_max_digit,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void MinRangeDigitChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	int gtk_intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, gtk_intvalue,
                                 value_max, i_max_digit,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void update_psf_minimum_label(struct kemoviewer_gl_type *kemo_gl, 
                                     struct psf_surface_range_view *psf_range_vws){
    char min_text[30];
    int i_digit;
    double current_value;
    double value_min = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_COLOR_MIN);
	sprintf(min_text, "Min(%1.2e): ", value_min);
	gtk_label_set_text(GTK_LABEL(psf_range_vws->label_range_min), min_text);
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_range_vws->spin_range_min), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_range_vws->spin_digit_min), i_digit);
    return;
}

static GtkWidget * init_gtk4_psf_set_minimum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_range_view *psf_range_vws){
    GtkWidget *hbox_min_range;
    
    GtkAdjustment *adj_range_min = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_min = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    psf_range_vws->spin_range_min = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_min),0,2);
    psf_range_vws->spin_digit_min = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_min),0,0);
    g_signal_connect(psf_range_vws->spin_range_min, "value-changed", 
                     G_CALLBACK(MinRangeValueChange_CB), (gpointer) kemo_gl);
    g_signal_connect(psf_range_vws->spin_digit_min, "value-changed",
                     G_CALLBACK(MinRangeDigitChange_CB), (gpointer) kemo_gl);
    
    psf_range_vws->label_range_min = gtk_label_new(" ");
    update_psf_minimum_label(kemo_gl, psf_range_vws);
    
    hbox_min_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_min_range), psf_range_vws->label_range_min);
    gtk_box_append(GTK_BOX(hbox_min_range), psf_range_vws->spin_range_min);
    gtk_box_append(GTK_BOX(hbox_min_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_min_range), psf_range_vws->spin_digit_min);
    return hbox_min_range;
}

static void MaxRangeValueChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, i_min_digit,
                                 gtk_floatvalue, i_max_digit,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void MaxRangeDigitChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	int gtk_intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, i_min_digit,
                                 value_max, gtk_intvalue,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void update_psf_maximum_label(struct kemoviewer_gl_type *kemo_gl, 
                                     struct psf_surface_range_view *psf_range_vws){
	char max_text[30];
    int i_digit;
    double current_value;
    double value_max = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_COLOR_MAX);
    sprintf(max_text, "Max(%1.2e): ", value_max);
    gtk_label_set_text(GTK_LABEL(psf_range_vws->label_range_max), max_text);
	
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_range_vws->spin_range_max), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_range_vws->spin_digit_max), i_digit);
    return;
}

static GtkWidget * init_gtk4_psf_set_maximum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_range_view *psf_range_vws){
    GtkWidget *hbox_max_range;
    
    GtkAdjustment *adj_range_max = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_max = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    psf_range_vws->spin_range_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_max),0,2);
    psf_range_vws->spin_digit_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_max),0,0);
    g_signal_connect(psf_range_vws->spin_range_max, "value-changed",
                     G_CALLBACK(MaxRangeValueChange_CB), (gpointer) kemo_gl);
    g_signal_connect(psf_range_vws->spin_digit_max, "value-changed",
                     G_CALLBACK(MaxRangeDigitChange_CB), (gpointer) kemo_gl);
    
    psf_range_vws->label_range_max = gtk_label_new(" ");
    update_psf_maximum_label(kemo_gl, psf_range_vws);
    
    hbox_max_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_max_range), psf_range_vws->label_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), psf_range_vws->spin_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_max_range), psf_range_vws->spin_digit_max);
    return hbox_max_range;
}


static void set_psf_opacity_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_constant_opacity(gtk_floatvalue,
                                  SURFACE_RENDERING,
                                  kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_gtk4_surface_opacity(struct kemoviewer_gl_type *kemo_gl,
                                     struct psf_surface_range_view *psf_range_vws){
    double current_value = kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                          SURFACE_RENDERING,
                                                          ISET_OPACITY_MAX);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_range_vws->spin_opacity1), current_value);
	
};

static GtkWidget * init_gtk4_psf_set_opacity_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_range_view *psf_range_vws){
    GtkWidget *hbox_one_opacity;
    
    GtkAdjustment *adj_opacity1 = gtk_adjustment_new(1.0, 0.0, 1.0, 0.01, 0.01, 0.0);
    psf_range_vws->spin_opacity1 = gtk_spin_button_new(GTK_ADJUSTMENT(adj_opacity1), 0, 2);
    g_signal_connect(psf_range_vws->spin_opacity1, "value-changed",
                     G_CALLBACK(set_psf_opacity_CB), (gpointer) kemo_gl);
    
    hbox_one_opacity = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_one_opacity), gtk_label_new("Opacity"));
    gtk_box_append(GTK_BOX(hbox_one_opacity), psf_range_vws->spin_opacity1);
    return hbox_one_opacity;
}


void set_gtk4_surf_range_values(struct kemoviewer_gl_type *kemo_gl,
                                struct psf_surface_range_view *psf_range_vws){
    update_psf_minimum_label(kemo_gl, psf_range_vws);
    update_psf_maximum_label(kemo_gl, psf_range_vws);
    set_gtk4_surface_opacity(kemo_gl, psf_range_vws);
};

struct psf_surface_range_view * init_gtk4_psf_surf_range_view(struct kemoviewer_gl_type *kemo_gl,
                                                              GtkWidget *window){
    struct psf_surface_range_view *psf_range_vws = alloc_psf_surface_range_view();
    
    GtkWidget *hbox_min_range =   init_gtk4_psf_set_minimum_box(kemo_gl, psf_range_vws);
    GtkWidget *hbox_max_range =   init_gtk4_psf_set_maximum_box(kemo_gl, psf_range_vws);
    GtkWidget *hbox_one_opacity = init_gtk4_psf_set_opacity_box(kemo_gl, psf_range_vws);
    
    psf_range_vws->hbox_psf_range = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(psf_range_vws->hbox_psf_range), gtk_label_new("Range"));
    gtk_box_append(GTK_BOX(psf_range_vws->hbox_psf_range), hbox_min_range);
    gtk_box_append(GTK_BOX(psf_range_vws->hbox_psf_range), hbox_max_range);
    gtk_box_append(GTK_BOX(psf_range_vws->hbox_psf_range), hbox_one_opacity);
    
    return psf_range_vws;
}

void dealloc_psf_surf_range_view(struct psf_surface_range_view *psf_range_vws){
    g_object_unref(psf_range_vws->label_range_min);
    g_object_unref(psf_range_vws->label_range_max);
    g_object_unref(psf_range_vws->spin_opacity1);
    g_object_unref(psf_range_vws->spin_range_min);
    g_object_unref(psf_range_vws->spin_digit_min);
    g_object_unref(psf_range_vws->spin_range_max);
    g_object_unref(psf_range_vws->spin_digit_max);
    g_object_unref(psf_range_vws->hbox_psf_range);
    free(psf_range_vws);
    return;
};
