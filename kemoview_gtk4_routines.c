/***************************************************************
 *  kemoview_gtk4_routines.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ****************************************************************/

#include "kemoview_gtk4_routines.h"

GtkWidget * wrap_into_frame_gtk4(const char *title, GtkWidget *box_in){
	GtkWidget *hbox;
	
	GtkWidget *Frame = gtk_frame_new(title);
	gtk_frame_set_child(GTK_FRAME(Frame), box_in);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox), gtk_label_new("  "));
	gtk_box_append(GTK_BOX(hbox), Frame);
	return hbox;
}

static void expander_CB(GObject *object, GParamSpec *param_spec, gpointer user_data)
{
    GtkWidget *expander = GTK_WIDGET(object);
    GtkWidget *window = GTK_WIDGET(user_data);
    
    if (gtk_expander_get_expanded(GTK_EXPANDER(expander))){
        printf("Expanded \n");
    }else{
        printf("Hided \n");
    }
    
    GtkRequisition *expander_minimum = gtk_requisition_new();
    GtkRequisition *expander_natural = gtk_requisition_new();
    gtk_widget_get_preferred_size(expander,
                                  expander_minimum, expander_natural);
    
    printf("minimum %d %d \n", expander_minimum->width, expander_minimum->height);
    printf("natural %d %d \n", expander_natural->width, expander_natural->height);
    
    
    gtk_widget_set_size_request(window, expander_minimum->width, expander_minimum->height);
    gtk_window_set_default_size(GTK_WINDOW(window),
                                expander_natural->width, expander_natural->height);
    gtk_widget_queue_draw(window);
}

GtkWidget * wrap_into_scrollbox_gtk4(int width, int height,
                                     GtkWidget *box_in){
    GtkWidget *hbox;
    GtkWidget *scroll;
    
    GtkWidget *Frame = gtk_frame_new("");
    gtk_frame_set_child(GTK_FRAME(Frame), box_in);
    
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox), gtk_label_new("  "));
    gtk_box_append(GTK_BOX(hbox), Frame);
    
    scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scroll, (gint) width, (gint) height);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), hbox);
    return scroll;
}


GtkWidget * wrap_into_expanded_frame_gtk4(const char *title, 
                                          GtkWidget *window,
                                          GtkWidget *box_in){
    GtkWidget *expander = gtk_expander_new_with_mnemonic(title);
    g_signal_connect(expander, "notify::expanded",
                     G_CALLBACK(expander_CB), (gpointer) window);
    gtk_expander_set_child(GTK_EXPANDER(expander), box_in);
    return expander;
}

GtkWidget * wrap_into_scroll_expansion_gtk4(const char *title, 
                                            int width, int height,
                                            GtkWidget *window, 
                                            GtkWidget *box_in){
    GtkWidget *scroll = wrap_into_scrollbox_gtk4(width, height, box_in);
    GtkWidget *expander = gtk_expander_new_with_mnemonic(title);
    g_signal_connect(expander, "notify::expanded", 
                     G_CALLBACK(expander_CB), (gpointer) window);
    gtk_expander_set_child(GTK_EXPANDER(expander), scroll);
    return expander;
}
/*
int gtk4_selected_combobox_index(GtkComboBox *combobox){
    GtkTreeModel *model_cmap = gtk_combo_box_get_model(combobox);
    GtkTreeIter iter;
    
    gchar *row_string;
    int index_field;
    int index_mode;
    
    gint idx = gtk_combo_box_get_active(combobox);
    if(idx < 0) return -1;
    
    GtkTreePath *path = gtk_tree_path_new_from_indices(idx, -1);
    
    gtk_tree_model_get_iter(model_cmap, &iter, path);  
    gtk_tree_model_get(model_cmap, &iter, COLUMN_FIELD_INDEX, &index_field, -1);
    gtk_tree_model_get(model_cmap, &iter, COLUMN_FIELD_NAME, &row_string, -1);
    gtk_tree_model_get(model_cmap, &iter, COLUMN_FIELD_MATH, &index_mode, -1);
    
	printf("Selected mode %d, %s\n", index_mode, row_string);
	return index_mode;
};
*/
