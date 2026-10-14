/***********************************************************************
 *  kemoview_gtk4_shading_selector.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "kemoview_gtk4_shading_selector.h"

const char * const hd_Smooth = "Smooth surface"; //  SMOOTH_SHADE
const char * const hd_Flat =   "Flat surface";   //  FLAT_SHADE

const char * const shading_mode_list[] = {hd_Smooth,
                                          hd_Flat,
                                          NULL};


/*    Buttons to set shading mode  */
static void init_shading_mode_dropdown(int iflag_shading,
                                       GtkWidget *shading_mode_button){
	if(iflag_shading == FLAT_SHADE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(shading_mode_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(shading_mode_button), 0);
	};
    return;
}

static int find_shading_mode_switch(const char *text){
    int iflag_shading = SMOOTH_SHADE;
    if(kemoview_compare_string((int) strlen(text), text, hd_Flat) > 0){iflag_shading = FLAT_SHADE;};
    
    printf("Selected shading mode %d %s\n", iflag_shading, text);
    return iflag_shading;
}

static void selected_shading_mode_CB(GtkDropDown *dropdown,
                                     GParamSpec *pspec,
                                     gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_shading = (int *) g_object_get_data(G_OBJECT(user_data), "shading_mode");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    *iflag_shading = find_shading_mode_switch(text);
    kemoview_set_object_property_flags(SHADING_SWITCH, *iflag_shading,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

GtkWidget * init_shading_mode_button(struct kemoviewer_gl_type *kemo_gl,
                                     int *iflag_shading){
    GtkWidget *shading_mode_button;
    
    GtkStringList *shading_mode_model = gtk_string_list_new(shading_mode_list);
    GListStore *shading_mode_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(shading_mode_store, shading_mode_model);
    GtkFlattenListModel *shading_mode_flat = gtk_flatten_list_model_new(G_LIST_MODEL(shading_mode_store));
    GtkExpression *shading_mode_expression = gtk_property_expression_new(GTK_TYPE_STRING_OBJECT,
                                                                         NULL,
                                                                         "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "shading_mode", (gpointer) iflag_shading);
    
    shading_mode_button = gtk_drop_down_new(G_LIST_MODEL(shading_mode_flat),
                                            shading_mode_expression);
    g_signal_connect(G_OBJECT(shading_mode_button), "notify::selected-item", 
                     G_CALLBACK (selected_shading_mode_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *shading_mode_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(shading_mode_button), shading_mode_factory);
    init_shading_mode_dropdown(*iflag_shading, shading_mode_button);
    
//    g_object_unref(shading_mode_expression);       Do not release GtkExpression!!
//    g_object_unref(shading_mode_factory);          Do not release GtkListItemFactory!!
//    g_object_unref(shading_mode_flat);             Do not release GtkFlattenListModel!!
    
    return shading_mode_button;
}
