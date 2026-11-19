/***********************************************************************
 *  kemoview_gtk4_PSF_window.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_window.h"

const char * const null_text_list[] = {NULL};

static void close_psf_CB(GtkButton *button, gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "evolution"));
    struct psf_gtk_menu *psf_gmenu 
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    kemoview_close_PSF_view(kemo_gl->kemoview_data);
    set_GLFW_viewtype_mode(VIEW_3D);
    kemoview_set_viewtype(VIEW_3D, kemo_gl->kemoview_data);
	
    init_psf_window(kemo_gl, psf_gmenu, main_window, itemTEvo);
    activate_evolution_menu(kemo_gl->kemoview_data, itemTEvo);
    gtk_widget_queue_draw(psf_gmenu->psfWin);
    draw_full_gl(kemo_gl);
    return;
};

static void current_psf_select_CB(GtkDropDown *dropdown,
                                  GParamSpec *pspec,
                                  gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "itemTEvo"));
    struct psf_gtk_menu *psf_gmenu 
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    printf("selected_item: %p \n", selected_item);
    
    if(selected_item == NULL) return;
    char *text = (char *) gtk_string_object_get_string(selected_item);
    printf("PSF selected: %s \n", text);
    
    int id_current = find_selected_psf_id(kemo_gl, text);
    if(id_current < 0) return;
    
    printf("New current PSF ID: %d \n", id_current);
    kemoview_set_PSF_loaded_params(SET_CURRENT, id_current, kemo_gl->kemoview_data);
    
    /*
    kemoview_set_VIZ_field_param(IZERO, SURFACE_RENDERING,
                                 FIELD_SEL_FLAG,     kemo_gl->kemoview_data);
    kemoview_set_VIZ_field_param(IZERO, SURFACE_RENDERING,
                                 COMPONENT_SEL_FLAG, kemo_gl->kemoview_data);
    */
    
    set_vector_plot_availablity(kemo_gl, SURFACE_RENDERING, psf_gmenu->psf_vector_menu);
    
    reset_psf_field_model(kemo_gl, SURFACE_RENDERING,
                          psf_gmenu->psf_field_model);
    reset_psf_component_model(kemo_gl, SURFACE_RENDERING,
                              psf_gmenu->psf_component_model);
    
    draw_full_gl(kemo_gl);
	return;
};

static void psf_field_select_CB(GtkDropDown *dropdown,
                                GParamSpec *pspec,
                                gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "itemTEvo"));
    struct psf_gtk_menu *psf_gmenu
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    printf("field selected_item: %p \n", selected_item);
    
    if(selected_item == NULL) return;
    char *text = (char *) gtk_string_object_get_string(selected_item);
    printf("selected: %s \n", text);
    
    int id_field = find_selected_field_id(kemo_gl, SURFACE_RENDERING, text);
    if(id_field < 0) return;
    kemoview_set_VIZ_field_param(id_field,
                                 SURFACE_RENDERING,
                                 FIELD_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    kemoview_set_VIZ_field_param(IZERO,
                                 SURFACE_RENDERING,
                                 COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    
    
    reset_psf_component_model(kemo_gl, SURFACE_RENDERING,
                              psf_gmenu->psf_component_model);
    
    draw_full_gl(kemo_gl);
	return;
};

static void psf_component_select_CB(GtkDropDown *dropdown,
                                    GParamSpec *pspec,
                                    gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    GtkWidget *itemTEvo = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "itemTEvo"));
    struct psf_gtk_menu *psf_gmenu
            = (struct psf_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "psfmenu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    printf("component selected_item: %p \n", selected_item);
    
    if(selected_item == NULL) return;
    char *text = (char *) gtk_string_object_get_string(selected_item);
    
    int id_pick = find_selected_component_id(kemo_gl, SURFACE_RENDERING, text);
    printf("selected: %s: %d\n", text, id_pick);
    if(id_pick < 0) return;
    kemoview_set_VIZ_field_param(id_pick,
                                 SURFACE_RENDERING,
                                 COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
};

static GtkWidget * init_psf_draw_component_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                struct psf_gtk_menu *psf_gmenu, 
                                                GtkWidget *main_window,
                                                GtkWidget *itemTEvo){
    GtkWidget *hbox_component;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  main_window);
    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  psf_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "evolution", (gpointer)  itemTEvo);
    
    psf_gmenu->psf_component_model = gtk_string_list_new(null_text_list);
    reset_psf_component_model(kemo_gl, SURFACE_RENDERING, psf_gmenu->psf_component_model);
    psf_gmenu->combobox_comp = init_list_button_by_list_model(psf_gmenu->psf_component_model);
    g_signal_connect(G_OBJECT(psf_gmenu->combobox_comp), "notify::selected-item",
                     G_CALLBACK(psf_component_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_component_list(kemo_gl, SURFACE_RENDERING,
                                                  psf_gmenu->psf_component_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(psf_gmenu->combobox_comp), id_menu);
    
    hbox_component = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_component), gtk_label_new("Component: "));
    gtk_box_append(GTK_BOX(hbox_component), psf_gmenu->combobox_comp);
    return hbox_component;
}

static GtkWidget * init_psf_draw_field_hbox(struct kemoviewer_gl_type *kemo_gl,
                                            struct psf_gtk_menu *psf_gmenu,
                                            GtkWidget *main_window,
                                            GtkWidget *itemTEvo){
    GtkWidget *hbox_field;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  main_window);
    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  psf_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "evolution", (gpointer)  itemTEvo);
    
    psf_gmenu->psf_field_model = gtk_string_list_new(null_text_list);
    reset_psf_field_model(kemo_gl, SURFACE_RENDERING, psf_gmenu->psf_field_model);
    psf_gmenu->combobox_field = init_list_button_by_list_model(psf_gmenu->psf_field_model);
    g_signal_connect(G_OBJECT(psf_gmenu->combobox_field), "notify::selected-item",
                     G_CALLBACK(psf_field_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_field_list(kemo_gl, SURFACE_RENDERING,
                                              psf_gmenu->psf_field_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(psf_gmenu->combobox_field), id_menu);
    
    hbox_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_field), gtk_label_new("Field: "));
    gtk_box_append(GTK_BOX(hbox_field), psf_gmenu->combobox_field);
    return hbox_field;
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

static GtkWidget * init_current_psf_set_hbox(struct kemoviewer_gl_type *kemo_gl,
                                             struct psf_gtk_menu *psf_gmenu,
                                             GtkWidget *main_window, 
                                             GtkWidget *itemTEvo){
    static GtkWidget *hbox_psfs;
    psf_gmenu->num_psfs = count_loaded_psf(kemo_gl->kemoview_data);

    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  main_window);
    g_object_set_data(G_OBJECT(entry_buf), "psfmenu", (gpointer)  psf_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "evolution", (gpointer)  itemTEvo);
    
    psf_gmenu->psf_list_model = gtk_string_list_new(null_text_list);
    reset_psf_list_model(kemo_gl, psf_gmenu->psf_list_model);
    psf_gmenu->combobox_psfs = init_list_button_by_list_model(psf_gmenu->psf_list_model);
    g_signal_connect(G_OBJECT(psf_gmenu->combobox_psfs), "notify::selected-item",
                     G_CALLBACK(current_psf_select_CB), G_OBJECT(entry_buf));
    
    int id_menu = set_selected_psf_list_address(kemo_gl, psf_gmenu->psf_list_model);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(psf_gmenu->combobox_psfs), id_menu);
    
    hbox_psfs = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_psfs), gtk_label_new("Current PSF: "));
    gtk_box_append(GTK_BOX(hbox_psfs), psf_gmenu->combobox_psfs);
    
    if(g_list_model_get_n_items(G_LIST_MODEL(psf_gmenu->psf_list_model)) <= 1){
        gtk_widget_set_sensitive(psf_gmenu->combobox_psfs, FALSE);
    }else{
        gtk_widget_set_sensitive(psf_gmenu->combobox_psfs, TRUE);
    }
    
    return hbox_psfs;
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
    
    psf_gmenu->hbox_psfs =       init_current_psf_set_hbox(kemo_gl, psf_gmenu, 
                                                           main_window, itemTEvo);
    psf_gmenu->hbox_field =      init_psf_draw_field_hbox(kemo_gl, psf_gmenu,
                                                          main_window, itemTEvo);
    psf_gmenu->hbox_component =  init_psf_draw_component_hbox(kemo_gl, psf_gmenu,
                                                              main_window, itemTEvo);
    
    init_psf_menu_hbox(kemo_gl, psf_gmenu);
    
    GtkWidget *psf_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->closeButton);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->hbox_psfs);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->hbox_field);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->hbox_component);
    
    append_psf_menus_to_hbox(psf_vbox, psf_gmenu);
    
    psf_menu_frame = wrap_into_frame_gtk4("Surfaces", psf_vbox);
    gtk_widget_set_margin_top(psf_menu_frame,    5);
    gtk_widget_set_margin_bottom(psf_menu_frame, 5);
    gtk_widget_set_margin_start(psf_menu_frame,  5);
    gtk_widget_set_margin_end(psf_menu_frame,    5);
    return psf_menu_frame;
}


static gboolean dummy_close_CB(GtkWidget *widget, GdkEvent *event, gpointer data){
    printf("Prevent close button!!\n");
    return TRUE;
}

void init_psf_window(struct kemoviewer_gl_type *kemo_gl,
                     struct psf_gtk_menu *psf_gmenu,
                     GtkWidget *main_window, GtkWidget *itemTEvo){
    if(psf_gmenu->iflag_psfBox > 0){gtk_window_destroy(GTK_WINDOW(psf_gmenu->psfWin));};
    psf_gmenu->iflag_psfBox = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data,
                                                             NUM_LOADED);
    if(psf_gmenu->iflag_psfBox == 0){return;}
    
    gint size_xy[2];
    size_xy[0] = gtk_widget_get_width (main_window);
    size_xy[1] = gtk_widget_get_height(main_window);
    
    psf_gmenu->psfWin = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(psf_gmenu->psfWin), "PSF");
    gtk_widget_set_size_request(psf_gmenu->psfWin, 150, -1);
    gtk_window_set_deletable(GTK_WINDOW(psf_gmenu->psfWin), FALSE);
    
    g_signal_connect(psf_gmenu->psfWin, "close-request",
                     G_CALLBACK(dummy_close_CB), NULL);
    
    g_object_set_data(G_OBJECT(itemTEvo), "psfmenu", (gpointer) psf_gmenu);
    g_object_set_data(G_OBJECT(itemTEvo), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(itemTEvo), "parents", (gpointer) main_window);
    psf_gmenu->psf_frame = init_psf_menu_frame(kemo_gl, psf_gmenu, 
                                               main_window, itemTEvo);
    gtk_window_set_child(GTK_WINDOW(psf_gmenu->psfWin), psf_gmenu->psf_frame);
    
    gtk_widget_set_visible(psf_gmenu->psfWin, TRUE);
    return;
}

