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
/*
static void fline_field_select_CB(GtkComboBox *combobox_field, gpointer user_data)
{
    GtkWidget *itemTEvo = GTK_WIDGET(user_data);
    struct fline_gtk_window_view *fline_win_vws
            = (struct fline_gtk_window_view *) g_object_get_data(G_OBJECT(user_data), "flineWinView");
    struct fieldline_gtk_menu *fline_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "fline_view");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    kemoview_field_select_CB(combobox_field, fline_gmenu->iflag_flinemode, kemo_gl);
    replace_fline_frame(kemo_gl, fline_win_vws, fline_gmenu, itemTEvo);
    draw_full_gl(kemo_gl);
    return;
};

static void fline_component_select_CB(GtkComboBox *combobox_comp, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    kemoview_component_select_CB(combobox_comp, *id_current_model, kemo_gl);
    draw_full_gl(kemo_gl);
    return;
};
*/

static void set_fieldline_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                   struct fline_gtk_window_view *fline_win_vws,
//                                   struct fieldline_gtk_menu *fline_gmenu,
                                   GtkWidget *itemTEvo){
    fline_win_vws->closeButton = gtk_button_new_with_label("Close Fieldline");
    
    g_object_set_data(G_OBJECT(fline_win_vws->closeButton), "kemoview_gl",  (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(fline_win_vws->closeButton), "flineWinView", (gpointer) fline_win_vws);
    g_signal_connect(G_OBJECT(fline_win_vws->closeButton), "clicked",
                     G_CALLBACK(close_fline_CB), itemTEvo);
    
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
	return;
}

/*
GtkWidget *  init_fline_frame(struct kemoviewer_gl_type *kemo_gl,
                              struct fline_gtk_window_view *fline_win_vws,
                              struct fieldline_gtk_menu *fline_gmenu,
                              GtkWidget *itemTEvo){
    GtkWidget * flineFrame;
    
    fline_gmenu->fline_color_vws = alloc_colormap_view();
    set_fieldline_menu_box(kemo_gl, fline_win_vws, fline_gmenu, itemTEvo);
    flineFrame = pack_fieldline_menu_frame(fline_gmenu);
    return flineFrame;
}

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
/*
    fline_win_vws->fline_frame = init_fline_frame(kemo_gl, fline_win_vws, 
                                                  fline_gmenu, itemTEvo);
    
    gtk_window_set_child(GTK_WINDOW(fline_win_vws->flineWin), fline_win_vws->fline_frame);
*/
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
