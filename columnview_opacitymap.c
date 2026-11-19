/***********************************************************************
 *  columnview_opacitymap.c
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/

#include "columnview_opacitymap.h"

static void sync_viewer_to_opacitymap_list(struct kemoviewer_gl_type *kemo_gl,
                                           int iflag_current_model,
                                           GtkDrawingArea *colormap_area,
                                           GListStore *store){
    Real2ColumnItem *item;
	int i;
	int num = (int) g_list_model_get_n_items(G_LIST_MODEL(store));
	for(i=0;i<num;i++){
        item = REAL2_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
		kemoview_set_VIZ_opacity_data(i, item->real1, item->real2, 
                                      iflag_current_model,
                                      kemo_gl->kemoview_data);
	};
    gtk_widget_queue_draw(colormap_area);
    draw_full_gl(kemo_gl);
	return;
};

static void sync_opacitymap_list_to_viewer(struct kemoviewer_gl_type *kemo_gl,
                                           int iflag_current_model,
                                           GtkDrawingArea *colormap_area,
                                           GListStore *store){
    double data, opacity;
	int i;
	int num_list = (int) g_list_model_get_n_items(G_LIST_MODEL(store));
    int num_data = kemoview_get_viz_colormap_param(kemo_gl->kemoview_data, 
                                                   iflag_current_model,
                                                   ISET_NUM_OPACITY);
    
    for(i=0;i<(num_list-num_data);i++){g_list_store_remove(store, IZERO);};
    for(i=0;i<(num_data-num_list);i++){
        g_list_store_append(store, real2_column_item_new(ZERO, ZERO));
    };
    
	for(i=0;i<num_data;i++){
        kemoview_get_PSF_opacity_items(kemo_gl->kemoview_data,
                                       iflag_current_model,
                                       i, &data, &opacity);
        g_list_store_splice(store, i, IONE, 
                            real2_column_item_new((float) data, (float) opacity),
                            IONE);
    };
    
    gtk_widget_queue_draw(colormap_area);
    draw_full_gl(kemo_gl);
	return;
};


/*  Callbacks  */
/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void opacity_data_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Real2ColumnItem *item = gtk_list_item_get_item(listitem);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(object), "kemoview_gl");
    int *iflag_current_model = (int *)      g_object_get_data(G_OBJECT(object), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(object), 
                                                                       "cbar_area"));
    
    if(item){
        item->real1 = gtk_spin_button_get_value(spin);
        g_print("data value updated! New data value: %.2f\n", item->real1);
        sync_viewer_to_opacitymap_list(kemo_gl, *iflag_current_model, 
                                       colormap_area, store);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_opacity_data_spin_CB(GtkSignalListItemFactory *factory,
                                       GtkListItem *listitem,
                                       gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, -1000.0, 1000.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 5.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_opacity_data_spin_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(user_data), 
                                                                       "cbar_area"));
    
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_object_set_data(G_OBJECT(spin), "kemoview_gl",   G_OBJECT(kemo_gl));
    g_object_set_data(G_OBJECT(spin), "current_model", G_OBJECT(iflag_current_model));
    g_object_set_data(G_OBJECT(spin), "cbar_area",     (gpointer) colormap_area);
    g_object_set_data(G_OBJECT(spin), "list_store",    G_OBJECT(store));
    Real2ColumnItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, opacity_data_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real1);
    g_signal_handlers_unblock_by_func(spin, opacity_data_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", G_CALLBACK(opacity_data_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_opacity_data_spin_CB(GtkSignalListItemFactory *factory,
                                        GtkListItem *listitem,
                                        gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, opacity_data_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */
static void contour_opacity_change_CB(GObject *object, gpointer user_data){
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Real2ColumnItem *item = gtk_list_item_get_item(listitem);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(object), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(object), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(object), 
                                                                       "cbar_area"));
    
    if(item){
        item->real2 = gtk_spin_button_get_value(spin);
        g_print("Real value updated! New float value: %.2f\n", item->real2);
        sync_viewer_to_opacitymap_list(kemo_gl, *iflag_current_model, 
                                       colormap_area, store);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_contour_opacity_spin_CB(GtkSignalListItemFactory *factory,
                                          GtkListItem *listitem,
                                          gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, -90.0, 90.0, 0.25, 10.0, 0.);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 90.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_contour_opacity_spin_CB(GtkSignalListItemFactory *factory,
                                         GtkListItem *listitem,
                                         gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(user_data), 
                                                                       "cbar_area"));
    
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_object_set_data(G_OBJECT(spin), "kemoview_gl",   (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(spin), "current_model", (gpointer) iflag_current_model);
    g_object_set_data(G_OBJECT(spin), "cbar_area",     (gpointer) colormap_area);
    g_object_set_data(G_OBJECT(spin), "list_store", G_OBJECT(store));
    Real2ColumnItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, contour_opacity_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real2);
    g_signal_handlers_unblock_by_func(spin, contour_opacity_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", G_CALLBACK(contour_opacity_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_contour_opacity_spin_CB(GtkSignalListItemFactory *factory,
                                           GtkListItem *listitem,
                                           gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, contour_opacity_change_CB, listitem);
}

/* =========================================================================
 *   Callbacks for buttuns to control list model data
 * ========================================================================= */
static void add_opacitymap_button_CB(GtkButton *button, gpointer user_data){
    Real2ColumnItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(user_data), 
                                                                       "cbar_area"));
    
    int original_index = find_original_index_of_real2_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        int n_color = kemoview_get_viz_colormap_param(kemo_gl->kemoview_data,
                                                      *iflag_current_model,
                                                      ISET_NUM_OPACITY);
        if(n_color > 16) return;
        kemoview_add_VIZ_opacity_list(ZERO, ZERO,
                                      *iflag_current_model,
                                      kemo_gl->kemoview_data);
        
        item = REAL2_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        g_list_store_insert(store, original_index, 
                            real2_column_item_new(item->real1, item->real2));
        sync_viewer_to_opacitymap_list(kemo_gl, *iflag_current_model, 
                                       colormap_area, store);
    }
    return;
};


static void delete_opacitymap_button_CB(GtkButton *button, gpointer user_data){
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(user_data), 
                                                                       "cbar_area"));
    
    int num_original =   g_list_model_get_n_items(G_LIST_MODEL(store));
    int original_index = find_original_index_of_real2_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(num_original < 2) {
        printf("Data point cannot be removed. \n");
        return;
    }else if(original_index < num_original){
        g_list_store_remove(store, original_index);
        kemoview_delete_VIZ_opacity_list(original_index, 
                                         *iflag_current_model,
                                         kemo_gl->kemoview_data);
        sync_viewer_to_opacitymap_list(kemo_gl, *iflag_current_model, 
                                       colormap_area, store);
    }
    return;
};


/* Construct columnview */
static void set_opacitymap_list_store(struct kemoviewer_gl_type *kemo_gl,
                                      int *iflag_current_model,
                                      struct kemo_real2_columnview *opacitymap_vws){
    double data, opacity;
    int i;
    // Create and populate custom list model
    int num = kemoview_get_viz_colormap_param(kemo_gl->kemoview_data, 
                                              *iflag_current_model,
                                              ISET_NUM_OPACITY);
    for(i=0;i<num;i++){
        kemoview_get_PSF_opacity_items(kemo_gl->kemoview_data,
                                       *iflag_current_model,
                                       i, &data, &opacity);
        g_list_store_append(opacitymap_vws->real2_store,
                            real2_column_item_new((float) data, (float) opacity));
    };
    return;
};

void reset_opacitymap_list_store(struct kemoviewer_gl_type *kemo_gl,
                                 int iflag_current_model,
                                 GtkDrawingArea *colormap_area,
                                 struct kemo_real2_columnview *opacitymap_vws){
    sync_opacitymap_list_to_viewer(kemo_gl, iflag_current_model,
                                   colormap_area, opacitymap_vws->real2_store);
    return;
};

static GtkWidget * opacitymap_columnview_init(struct kemoviewer_gl_type *kemo_gl,
                                              int *iflag_current_model, 
                                              GtkDrawingArea *colormap_area,
                                              GListStore *store){
    GtkWidget *cview = gtk_column_view_new(NULL);
    g_object_set_data(G_OBJECT(store), "kemoview_gl",   G_OBJECT(kemo_gl));
    g_object_set_data(G_OBJECT(store), "current_model", G_OBJECT(iflag_current_model));
    g_object_set_data(G_OBJECT(store), "cbar_area",     (gpointer) colormap_area);
    
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW(cview)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(cview), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup",  G_CALLBACK(setup_opacity_data_spin_CB),  NULL);
    g_signal_connect(factory1, "bind",   G_CALLBACK(bind_opacity_data_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_opacity_data_spin_CB), NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Data", factory1);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new (REAL2_COLUMN_TYPE_ITEM,
                                                                             NULL, "real1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup",  G_CALLBACK(setup_contour_opacity_spin_CB),  NULL);
    g_signal_connect(factory2, "bind",   G_CALLBACK(bind_contour_opacity_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_contour_opacity_spin_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Opacity", factory2);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new (REAL2_COLUMN_TYPE_ITEM,
                                                                             NULL, "real2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);
    
    return cview;
}


struct kemo_real2_columnview * init_opacitymap_list_scroll(struct kemoviewer_gl_type *kemo_gl,
                                                           int *iflag_current_model,
                                                           GtkDrawingArea *colormap_area,
                                                           GtkWidget *psf_window){
    struct kemo_real2_columnview *opacitymap_vws = alloc_kemo_real2_columnview();
    
    // Add column view.  Put in scrolled window.  
    opacitymap_vws->real2_store = g_list_store_new(G_TYPE_OBJECT);
    set_opacitymap_list_store(kemo_gl, iflag_current_model, opacitymap_vws);
    opacitymap_vws->real2_cview = opacitymap_columnview_init(kemo_gl, 
                                                             iflag_current_model, 
                                                             colormap_area,
                                                             opacitymap_vws->real2_store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW (opacitymap_vws->real2_cview),TRUE);
    g_object_set_data(G_OBJECT(opacitymap_vws->real2_cview), "list_store",
                      G_OBJECT(opacitymap_vws->real2_store));
    g_object_set_data(G_OBJECT(opacitymap_vws->real2_cview), "kemoview_gl",
                      G_OBJECT(kemo_gl));
    g_object_set_data(G_OBJECT(opacitymap_vws->real2_cview), "current_model",
                      G_OBJECT(iflag_current_model));
    g_object_set_data(G_OBJECT(opacitymap_vws->real2_cview), "cbar_area",
                      (gpointer) colormap_area);
    
    GtkWidget *add_item_button = gtk_button_new_with_label("Add");
    g_signal_connect(G_OBJECT(add_item_button), "clicked",
                     G_CALLBACK(add_opacitymap_button_CB), G_OBJECT(opacitymap_vws->real2_cview));
    
    GtkWidget *del_item_button = gtk_button_new_with_label("Delete");
    g_signal_connect(G_OBJECT(del_item_button), "clicked",
                     G_CALLBACK(delete_opacitymap_button_CB), G_OBJECT(opacitymap_vws->real2_cview));
    
    /*
    GtkWidget *show_all_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_all_button), "clicked",
                     G_CALLBACK(show_all_real2_button_CB), G_OBJECT(opacitymap_vws->real2_store));
    
    GtkWidget *show_each_button = gtk_button_new_with_label("Show selected");
    g_signal_connect(G_OBJECT(show_each_button), "clicked",
                     G_CALLBACK(show_each_real2_button_CB), G_OBJECT(opacitymap_vws->real2_cview));
    */
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), opacitymap_vws->real2_cview, 0, 0, 4, 1);
    
    gtk_grid_attach(GTK_GRID(grid), add_item_button,         0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), del_item_button,         2, 1, 2, 1);
    /*
    gtk_grid_attach(GTK_GRID(grid), show_all_button,         0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), show_each_button,        2, 2, 2, 1);
    */
    // scrolled window
    GtkWidget *scroll_win = gtk_scrolled_window_new(); 
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scroll_win),  360);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scroll_win), 320);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_win), grid);
    
    
    opacitymap_vws->real2_cview_scroll = wrap_into_scroll_expansion_gtk4("Opacity", 360, 320,
                                                                       psf_window, scroll_win);
    return opacitymap_vws;
};

