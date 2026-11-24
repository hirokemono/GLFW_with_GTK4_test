/***********************************************************************
 *  kemoview_gtk4_Fline_thickness.c
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Fline_thickness.h"

static void fline_thickness_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
	double current_thick;
	int current_digit;
    
	double thick_in = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	if(thick_in < 0) return;
	
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_WIDTH,
                                 &current_thick, &current_digit);
    kemoview_set_VIZ_color_value_w_exp(*id_current_model, ISET_WIDTH,
                                       thick_in, current_digit,
                                       kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
}

static void fline_digit_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
	double current_thick;
	int current_digit;
    
	int in_digit = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 *id_current_model, ISET_WIDTH,
                                 &current_thick, &current_digit);
    kemoview_set_VIZ_color_value_w_exp(*id_current_model, ISET_WIDTH,
                                       current_thick, in_digit,
                                       kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
}

static struct line_thickness_menu * alloc_line_thickness_menu(void){
    struct line_thickness_menu *thickness_vws;
    thickness_vws = (struct line_thickness_menu *) malloc(sizeof(struct line_thickness_menu));
    if(thickness_vws == NULL) {
        printf("malloc error for line_thickness_menu\n");
        exit(0);
    }
    return thickness_vws;
}


struct line_thickness_menu * init_line_thickness_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                      int *id_current_model){
    struct line_thickness_menu *thickness_vws = alloc_line_thickness_menu();
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer)  id_current_model);
    
    GtkAdjustment *adj_thick = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    GtkAdjustment *adj_digit = gtk_adjustment_new(-3, -30, 30, 1, 1, 0.0);
    thickness_vws->spin_thick = gtk_spin_button_new(GTK_ADJUSTMENT(adj_thick), 0, 0);
    thickness_vws->spin_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
    g_signal_connect(thickness_vws->spin_thick, "value-changed",
                     G_CALLBACK(fline_thickness_CB), G_OBJECT(entry_buf));
    g_signal_connect(thickness_vws->spin_digit, "value-changed",
                     G_CALLBACK(fline_digit_CB), G_OBJECT(entry_buf));
    
    thickness_vws->hbox_thickness = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(thickness_vws->hbox_thickness), gtk_label_new("Thickness: "));
	gtk_box_append(GTK_BOX(thickness_vws->hbox_thickness), thickness_vws->spin_thick);
	gtk_box_append(GTK_BOX(thickness_vws->hbox_thickness), gtk_label_new("X 10^"));
	gtk_box_append(GTK_BOX(thickness_vws->hbox_thickness), thickness_vws->spin_digit);
    return thickness_vws;
}

void set_line_thickness_menu(struct kemoviewer_gl_type *kemo_gl,
                             int id_current_mode,
                             struct line_thickness_menu *thickness_vws){
	double current_thick;
    int int_thick;
    int  current_digit;
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 id_current_mode, ISET_WIDTH,
                                 &current_thick, &current_digit);
	int_thick = (int) current_thick;
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(thickness_vws->spin_thick), (double) int_thick);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(thickness_vws->spin_digit), (double) current_digit);
    return;
};

void dealloc_line_thickness_menu(struct line_thickness_menu *thickness_vws){
    g_object_unref(thickness_vws->spin_thick);
    g_object_unref(thickness_vws->spin_digit);
    g_object_unref(thickness_vws->hbox_thickness);
    free(thickness_vws);
    return;
};
