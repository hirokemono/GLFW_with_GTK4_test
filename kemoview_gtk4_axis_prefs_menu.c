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
                     G_CALLBACK (selected_line_format_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *line_format_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(line_format_button), line_format_factory);
    init_tube_switch_dropdown(*iflag_tube, line_format_button);
    
//    g_object_unref(line_format_expression);       Do not release GtkExpression!!
//    g_object_unref(line_format_factory);          Do not release GtkListItemFactory!!
//    g_object_unref(line_format_flat);             Do not release GtkFlattenListModel!!
    
    return line_format_button;
}


GtkWidget * init_coastline_pref_menu(struct kemoviewer_gl_type *kemo_gl){
    int iflag_tube = kemoview_get_view_integer(kemo_gl->kemoview_data, COASTLINE_TUBE);
    GtkWidget *combobox_coasttube = init_coastline_tube_button(kemo_gl,
                                                               &iflag_tube);
    
    GtkWidget *hbox_coasttube = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_coasttube), gtk_label_new("Coastline type: "));
    gtk_box_append(GTK_BOX(hbox_coasttube), combobox_coasttube);
    return wrap_into_frame_gtk4("Coastline type", hbox_coasttube);
}

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
    int iflag_lowerleft = kemoview_get_object_property_flags(kemo_gl->kemoview_data, AXIS_POSITION);
    GtkWidget *combobox_axispos = init_axis_position_button(kemo_gl, 
                                                            &iflag_lowerleft);
    
    int int_thick, current_digit;
    double current_thick;
    GtkAdjustment *adj_thick = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    GtkWidget *spin_axis_thick = gtk_spin_button_new(GTK_ADJUSTMENT(adj_thick), 0, 0);
    gtk_editable_set_width_chars(GTK_EDITABLE(spin_axis_thick), 1);
    g_signal_connect(spin_axis_thick, "value-changed",
                     G_CALLBACK(axis_thickness_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_digit = gtk_adjustment_new(-3, -30, 30, 1, 1, 0.0);
    GtkWidget *spin_axis_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
    gtk_editable_set_width_chars(GTK_EDITABLE(spin_axis_digit), 3);
    g_signal_connect(spin_axis_digit, "value-changed",
                     G_CALLBACK(axis_digit_CB), (gpointer) kemo_gl);
 
    kemoview_get_axis_thickness_w_exp(kemo_gl->kemoview_data,
                                      &current_thick, &current_digit);
    int_thick = (int) current_thick;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_axis_thick), (double) int_thick);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_axis_digit), (double) current_digit);
    
    GtkWidget *grid_axis_pref = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid_axis_pref), gtk_label_new("Axis position: "), 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), combobox_axispos,                 3, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid_axis_pref), gtk_label_new("Thickness: "), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), spin_axis_thick,              1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), gtk_label_new("X 10^"),       2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_axis_pref), spin_axis_digit,              3, 1, 1, 1);
    
    return wrap_into_frame_gtk4("Axis parameters", grid_axis_pref);
}

