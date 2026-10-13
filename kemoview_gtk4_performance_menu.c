/*
 *  kemoview_gtk4_performance_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_performance_menu.h"

const int num_rotation =   2;
const int iaxis_rotation = 3;
const int increment_deg =  2;

static void NumThreadsChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
    int ivalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_number_of_threads(ivalue, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

static void rot_FPS_view_CB(GtkButton *button, gpointer user_data){
    GtkEntryBuffer *fps_entry_buf = GTK_ENTRY_BUFFER(user_data);
    GtkWidget *window
            = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");

    gtk_window_set_focus(GTK_WINDOW(window), NULL);
    double AverageFPS = draw_rotate_views(kemo_gl,
                                          iaxis_rotation, 
                                          increment_deg,
                                          num_rotation);
    gchar text_AverageFPS[25];
    sprintf(text_AverageFPS, "%f", (float) AverageFPS);
    gtk_entry_buffer_set_text(fps_entry_buf, text_AverageFPS, 
                              strlen(text_AverageFPS)+1);
    return;
};


GtkWidget * init_num_threads_menu_frame(struct kemoviewer_gl_type *kemo_gl){
    int current_int = kemoview_get_number_of_threads(kemo_gl->kemoview_data);
    GtkAdjustment *adj_t = gtk_adjustment_new(current_int, 1, 256, 1, 1, 0.0);
    GtkWidget *spin_threads = gtk_spin_button_new(GTK_ADJUSTMENT(adj_t),0,2);
    g_signal_connect(spin_threads, "value-changed",
                     G_CALLBACK(NumThreadsChange_CB), (gpointer) kemo_gl);
    
    GtkWidget *nthread_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(nthread_hbox), gtk_label_new("# of threads:  "));
    gtk_box_append(GTK_BOX(nthread_hbox), spin_threads);
    
    return wrap_into_frame_gtk4("Thread parameter", nthread_hbox);
}

GtkWidget * init_FPS_test_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                     GtkWidget *window){
    GtkEntryBuffer *fps_entry_buf = gtk_entry_buffer_new("0.0", -1);
    g_object_set_data(G_OBJECT(fps_entry_buf), "parent", (gpointer) window);
    g_object_set_data(G_OBJECT(fps_entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    GtkWidget * fpsTextBox = gtk_entry_new_with_buffer(fps_entry_buf);
    gtk_editable_set_width_chars(GTK_EDITABLE(fpsTextBox), 12);

    GtkWidget * fpsButton = gtk_button_new_with_label("Start FPS test");
    g_signal_connect(G_OBJECT(fpsButton), "clicked",
                     G_CALLBACK(rot_FPS_view_CB), (gpointer)fps_entry_buf);

    GtkWidget * FPStest_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(FPStest_hbox), gtk_label_new("Average FPS:"));
    gtk_box_append(GTK_BOX(FPStest_hbox), fpsTextBox);
    
    GtkWidget * FPStest_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(FPStest_vbox), FPStest_hbox);
    gtk_box_append(GTK_BOX(FPStest_vbox), fpsButton);
    
    return wrap_into_frame_gtk4("FPS test", FPStest_vbox);
}
