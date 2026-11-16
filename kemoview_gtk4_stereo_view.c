/*****************************************************************************************
 *  kemoview_gtk4_stereo_view.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Nov., 16, 2026.
 *  Copyright 2026
 *  Dept. of Earth and Planetary Sciences, Inst. of Science Tokyo. 
 *  All rights reserved.
 *
 *****************************************************************************************/
#include "kemoview_gtk4_stereo_view.h"

struct stereo_view_widgets * init_stereo_view_widgets(void){
    struct stereo_view_widgets *stereo_menu;
    stereo_menu = (struct stereo_view_widgets *) malloc(sizeof(struct stereo_view_widgets));
    if (stereo_menu == NULL) {
        printf("malloc error for stereo_view_widgets \n");
        exit(0);
    }
    return stereo_menu;
};

void dealloc_stereo_view_widgets(struct stereo_view_widgets *stereo_menu){
    free(stereo_menu);
    return;
};


static void focus_CB(GtkWidget *spin_focus, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_focus));
    kemoview_set_stereo_parameter(ISET_FOCUS, gtk_floatvalue,
                                  kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

static void eye_sep_dist_CB(GtkWidget *spin_eye_sep, gpointer user_data){
    struct stereo_view_widgets *stereo_menu =  g_object_get_data(G_OBJECT(user_data), "stereo");
    struct kemoviewer_gl_type *kemo_gl= g_object_get_data(G_OBJECT(user_data), "kemoview_gl");

    if(stereo_menu->iflag_updated_eye_sep_angle > 0){
        stereo_menu->iflag_updated_eye_sep_angle = 0;
    }else{
        double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_eye_sep));
        kemoview_set_stereo_parameter(ISET_EYESEP, gtk_floatvalue,
                                      kemo_gl->kemoview_data);

        stereo_menu->iflag_updated_eye_separation = 1;
        double angle = kemoview_get_view_parameter(kemo_gl->kemoview_data,
                                                   ISET_EYEAGL, 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(stereo_menu->spin_sep_angle), angle);

        draw_fast_gl(kemo_gl);
    }
    stereo_menu->iflag_updated_eye_separation = 0;
    return;
};

static void eye_sep_angle_CB(GtkWidget *spin_sep_angle, gpointer user_data){
    struct stereo_view_widgets *stereo_menu =  g_object_get_data(G_OBJECT(user_data), "stereo");
    struct kemoviewer_gl_type *kemo_gl= g_object_get_data(G_OBJECT(user_data), "kemoview_gl");

    if(stereo_menu->iflag_updated_eye_separation > 0){
        stereo_menu->iflag_updated_eye_separation = 0;
    }else{
        double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_sep_angle));
        kemoview_set_stereo_parameter(ISET_EYEAGL, gtk_floatvalue,
                                      kemo_gl->kemoview_data);

        stereo_menu->iflag_updated_eye_sep_angle = 1;
        double separation = kemoview_get_view_parameter(kemo_gl->kemoview_data,
                                                        ISET_EYESEP, 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(stereo_menu->spin_eye_sep), separation);
        draw_fast_gl(kemo_gl);
    }
    stereo_menu->iflag_updated_eye_sep_angle = 0;
    return;
};

GtkWidget * init_stereo_view_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                        struct stereo_view_widgets *stereo_menu){
    GtkWidget *Frame_stereo;
    
    stereo_menu->iflag_updated_eye_separation = 0;
    stereo_menu->iflag_updated_eye_sep_angle =  0;
    
    stereo_menu->adj_focus =     gtk_adjustment_new( 9.5, 0.0, 1000.0, 0.01, 0.01, 0.0);
    stereo_menu->adj_eye_sep =   gtk_adjustment_new( 0.0, 0.0,  100.0, 0.01, 0.01, 0.0);
    stereo_menu->adj_sep_angle = gtk_adjustment_new(35.0, 0.1,  180.0, 0.1,  0.1,  0.0);

    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "stereo",      (gpointer) stereo_menu);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    stereo_menu->spin_focus = gtk_spin_button_new(GTK_ADJUSTMENT(stereo_menu->adj_focus), 0, 3);
    g_signal_connect(stereo_menu->spin_focus, "value-changed", 
                     G_CALLBACK(focus_CB), (gpointer) kemo_gl);
    stereo_menu->spin_eye_sep =   gtk_spin_button_new(GTK_ADJUSTMENT(stereo_menu->adj_eye_sep),
                                                    0, 3);
    g_signal_connect(stereo_menu->spin_eye_sep, "value-changed",
                     G_CALLBACK(eye_sep_dist_CB), (gpointer) entry_buf);
    
    stereo_menu->spin_sep_angle = gtk_spin_button_new(GTK_ADJUSTMENT(stereo_menu->adj_sep_angle),
                                                    0, 3);
    g_signal_connect(stereo_menu->spin_sep_angle, "value-changed",
                     G_CALLBACK(eye_sep_angle_CB), (gpointer) entry_buf);
    
    stereo_menu->hbox_focus = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(stereo_menu->hbox_focus), gtk_label_new(" Focul: "));
    gtk_box_append(GTK_BOX(stereo_menu->hbox_focus), stereo_menu->spin_focus);
    
    stereo_menu->hbox_eye_sep = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(stereo_menu->hbox_eye_sep), gtk_label_new(" Eye separation: "));
    gtk_box_append(GTK_BOX(stereo_menu->hbox_eye_sep), stereo_menu->spin_eye_sep);
    
    stereo_menu->hbox_sep_angle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(stereo_menu->hbox_sep_angle), gtk_label_new(" Separation angle: "));
    gtk_box_append(GTK_BOX(stereo_menu->hbox_sep_angle), stereo_menu->spin_sep_angle);
    
    stereo_menu->vbox_stereo = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(stereo_menu->vbox_stereo), stereo_menu->hbox_focus);
    gtk_box_append(GTK_BOX(stereo_menu->vbox_stereo), stereo_menu->hbox_eye_sep);
    gtk_box_append(GTK_BOX(stereo_menu->vbox_stereo), stereo_menu->hbox_sep_angle);
    
    Frame_stereo = gtk_frame_new("Stereo parameter");
    gtk_frame_set_child(GTK_FRAME(Frame_stereo), stereo_menu->vbox_stereo);
    if(kemoview_get_view_type_flag(kemo_gl->kemoview_data) != VIEW_STEREO){
        gtk_widget_set_sensitive(Frame_stereo, FALSE);
    };
    return Frame_stereo;
}

void set_gtk4_stereo_viewmat_value(struct kemoviewer_type *kemo_sgl,
                                   struct stereo_view_widgets *stereo_menu){
    gtk_adjustment_set_value(stereo_menu->adj_focus,
                             kemoview_get_view_parameter(kemo_sgl, ISET_FOCUS, 0));

    gtk_adjustment_set_value(stereo_menu->adj_eye_sep,
                             kemoview_get_view_parameter(kemo_sgl, ISET_EYESEP, 0));
    gtk_adjustment_set_value(stereo_menu->adj_sep_angle,
                             kemoview_get_view_parameter(kemo_sgl, ISET_EYEAGL, 0));
    return;
};
