/***********************************************************************
 *  columnview_draw_mesh.c
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/

#include "columnview_draw_mesh.h"

static void sync_kemoview_draw_mesh_from_list(GListStore *store, int iflag_group,
                                              struct kemoviewer_type *kemo_sgl){
    Sw3cColumnItem *item;
    int i;
	int num = (int) g_list_model_get_n_items(G_LIST_MODEL(store));
	for(i=0;i<num;i++){
        item = SW3C_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
        kemoview_set_draw_mesh_item(iflag_group, SURFSOLID_TOGGLE, i,
                                    item->int1, kemo_sgl);
        kemoview_set_draw_mesh_item(iflag_group, SURFGRID_TOGGLE, i,
                                    item->int2, kemo_sgl);
        kemoview_set_draw_mesh_item(iflag_group, SURFNOD_TOGGLE, i,
                                    item->int3, kemo_sgl);
	};
	return;
};

static void sync_list_from_kemoview_draw_mesh(struct kemoviewer_type *kemo_sgl,
                                              int iflag_group,
                                              struct kemo_sw3c_columnview *draw_mesh_vws){
    char label[64];
    int iflag_patch, iflag_grid, iflag_node;
	int i;
    gpointer additions[1];
	int num_list = (int) g_list_model_get_n_items(G_LIST_MODEL(draw_mesh_vws->real3_store));
    for(i=0;i<num_list;i++){
        sprintf(label, "Domain %d", i);
        iflag_patch = kemoview_get_draw_mesh_item(kemo_sgl, iflag_group, SURFSOLID_TOGGLE, i);
        iflag_grid =  kemoview_get_draw_mesh_item(kemo_sgl, iflag_group, SURFGRID_TOGGLE,  i);
        iflag_node =  kemoview_get_draw_mesh_item(kemo_sgl, iflag_group, SURFNOD_TOGGLE,   i);
        additions[0] = G_OBJECT(sw3c_column_item_new((float) iflag_patch, 
                                                     (float) iflag_grid,
                                                     (float) iflag_node,
                                                     label));
        g_list_store_splice(draw_mesh_vws->real3_store, i, IONE, additions, IONE);
    };
    gtk_widget_queue_draw(GTK_WIDGET(draw_mesh_vws->real3_cview_scroll));
    return;
};

/*  Callbacks  */
/* =========================================================================
 *   GtkColumnView Factory Callbacks for draw patch button
 * ========================================================================= */

/*  draw button callbacks */
static void draw_patch_flag_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *button = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(object), "kemoview_gl");
    
    if(item){
        item->int1 = (int) gtk_check_button_get_active(button);
        g_print("Patch draw flag is updated! New integer value: %d\n", item->int1);
        sync_kemoview_draw_mesh_from_list(store, DOMAIN_FLAG, kemo_gl->kemoview_data);
        draw_full_gl(kemo_gl);
    }
}

/* Setup: Allocate and configure the check button */
static void setup_draw_patch_check_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_list_item_set_child(listitem, button);
}

// Bind: Populate widget data from model and listen for changes
static void bind_draw_patch_check_CB(GtkSignalListItemFactory *factory,
                                     GtkListItem *listitem,
                                     gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_object_set_data(G_OBJECT(button), "kemoview_gl",   G_OBJECT(kemo_gl));
    g_object_set_data(G_OBJECT(button), "list_store", G_OBJECT(store));
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, draw_patch_flag_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int1);
    g_signal_handlers_unblock_by_func(button, draw_patch_flag_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(button, "toggled", G_CALLBACK(draw_patch_flag_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_draw_patch_check_CB(GtkSignalListItemFactory *factory,
                                       GtkListItem *listitem,
                                       gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, draw_patch_flag_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for draw grid button
 * ========================================================================= */
static void draw_grid_flag_change_CB(GObject *object, gpointer user_data){
    GtkCheckButton *button = GTK_CHECK_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(object), "kemoview_gl");
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int2 = (int) gtk_check_button_get_active(button);
        g_print("Grid draw flag is updated! New integer value: %d\n", item->int2);
        sync_kemoview_draw_mesh_from_list(store, DOMAIN_FLAG, kemo_gl->kemoview_data);
        draw_full_gl(kemo_gl);
    }
}

/* Setup: Allocate and configure the check button */
static void setup_draw_grid_check_CB(GtkSignalListItemFactory *factory,
                                     GtkListItem *listitem,
                                     gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_list_item_set_child(listitem, button);
}

// Bind: Populate widget data from model and listen for changes
static void bind_draw_grid_check_CB(GtkSignalListItemFactory *factory,
                                    GtkListItem *listitem,
                                    gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_object_set_data(G_OBJECT(button), "kemoview_gl",    G_OBJECT(kemo_gl));
    g_object_set_data(G_OBJECT(button), "list_store", G_OBJECT(store));
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, draw_grid_flag_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int2);
    g_signal_handlers_unblock_by_func(button, draw_grid_flag_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(button, "toggled", G_CALLBACK(draw_grid_flag_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_draw_grid_check_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, draw_grid_flag_change_CB, listitem);
}


/* =========================================================================
 *   GtkColumnView Factory Callbacks for draw node button
 * ========================================================================= */
static void draw_node_flag_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *button = GTK_CHECK_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(object), "kemoview_gl");
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int3 = (int) gtk_check_button_get_active(button);
        g_print("Value updated! New integer value: %d\n", item->int3);
        sync_kemoview_draw_mesh_from_list(store, DOMAIN_FLAG, kemo_gl->kemoview_data);
        draw_full_gl(kemo_gl);
    }
}

/* Setup: Allocate and configure the check button */
static void setup_draw_node_check_CB(GtkSignalListItemFactory *factory,
                                     GtkListItem *listitem,
                                     gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_list_item_set_child(listitem, button);
}

// Bind: Populate widget data from model and listen for changes
static void bind_draw_node_check_CB(GtkSignalListItemFactory *factory,
                                    GtkListItem *listitem,
                                    gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_object_set_data(G_OBJECT(button), "kemoview_gl",   G_OBJECT(kemo_gl));
    g_object_set_data(G_OBJECT(button), "list_store", G_OBJECT(store));
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, draw_node_flag_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int3);
    g_signal_handlers_unblock_by_func(button, draw_node_flag_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(button, "toggled", G_CALLBACK(draw_node_flag_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_draw_node_check_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, draw_node_flag_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for label box
 * ========================================================================= */
/* Bind: Populate widget data from model and listen for changes */
/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_str_label_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(GTK_LIST_ITEM(listitem),label);
}

static void bind_str_label_CB(GtkSignalListItemFactory *factory,
                              GtkListItem *listitem,
                              gpointer user_data){
    GtkWidget *label = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = item->str;
    g_print("bind_col1_CB value is %s\n",string); // Show on std out
    gtk_label_set_text(GTK_LABEL(label), string);
}

/* =========================================================================
 *   Callbacks for buttuns to control list model data
 * ========================================================================= */

/* Construct columnview */
static GListStore *init_lightposition_list_store(int iflag_group, 
                                                 struct kemoviewer_type *kemo_sgl){
    char label[64];
    float rtp[3];
    int i;
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    for(i=0;i<kemoview_get_num_of_mesh_group(kemo_sgl, iflag_group);i++){
        sprintf(label, "Domain %d", i);
        rtp[0] = (float) kemoview_get_draw_mesh_item(kemo_sgl, iflag_group, SURFSOLID_TOGGLE, i);
        rtp[1] = (float) kemoview_get_draw_mesh_item(kemo_sgl, iflag_group, SURFGRID_TOGGLE, i);
        rtp[2] = (float) kemoview_get_draw_mesh_item(kemo_sgl, iflag_group, SURFNOD_TOGGLE, i);
        g_list_store_append(store, sw3c_column_item_new(rtp[0], rtp[1], rtp[2], label));
    };
    return store;
};

static GtkWidget * light_position_columnview_init(struct kemoviewer_gl_type *kemo_gl,
                                                  GListStore *store){
    GtkWidget *cview = gtk_column_view_new(NULL);
    g_object_set_data(G_OBJECT(store), "kemoview_gl",   G_OBJECT(kemo_gl));
    
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW(cview)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(cview), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup",  G_CALLBACK(setup_draw_patch_check_CB),  NULL);
    g_signal_connect(factory1, "bind",   G_CALLBACK(bind_draw_patch_check_CB),   G_OBJECT(store));
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_draw_patch_check_CB), NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Patch", factory1);
    sorter = GTK_SORTER(gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM, 
                                                                           NULL, "int1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup",  G_CALLBACK(setup_draw_grid_check_CB),  NULL);
    g_signal_connect(factory2, "bind",   G_CALLBACK(bind_draw_grid_check_CB),   G_OBJECT(store));
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_draw_grid_check_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Grid", factory2);
    sorter = GTK_SORTER(gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM, 
                                                                           NULL, "int2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_draw_node_check_CB),  NULL);
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_draw_node_check_CB),   G_OBJECT(store));
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_draw_node_check_CB), NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Node", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "int3")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER (sorter));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(cview), column3);
    
    // Factory setup and bind for column 4
    GtkListItemFactory *factory4 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory4, "setup",  G_CALLBACK(setup_str_label_CB),  NULL);
    g_signal_connect(factory4, "bind",   G_CALLBACK(bind_str_label_CB),   NULL);
//    g_signal_connect(factory4, "unbind", G_CALLBACK(unbind_str_label_CB), NULL);
    GtkColumnViewColumn *column4 = gtk_column_view_column_new("Group", factory4);
    sorter = GTK_SORTER (gtk_string_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                           NULL, "string")));
    gtk_column_view_column_set_sorter (column4, GTK_SORTER(sorter));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(cview), column4);
    return cview;
}


struct kemo_sw3c_columnview * init_draw_mesh_list_scroll(struct kemoviewer_gl_type *kemo_gl,
                                                          GtkWidget *mesh_window){
    struct kemo_sw3c_columnview *draw_mesh_vws = alloc_kemo_sw3c_columnview();
    
    // Add column view.  Put in scrolled window.  
    draw_mesh_vws->real3_store = init_lightposition_list_store(DOMAIN_FLAG, kemo_gl->kemoview_data); 
    draw_mesh_vws->real3_cview = light_position_columnview_init(kemo_gl, draw_mesh_vws->real3_store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW (draw_mesh_vws->real3_cview),TRUE);
    g_object_set_data(G_OBJECT(draw_mesh_vws->real3_cview), "list_store",
                      G_OBJECT(draw_mesh_vws->real3_store));
    g_object_set_data(G_OBJECT(draw_mesh_vws->real3_cview), "kemoview_gl",
                      G_OBJECT(kemo_gl));
    
    GtkWidget *show_all_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_all_button), "clicked",
                     G_CALLBACK(show_all_sw3c_button_CB), G_OBJECT(draw_mesh_vws->real3_store));
    
    GtkWidget *show_each_button = gtk_button_new_with_label("Show selected");
    g_signal_connect(G_OBJECT(show_each_button), "clicked",
                     G_CALLBACK(show_each_sw3c_button_CB), G_OBJECT(draw_mesh_vws->real3_cview));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), draw_mesh_vws->real3_cview, 0, 0, 4, 1);
    
    gtk_grid_attach(GTK_GRID(grid), show_all_button,         0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), show_each_button,        2, 2, 2, 1);
    // scrolled window
    GtkWidget *scroll_win = gtk_scrolled_window_new(); 
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scroll_win),  480);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scroll_win), 320);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_win), grid);
    
    
    draw_mesh_vws->real3_cview_scroll = wrap_into_scrollbox_gtk4(480, 320, scroll_win);
//    update_draw_mesh_list(kemo_gl, draw_mesh_vws);
    return draw_mesh_vws;
};

void update_draw_mesh_list(struct kemoviewer_gl_type *kemo_gl,
                           struct kemo_sw3c_columnview *draw_mesh_vws){
    sync_list_from_kemoview_draw_mesh(kemo_gl->kemoview_data, DOMAIN_FLAG,
                                      draw_mesh_vws);
    return;
};

