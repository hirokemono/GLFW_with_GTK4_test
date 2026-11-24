/***********************************************************************
 *  kemoview_gtk4_Tracer_ref_vector.c
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Tracer_ref_vector.h"


static struct tracer_ref_vector_menu * alloc_tracer_ref_vector_menu(void){
    struct tracer_ref_vector_menu *tracer_refvect_vws;
    tracer_refvect_vws = (struct tracer_ref_vector_menu *) malloc(sizeof(struct tracer_ref_vector_menu));
    if(tracer_refvect_vws == NULL) {
        printf("malloc error for tracer_ref_vector_menu\n");
        exit(0);
    }
    return tracer_refvect_vws;
}

static void set_ref_vector_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                       gtk_floatvalue, i_digit,
                                       TRACER_RENDERING,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

void set_ref_digit_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_intvalue = (double) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                      current_value, gtk_intvalue,
                                      TRACER_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

struct tracer_ref_vector_menu * init_tracer_ref_vector_hbox(struct kemoviewer_gl_type *kemo_gl){
    struct tracer_ref_vector_menu *tracer_refvect_vws = alloc_tracer_ref_vector_menu();
    
	GtkAdjustment *adj_ref_vect = gtk_adjustment_new(1, 1, 9, 1, 1, 0);
	tracer_refvect_vws->spin_ref_vect = gtk_spin_button_new(GTK_ADJUSTMENT(adj_ref_vect), 0, 0);
	g_signal_connect(tracer_refvect_vws->spin_ref_vect, "value-changed", 
                     G_CALLBACK(set_ref_vector_CB), (gpointer) kemo_gl);
	
	GtkAdjustment *adj_ref_digit = gtk_adjustment_new(0, -10, 10, 1, 1, 0);
	tracer_refvect_vws->spin_ref_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_ref_digit), 0, 0);
	g_signal_connect(tracer_refvect_vws->spin_ref_digit, "value-changed",
                     G_CALLBACK(set_ref_digit_CB), (gpointer) kemo_gl);
    
	
	tracer_refvect_vws->hbox_ref_vect = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_append(GTK_BOX(tracer_refvect_vws->hbox_ref_vect), gtk_label_new("Reference: "));
	gtk_box_append(GTK_BOX(tracer_refvect_vws->hbox_ref_vect), tracer_refvect_vws->spin_ref_vect);
	gtk_box_append(GTK_BOX(tracer_refvect_vws->hbox_ref_vect), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(tracer_refvect_vws->hbox_ref_vect), tracer_refvect_vws->spin_ref_digit);
    return tracer_refvect_vws;
};

void set_tracer_ref_vector_menu(struct kemoviewer_gl_type *kemo_gl,
                                struct tracer_ref_vector_menu *tracer_refvect_vws){
	double current_thick;
    int int_thick;
    int  current_digit;
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 TRACER_RENDERING, ISET_PSF_REFVECT,
                                 &current_thick, &current_digit);
	int_thick = (int) current_thick;
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tracer_refvect_vws->spin_ref_vect), (double) int_thick);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tracer_refvect_vws->spin_ref_digit), (double) current_digit);
    return;
};

void dealloc_tracer_ref_vector_menu(struct tracer_ref_vector_menu *tracer_refvect_vws){
    g_object_unref(tracer_refvect_vws->spin_ref_vect);
    g_object_unref(tracer_refvect_vws->spin_ref_digit);
    g_object_unref(tracer_refvect_vws->hbox_ref_vect);
    free(tracer_refvect_vws);
    return;
};
