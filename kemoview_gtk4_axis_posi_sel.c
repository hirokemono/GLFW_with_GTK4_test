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
                                      gpointer user_data)
{
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
    draw_full_gl(kemo_gl);
    return;
}

GtkWidget * init_axis_position_button(struct kemoviewer_gl_type *kemo_gl,
                                      int *iflag_lowerleft){
    GtkWidget *axis_position_button;
    
    GtkStringList *axis_position_model = gtk_string_list_new(axis_position_list);
    GListStore *axis_position_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(axis_position_store, axis_position_model);
    GtkFlattenListModel *axis_position_flat = gtk_flatten_list_model_new(G_LIST_MODEL(axis_position_store));
    GtkExpression *axis_position_expression = gtk_property_expression_new(GTK_TYPE_STRING_OBJECT,
                                                                         NULL,
                                                                         "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "axis_position", (gpointer) iflag_lowerleft);
    
    axis_position_button = gtk_drop_down_new(G_LIST_MODEL(axis_position_flat),
                                             axis_position_expression);
    g_signal_connect(G_OBJECT(axis_position_button), "notify::selected-item", 
                     G_CALLBACK (selected_axis_position_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *axis_position_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(axis_position_button), axis_position_factory);
    init_axis_position_dropdown(*iflag_lowerleft, axis_position_button);
    
//    g_object_unref(axis_position_expression);       Do not release GtkExpression!!
//    g_object_unref(axis_position_factory);          Do not release GtkListItemFactory!!
//    g_object_unref(axis_position_flat);             Do not release GtkFlattenListModel!!
    
    return axis_position_button;
}
