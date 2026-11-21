/***********************************************************************
 *  kemoview_gtk4_Fline_window.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Fline_window.h"


static void close_fline_CB(GtkButton *button, gpointer user_data){
    GtkWidget *itemTEvo = GTK_WIDGET(user_data);
    struct fline_gtk_window_view *fline_win_vws
            = (struct fline_gtk_window_view *) g_object_get_data(G_OBJECT(button), "flineWinView");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(button), "kemoview_gl");

	kemoview_close_fieldline_view(kemo_gl->kemoview_data);
	
    gtk_window_destroy(GTK_WINDOW(fline_win_vws->flineWin));
    activate_evolution_menu(kemo_gl->kemoview_data, itemTEvo);
    draw_full_gl(kemo_gl);
};


static void fline_field_select_CB(GtkDropDown *dropdown,
                                  GParamSpec *pspec,
                                  gpointer user_data){
    GtkWidget *itemTEvo = GTK_WIDGET(user_data);
    struct fline_gtk_window_view *fline_win_vws
            = (struct fline_gtk_window_view *) g_object_get_data(G_OBJECT(user_data), "flineWinView");
    struct fieldline_gtk_menu *fline_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "fline_view");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    if(kemoview_field_select_CB(kemo_gl, FIELDLINE_RENDERING, dropdown) < 0) return;
//    replace_fline_frame(kemo_gl, fline_win_vws, fline_gmenu, itemTEvo);
    
    draw_full_gl(kemo_gl);
    return;
};

static GtkWidget * init_fline_draw_field_hbox(struct kemoviewer_gl_type *kemo_gl,
                                              struct fline_gtk_window_view *fline_win_vws,
                                              GtkWidget *itemTEvo){
    GtkWidget *hbox_field;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",  (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "flineWinView", (gpointer) fline_win_vws);
    g_object_set_data(G_OBJECT(entry_buf), "evolution",    (gpointer) itemTEvo);
    
    const char * const null_text_list[] = {NULL};
    fline_win_vws->fline_field_model = gtk_string_list_new(null_text_list);
    reset_psf_field_model(kemo_gl, FIELDLINE_RENDERING, fline_win_vws->fline_field_model);
    fline_win_vws->combobox_field = init_list_button_by_list_model(fline_win_vws->fline_field_model);
    g_signal_connect(G_OBJECT(fline_win_vws->combobox_field), "notify::selected-item",
                     G_CALLBACK(fline_field_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_field_list(kemo_gl, FIELDLINE_RENDERING,
                                              fline_win_vws->fline_field_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(fline_win_vws->combobox_field), id_menu);
    
    hbox_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_field), gtk_label_new("Field: "));
    gtk_box_append(GTK_BOX(hbox_field), fline_win_vws->combobox_field);
    return hbox_field;
}


static void fline_component_select_CB(GtkDropDown *dropdown,
                                      GParamSpec *pspec,
                                      gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    if(kemoview_component_select(kemo_gl, FIELDLINE_RENDERING, dropdown) < 0) return;
    
    draw_full_gl(kemo_gl);
    return;
};


static GtkWidget * init_fline_color_component_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                   struct fline_gtk_window_view *fline_win_vws){
    GtkWidget *hbox_component;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
//    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  fline_win_vws->psf_gmenu);
    
    const char * const null_text_list[] = {NULL};
    fline_win_vws->fline_component_model = gtk_string_list_new(null_text_list);
    reset_psf_component_model(kemo_gl, FIELDLINE_RENDERING, fline_win_vws->fline_component_model);
    fline_win_vws->combobox_comp = init_list_button_by_list_model(fline_win_vws->fline_component_model);
    g_signal_connect(G_OBJECT(fline_win_vws->combobox_comp), "notify::selected-item",
                     G_CALLBACK(fline_component_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_component_list(kemo_gl, FIELDLINE_RENDERING,
                                                  fline_win_vws->fline_component_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(fline_win_vws->combobox_comp), id_menu);
    
    hbox_component = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_component), gtk_label_new("Component: "));
    gtk_box_append(GTK_BOX(hbox_component), fline_win_vws->combobox_comp);
    return hbox_component;
}


static GtkWidget * set_fieldline_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                          struct fline_gtk_window_view *fline_win_vws,
//                                       struct fieldline_gtk_menu *fline_gmenu,
                                          GtkWidget *itemTEvo){
    fline_win_vws->closeButton = gtk_button_new_with_label("Close Fieldline");
    
    g_object_set_data(G_OBJECT(fline_win_vws->closeButton), "kemoview_gl",  (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(fline_win_vws->closeButton), "flineWinView", (gpointer) fline_win_vws);
    g_signal_connect(G_OBJECT(fline_win_vws->closeButton), "clicked",
                     G_CALLBACK(close_fline_CB), itemTEvo);
    
    GtkWidget *hbox_field =     init_fline_draw_field_hbox(kemo_gl, fline_win_vws, itemTEvo);
    GtkWidget *hbox_component = init_fline_color_component_hbox(kemo_gl, fline_win_vws);
    
    GtkWidget *fline_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(fline_hbox), fline_win_vws->closeButton);
    gtk_box_append(GTK_BOX(fline_hbox), hbox_field);
    gtk_box_append(GTK_BOX(fline_hbox), hbox_component);
    
    /*
	init_fieldline_menu_hbox(kemo_gl, fline_gmenu);
    
    g_object_set_data(G_OBJECT(itemTEvo), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(itemTEvo), "current_model", (gpointer) tracer_gmenu->iflag_flinemode);
    g_object_set_data(G_OBJECT(itemTEvo), "flineWinView", (gpointer) fline_win_vws);
    g_object_set_data(G_OBJECT(itemTEvo), "flinemenu", (gpointer) fline_gmenu);
    
    fline_gmenu->combobox_field = draw_viz_field_gtk_box(kemo_gl, fline_gmenu->iflag_flinemode);
    fline_gmenu->combobox_comp = draw_viz_component_gtk_box(kemo_gl, fline_gmenu->iflag_flinemode);
    g_signal_connect(G_OBJECT(fline_gmenu->combobox_field), "changed",
                     G_CALLBACK(fline_field_select_CB), (gpointer) itemTEvo);
    g_signal_connect(G_OBJECT(fline_gmenu->combobox_comp), "changed",
                     G_CALLBACK(fline_component_select_CB), (gpointer) itemTEvo);
    
    int itype_fline = kemoview_get_line_type_flag(kemo_gl->kemoview_data);
    if(itype_fline == 0){
        gtk_switch_set_active(GTK_SWITCH(fline_gmenu->switch_tube), FALSE);
    } else {
        gtk_switch_set_active(GTK_SWITCH(fline_gmenu->switch_tube), TRUE);
    };
    
    set_gtk_fieldline_menu(kemo_gl, fline_gmenu);
*/
	return fline_hbox;
}

GtkWidget *  init_fline_frame(struct kemoviewer_gl_type *kemo_gl,
                              struct fline_gtk_window_view *fline_win_vws,
//                              struct fieldline_gtk_menu *fline_gmenu,
                              GtkWidget *itemTEvo){
    GtkWidget * flineFrame;
    
    GtkWidget *fline_hbox = set_fieldline_menu_box(kemo_gl, fline_win_vws, 
                                                   itemTEvo);
    flineFrame = wrap_into_frame_gtk4("Fieldline", fline_hbox);
    return flineFrame;
}

/*
void replace_fline_frame(struct kemoviewer_gl_type *kemo_gl,
                         struct fline_gtk_window_view *fline_win_vws,
                         struct fieldline_gtk_menu *fline_gmenu,
                         GtkWidget *itemTEvo){
    gtk_window_destroy(GTK_WINDOW(fline_win_vws->fline_frame));
    fline_win_vws->fline_frame = init_fline_frame(kemo_gl, fline_win_vws, 
                                                fline_gmenu, itemTEvo);
                                                
    gtk_window_set_child(GTK_WINDOW(fline_win_vws->flineWin), fline_win_vws->fline_frame);
    gtk_widget_set_visible(fline_win_vws->flineWin, TRUE);
    gtk_widget_queue_draw(fline_win_vws->flineWin);
    return;
}
*/

void init_fline_window(struct kemoviewer_gl_type *kemo_gl,
                       struct fline_gtk_window_view *fline_win_vws,
//                       struct fieldline_gtk_menu *fline_gmenu,
                       GtkWidget *main_window, GtkWidget *itemTEvo){
    if(fline_win_vws->iflag_flineBox > 0){gtk_window_destroy(GTK_WINDOW(fline_win_vws->flineWin));};
    fline_win_vws->iflag_flineBox = kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data,
                                                             FIELDLINE_RENDERING);
    if(fline_win_vws->iflag_flineBox == 0){return;};

    fline_win_vws->flineWin = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(fline_win_vws->flineWin), "Fieldline");
    gtk_widget_set_size_request(fline_win_vws->flineWin, 150, -1);
    
    fline_win_vws->fline_frame = init_fline_frame(kemo_gl, fline_win_vws, 
                                                  itemTEvo);
    
    gtk_window_set_child(GTK_WINDOW(fline_win_vws->flineWin), fline_win_vws->fline_frame);
    gtk_widget_set_visible(fline_win_vws->flineWin, TRUE);
    return;
}

struct fline_gtk_window_view * alloc_fline_gtk_window_view(void){
    struct fline_gtk_window_view *fline_win_vws;
    fline_win_vws = (struct fline_gtk_window_view *) malloc(sizeof(struct fline_gtk_window_view));
    if (fline_win_vws == NULL) {
        printf("malloc error for fline_gtk_window_view \n");
        exit(0);
    }
    return fline_win_vws;
};

void dealloc_fline_gtk_window(struct fline_gtk_window_view *fline_win_vws){
    g_object_unref(fline_win_vws->closeButton);
    g_object_unref(fline_win_vws->fline_frame);
    g_object_unref(fline_win_vws->flineWin);
    free(fline_win_vws);
    return;
};
