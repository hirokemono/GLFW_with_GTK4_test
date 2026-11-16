/*****************************************************************************************
 *  kemoview_gtk4_viewmatrix_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *****************************************************************************************/
#include "kemoview_gtk4_viewmatrix_menu.h"

struct view_widgets * init_viewmat_widgets(void){
    struct view_widgets *view_menu;
    view_menu = (struct view_widgets *) malloc(sizeof(struct view_widgets));
    if (view_menu == NULL) {
        printf("malloc error for view_widgets\n");
        exit(0);
    }
    
    view_menu->rot_view_menu = init_rot_view_widgets();
    view_menu->stereo_menu =   init_stereo_view_widgets();
    view_menu->viewmat_IO =    init_viewmat_IO_widgets();
    
    return view_menu;
};

void dealloc_viewmat_widgets(struct view_widgets *view_menu){
    dealloc_stereo_view_widgets(view_menu->stereo_menu);
    dealloc_rot_view_widgets(view_menu->rot_view_menu);
    free(view_menu);
    return;
};


void update_windowsize_menu(struct kemoviewer_gl_type *kemo_gl,
                            struct view_widgets *view_menu,
                            GtkWidget *window){
    char windowsize_x_text[30];
    char windowsize_y_text[30];
    
    sprintf(windowsize_x_text, "    %d    ",
            kemoview_get_view_integer(kemo_gl->kemoview_data, ISET_PIXEL_X));
    sprintf(windowsize_y_text, "    %d    ",
            kemoview_get_view_integer(kemo_gl->kemoview_data, ISET_PIXEL_Y));
    
    gtk_label_set_text(GTK_LABEL(view_menu->spin_win_x), windowsize_x_text);
    gtk_label_set_text(GTK_LABEL(view_menu->spin_win_y), windowsize_y_text);
    
    gtk_widget_queue_draw(window);
    draw_full_gl(kemo_gl);
    return;
};

GtkWidget * init_windowsize_frame(struct kemoviewer_gl_type *kemo_gl,
                                          struct view_widgets *view_menu,
                                          GtkWidget *window){
    GtkWidget *Frame_win;
    
    char windowsize_x_text[30];
    char windowsize_y_text[30];
    
    sprintf(windowsize_x_text, "    %d    ",
            kemoview_get_view_integer(kemo_gl->kemoview_data, ISET_PIXEL_X));
    sprintf(windowsize_y_text, "    %d    ",
            kemoview_get_view_integer(kemo_gl->kemoview_data, ISET_PIXEL_Y));
    
    view_menu->spin_win_x = gtk_label_new(windowsize_x_text);
    view_menu->spin_win_y = gtk_label_new(windowsize_y_text);
    
    update_windowsize_menu(kemo_gl, view_menu, window);
    
    view_menu->hbox_win_x = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_win_x), gtk_label_new(" X: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_win_x), view_menu->spin_win_x);
    
    view_menu->hbox_win_y = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_win_y), gtk_label_new(" Y: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_win_y), view_menu->spin_win_y);
    
    view_menu->vbox_win = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(view_menu->vbox_win), view_menu->hbox_win_x);
    gtk_box_append(GTK_BOX(view_menu->vbox_win), view_menu->hbox_win_y);
    
    Frame_win = gtk_frame_new("Number of pixel");
    gtk_frame_set_child(GTK_FRAME(Frame_win), view_menu->vbox_win);
    return Frame_win;
}

void set_eye_position_value(struct kemoviewer_type *kemo_sgl,
                            struct view_widgets *view_menu){
    gtk_adjustment_set_value(view_menu->adj_eye_x,
                             (-1.0 * kemoview_get_view_parameter(kemo_sgl, ISET_SHIFT, 0)));
    gtk_adjustment_set_value(view_menu->adj_eye_y,
                             (-1.0 * kemoview_get_view_parameter(kemo_sgl, ISET_SHIFT, 1)));
    gtk_adjustment_set_value(view_menu->adj_eye_z,
                             (-1.0 * kemoview_get_view_parameter(kemo_sgl, ISET_SHIFT, 2)));
    return;
};

static void eye_position_x_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = - gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_SHIFT, 0, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};
static void eye_position_y_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = - gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_SHIFT, 1, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};
static void eye_position_z_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = - gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_SHIFT, 2, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

GtkWidget * init_eye_position_frame(struct kemoviewer_gl_type *kemo_gl,
                                    struct view_widgets *view_menu){
    GtkWidget *Frame_eye;
    
    view_menu->adj_eye_x = gtk_adjustment_new(0.0, -1000.0, 1000.0, 1., 1., 0.0);
    view_menu->adj_eye_y = gtk_adjustment_new(0.0, -1000.0, 1000.0, 1., 1., 0.0);
    view_menu->adj_eye_z = gtk_adjustment_new(0.0, -1000.0, 1000.0, 1., 1., 0.0);
    
    view_menu->spin_eye_x = gtk_spin_button_new(GTK_ADJUSTMENT(view_menu->adj_eye_x), 0, 3);
    g_signal_connect(view_menu->spin_eye_x, "value-changed", 
                     G_CALLBACK(eye_position_x_CB), (gpointer) kemo_gl);
    view_menu->spin_eye_y = gtk_spin_button_new(GTK_ADJUSTMENT(view_menu->adj_eye_y), 0, 3);
    g_signal_connect(view_menu->spin_eye_y, "value-changed", 
                     G_CALLBACK(eye_position_y_CB), (gpointer) kemo_gl);
    view_menu->spin_eye_z = gtk_spin_button_new(GTK_ADJUSTMENT(view_menu->adj_eye_z), 0, 3);
    g_signal_connect(view_menu->spin_eye_z, "value-changed", 
                     G_CALLBACK(eye_position_z_CB), (gpointer) kemo_gl);
    
    view_menu->hbox_eye_x = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_eye_x), gtk_label_new(" X: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_eye_x), view_menu->spin_eye_x);
    
    view_menu->hbox_eye_y = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_eye_y), gtk_label_new(" Y: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_eye_y), view_menu->spin_eye_y);
    
    view_menu->hbox_eye_z = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_eye_z), gtk_label_new(" Z: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_eye_z), view_menu->spin_eye_z);
    
    view_menu->vbox_eye = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(view_menu->vbox_eye), view_menu->hbox_eye_x);
    gtk_box_append(GTK_BOX(view_menu->vbox_eye), view_menu->hbox_eye_y);
    gtk_box_append(GTK_BOX(view_menu->vbox_eye), view_menu->hbox_eye_z);
    
    Frame_eye = gtk_frame_new("Eye position");
    gtk_frame_set_child(GTK_FRAME(Frame_eye), view_menu->vbox_eye);
    return Frame_eye;
}


static void scale_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    printf("updating value: %f, %f \n", gtk_floatvalue, 
           kemo_gl->kemoview_data->view_s->iso_scale);
    kemoview_set_view_parameter(ISET_SCALE, 0, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    printf("updated value: %f, %f \n", gtk_floatvalue, 
           kemo_gl->kemoview_data->view_s->iso_scale);
    draw_fast_gl(kemo_gl);
	gtk_spin_button_update(GTK_SPIN_BUTTON(entry));
    return;
};

GtkWidget * init_scale_viewmat_frame(struct kemoviewer_gl_type *kemo_gl,
                                     struct view_widgets *view_menu){
    GtkWidget *Frame_scale;
    
    view_menu->adj_scale =  gtk_adjustment_new(0.0, 0.0, 1000.0, 0.01, 0.01, 0.0);
    view_menu->spin_scale = gtk_spin_button_new(GTK_ADJUSTMENT(view_menu->adj_scale), 0, 3);
    g_signal_connect(view_menu->spin_scale, "value-changed", 
                     G_CALLBACK(scale_CB), (gpointer) kemo_gl);
    
    view_menu->hbox_scale = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_scale), gtk_label_new("Scale: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_scale), view_menu->spin_scale);
    
    view_menu->vbox_scale = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(view_menu->vbox_scale), view_menu->hbox_scale);
    
    Frame_scale = gtk_frame_new("");
    gtk_frame_set_child(GTK_FRAME(Frame_scale), view_menu->vbox_scale);
    return Frame_scale;
}

static void aperture_CB(GtkWidget *entry, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double gtk_floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_view_parameter(ISET_APERTURE, 0, gtk_floatvalue,
                                kemo_gl->kemoview_data);
    
    draw_fast_gl(kemo_gl);
    return;
};

GtkWidget * init_view_aparture_frame(struct kemoviewer_gl_type *kemo_gl,
                                     struct view_widgets *view_menu){
    GtkWidget *Frame_aperture;
    
    view_menu->adj_aperture = gtk_adjustment_new(0.0, 0.0, 180.0, 0.01, 0.01, 0.0);
    view_menu->spin_aperture = gtk_spin_button_new(GTK_ADJUSTMENT(view_menu->adj_aperture), 0, 3);
    g_signal_connect(view_menu->spin_aperture, "value-changed", 
                     G_CALLBACK(aperture_CB), (gpointer) kemo_gl);
    
    view_menu->hbox_aperture = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_aperture), gtk_label_new(" Aperture: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_aperture), view_menu->spin_aperture);
    
    view_menu->vbox_aperture = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(view_menu->vbox_aperture), view_menu->hbox_aperture);
    
    Frame_aperture = gtk_frame_new("Projection");
    gtk_frame_set_child(GTK_FRAME(Frame_aperture), view_menu->vbox_aperture);
    return Frame_aperture;
}

GtkWidget * init_lookat_viewmat_frame(struct kemoviewer_gl_type *kemo_gl,
                                      struct view_widgets *view_menu){
    GtkWidget *Frame_looking;
    
    char current_lookat_x_text[30];
    char current_lookat_y_text[30];
    char current_lookat_z_text[30];
    
    sprintf(current_lookat_x_text, "    %f    ",
            (float) kemoview_get_view_parameter(kemo_gl->kemoview_data, ISET_VWPOINT, 0));
    sprintf(current_lookat_y_text, "    %f    ",
            (float) kemoview_get_view_parameter(kemo_gl->kemoview_data, ISET_VWPOINT, 1));
    sprintf(current_lookat_z_text, "    %f    ",
            (float) kemoview_get_view_parameter(kemo_gl->kemoview_data, ISET_VWPOINT, 2));
    
    view_menu->hbox_looking_x = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_looking_x), gtk_label_new(" X: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_looking_x), 
                   gtk_label_new(current_lookat_x_text));
    
    view_menu->hbox_looking_y = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_looking_y), gtk_label_new(" Y: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_looking_y), 
                   gtk_label_new(current_lookat_y_text));
    
    view_menu->hbox_looking_z = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(view_menu->hbox_looking_z), gtk_label_new(" Z: "));
    gtk_box_append(GTK_BOX(view_menu->hbox_looking_z),
                   gtk_label_new(current_lookat_z_text));
    
    view_menu->vbox_looking = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(view_menu->vbox_looking), view_menu->hbox_looking_x);
    gtk_box_append(GTK_BOX(view_menu->vbox_looking), view_menu->hbox_looking_y);
    gtk_box_append(GTK_BOX(view_menu->vbox_looking), view_menu->hbox_looking_z);
    
    Frame_looking = gtk_frame_new("Look at position");
    gtk_frame_set_child(GTK_FRAME(Frame_looking), view_menu->vbox_looking);
    return Frame_looking;
}


void set_viewmatrix_value(struct kemoviewer_type *kemo_sgl,
                          struct view_widgets *view_menu,
                          GtkWidget *window){
    
    set_eye_position_value(kemo_sgl, view_menu);
    set_gtk4_rotation_viewmat_value(kemo_sgl, view_menu->rot_view_menu);
    set_gtk4_stereo_viewmat_value(kemo_sgl, view_menu->stereo_menu);
    
    gtk_adjustment_set_value(view_menu->adj_scale,
                             kemoview_get_view_parameter(kemo_sgl, ISET_SCALE, 0));
    gtk_adjustment_set_value(view_menu->adj_aperture,
                             kemoview_get_view_parameter(kemo_sgl, ISET_APERTURE, 0));
    
	gtk_widget_queue_draw(window);
    return;
};

GtkWidget * init_viewmatrix_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                          struct view_widgets *view_menu,
                                          GtkWidget *window){
    GtkWidget *expander_view;
    
    view_menu->Frame_win =      init_windowsize_frame(kemo_gl, view_menu, window);
    view_menu->Frame_eye =      init_eye_position_frame(kemo_gl, view_menu);
    view_menu->Frame_looking =  init_lookat_viewmat_frame(kemo_gl, view_menu);
    view_menu->Frame_scale =    init_scale_viewmat_frame(kemo_gl, view_menu);
    view_menu->Frame_rotation = init_rotation_viewmat_menu_frame(kemo_gl, 
                                                                 view_menu->rot_view_menu);
    view_menu->Frame_aperture = init_view_aparture_frame(kemo_gl, view_menu);
    
    GtkWidget *box_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_win);
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_eye);
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_looking);
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_scale);
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_rotation);
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_aperture);
    
    view_menu->Frame_stereo = init_stereo_view_menu_frame(kemo_gl, view_menu->stereo_menu);
    set_viewmatrix_value(kemo_gl->kemoview_data, view_menu, window);
    
    gtk_box_append(GTK_BOX(box_view), view_menu->Frame_stereo);
    
    view_menu->hbox_viewmatrix_save = init_viewmatrix_save_box(kemo_gl, 
                                                               view_menu->viewmat_IO, 
                                                               window);
    gtk_box_append(GTK_BOX(box_view), view_menu->hbox_viewmatrix_save);
    
    expander_view = wrap_into_scroll_expansion_gtk4("View parameters", 240, 480,
                                                    window, box_view);
    return expander_view;
}

