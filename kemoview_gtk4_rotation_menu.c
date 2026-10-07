/***********************************************************************
 *  kemoview_gtk4_rotation_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_rotation_menu.h"

const char * const hd_Xaxis =  "X-axis";     //  X_AXIS
const char * const hd_Yaxis =  "Y-axis";     //  Y_AXIS
const char * const hd_Zaxis =  "Z-axis";     //  Z_AXIS

const char * const axis_list[] = {hd_Xaxis,
                                  hd_Yaxis,
                                  hd_Zaxis,
                                  NULL};

struct rotation_gtk_menu * init_rotation_menu_box(void){
	struct rotation_gtk_menu *rot_gmenu
			= (struct rotation_gtk_menu *)  malloc(sizeof(struct rotation_gtk_menu));
	rot_gmenu->id_fmt_rot = 0;
	
	rot_gmenu->inc_deg = 2;
	rot_gmenu->iaxis_rot = Z_AXIS;
	return rot_gmenu;
};


/*    Buttons to set rotation direction  */
static void init_rotation_axis_dropdown(struct rotation_gtk_menu *rot_gmenu,
                                        GtkWidget *rot_axis_button){
	if(rot_gmenu->iaxis_rot == Z_AXIS){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(rot_axis_button), 2);
	} else if(rot_gmenu->iaxis_rot == Y_AXIS){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(rot_axis_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(rot_axis_button), 0);
	};
    return;
}

static int find_selected_rotation_axis(const char *text){
    int index_mode = -100;
    if(kemoview_compare_string((int) strlen(text), text, hd_Xaxis) > 0){index_mode = X_AXIS;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_Yaxis) > 0){index_mode = Y_AXIS;}
    else {index_mode = Z_AXIS;};
    return index_mode;
}


static void selected_rot_axis_CB(GtkDropDown *dropdown,
                                 GParamSpec *pspec,
                                 gpointer user_data){
    struct rotation_gtk_menu *rot_gmenu
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    rot_gmenu->iaxis_rot = find_selected_rotation_axis(text);
    return;
}

GtkWidget * init_rotation_direction_hbox(struct kemoviewer_gl_type *kemo_gl,
                                         struct rotation_gtk_menu *rot_gmenu){
    GtkWidget *hbox_rotation_dir;
    
    GtkStringList *rot_axis_model = gtk_string_list_new(axis_list);
    GListStore *rot_axis_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(rot_axis_store, rot_axis_model);
    g_object_unref(rot_axis_model);
    GtkFlattenListModel *rot_axis_flat = gtk_flatten_list_model_new(G_LIST_MODEL(rot_axis_store));
    GtkExpression *rot_axis_expression = gtk_property_expression_new (GTK_TYPE_STRING_OBJECT,
                                                                      NULL,
                                                                      "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "rotation_menu", (gpointer) rot_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    GtkWidget *rot_axis_button = gtk_drop_down_new(G_LIST_MODEL(rot_axis_flat), rot_axis_expression);
    g_signal_connect(G_OBJECT(rot_axis_button), "notify::selected-item", 
                     G_CALLBACK (selected_rot_axis_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *rot_axis_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(rot_axis_button), rot_axis_factory);
//    g_object_unref(rot_axis_expression);     Do not release GtkExpression!!
    g_object_unref(rot_axis_factory);
    g_object_unref(rot_axis_flat);
    init_rotation_axis_dropdown(rot_gmenu, rot_axis_button);
    
    hbox_rotation_dir = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_rotation_dir), gtk_label_new("Rotation axis: "));
	gtk_box_append(GTK_BOX(hbox_rotation_dir), rot_axis_button);
    return hbox_rotation_dir;
}


/*    box to set movie FPS   */
static void rotation_FPS_CB(GtkWidget *entry, gpointer user_data){
    struct rotation_gtk_menu *rot_gmenu
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation_menu");
    rot_gmenu->i_FPS = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
/*    printf("FPS %d\n", rot_gmenu->i_FPS);*/
}

GtkWidget * init_rotation_movie_FPS_hbox(struct rotation_gtk_menu *rot_gmenu){
    GtkWidget *hbox_rot_FPS;
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "rotation_menu", (gpointer) rot_gmenu);
    
    rot_gmenu->i_FPS = 30;
    GtkAdjustment *adj_rot_FPS = gtk_adjustment_new(rot_gmenu->i_FPS, 1, 180, 1, 1, 0.0);
    rot_gmenu->spin_rot_FPS = gtk_spin_button_new(GTK_ADJUSTMENT(adj_rot_FPS), 0, 1);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(rot_gmenu->spin_rot_FPS), 0);
    g_signal_connect(G_OBJECT(rot_gmenu->spin_rot_FPS), "value-changed",
                     G_CALLBACK(rotation_FPS_CB), G_OBJECT(entry_buf));
    
    hbox_rot_FPS = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_rot_FPS), gtk_label_new("FPS for movie: "));
    gtk_box_append(GTK_BOX(hbox_rot_FPS), rot_gmenu->spin_rot_FPS);
    return hbox_rot_FPS;
}


/*    box to set rotation increment   */
static void rotation_increment_CB(GtkWidget *entry, gpointer user_data)
{
	struct rotation_gtk_menu *rot_gmenu 
			= (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation_menu");
	 rot_gmenu->inc_deg = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
/*	printf("radius %d\n", radius);*/
}

GtkWidget * init_rotation_increment_hbox(struct rotation_gtk_menu *rot_gmenu){
    GtkWidget *expander_rot;
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "rotation_menu", (gpointer) rot_gmenu);
    
    GtkAdjustment *adj_rot_increment = gtk_adjustment_new(rot_gmenu->inc_deg, 0.0, 180.0, 1, 1, 0.0);
    rot_gmenu->spin_rot_increment = gtk_spin_button_new(GTK_ADJUSTMENT(adj_rot_increment), 0, 1);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(rot_gmenu->spin_rot_increment), 0);
    g_signal_connect(G_OBJECT(rot_gmenu->spin_rot_increment), "value-changed",
                     G_CALLBACK(rotation_increment_CB),G_OBJECT(entry_buf));
        
    GtkWidget *hbox_rot_increment = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_rot_increment), gtk_label_new("Step (Deg.): "));
    gtk_box_append(GTK_BOX(hbox_rot_increment), rot_gmenu->spin_rot_increment);
    return hbox_rot_increment;
}
