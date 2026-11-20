/***********************************************************************
 *  kemoview_gtk4_range_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_range_menu.h"

static struct surface_range_view * alloc_psf_surface_range_view(void){
    struct surface_range_view *gtk_range_vws;
    gtk_range_vws = (struct surface_range_view *) malloc(sizeof(struct surface_range_view));
    if(gtk_range_vws == NULL) {
        printf("malloc error for surface_range_view\n");
        exit(0);
    }
    return gtk_range_vws;
}

static void MinRangeValueChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(data), "current_model");
    
    double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    double value_min, value_max;
    int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(gtk_floatvalue, i_min_digit,
                                 value_max, i_max_digit,
                                 *id_current_model,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void MinRangeDigitChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(data), "current_model");
    
    int gtk_intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, gtk_intvalue,
                                 value_max, i_max_digit,
                                 *id_current_model,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void update_psf_minimum_label(struct kemoviewer_gl_type *kemo_gl, 
                                     int id_current_model,
                                     struct surface_range_view *gtk_range_vws){
    char min_text[30];
    int i_digit;
    double current_value;
    double value_min = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   id_current_model,
                                                   ISET_COLOR_MIN);
	sprintf(min_text, "Min(%1.2e): ", value_min);
	gtk_label_set_text(GTK_LABEL(gtk_range_vws->label_range_min), min_text);
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 id_current_model, ISET_COLOR_MIN,
                                 &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_range_vws->spin_range_min), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_range_vws->spin_digit_min), i_digit);
    return;
}

static GtkWidget * init_gtk4_psf_set_minimum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 int *id_current_model,
                                                 struct surface_range_view *gtk_range_vws){
    GtkWidget *hbox_min_range;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer) id_current_model);
    
    GtkAdjustment *adj_range_min = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_min = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    gtk_range_vws->spin_range_min = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_min),0,2);
    gtk_range_vws->spin_digit_min = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_min),0,0);
    
    g_signal_connect(gtk_range_vws->spin_range_min, "value-changed", 
                     G_CALLBACK(MinRangeValueChange_CB), entry_buf);
    g_signal_connect(gtk_range_vws->spin_digit_min, "value-changed",
                     G_CALLBACK(MinRangeDigitChange_CB), entry_buf);
    
    gtk_range_vws->label_range_min = gtk_label_new(" ");
    update_psf_minimum_label(kemo_gl, *id_current_model, gtk_range_vws);
    
    hbox_min_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_min_range), gtk_range_vws->label_range_min);
    gtk_box_append(GTK_BOX(hbox_min_range), gtk_range_vws->spin_range_min);
    gtk_box_append(GTK_BOX(hbox_min_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_min_range), gtk_range_vws->spin_digit_min);
    return hbox_min_range;
}

static void MaxRangeValueChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(data), "current_model");
    
    double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, i_min_digit,
                                 gtk_floatvalue, i_max_digit,
                                 *id_current_model,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void MaxRangeDigitChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(data), "current_model");
    
    int gtk_intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, i_min_digit,
                                 value_max, gtk_intvalue,
                                 *id_current_model,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void update_psf_maximum_label(struct kemoviewer_gl_type *kemo_gl, 
                                     int id_current_model,
                                     struct surface_range_view *gtk_range_vws){
	char max_text[30];
    int i_digit;
    double current_value;
    double value_max = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   id_current_model,
                                                   ISET_COLOR_MAX);
    sprintf(max_text, "Max(%1.2e): ", value_max);
    gtk_label_set_text(GTK_LABEL(gtk_range_vws->label_range_max), max_text);
	
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 id_current_model, ISET_COLOR_MAX,
                                 &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_range_vws->spin_range_max), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_range_vws->spin_digit_max), i_digit);
    return;
}

static GtkWidget * init_gtk4_psf_set_maximum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 int *id_current_model,
                                                 struct surface_range_view *gtk_range_vws){
    GtkWidget *hbox_max_range;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer) id_current_model);
    
    GtkAdjustment *adj_range_max = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_max = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    gtk_range_vws->spin_range_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_max),0,2);
    gtk_range_vws->spin_digit_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_max),0,0);
    g_signal_connect(gtk_range_vws->spin_range_max, "value-changed",
                     G_CALLBACK(MaxRangeValueChange_CB), entry_buf);
    g_signal_connect(gtk_range_vws->spin_digit_max, "value-changed",
                     G_CALLBACK(MaxRangeDigitChange_CB), entry_buf);
    
    gtk_range_vws->label_range_max = gtk_label_new(" ");
    update_psf_maximum_label(kemo_gl, *id_current_model, gtk_range_vws);
    
    hbox_max_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_range_vws->label_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_range_vws->spin_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_range_vws->spin_digit_max);
    return hbox_max_range;
}

void set_gtk4_surf_range_values(struct kemoviewer_gl_type *kemo_gl,
                                int id_current_model,
                                struct surface_range_view *gtk_range_vws){
    update_psf_minimum_label(kemo_gl, id_current_model, gtk_range_vws);
    update_psf_maximum_label(kemo_gl, id_current_model, gtk_range_vws);
};

struct surface_range_view * init_gtk_range_view(struct kemoviewer_gl_type *kemo_gl,
                                                int *id_current_model,
                                                GtkWidget *window){
    struct surface_range_view *gtk_range_vws = alloc_psf_surface_range_view();
    
    GtkWidget *hbox_min_range =   init_gtk4_psf_set_minimum_box(kemo_gl, id_current_model, 
                                                                gtk_range_vws);
    GtkWidget *hbox_max_range =   init_gtk4_psf_set_maximum_box(kemo_gl, id_current_model, 
                                                                gtk_range_vws);
    
    gtk_range_vws->hbox_psf_range = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(gtk_range_vws->hbox_psf_range), gtk_label_new("Range"));
    gtk_box_append(GTK_BOX(gtk_range_vws->hbox_psf_range), hbox_min_range);
    gtk_box_append(GTK_BOX(gtk_range_vws->hbox_psf_range), hbox_max_range);
    
    return gtk_range_vws;
}

void dealloc_gtk_range_view(struct surface_range_view *gtk_range_vws){
    g_object_unref(gtk_range_vws->label_range_min);
    g_object_unref(gtk_range_vws->label_range_max);
    g_object_unref(gtk_range_vws->spin_range_min);
    g_object_unref(gtk_range_vws->spin_digit_min);
    g_object_unref(gtk_range_vws->spin_range_max);
    g_object_unref(gtk_range_vws->spin_digit_max);
    g_object_unref(gtk_range_vws->hbox_psf_range);
    free(gtk_range_vws);
    return;
};
