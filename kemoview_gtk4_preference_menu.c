/***********************************************************************
 *  kemoview_gtk4_preference_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_preference_menu.h"

static void set_background_CB(GObject    *object,
                              GParamSpec *pspec,
                              gpointer    user_data){
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON(object);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
    float color[4];
    set_RGB_from_GTK(rgba, color);
    printf("updated %f %f %f %f\n", color[0], color[1], color[2], color[3]);
    
    printf("updated background %p\n", kemo_gl->kemoview_data);
    kemoview_set_background_color(color, kemo_gl->kemoview_data);
    kemoview_gl_background_color(kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
};

GtkWidget * init_background_hbox(struct kemoviewer_gl_type *kemo_gl){
    GtkWidget *hbox_bg_hbox;
    
    GdkRGBA bg_rgba;
    float color[4];
    kemoview_get_background_color(kemo_gl->kemoview_data, color);
    
    set_RGB_to_GTK(color, &bg_rgba);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha (color_dialog, TRUE); // Allow transparency adjustments
    
    GtkWidget *color_button = gtk_color_dialog_button_new(color_dialog);
    g_signal_connect (color_button, "notify::rgba", 
                      G_CALLBACK(set_background_CB), G_OBJECT(entry_buf));
    
    gtk_color_dialog_button_set_rgba (GTK_COLOR_DIALOG_BUTTON(color_button), &bg_rgba);
    
    hbox_bg_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_bg_hbox), gtk_label_new("Background: "));
    gtk_box_append(GTK_BOX(hbox_bg_hbox), color_button);
    return hbox_bg_hbox;
}


GtkWidget * init_preference_vbox(struct kemoviewer_gl_type *kemo_gl,
                                 struct lightparams_view *lightparams_vws,
                                 GtkWidget *window){
    int id_def_format[1];
    GtkWidget *pref_vbox;
    
    float color[4];
    
    GtkWidget *background_box = init_background_hbox(kemo_gl);
//    GtkWidget *lighting_frame =  init_lighting_frame(kemo_gl,
//                                                     lightparams_vws);
//    GtkWidget *Shading_frame =   shading_mode_menu_frame(kemo_gl);
//    GtkWidget *Tube_frame =      init_tube_pref_frame(kemo_gl);
//    GtkWidget *coastline_frame = init_coastline_pref_menu(kemo_gl);
//    GtkWidget *Axis_frame =      init_axis_position_menu(kemo_gl);
//    GtkWidget *FPS_frame =       init_FPS_test_menu_frame(kemo_gl, window);
//    GtkWidget *NumThread_frame = init_num_threads_menu_frame(kemo_gl);
    GtkWidget *ImgFormat_frame = init_image_format_hbox(kemo_gl, id_def_format);

    
    pref_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(pref_vbox), background_box);
//    gtk_box_append(GTK_BOX(pref_vbox), NumThread_frame);
    gtk_box_append(GTK_BOX(pref_vbox), ImgFormat_frame);
//    gtk_box_append(GTK_BOX(pref_vbox), lighting_frame);
//    gtk_box_append(GTK_BOX(pref_vbox), Tube_frame);
//    gtk_box_append(GTK_BOX(pref_vbox), Shading_frame);
//    gtk_box_append(GTK_BOX(pref_vbox), coastline_frame);
//    gtk_box_append(GTK_BOX(pref_vbox), Axis_frame);
//    gtk_box_append(GTK_BOX(pref_vbox), FPS_frame);
    return pref_vbox;
}

GtkWidget * init_preference_frame(struct kemoviewer_gl_type *kemo_gl,
                                  struct lightparams_view *lightparams_vws,
                                  GtkWidget *window){
    GtkWidget *frame_pref  = gtk_frame_new("Preferences");
    GtkWidget *pref_vbox = init_preference_vbox(kemo_gl, lightparams_vws,  window);
	gtk_frame_set_child(GTK_FRAME(frame_pref), pref_vbox);
    return frame_pref;
}

GtkWidget * init_preference_scrollbox(struct kemoviewer_gl_type *kemo_gl,
                                     struct lightparams_view *lightparams_vws,
                                     GtkWidget *window){
    GtkWidget *scroll_pref;
    GtkWidget *pref_vbox = init_preference_vbox(kemo_gl, lightparams_vws, window);
    scroll_pref = wrap_into_scrollbox_gtk4(400, 400, pref_vbox);
    return scroll_pref;
}

GtkWidget * init_preference_expander(struct kemoviewer_gl_type *kemo_gl,
                                     struct lightparams_view *lightparams_vws,
                                     GtkWidget *window){
    GtkWidget *expander_pref;
    GtkWidget *pref_vbox = init_preference_vbox(kemo_gl, lightparams_vws, window);
    expander_pref = wrap_into_scroll_expansion_gtk4("Preferences", 400, 400,
                                                    window, pref_vbox);
    return expander_pref;
}
