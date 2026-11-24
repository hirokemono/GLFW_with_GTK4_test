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

static void fline_tracer_select_CB(GtkDropDown *dropdown,
                                   GParamSpec *pspec,
                                   gpointer user_data){
    GtkWidget *itemTEvo = GTK_WIDGET(user_data);
    struct tracer_gtk_window_view *tracer_win_vws
            = (struct tracer_gtk_window_view *) g_object_get_data(G_OBJECT(user_data), "tracerWinView");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    if(kemoview_field_select_CB(kemo_gl, tracer_win_vws->tracer_gmenu->id_tracer_mode, dropdown) < 0) return;
//    replace_tracer_frame(kemo_gl, tracer_win_vws, fline_gmenu, itemTEvo);
    set_tracer_menu_parameters(kemo_gl, tracer_win_vws->tracer_gmenu);
    
    draw_full_gl(kemo_gl);
    return;
};

static GtkWidget * init_tracer_draw_field_hbox(struct kemoviewer_gl_type *kemo_gl,
                                               struct tracer_gtk_window_view *tracer_win_vws,
                                               GtkWidget *itemTEvo){
    GtkWidget *hbox_field;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",  (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "tracerWinView", (gpointer) tracer_win_vws);
    g_object_set_data(G_OBJECT(entry_buf), "evolution",    (gpointer) itemTEvo);
    
    const char * const null_text_list[] = {NULL};
    tracer_win_vws->tracer_field_model = gtk_string_list_new(null_text_list);
    reset_psf_field_model(kemo_gl, tracer_win_vws->tracer_gmenu->id_tracer_mode, 
                          tracer_win_vws->tracer_field_model);
    tracer_win_vws->combobox_field = init_list_button_by_list_model(tracer_win_vws->tracer_field_model);
    g_signal_connect(G_OBJECT(tracer_win_vws->combobox_field), "notify::selected-item",
                     G_CALLBACK(fline_tracer_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_field_list(kemo_gl, 
                                              tracer_win_vws->tracer_gmenu->id_tracer_mode,
                                              tracer_win_vws->tracer_field_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(tracer_win_vws->combobox_field), id_menu);
    
    hbox_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_field), gtk_label_new("Field: "));
    gtk_box_append(GTK_BOX(hbox_field), tracer_win_vws->combobox_field);
    return hbox_field;
}


static void tracer_component_select_CB(GtkDropDown *dropdown,
                                       GParamSpec *pspec,
                                       gpointer user_data){
    struct tracer_gtk_window_view *tracer_win_vws
            = (struct tracer_gtk_window_view *) g_object_get_data(G_OBJECT(user_data), "tracerWinView");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    if(kemoview_component_select(kemo_gl, tracer_win_vws->tracer_gmenu->id_tracer_mode, dropdown) < 0) return;
    set_tracer_menu_parameters(kemo_gl, tracer_win_vws->tracer_gmenu);
    
    draw_full_gl(kemo_gl);
    return;
};


static GtkWidget * init_tracer_color_component_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                    struct tracer_gtk_window_view *tracer_win_vws){
    GtkWidget *hbox_component;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "tracerWinView", (gpointer) tracer_win_vws);
    
    const char * const null_text_list[] = {NULL};
    tracer_win_vws->tracer_component_model = gtk_string_list_new(null_text_list);
    reset_psf_component_model(kemo_gl, tracer_win_vws->tracer_gmenu->id_tracer_mode,
                              tracer_win_vws->tracer_component_model);
    tracer_win_vws->combobox_comp = init_list_button_by_list_model(tracer_win_vws->tracer_component_model);
    g_signal_connect(G_OBJECT(tracer_win_vws->combobox_comp), "notify::selected-item",
                     G_CALLBACK(tracer_component_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_component_list(kemo_gl, 
                                                  tracer_win_vws->tracer_gmenu->id_tracer_mode,
                                                  tracer_win_vws->tracer_component_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(tracer_win_vws->combobox_comp), id_menu);
    
    hbox_component = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_component), gtk_label_new("Component: "));
    gtk_box_append(GTK_BOX(hbox_component), tracer_win_vws->combobox_comp);
    return hbox_component;
}


static GtkWidget * set_tracer_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                       struct tracer_gtk_window_view *tracer_win_vws,
                                       GtkWidget *itemTEvo){
    tracer_win_vws->closeButton = gtk_button_new_with_label("Close Fieldline");

    g_object_set_data(G_OBJECT(tracer_win_vws->closeButton), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(tracer_win_vws->closeButton), "tracerWinView", (gpointer) tracer_win_vws);
    g_signal_connect(G_OBJECT(tracer_win_vws->closeButton), "clicked",
                     G_CALLBACK(close_tracer_CB), itemTEvo);
    
    tracer_win_vws->tracer_gmenu = init_tracer_menu_hbox(kemo_gl, tracer_win_vws->tracerWin);
    GtkWidget *hbox_field =     init_tracer_draw_field_hbox(kemo_gl, tracer_win_vws, itemTEvo);
    GtkWidget *hbox_component = init_tracer_color_component_hbox(kemo_gl, tracer_win_vws);
    
    GtkWidget *fline_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(fline_hbox), tracer_win_vws->closeButton);
    gtk_box_append(GTK_BOX(fline_hbox), hbox_field);
    gtk_box_append(GTK_BOX(fline_hbox), hbox_component);
    gtk_box_append(GTK_BOX(fline_hbox), tracer_win_vws->tracer_gmenu->tracer_menu_frame);
    
    return fline_hbox;
}

GtkWidget * init_tracer_frame(struct kemoviewer_gl_type *kemo_gl,
                              struct tracer_gtk_window_view *tracer_win_vws,
                              GtkWidget *itemTEvo){
    GtkWidget * tracerFrame;
    
    GtkWidget *fline_hbox = set_tracer_menu_box(kemo_gl, tracer_win_vws, itemTEvo);
    
    tracerFrame = wrap_into_frame_gtk4("Tracer", fline_hbox);
    return tracerFrame;
}

/*
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
                        GtkWidget *main_window, GtkWidget *itemTEvo){
    if(tracer_win_vws->iflag_tracerBox > 0){gtk_window_destroy(GTK_WINDOW(tracer_win_vws->tracerWin));};
    tracer_win_vws->iflag_tracerBox = kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data,
                                                                  TRACER_RENDERING);
    if(tracer_win_vws->iflag_tracerBox == 0){return;};
    tracer_win_vws->tracerWin = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(tracer_win_vws->tracerWin), "Tracer");
    gtk_widget_set_size_request(tracer_win_vws->tracerWin, 150, -1);
    
    tracer_win_vws->tracer_frame = init_tracer_frame(kemo_gl, tracer_win_vws, itemTEvo);
    gtk_window_set_child(GTK_WINDOW(tracer_win_vws->tracerWin), tracer_win_vws->tracer_frame);
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
    tracer_win_vws->iflag_tracerBox = 0;
    return tracer_win_vws;
};

void dealloc_tracer_gtk_window(struct tracer_gtk_window_view *tracer_win_vws){
    g_object_unref(tracer_win_vws->closeButton);
    g_object_unref(tracer_win_vws->tracer_frame);
    g_object_unref(tracer_win_vws->tracerWin);
    free(tracer_win_vws);
    return;
};
