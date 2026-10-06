/*
 *  kemoview_gtk4_viewmode_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_viewmode_menu.h"

const char * const hd_3Dview =  "3D-View";     //  VIEW_3D
const char * const hd_Stereo =  "Stereo-View"; //  VIEW_STEREO
const char * const hd_Mapview = "Map-View";    //  VIEW_MAP
const char * const hd_XYview =  "XY-View";     //  VIEW_XY
const char * const hd_XZview =  "XZ-View";     //  VIEW_XZ
const char * const hd_YZview =  "YZ-View";     //  VIEW_YZ

const char * const view_modes[] = {hd_3Dview,
                                   hd_Stereo,
                                   hd_Mapview,
                                   hd_XYview,
                                   hd_XZview,
                                   hd_YZview,
                                   NULL};

/* Append new data at the end of list */
int append_ci_item_to_tree(const int index, const char *c_tbl, const int i_data, GtkTreeModel *child_model)
{
    GtkTreeIter iter;
    
    gtk_list_store_append(GTK_LIST_STORE(child_model), &iter);
    gtk_list_store_set(GTK_LIST_STORE(child_model), &iter,
                       COLUMN_FIELD_INDEX, index,
                       COLUMN_FIELD_NAME,  c_tbl,
                       COLUMN_FIELD_MATH,  i_data,
                       -1);
    return index + 1;
}

GtkWidget * create_fixed_label_w_index_tree(void){
    /* Construct empty list storage */
    GtkListStore *child_model = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);    
    /* Construct model for sorting and set to tree view */
    GtkTreeModel *model = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(child_model));
    GtkWidget *label_tree = gtk_tree_view_new();

    gtk_tree_view_set_model(GTK_TREE_VIEW(label_tree), model);
    return label_tree;
}


int gtk_selected_combobox_index(GtkComboBox *combobox){
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



static void set_GLFW_viewtype(int index_mode, 
                              struct kemoviewer_gl_type *kemo_gl,
                              struct view_widgets *view_menu){
    set_GLFW_viewtype_mode(index_mode);
    kemoview_set_viewtype(index_mode, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    
    if(kemoview_get_view_type_flag(kemo_gl->kemoview_data) == VIEW_STEREO){
        gtk_widget_set_sensitive(view_menu->Frame_stereo, TRUE);
    }else{
        gtk_widget_set_sensitive(view_menu->Frame_stereo, FALSE);
    };
    return;
}

static void set_viewtype_CB(GtkComboBox *combobox_viewtype, gpointer user_data)
{
    struct view_widgets *view_menu = (struct view_widgets *) user_data;
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(combobox_viewtype), "kemoview_gl");

	int index_mode = gtk_selected_combobox_index(combobox_viewtype);
    set_GLFW_viewtype(index_mode, kemo_gl, view_menu);
	return;
};

static void setup_header_CB(GtkSignalListItemFactory *factory,
                            GObject *list_item,
                            gpointer data){
    GtkListHeader *self = GTK_LIST_HEADER (list_item);
    GtkWidget *child = gtk_label_new ("");
    gtk_label_set_xalign(GTK_LABEL (child), 0);
    gtk_label_set_use_markup(GTK_LABEL (child), TRUE);
    gtk_widget_set_margin_top (child, 10);
    gtk_widget_set_margin_bottom(child, 10);
    
    gtk_list_header_set_child(self, child);
}

static void bind_header_CB(GtkSignalListItemFactory *factory,
                           GObject *list_item,
                           gpointer data)
{
    GtkListHeader *self = GTK_LIST_HEADER (list_item);
    GtkWidget *child = gtk_list_header_get_child (self);
    GObject *item = gtk_list_header_get_item (self);
    printf("bind_header_CB\n");
}

static void on_dropdown_selected_changed(GtkDropDown *dropdown,
                                         GParamSpec *pspec,
                                         gpointer user_data)
{
    struct view_widgets *view_menu
            = (struct view_widgets *) g_object_get_data(G_OBJECT(user_data), "view_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    int index_mode = -100;
    
    if (selected_item != NULL){
        const char *text = gtk_string_object_get_string (selected_item);
        g_print ("Selected option: %ld %s \n", strlen(text), text);
        if(compare_string((int) strlen(text), text, hd_Stereo) > 0){index_mode = VIEW_STEREO;}
        else if(compare_string((int) strlen(text), text, hd_Mapview) > 0){index_mode = VIEW_MAP;}
        else if(compare_string((int) strlen(text), text, hd_XYview) > 0){index_mode = VIEW_XY;}
        else if(compare_string((int) strlen(text), text, hd_XZview) > 0){index_mode = VIEW_XZ;}
        else if(compare_string((int) strlen(text), text, hd_YZview) > 0){index_mode = VIEW_YZ;}
        else {index_mode = VIEW_3D;};
        g_print ("Selected ID: %d\n", index_mode);
        set_GLFW_viewtype(index_mode, kemo_gl, view_menu);
    }
}

GtkWidget * make_gtk4_viewmode_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                        struct view_widgets *view_menu){
    /* A dropdown using an expression to obtain strings */
    GtkStringList *viewmode_model = gtk_string_list_new(view_modes);
    GListStore *viewmode_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(viewmode_store, viewmode_model);
    g_object_unref(viewmode_model);
    GtkFlattenListModel *viewmode_flat = gtk_flatten_list_model_new(G_LIST_MODEL(viewmode_store));
    GtkExpression *viewmode_expression = gtk_property_expression_new (GTK_TYPE_STRING_OBJECT,
                                                             NULL,
                                                             "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "view_menu",  (gpointer) view_menu);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",  (gpointer) kemo_gl);
    
    GtkWidget *viewmode_button = gtk_drop_down_new(G_LIST_MODEL(viewmode_flat), viewmode_expression);
    g_signal_connect (viewmode_button, "notify::selected-item", 
                      G_CALLBACK (on_dropdown_selected_changed), G_OBJECT(entry_buf));
/*    gtk_drop_down_set_enable_search (GTK_DROP_DOWN (viewmode_button), TRUE); */
    GtkListItemFactory *viewmode_factory = gtk_signal_list_item_factory_new ();
//    g_signal_connect (viewmode_factory, "setup", G_CALLBACK (setup_header_CB), NULL);
//    g_signal_connect (viewmode_factory, "bind", G_CALLBACK (bind_header_CB), NULL);
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN (viewmode_button), viewmode_factory);
    g_object_unref (viewmode_factory);
    
    GtkWidget *hbox_viewtype;
    
    GtkWidget *label_tree_viewtype = create_fixed_label_w_index_tree();
	GtkTreeModel *model_viewtype = gtk_tree_view_get_model(GTK_TREE_VIEW(label_tree_viewtype));  
    GtkTreeModel *child_model_viewtype
            = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(model_viewtype));
	int index = 0;
	index = append_ci_item_to_tree(index, "3D-View", VIEW_3D, child_model_viewtype);
	index = append_ci_item_to_tree(index, "Stereo-View", VIEW_STEREO, child_model_viewtype);
	index = append_ci_item_to_tree(index, "Map-View", VIEW_MAP, child_model_viewtype);
	index = append_ci_item_to_tree(index, "XY-View", VIEW_XY, child_model_viewtype);
	index = append_ci_item_to_tree(index, "XZ-View", VIEW_XZ, child_model_viewtype);
	index = append_ci_item_to_tree(index, "YZ-View", VIEW_YZ, child_model_viewtype);
	
	GtkWidget *combobox_viewtype = gtk_combo_box_new_with_model(child_model_viewtype);
	GtkCellRenderer *renderer_viewtype = gtk_cell_renderer_text_new();
	int iflag_mode = kemoview_get_view_type_flag(kemo_gl->kemoview_data);
	if(iflag_mode == VIEW_YZ){
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_viewtype), 5);
	}else if(iflag_mode == VIEW_XZ){
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_viewtype), 4);
	}else if(iflag_mode == VIEW_XY){
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_viewtype), 3);
	}else if(iflag_mode == VIEW_MAP){
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_viewtype), 2);
	}else if(iflag_mode == VIEW_STEREO){
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_viewtype), 1);
	} else {
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_viewtype), 0);
	};
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_viewtype), renderer_viewtype, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_viewtype), renderer_viewtype,
				"text", COLUMN_FIELD_NAME, NULL);
    g_object_set_data(G_OBJECT(combobox_viewtype), "kemoview_gl",  (gpointer) kemo_gl);
	g_signal_connect(G_OBJECT(combobox_viewtype), "changed",
                     G_CALLBACK(set_viewtype_CB), (gpointer) view_menu);
	
	hbox_viewtype = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_viewtype), gtk_label_new("View type: "));
    gtk_box_append(GTK_BOX(hbox_viewtype), viewmode_button);
    gtk_box_append(GTK_BOX(hbox_viewtype), combobox_viewtype);
    
    return hbox_viewtype;
}

