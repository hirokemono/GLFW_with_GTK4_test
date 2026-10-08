/*
 *  kemoview_gtk4_PSF_window.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_PSF_window.h"

static void current_psf_select_CB(GtkComboBox *combobox_psfs, gpointer user_data)
{
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "itemTEvo"));
    struct psf_gtk_menu *psf_gmenu 
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
//    kemoview_psf_select_CB(combobox_psfs, SURFACE_RENDERING, kemo_gl);
    set_vector_plot_availablity(SURFACE_RENDERING, kemo_gl, psf_gmenu);
    replace_psf_menu_frame(kemo_gl, psf_gmenu, main_window, itemTEvo);
    draw_full_gl(kemo_gl);
	return;
};

static void close_psf_CB(GtkButton *button, gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "itemTEvo"));
    struct psf_gtk_menu *psf_gmenu 
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");

    int num_loaded = kemoview_close_PSF_view(kemo_gl->kemoview_data);
    set_GLFW_viewtype_mode(VIEW_3D);
    kemoview_set_viewtype(VIEW_3D, kemo_gl->kemoview_data);
	
    init_psf_window(kemo_gl, psf_gmenu, main_window, itemTEvo);
//    activate_evolution_menu(kemo_gl->kemoview_data, itemTEvo);
    gtk_widget_queue_draw(psf_gmenu->psfWin);
    draw_full_gl(kemo_gl);
};

static void psf_field_select_CB(GtkComboBox *combobox_field, gpointer user_data)
{
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "itemTEvo"));
    struct psf_gtk_menu *psf_gmenu
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
//    kemoview_field_select_CB(combobox_field, SURFACE_RENDERING, kemo_gl);
    replace_psf_menu_frame(kemo_gl, psf_gmenu, main_window, itemTEvo);
    draw_full_gl(kemo_gl);
	return;
};

static void psf_component_select_CB(GtkComboBox *combobox_comp, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
//    kemoview_component_select_CB(combobox_comp, SURFACE_RENDERING, kemo_gl);
    draw_full_gl(kemo_gl);
    return;
};

static void init_psf_draw_component_hbox(struct kemoviewer_gl_type *kemo_gl,
                                         struct psf_gtk_menu *psf_gmenu, GtkWidget *itemTEvo){
//    psf_gmenu->combobox_comp = draw_viz_component_gtk_box(kemo_gl, SURFACE_RENDERING);
    g_signal_connect(G_OBJECT(psf_gmenu->combobox_comp), "changed",
                     G_CALLBACK(psf_component_select_CB), (gpointer) itemTEvo);
    
    psf_gmenu->hbox_comp = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(psf_gmenu->hbox_comp), gtk_label_new("Component: "));
//    gtk_box_append(GTK_BOX(psf_gmenu->hbox_comp), psf_gmenu->combobox_comp);
    return;
}

static void init_psf_draw_field_hbox(struct kemoviewer_gl_type *kemo_gl,
                                     struct psf_gtk_menu *psf_gmenu,
                                     GtkWidget *main_window,
                                     GtkWidget *itemTEvo){
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  main_window);
    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  psf_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "evolution", (gpointer)  itemTEvo);
    
//    psf_gmenu->combobox_field = draw_viz_field_gtk_box(kemo_gl, SURFACE_RENDERING);
    g_signal_connect(G_OBJECT(psf_gmenu->combobox_field), "changed",
                     G_CALLBACK(psf_field_select_CB), G_OBJECT(entry_buf));
    
    psf_gmenu->hbox_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(psf_gmenu->hbox_field), gtk_label_new("Field: "));
//    gtk_box_append(GTK_BOX(psf_gmenu->hbox_field), psf_gmenu->combobox_field);
    return;
}


static int count_loaded_psf(struct kemoviewer_type *kemo_sgl){
    int ipsf;
    int num_psfs = 0;
    for (ipsf=0; ipsf< kemoview_get_PSF_loaded_params(kemo_sgl, MAX_LOADED); ipsf++){
        if(kemoview_get_PSF_loaded_flag(kemo_sgl, ipsf) > 0) {
            num_psfs = num_psfs + 1;
        };
    };
    return num_psfs;
}

static void init_current_psf_set_hbox(struct kemoviewer_gl_type *kemo_gl,
                                      struct psf_gtk_menu *psf_gmenu,
                                      GtkWidget *itemTEvo){
	int id_current_psf = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data,
                                                        SET_CURRENT);
	int index = 0;
    psf_gmenu->num_psfs =      count_loaded_psf(kemo_gl->kemoview_data);
/*
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  main_window);
    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  psf_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "evolution", (gpointer)  itemTEvo);
    
    psf_gmenu->combobox_psfs = draw_current_psf_set_hbox(id_current_psf, kemo_gl, &index);
    g_signal_connect(G_OBJECT(psf_gmenu->combobox_psfs), "changed",
                     G_CALLBACK(current_psf_select_CB), G_OBJECT(entry_buf));
    
    psf_gmenu->hbox_psfs = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(psf_gmenu->hbox_psfs), gtk_label_new("Current PSF: "));
    gtk_box_append(GTK_BOX(psf_gmenu->hbox_psfs), psf_gmenu->combobox_psfs);
    
    if(index <= 1){
        gtk_widget_set_sensitive(psf_gmenu->combobox_psfs, FALSE);
    }else{
        gtk_widget_set_sensitive(psf_gmenu->combobox_psfs, TRUE);
    }
*/
    return;
}

static GtkWidget * init_psf_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                       struct psf_gtk_menu *psf_gmenu,
                                       GtkWidget *main_window, 
                                       GtkWidget *itemTEvo){
    GtkWidget *psf_menu_frame;
    psf_gmenu->closeButton = gtk_button_new_with_label("Close Current PSF");
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  main_window);
    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  psf_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "evolution", (gpointer)  itemTEvo);
    
    g_signal_connect(G_OBJECT(psf_gmenu->closeButton), "clicked",
                     G_CALLBACK(close_psf_CB), G_OBJECT(entry_buf));
    
    init_current_psf_set_hbox(kemo_gl, psf_gmenu, itemTEvo);
    init_psf_draw_field_hbox(kemo_gl, psf_gmenu, main_window, itemTEvo);
    
    init_psf_draw_component_hbox(kemo_gl, psf_gmenu, itemTEvo);
    init_psf_menu_hbox(kemo_gl, psf_gmenu);
    
    GtkWidget *psf_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->closeButton);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->hbox_psfs);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->hbox_field);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->hbox_comp);
    
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_iso);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_surf);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_color);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_vect);
    
    psf_menu_frame = wrap_into_frame_gtk4("Surfaces", psf_vbox);
    gtk_widget_set_margin_top(psf_menu_frame,    5);
    gtk_widget_set_margin_bottom(psf_menu_frame, 5);
    gtk_widget_set_margin_start(psf_menu_frame,  5);
    gtk_widget_set_margin_end(psf_menu_frame,    5);
    return psf_menu_frame;
}

void replace_psf_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                            struct psf_gtk_menu *psf_gmenu,
                            GtkWidget *main_window, 
                            GtkWidget *itemTEvo){
    gtk_window_set_child(psf_gmenu->psf_frame, NULL);
    g_object_unref(psf_gmenu->psf_frame);
    psf_gmenu->psf_frame = init_psf_menu_frame(kemo_gl, psf_gmenu, 
                                               main_window, itemTEvo);
    gtk_window_set_child(GTK_WINDOW(psf_gmenu->psfWin), psf_gmenu->psf_frame);
    gtk_widget_set_visible(psf_gmenu->psfWin, TRUE);
    gtk_widget_queue_draw(psf_gmenu->psfWin);
    return;
}

void init_psf_window(struct kemoviewer_gl_type *kemo_gl,
                     struct psf_gtk_menu *psf_gmenu,
                     GtkWidget *main_window, GtkWidget *itemTEvo){
    if(psf_gmenu->iflag_psfBox > 0){gtk_window_destroy(psf_gmenu->psfWin);};
    psf_gmenu->iflag_psfBox = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data,
                                                             NUM_LOADED);
    if(psf_gmenu->iflag_psfBox == 0){return;}

    gint size_xy[2];
    size_xy[0] = gtk_widget_get_width (main_window);
    size_xy[1] = gtk_widget_get_height(main_window);

    psf_gmenu->psfWin = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(psf_gmenu->psfWin), "PSF");
    gtk_widget_set_size_request(psf_gmenu->psfWin, 150, -1);
    
    g_object_set_data(G_OBJECT(itemTEvo), "psfmenu", (gpointer) psf_gmenu);
    g_object_set_data(G_OBJECT(itemTEvo), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(itemTEvo), "parents", (gpointer) main_window);
    psf_gmenu->psf_frame = init_psf_menu_frame(kemo_gl, psf_gmenu, 
                                               main_window, itemTEvo);
    gtk_window_set_child(GTK_WINDOW(psf_gmenu->psfWin), psf_gmenu->psf_frame);
    gtk_widget_set_visible(psf_gmenu->psfWin, TRUE);
    return;
}

