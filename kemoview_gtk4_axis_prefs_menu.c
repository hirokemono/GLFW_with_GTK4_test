/***********************************************************************
 *  kemoview_gtk4_axis_prefs_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "kemoview_gtk4_axis_prefs_menu.h"

const char * const hd_Tube =  "Tube";  //  IFLAG_ON
const char * const hd_Line =  "Line";  //  IFLAG_OFF

const char * const line_fmt_list[] = {hd_Tube,
                                       hd_Line,
                                       NULL};


const char * const hd_Centre =  "Center";       //  IFLAG_OFF
const char * const hd_Lower =   "Lower left";  //   IFLAG_ON

const char * const line_fmt_list[] = {hd_Centre,
                                      hd_Lower,
                                       NULL};



/*    Buttons to set line format  */
static void init_tube_switch_dropdown(int iflag_tube,
                                      GtkWidget *line_format_button){
	if(iflag_tube == IFLAG_ON){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(line_format_button), 0);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(line_format_button), 1);
	};
    return;
}

static int find_selected_tube_switch(const char *text){
    int iflag_tube = IFLAG_OFF;
    if(kemoview_compare_string((int) strlen(text), text, hd_Tube) > 0){iflag_tube = IFLAG_ON;};
    
    printf("Selected tube switch %d %s\n", iflag_tube, text);
    return iflag_tube;
}

static void selected_line_format_CB(GtkDropDown *dropdown,
                                    GParamSpec *pspec,
                                    gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_tube = (int *) g_object_get_data(G_OBJECT(user_data), "line_format");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    *iflag_tube = find_selected_tube_switch(text);
    kemoview_set_view_integer(COASTLINE_TUBE, *iflag_tube,
                              kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

static GtkWidget * init_coastline_tube_button(struct kemoviewer_gl_type *kemo_gl,
                                              int *iflag_tube){
    GtkWidget *line_format_button;
    
    GtkStringList *line_format_model = gtk_string_list_new(line_fmt_list);
    GListStore *line_format_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(line_format_store, line_format_model);
    GtkFlattenListModel *line_format_flat = gtk_flatten_list_model_new(G_LIST_MODEL(line_format_store));
    GtkExpression *line_format_expression = gtk_property_expression_new(GTK_TYPE_STRING_OBJECT,
                                                                         NULL,
                                                                         "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "line_format", (gpointer) iflag_tube);
    
    line_format_button = gtk_drop_down_new(G_LIST_MODEL(line_format_flat),
                                           line_format_expression);
    g_signal_connect(G_OBJECT(line_format_button), "notify::selected-item", 
                     G_CALLBACK (selected_movie_format_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *rot_file_fmt_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(line_format_button), rot_file_fmt_factory);
    init_tube_switch_dropdown(*iflag_tube, line_format_button);
    
//    g_object_unref(line_format_expression);       Do not release GtkExpression!!
//    g_object_unref(rot_file_fmt_factory);          Do not release GtkListItemFactory!!
//    g_object_unref(line_format_flat);             Do not release GtkFlattenListModel!!
    
    return line_format_button;
}


/*    Buttons to set axis position  */
static void init_axis_position_dropdown(int iflag_tube,
                                        GtkWidget *line_format_button){
	if(iflag_tube == IFLAG_ON){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(line_format_button), 0);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(line_format_button), 1);
	};
    return;
}

static int find_selected_tube_switch(const char *text){
    int iflag_tube = IFLAG_OFF;
    if(kemoview_compare_string((int) strlen(text), text, hd_Tube) > 0){iflag_tube = IFLAG_ON;};
    
    printf("Selected tube switch %d %s\n", iflag_tube, text);
    return iflag_tube;
}



/*
static void set_coasttube_CB(GtkComboBox *combobox_coasttube, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
    int index_mode = gtk_selected_combobox_index(combobox_coasttube);
    
    kemoview_set_view_integer(COASTLINE_TUBE, index_mode,
                              kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
};
*/

GtkWidget * init_coastline_pref_menu(struct kemoviewer_gl_type *kemo_gl){
    /*
    GtkWidget * label_tree_coasttube = create_fixed_label_w_index_tree();
    GtkTreeModel * model_coasttube = gtk_tree_view_get_model(GTK_TREE_VIEW(label_tree_coasttube));
    GtkTreeModel * child_model_coasttube = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(model_coasttube));
    int index = 0;
    index = append_ci_item_to_tree(index, "Tube",
                                   ON, child_model_coasttube);
    index = append_ci_item_to_tree(index, "Line",
                                   OFF, child_model_coasttube);

    GtkWidget *combobox_coasttube = gtk_combo_box_new_with_model(child_model_coasttube);
    GtkCellRenderer *renderer_coasttube = gtk_cell_renderer_text_new();
    if(kemoview_get_view_integer(kemo_gl->kemoview_data, COASTLINE_TUBE) == ON){
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_coasttube), 0);
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_coasttube), 1);
    };
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_coasttube),
                               renderer_coasttube, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_coasttube),
                                   renderer_coasttube, "text",
                                   COLUMN_FIELD_NAME, NULL);
    g_signal_connect(G_OBJECT(combobox_coasttube), "changed",
                G_CALLBACK(set_coasttube_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_coasttube = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    */
    
    int iflag_tube = kemoview_get_view_integer(kemo_gl->kemoview_data, COASTLINE_TUBE)
    GtkWidget *combobox_coasttube = init_coastline_tube_button(kemo_gl,
                                                               &iflag_tube);
    gtk_box_append(GTK_BOX(hbox_coasttube), gtk_label_new("Coastline type: "));
    gtk_box_append(GTK_BOX(hbox_coasttube), combobox_coasttube);
    return wrap_into_frame_gtk4("Coastline type", hbox_coasttube);
}


static void set_axisposition_CB(GtkComboBox *combobox_axispos, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
    int index_mode = gtk_selected_combobox_index(combobox_axispos);
    
    kemoview_set_object_property_flags(AXIS_POSITION, index_mode,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
};

static void axis_thickness_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double current_thick;
    int current_digit;
    
    double thick_in = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    if(thick_in < 0) return;
    
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                 &current_thick, &current_digit);
    kemoview_set_axis_thickness_w_exp(thick_in, current_digit,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}
static void axis_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    double current_thick;
    int current_digit;
    
    int in_digit = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                      &current_thick, &current_digit);
    kemoview_set_axis_thickness_w_exp(current_thick, in_digit,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

GtkWidget * init_axis_position_menu(struct kemoviewer_gl_type *kemo_gl){
    GtkWidget * label_tree_axispos = create_fixed_label_w_index_tree();
    GtkTreeModel * model_axispos
            = gtk_tree_view_get_model(GTK_TREE_VIEW(label_tree_axispos));
    GtkTreeModel * child_model_axispos
            = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(model_axispos));
    int index = 0;
    index = append_ci_item_to_tree(index, "Center",
                                   OFF, child_model_axispos);
    index = append_ci_item_to_tree(index, "Lower left",
                                   ON, child_model_axispos);

    GtkWidget *combobox_axispos
            = gtk_combo_box_new_with_model(child_model_axispos);
    GtkCellRenderer *renderer_axisposition = gtk_cell_renderer_text_new();
    if(kemoview_get_object_property_flags(kemo_gl->kemoview_data,
                                          AXIS_POSITION) == ON){
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_axispos), 1);
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_axispos), 0);
    };
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_axispos),
                               renderer_axisposition, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_axispos),
                                   renderer_axisposition, "text",
                                   COLUMN_FIELD_NAME, NULL);
    g_signal_connect(G_OBJECT(combobox_axispos), "changed",
                G_CALLBACK(set_axisposition_CB), (gpointer) kemo_gl);
    
    
    int int_thick, current_digit;
    double current_thick;
    GtkAdjustment *adj_thick = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    GtkAdjustment *adj_digit = gtk_adjustment_new(-3, -30, 30, 1, 1, 0.0);
    GtkWidget *spin_axis_thick = gtk_spin_button_new(GTK_ADJUSTMENT(adj_thick), 0, 0);
    GtkWidget *spin_axis_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
    g_signal_connect(spin_axis_thick, "value-changed",
                     G_CALLBACK(axis_thickness_CB), (gpointer) kemo_gl);
    g_signal_connect(spin_axis_digit, "value-changed",
                     G_CALLBACK(axis_digit_CB), (gpointer) kemo_gl);
 
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                      &current_thick, &current_digit);
    int_thick = (int) current_thick;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_axis_thick), (double) int_thick);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_axis_digit), (double) current_digit);

    
    GtkWidget *hbox_axispos = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_axispos), gtk_label_new("Axis position: "));
    gtk_box_append(GTK_BOX(hbox_axispos), combobox_axispos);
    
    GtkWidget *hbox_axiswidth = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_axiswidth), gtk_label_new("Thickness: "));
    gtk_box_append(GTK_BOX(hbox_axiswidth), spin_axis_thick);
    gtk_box_append(GTK_BOX(hbox_axiswidth), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(hbox_axiswidth), spin_axis_digit);
    
    GtkWidget *axis_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(axis_vbox), hbox_axispos);
    gtk_box_append(GTK_BOX(axis_vbox), hbox_axiswidth);

    return wrap_into_frame_gtk4("Axis parameters", axis_vbox);
}

