/*****************************************************************************************
 *  kemoview_gtk4_rotation_view.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Nov., 16, 2026.
 *  Copyright 2026
 *  Dept. of Earth and Planetary Sciences, Inst. of Science Tokyo. 
 *  All rights reserved.
 *
 *****************************************************************************************/
#include "kemoview_gtk4_rotation_view.h"

struct rot_view_widgets * init_rot_view_widgets(void){
    struct rot_view_widgets *rot_view_menu;
    rot_view_menu = (struct rot_view_widgets *) malloc(sizeof(struct rot_view_widgets));
    if (rot_view_menu == NULL) {
        printf("malloc error for rot_view_widgets \n");
        exit(0);
    }
    return rot_view_menu;
};

void dealloc_rot_view_widgets(struct rot_view_widgets *rot_view_menu){
    free(rot_view_menu);
    return;
};



static void spin_x_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_ROTATE, 1, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

static void spin_y_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_ROTATE, 2, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

static void spin_z_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_ROTATE, 3, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

static void spin_deg_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_ROTATE, 0, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

GtkWidget * init_rotation_viewmat_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                             struct rot_view_widgets *rot_view_menu){
    GtkWidget *Frame_rotation;
    
    rot_view_menu->adj_rotation_x = gtk_adjustment_new(0.0, -1.0, 1.0, 0.01, 0.01, 0.0);
    rot_view_menu->spin_rotation_x
            = gtk_spin_button_new(GTK_ADJUSTMENT(rot_view_menu->adj_rotation_x), 0, 3);
    g_signal_connect(rot_view_menu->spin_rotation_x, "value-changed", 
                     G_CALLBACK(spin_x_CB), (gpointer) kemo_gl);
    
    rot_view_menu->adj_rotation_y = gtk_adjustment_new(0.0, -1.0, 1.0, 0.01, 0.01, 0.0);
    rot_view_menu->spin_rotation_y
            = gtk_spin_button_new(GTK_ADJUSTMENT(rot_view_menu->adj_rotation_y), 0, 3);
    g_signal_connect(rot_view_menu->spin_rotation_y, "value-changed", 
                     G_CALLBACK(spin_y_CB), (gpointer) kemo_gl);
    
    rot_view_menu->adj_rotation_z = gtk_adjustment_new(0.0, -1.0, 1.0, 0.01, 0.01, 0.0);
    rot_view_menu->spin_rotation_z
            = gtk_spin_button_new(GTK_ADJUSTMENT(rot_view_menu->adj_rotation_z), 0, 3);
    g_signal_connect(rot_view_menu->spin_rotation_z, "value-changed", 
                     G_CALLBACK(spin_z_CB), (gpointer) kemo_gl);
    
    rot_view_menu->adj_rotation_deg = gtk_adjustment_new(0.0, -180.0, 360.0, 1, 1, 0.0);
    rot_view_menu->spin_rotation_deg
            = gtk_spin_button_new(GTK_ADJUSTMENT(rot_view_menu->adj_rotation_deg), 0, 3);
    g_signal_connect(rot_view_menu->spin_rotation_deg, "value-changed", 
                     G_CALLBACK(spin_deg_CB), (gpointer) kemo_gl);
    
    rot_view_menu->hbox_rotation_x = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_x), gtk_label_new(" X: "));
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_x), rot_view_menu->spin_rotation_x);
    
    rot_view_menu->hbox_rotation_y = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_y), gtk_label_new(" Y: "));
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_y), rot_view_menu->spin_rotation_y);
    
    rot_view_menu->hbox_rotation_z = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_z), gtk_label_new(" Z: "));
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_z), rot_view_menu->spin_rotation_z);
    
    rot_view_menu->hbox_rotation_deg = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_deg), gtk_label_new(" Angle (Deg.): "));
    gtk_box_append(GTK_BOX(rot_view_menu->hbox_rotation_deg), rot_view_menu->spin_rotation_deg);
    
    rot_view_menu->vbox_rotation = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(rot_view_menu->vbox_rotation), rot_view_menu->hbox_rotation_x);
    gtk_box_append(GTK_BOX(rot_view_menu->vbox_rotation), rot_view_menu->hbox_rotation_y);
    gtk_box_append(GTK_BOX(rot_view_menu->vbox_rotation), rot_view_menu->hbox_rotation_z);
    gtk_box_append(GTK_BOX(rot_view_menu->vbox_rotation), rot_view_menu->hbox_rotation_deg);
    printf("rot_view_menu->adj_rotation_deg init %p\n", rot_view_menu->adj_rotation_deg);
    
    Frame_rotation = gtk_frame_new("Rotation");
    gtk_frame_set_child(GTK_FRAME(Frame_rotation), rot_view_menu->vbox_rotation);
    return Frame_rotation;
};

void set_gtk4_rotation_viewmat_value(struct kemoviewer_type *kemo_sgl,
                                     struct rot_view_widgets *rot_view_menu){
    gtk_adjustment_set_value(rot_view_menu->adj_rotation_x,
                             kemoview_get_view_parameter(kemo_sgl, ISET_ROTATE, 1));
    gtk_adjustment_set_value(rot_view_menu->adj_rotation_y,
                             kemoview_get_view_parameter(kemo_sgl, ISET_ROTATE, 2));
    gtk_adjustment_set_value(rot_view_menu->adj_rotation_z,
                             kemoview_get_view_parameter(kemo_sgl, ISET_ROTATE, 3));
    gtk_adjustment_set_value(rot_view_menu->adj_rotation_deg,
                             kemoview_get_view_parameter(kemo_sgl, ISET_ROTATE, 0));
    return;
};