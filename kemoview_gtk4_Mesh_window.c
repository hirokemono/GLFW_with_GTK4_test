/***********************************************************************
 *  kemoview_gtk4_Mesh_window.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_window.h"


static void close_mesh_CB(GtkButton *button, gpointer user_data){
    GtkWidget *meshWin = GTK_WIDGET(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    kemoview_close_mesh_view(kemo_gl->kemoview_data);
    
    gtk_window_destroy(GTK_WINDOW(meshWin));
    draw_full_gl(kemo_gl);
};

static GtkWidget * set_mesh_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                     struct mesh_gtk_window_view *mesh_win_vws){
    mesh_win_vws->closeButton = gtk_button_new_with_label("Close Mesh");

    g_object_set_data(G_OBJECT(mesh_win_vws->meshWin), "kemoview_gl", (gpointer) kemo_gl);
    g_signal_connect(G_OBJECT(mesh_win_vws->closeButton), "clicked",
                     G_CALLBACK(close_mesh_CB), mesh_win_vws->meshWin);
    
    mesh_win_vws->mesh_gmenu = init_gtk_mesh_menu(kemo_gl, mesh_win_vws->meshWin);
    
    GtkWidget *fline_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(fline_hbox), mesh_win_vws->closeButton);
    gtk_box_append(GTK_BOX(fline_hbox), mesh_win_vws->mesh_gmenu->mesh_menu_box);
    
    return fline_hbox;
}

void init_mesh_window(struct kemoviewer_gl_type *kemo_gl,
                      struct mesh_gtk_window_view *mesh_win_vws,
                      GtkWidget *main_window){
    if(mesh_win_vws->iflag_meshBox > 0){gtk_window_destroy(GTK_WINDOW(mesh_win_vws->meshWin));};
    mesh_win_vws->iflag_meshBox = kemoview_get_draw_mesh_flag(kemo_gl->kemoview_data);
    if(mesh_win_vws->iflag_meshBox == 0){return;};
    
    mesh_win_vws->meshWin = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(mesh_win_vws->meshWin), "Mesh");
    gtk_widget_set_size_request(mesh_win_vws->meshWin, 150, -1);
    
    mesh_win_vws->mesh_frame = set_mesh_menu_box(kemo_gl, mesh_win_vws);
    gtk_widget_set_margin_top(mesh_win_vws->mesh_frame,    5);
    gtk_widget_set_margin_bottom(mesh_win_vws->mesh_frame, 5);
    gtk_widget_set_margin_start(mesh_win_vws->mesh_frame,  5);
    gtk_widget_set_margin_end(mesh_win_vws->mesh_frame,    5);
    gtk_window_set_child(GTK_WINDOW(mesh_win_vws->meshWin), mesh_win_vws->mesh_frame);
    gtk_widget_set_visible(mesh_win_vws->meshWin, TRUE);
    return;
}


struct mesh_gtk_window_view * alloc_mesh_window_view(void){
    struct mesh_gtk_window_view *mesh_win_vws;
    mesh_win_vws = (struct mesh_gtk_window_view *) malloc(sizeof(struct mesh_gtk_window_view));
    if (mesh_win_vws == NULL) {
        printf("malloc error for mesh_gtk_window_view \n");
        exit(0);
    }
    mesh_win_vws->iflag_meshBox = 0;
    return mesh_win_vws;
};

void dealloc_mesh_window_view(struct mesh_gtk_window_view *mesh_win_vws){
    dealloc_mesh_menu(mesh_win_vws->mesh_gmenu);
    
    g_object_unref(mesh_win_vws->closeButton);
    g_object_unref(mesh_win_vws->mesh_frame);
    g_object_unref(mesh_win_vws->meshWin);
    free(mesh_win_vws);
    return;
};
