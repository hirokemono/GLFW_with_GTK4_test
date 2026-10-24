/***********************************************************************
 *  kemoview_gtk4_axis_posi_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "kemoview_gtk4_axis_posi_sel.h"

const char * const hd_Centre =  "Center";       //  IFLAG_OFF
const char * const hd_Lower =   "Lower left";  //   IFLAG_ON

const char * const axis_position_list[] = {hd_Centre,
                                           hd_Lower,
                                           NULL};


/*    Buttons to set axis position  */
static void init_axis_position_dropdown(int iflag_lowerleft,
                                        GtkWidget *axis_position_button){
	if(iflag_lowerleft == IFLAG_ON){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(axis_position_button), 0);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(axis_position_button), 1);
	};
    return;
}

static int find_axis_position_switch(const char *text){
    int iflag_lowerleft = IFLAG_OFF;
    if(kemoview_compare_string((int) strlen(text), text, hd_Lower) > 0){iflag_lowerleft = IFLAG_ON;};
    
    printf("Selected tube switch %d %s\n", iflag_lowerleft, text);
    return iflag_lowerleft;
}

static void selected_axis_position_CB(GtkDropDown *dropdown,
                                      GParamSpec *pspec,
                                      gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_lowerleft = (int *) g_object_get_data(G_OBJECT(user_data), "axis_position");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    *iflag_lowerleft = find_axis_position_switch(text);
    kemoview_set_object_property_flags(AXIS_POSITION, *iflag_lowerleft,
                                       kemo_gl->kemoview_data);
    draw_triple_gl(kemo_gl);
    return;
}

GtkWidget * init_axis_position_button(struct kemoviewer_gl_type *kemo_gl,
                                      int *iflag_lowerleft){
    GtkWidget *axis_position_button;
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "axis_position", (gpointer) iflag_lowerleft);
    
    GtkStringList *axis_position_model = gtk_string_list_new(axis_position_list);
    axis_position_button = init_list_button_by_list_model(axis_position_model);
    g_signal_connect(G_OBJECT(axis_position_button), "notify::selected-item", 
                     G_CALLBACK (selected_axis_position_CB), G_OBJECT(entry_buf));
    
    init_axis_position_dropdown(*iflag_lowerleft, axis_position_button);
    
    return axis_position_button;
}
