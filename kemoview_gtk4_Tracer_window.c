/***********************************************************************
 *  kemoview_gtk4_Tracer_window.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Tracer_window.h"


static void close_tracer_CB(GtkButton *button, gpointer user_data){
    GtkWidget *itemTEvo = GTK_WIDGET(user_data);
    struct tracer_gtk_window_view *tracer_win_vws
            = (struct tracer_gtk_window_view *) g_object_get_data(G_OBJECT(button), "tracer_win_vws");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(button), "kemoview_gl");

    kemoview_close_tracer_view(kemo_gl->kemoview_data);
    
    gtk_window_destroy(GTK_WINDOW(tracer_win_vws->tracerWin));
    activate_evolution_menu(kemo_gl->kemoview_data, itemTEvo);
    draw_full_gl(kemo_gl);
};

/*
static void tracer_field_select_CB(GtkComboBox *combobox_field, gpointer user_data)
{
    GtkWidget *itemTEvo = GTK_WIDGET(user_data);
    struct tracer_gtk_window_view *tracer_win_vws
            = (struct tracer_gtk_window_view *) g_object_get_data(G_OBJECT(button), "tracer_win_vws");
    struct fieldline_gtk_menu *tracer_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "tracerMenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    kemoview_field_select_CB(combobox_field, tracer_gmenu->iflag_flinemode, kemo_gl);
    replace_tracer_frame(kemo_gl, tracer_gmenu, itemTEvo);
    draw_full_gl(kemo_gl);
    return;
};

static void tracer_component_select_CB(GtkComboBox *combobox_comp, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    kemoview_component_select_CB(combobox_comp, TRACER_RENDERING, kemo_gl);
    draw_full_gl(kemo_gl);
    return;
};
*/

static void set_tracer_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                struct tracer_gtk_window_view *tracer_win_vws,
//                                struct fieldline_gtk_menu *tracer_win_vws,
                                GtkWidget *itemTEvo){
    tracer_win_vws->closeButton = gtk_button_new_with_label("Close Fieldline");

    g_object_set_data(G_OBJECT(tracer_win_vws->closeButton), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(tracer_win_vws->closeButton), "tracerWinView", (gpointer) tracer_win_vws);
    g_signal_connect(G_OBJECT(tracer_win_vws->closeButton), "clicked",
                     G_CALLBACK(close_tracer_CB), itemTEvo);
    
/*
    init_tracer_menu_hbox(kemo_gl, tracer_gmenu);
    
    g_object_set_data(G_OBJECT(itemTEvo), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(itemTEvo), "current_model", (gpointer) tracer_gmenu->iflag_flinemode);
    g_object_set_data(G_OBJECT(itemTEvo), "tracerMenu", (gpointer) tracer_gmenu);
    tracer_win_vws->combobox_field = draw_viz_field_gtk_box(kemo_gl, tracer_gmenu->iflag_flinemode);
    tracer_gmenu->combobox_comp = draw_viz_component_gtk_box(kemo_gl, tracer_gmenu->iflag_flinemode);
    g_signal_connect(G_OBJECT(tracer_win_vws->combobox_field), "changed",
                G_CALLBACK(tracer_field_select_CB), (gpointer) itemTEvo);
    g_signal_connect(G_OBJECT(tracer_gmenu->combobox_comp), "changed",
                G_CALLBACK(tracer_component_select_CB), (gpointer) itemTEvo);
    
    set_gtk_fieldline_menu(kemo_gl, tracer_gmenu);
*/
    return;
}

/*
GtkWidget * init_tracer_frame(struct kemoviewer_gl_type *kemo_gl,
                              struct tracer_gtk_window_view *tracer_win_vws,
                              struct fieldline_gtk_menu *tracer_gmenu,
                              GtkWidget *itemTEvo){
    GtkWidget * tracerFrame;
    tracer_gmenu->fline_color_vws = alloc_colormap_view();
    set_tracer_menu_box(kemo_gl, tracer_win_vws, tracer_gmenu, itemTEvo);
    tracerFrame = pack_tracer_menu_frame(tracer_gmenu);
    return tracerFrame;
}

void replace_tracer_frame(struct kemoviewer_gl_type *kemo_gl,
                          struct fieldline_gtk_menu *tracer_gmenu,
                          GtkWidget *itemTEvo){
    gtk_window_destroy(GTK_WINDOW(tracer_gmenu->fline_frame));
    tracer_gmenu->fline_frame = init_tracer_frame(kemo_gl, tracer_gmenu, itemTEvo);
    gtk_window_set_child(GTK_WINDOW(tracer_win_vws->tracerWin), tracer_gmenu->fline_frame);
    
    gtk_widget_set_visible(tracer_win_vws->tracerWin, TRUE);
    gtk_widget_queue_draw(tracer_win_vws->tracerWin);
    return;
}
*/

void init_tracer_window(struct kemoviewer_gl_type *kemo_gl,
                        struct tracer_gtk_window_view *tracer_win_vws,
//                        struct fieldline_gtk_menu *tracer_gmenu,
                        GtkWidget *main_window, GtkWidget *itemTEvo){
    if(tracer_win_vws->iflag_tracerBox > 0){gtk_window_destroy(GTK_WINDOW(tracer_win_vws->tracerWin));};
    tracer_win_vws->iflag_tracerBox = kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data,
                                                               TRACER_RENDERING);
    if(tracer_win_vws->iflag_tracerBox == 0){return;};
    
    tracer_win_vws->tracerWin = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(tracer_win_vws->tracerWin), "Tracer");
    gtk_widget_set_size_request(tracer_win_vws->tracerWin, 150, -1);
/*
    tracer_gmenu->fline_frame = init_tracer_frame(kemo_gl, tracer_gmenu, itemTEvo);
    
    gtk_window_set_child(GTK_WINDOW(tracer_win_vws->tracerWin), tracer_gmenu->fline_frame);
*/
    gtk_widget_set_visible(tracer_win_vws->tracerWin, TRUE);
    return;
}


struct tracer_gtk_window_view * alloc_tracer_gtk_window_view(void){
    struct tracer_gtk_window_view *tracer_win_vws;
    tracer_win_vws = (struct tracer_gtk_window_view *) malloc(sizeof(struct tracer_gtk_window_view));
    if (tracer_win_vws == NULL) {
        printf("malloc error for tracer_gtk_window_view \n");
        exit(0);
    }
    return tracer_win_vws;
};

void dealloc_tracer_gtk_window(struct tracer_gtk_window_view *tracer_win_vws){
    g_object_unref(tracer_win_vws->closeButton);
    g_object_unref(tracer_win_vws->tracer_frame);
    g_object_unref(tracer_win_vws->tracerWin);
    free(tracer_win_vws);
    return;
};
