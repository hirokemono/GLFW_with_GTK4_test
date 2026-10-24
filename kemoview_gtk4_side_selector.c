/***********************************************************************
 *  kemoview_gtk4_side_selector.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "kemoview_gtk4_side_selector.h"

const char * const hd_Normal =  "Normal surface";   //  NORMAL_POLYGON
const char * const hd_Reverse = "Reverse surface";  //  REVERSE_POLYGON

const char * const surface_side_list[] = {hd_Normal,
                                          hd_Reverse,
                                          NULL};


/*    Buttons to set surface mode  */
static void init_surface_side_dropdown(int iflag_surface_mode,
                                       GtkWidget *surface_mode_button){
	if(iflag_surface_mode == REVERSE_POLYGON){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surface_mode_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surface_mode_button), 0);
	};
    return;
}

static int find_surface_side_switch(const char *text){
    int iflag = SMOOTH_SHADE;
    if(kemoview_compare_string((int) strlen(text), text, hd_Reverse) > 0){iflag = REVERSE_POLYGON;};
    
    printf("Selected surface mode %d %s\n", iflag, text);
    return iflag;
}

static void selected_surface_mode_CB(GtkDropDown *dropdown,
                                     GParamSpec *pspec,
                                     gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_surface_mode = (int *) g_object_get_data(G_OBJECT(user_data), "surface_mode");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    *iflag_surface_mode = find_surface_side_switch(text);
    kemoview_set_object_property_flags(POLYGON_SWITCH, *iflag_surface_mode,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

GtkWidget * init_surface_mode_button(struct kemoviewer_gl_type *kemo_gl,
                                     int *iflag_surface_mode){
    GtkWidget *surface_mode_button;
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "surface_mode", (gpointer) iflag_surface_mode);
    
    GtkStringList *surface_mode_model = gtk_string_list_new(surface_side_list);
    surface_mode_button = init_list_button_by_list_model(surface_mode_model);
    
    g_signal_connect(G_OBJECT(surface_mode_button), "notify::selected-item", 
                     G_CALLBACK (selected_surface_mode_CB), G_OBJECT(entry_buf));
    
    init_surface_side_dropdown(*iflag_surface_mode, surface_mode_button);
    
    return surface_mode_button;
}
