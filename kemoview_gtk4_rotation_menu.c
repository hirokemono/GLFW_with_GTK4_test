/*
 *  kemoview_gtk4_rotation_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_rotation_menu.h"

const char * const hd_Xaxis =  "X-axis";     //  X_AXIS
const char * const hd_Yaxis =  "Y-axis";     //  Y_AXIS
const char * const hd_Zaxis =  "Z-axis";     //  Z_AXIS

const char * const axis_list[] = {hd_Xaxis,
                                  hd_Yaxis,
                                  hd_Zaxis,
                                  NULL};

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
                                 gpointer user_data)
{
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
    g_object_unref(rot_axis_expression);
    g_object_unref(rot_axis_factory);
    g_object_unref(rot_axis_flat);
    init_rotation_axis_dropdown(rot_gmenu, rot_axis_button);
    
    hbox_rotation_dir = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_rotation_dir), gtk_label_new("Rotation axis: "));
	gtk_box_append(GTK_BOX(hbox_rotation_dir), rot_axis_button);
    return hbox_rotation_dir;
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

GtkWidget * create_fixed_label_w_index_tree(void){
    /* Construct empty list storage */
    GtkListStore *child_model = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);    
    /* Construct model for sorting and set to tree view */
    GtkTreeModel *model = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(child_model));
    GtkWidget *label_tree = gtk_tree_view_new();

    gtk_tree_view_set_model(GTK_TREE_VIEW(label_tree), model);
    return label_tree;
}


int append_ci_item_to_tree(const int index, const char *c_tbl, 
                           const int i_data, GtkTreeModel *child_model)
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



struct rotation_gtk_menu * init_rotation_menu_box(void){
	struct rotation_gtk_menu *rot_gmenu
			= (struct rotation_gtk_menu *)  malloc(sizeof(struct rotation_gtk_menu));
	rot_gmenu->id_fmt_rot = 0;
	
	rot_gmenu->inc_deg = 2;
	rot_gmenu->iaxis_rot = Z_AXIS;
	return rot_gmenu;
};

static void set_rotation_fileformat_CB(GtkComboBox *combobox_filefmt, gpointer user_data)
{
	struct rotation_gtk_menu *rot_gmenu 
			= (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    rot_gmenu->id_fmt_rot = gtk_selected_combobox_index(combobox_filefmt);
    draw_full_gl(kemo_gl);
	return;
};

static void rotation_FPS_CB(GtkWidget *entry, gpointer user_data)
{
    struct rotation_gtk_menu *rot_gmenu
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation");
     rot_gmenu->i_FPS = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
/*    printf("FPS %d\n", rot_gmenu->i_FPS);*/
}

static void rotation_increment_CB(GtkWidget *entry, gpointer user_data)
{
	struct rotation_gtk_menu *rot_gmenu 
			= (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation");
	 rot_gmenu->inc_deg = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
/*	printf("radius %d\n", radius);*/
}

static void rotation_view_CB(GtkButton *button, gpointer user_data){
	GtkEntry *entry = GTK_ENTRY(user_data);
	GtkWidget *window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parent"));
	struct rotation_gtk_menu *rot_gmenu 
			= (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
	
	gtk_window_set_focus(GTK_WINDOW(window), NULL);
    draw_rotate_views(kemo_gl, rot_gmenu->iaxis_rot, rot_gmenu->inc_deg, IONE);
	return;
};

static void rotation_save_CB(GtkButton *button, gpointer user_data){
	GtkEntryBuffer *entry_buf = GTK_ENTRY_BUFFER(user_data);
	GtkWidget *window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parent"));
	struct rotation_gtk_menu *rot_gmenu 
			= (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");

	int id_image;
    kemoview_gtk4_save_file_select(button, G_OBJECT(entry_buf));
	struct kv_string *filename = kemoview_init_kvstring_by_string(gtk_entry_buffer_get_text(entry_buf));
    struct kv_string *stripped_ext = kemoview_alloc_kvstring();
	struct kv_string *file_prefix = kemoview_alloc_kvstring();
	
	kemoview_get_ext_from_file_name(filename, file_prefix, stripped_ext);
	id_image = kemoview_set_image_file_format_id(stripped_ext);
	if(id_image < 0) {
		id_image = rot_gmenu->id_fmt_rot;
	};
	if(id_image == 0) return;
	kemoview_free_kvstring(stripped_ext);
	kemoview_free_kvstring(filename);
	
	gtk_window_set_focus(GTK_WINDOW(window), NULL);
    sel_write_rotate_views(kemo_gl, rot_gmenu->id_fmt_rot, file_prefix,
                           rot_gmenu->i_FPS, rot_gmenu->iaxis_rot, rot_gmenu->inc_deg);
	
	return;
};


GtkWidget * init_rotation_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                        struct rotation_gtk_menu *rot_gmenu,
                                        GtkWidget *window){
    GtkWidget *expander_rot;
    
    GtkWidget *entry_rotation_file = gtk_entry_new();
    g_object_set_data(G_OBJECT(entry_rotation_file), "kemoview_gl",  (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_rotation_file), "parent", (gpointer) window);
    g_object_set_data(G_OBJECT(entry_rotation_file), "rotation", (gpointer) rot_gmenu);
    
    
	GtkWidget *hbox_rotation_dir = init_rotation_direction_hbox(kemo_gl, rot_gmenu);
    
	GtkWidget *label_tree_rotation_fileformat = create_fixed_label_w_index_tree();
	GtkTreeModel *model_rotation_fileformat = gtk_tree_view_get_model(GTK_TREE_VIEW(label_tree_rotation_fileformat));  
	GtkTreeModel *child_model_rotation_fileformat = 
			gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(model_rotation_fileformat));
	int index = 0;
	index = append_ci_item_to_tree(index, "No Image", NO_SAVE_FILE,  child_model_rotation_fileformat);
	index = append_ci_item_to_tree(index, "PNG",      SAVE_PNG,      child_model_rotation_fileformat);
	index = append_ci_item_to_tree(index, "BMP",      SAVE_BMP,      child_model_rotation_fileformat);
#ifdef FFMPEG
    index = append_ci_item_to_tree(index, "Movie",    SAVE_QT_MOVIE, child_model_rotation_fileformat);
#endif

	rot_gmenu->combobox_rotation_fileformat = 
			gtk_combo_box_new_with_model(child_model_rotation_fileformat);
	GtkCellRenderer *renderer_rotation_fileformat = gtk_cell_renderer_text_new();
	rot_gmenu->id_fmt_rot = NO_SAVE_FILE;
	if(rot_gmenu->id_fmt_rot == SAVE_BMP){
		gtk_combo_box_set_active(GTK_COMBO_BOX(rot_gmenu->combobox_rotation_fileformat), 2);
	} else if(rot_gmenu->id_fmt_rot == SAVE_PNG){
		gtk_combo_box_set_active(GTK_COMBO_BOX(rot_gmenu->combobox_rotation_fileformat), 1);
	} else {
		gtk_combo_box_set_active(GTK_COMBO_BOX(rot_gmenu->combobox_rotation_fileformat), 0);
	};
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(rot_gmenu->combobox_rotation_fileformat), 
							   renderer_rotation_fileformat, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(rot_gmenu->combobox_rotation_fileformat), 
								   renderer_rotation_fileformat, "text", COLUMN_FIELD_NAME, NULL);
	g_signal_connect(G_OBJECT(rot_gmenu->combobox_rotation_fileformat), "changed",
                     G_CALLBACK(set_rotation_fileformat_CB), entry_rotation_file);
	
	
    rot_gmenu->i_FPS = 30;
	GtkAdjustment *adj_rot_FPS = gtk_adjustment_new(rot_gmenu->i_FPS, 1, 180, 1, 1, 0.0);
	rot_gmenu->spin_rot_FPS = gtk_spin_button_new(GTK_ADJUSTMENT(adj_rot_FPS), 0, 1);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(rot_gmenu->spin_rot_FPS), 0);
	g_signal_connect(G_OBJECT(rot_gmenu->spin_rot_FPS), "value-changed",
					 G_CALLBACK(rotation_FPS_CB),entry_rotation_file);
		
    GtkAdjustment *adj_rot_increment = gtk_adjustment_new(rot_gmenu->inc_deg, 0.0, 180.0, 1, 1, 0.0);
    rot_gmenu->spin_rot_increment = gtk_spin_button_new(GTK_ADJUSTMENT(adj_rot_increment), 0, 1);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(rot_gmenu->spin_rot_increment), 0);
    g_signal_connect(G_OBJECT(rot_gmenu->spin_rot_increment), "value-changed",
                     G_CALLBACK(rotation_increment_CB),entry_rotation_file);
        
	rot_gmenu->rotView_Button = gtk_button_new_with_label("View Rotation");
	g_signal_connect(G_OBJECT(rot_gmenu->rotView_Button), "clicked", 
					 G_CALLBACK(rotation_view_CB), (gpointer)entry_rotation_file);
	rot_gmenu->rotSave_Button = gtk_button_new_with_label("Save Rotation");
	g_signal_connect(G_OBJECT(rot_gmenu->rotSave_Button), "clicked", 
					 G_CALLBACK(rotation_save_CB), (gpointer)entry_rotation_file);
	
	
	GtkWidget *hbox_rot_increment = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_rot_increment), gtk_label_new("Step (Deg.): "));
	gtk_box_append(GTK_BOX(hbox_rot_increment), rot_gmenu->spin_rot_increment);
	
    GtkWidget *hbox_rot_FPS = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_rot_FPS), gtk_label_new("FPS for movie: "));
    gtk_box_append(GTK_BOX(hbox_rot_FPS), rot_gmenu->spin_rot_FPS);
    	
	GtkWidget *hbox_rotation_fileformat = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_rotation_fileformat), gtk_label_new("File format: "));
	gtk_box_append(GTK_BOX(hbox_rotation_fileformat), rot_gmenu->combobox_rotation_fileformat);
	
	GtkWidget *hbox_rotation_save = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_rotation_save), rot_gmenu->rotView_Button);
	gtk_box_append(GTK_BOX(hbox_rotation_save), rot_gmenu->rotSave_Button);
	
	
    GtkWidget *rot_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_append(GTK_BOX(rot_box), hbox_rotation_dir);
	gtk_box_append(GTK_BOX(rot_box), hbox_rot_increment);
    gtk_box_append(GTK_BOX(rot_box), hbox_rot_FPS);
	gtk_box_append(GTK_BOX(rot_box), hbox_rotation_fileformat);
	gtk_box_append(GTK_BOX(rot_box), hbox_rotation_save);
	
	expander_rot = wrap_into_scroll_expansion_gtk4("Rotation", 360, 240, window, rot_box);
	return expander_rot;
}
