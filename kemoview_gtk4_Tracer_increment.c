/***********************************************************************
 *  kemoview_gtk4_Tracer_increment.c
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Tracer_increment.h"


static struct tracer_vec_increment_menu * alloc_tracer_vec_increment_menu(void){
    struct tracer_vec_increment_menu *tracer_increment_vws;
    tracer_increment_vws = (struct tracer_vec_increment_menu *) malloc(sizeof(struct tracer_vec_increment_menu));
    if(tracer_increment_vws == NULL) {
        printf("malloc error for tracer_vec_increment_menu\n");
        exit(0);
    }
    return tracer_increment_vws;
}

static void set_vect_increment_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
    double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_VECTOR_INC,
                                      gtk_floatvalue, i_digit,
                                      TRACER_RENDERING,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_increment_digit_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	int gtk_intvalue = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_VECTOR_INC,
                                      current_value, gtk_intvalue,
                                      TRACER_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}


struct tracer_vec_increment_menu * init_tracer_vec_increment_hbox(struct kemoviewer_gl_type *kemo_gl){
    struct tracer_vec_increment_menu *tracer_increment_vws = alloc_tracer_vec_increment_menu();
    
    GtkAdjustment *adj_vect_inc = gtk_adjustment_new(1, 1, 9, 1, 1, 0);
    tracer_increment_vws->spin_vect_inc = gtk_spin_button_new(GTK_ADJUSTMENT(adj_vect_inc), 0, 0);
    g_signal_connect(tracer_increment_vws->spin_vect_inc, "value-changed",
                     G_CALLBACK(set_vect_increment_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_inc_digit = gtk_adjustment_new(0, 0, 10, 1, 1, 0);
    tracer_increment_vws->spin_inc_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_inc_digit), 0, 0);
    g_signal_connect(tracer_increment_vws->spin_inc_digit, "value-changed",
                     G_CALLBACK(set_increment_digit_CB), (gpointer) kemo_gl);
    
    
    tracer_increment_vws->hbox_vect_inc = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(tracer_increment_vws->hbox_vect_inc), gtk_label_new("Increment: "));
    gtk_box_append(GTK_BOX(tracer_increment_vws->hbox_vect_inc), tracer_increment_vws->spin_vect_inc);
    gtk_box_append(GTK_BOX(tracer_increment_vws->hbox_vect_inc), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(tracer_increment_vws->hbox_vect_inc), tracer_increment_vws->spin_inc_digit);
    return tracer_increment_vws;
};

void set_tracer_vec_increment_menu(struct kemoviewer_gl_type *kemo_gl,
                                   struct tracer_vec_increment_menu *tracer_increment_vws){
	double current_thick;
    int int_thick;
    int current_digit;
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 TRACER_RENDERING, ISET_VECTOR_INC,
                                 &current_thick, &current_digit);
	int_thick = (int) current_thick;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(tracer_increment_vws->spin_vect_inc),
                              (double) int_thick);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(tracer_increment_vws->spin_inc_digit),
                              (double) current_digit);
    return;
};

void dealloc_tracer_vec_increment_menu(struct tracer_vec_increment_menu *tracer_increment_vws){
    g_object_unref(tracer_increment_vws->spin_vect_inc);
    g_object_unref(tracer_increment_vws->spin_inc_digit);
    g_object_unref(tracer_increment_vws->hbox_vect_inc);
    free(tracer_increment_vws);
    return;
};

