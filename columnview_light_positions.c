/***********************************************************************
 *  columnview_light_positions.c
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/

#include "columnview_light_positions.h"

/* 
 * 1. Define private instance data here
 */
struct _TestColItemClass
{
    GObjectClass parent_class;
};

/* 
 * 2. Register the type with the GObject library.
 * This wires up my_viewer_get_type() and boilerplate wiring.
 */
G_DEFINE_TYPE(TestColItem, testcol_item, G_TYPE_OBJECT)


// Setup custom type - each item has two strings which will be displayed in two columns
static void
testcol_item_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    TestColItem *self = (TestColItem *)object;

    switch (prop_id) {
    case PROP_REAL1:
        g_value_set_float(value, (gfloat) self->real1);
        break;
    case PROP_REAL2:
        g_value_set_float(value, (gfloat) self->real2);
        break;
    case PROP_REAL3:
        g_value_set_float(value, (gfloat) self->real3);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void
testcol_item_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    TestColItem *self = (TestColItem *)object;

    switch (prop_id) {
    case PROP_REAL1:
        self->real1 = (float) g_value_get_float(value);
        break;
    case PROP_REAL2:
        self->real2 = (float) g_value_get_float(value);
        break;
    case PROP_REAL3:
        self->real3 = (float) g_value_get_float(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void testcol_item_init(TestColItem *item)
{
    item->real1 = 0.;
    item->real2 = 0.;
    item->real3 = 0.;
}


static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void testcol_item_class_init(TestColItemClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);

    gobject_class->get_property = testcol_item_get_property;
    gobject_class->set_property = testcol_item_set_property;

    obj_properties[PROP_REAL1] = g_param_spec_float("radius",
                                                    "Radius",
                                                    "first column",
                                                      0.0,
                                                    180.0,
                                                      0.0,
                                                    G_PARAM_READWRITE);

    obj_properties[PROP_REAL2] = g_param_spec_float("elevation",
                                                    "Elevation",
                                                    "second column",
                                                       0.0,
                                                     360.0,
                                                       0.0,
                                                    G_PARAM_READWRITE);

    obj_properties[PROP_REAL3] = g_param_spec_float("azimuth",
                                                    "Azimuth",
                                                    "third real",
                                                    -1.0e+30,
                                                     1.0e+30,
                                                     0.0,
                                                    G_PARAM_READWRITE);
    
    g_object_class_install_properties(gobject_class, N_PROPERTIES, obj_properties);
}


// Create the custom object
static TestColItem * testcol_item_new(const float real1,
                                      const float real2,
                                      const float real3){
    TestColItem  *item = g_object_new(TESTCOL_TYPE_ITEM, NULL);
    item->real1 = real1;
    item->real2 = real2;
    item->real3 = real3;
    return item;
}   

static void sync_kemoview_light_position_from_list(GListStore *store,
                                                   struct kemoviewer_type *kemo_sgl){
    TestColItem *item;
	int i;
	int num = (int) g_list_model_get_n_items(G_LIST_MODEL(store));
    kemoview_realloc_phong_light_list(num, kemo_sgl);
	for(i=0;i<num;i++){
        item = TESTCOL_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
		kemoview_set_each_light_position(i, item->real1, item->real2, item->real3,
                                         kemo_sgl);
	};
	return;
};

/*  Callbacks  */
/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void real_spin1_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(object), "kemoview");
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->real1 = gtk_spin_button_get_value(spin);
        g_print("Real value updated! New float value: %.2f\n", item->real1);
        sync_kemoview_light_position_from_list(store, kemo_sgl);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real1_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, 0.0, 1000.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 5.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_real1_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(user_data), "kemoview");
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin1_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real1);
    g_signal_handlers_unblock_by_func(spin, real_spin1_value_change_CB, listitem);

    /* Track user adjustments */
    g_object_set_data(G_OBJECT(spin), "kemoview",   G_OBJECT(kemo_sgl));
    g_object_set_data(G_OBJECT(spin), "list_store", G_OBJECT(store));
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin1_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_real1_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin1_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void real_spin2_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(object), "kemoview");
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->real2 = gtk_spin_button_get_value(spin);
        g_print("Real value updated! New float value: %.2f\n", item->real2);
        sync_kemoview_light_position_from_list(store, kemo_sgl);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real2_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, 0.0, 360.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 90.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_real2_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(user_data), "kemoview");
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin2_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real2);
    g_signal_handlers_unblock_by_func(spin, real_spin2_value_change_CB, listitem);

    /* Track user adjustments */
    g_object_set_data(G_OBJECT(spin), "kemoview",   G_OBJECT(kemo_sgl));
    g_object_set_data(G_OBJECT(spin), "list_store", G_OBJECT(store));
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin2_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_real2_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin2_value_change_CB, listitem);
}


/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void real_spin3_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(object), "kemoview");
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->real3 = gtk_spin_button_get_value(spin);
        g_print("Real value updated! New float value: %.2f\n", item->real3);
        sync_kemoview_light_position_from_list(store, kemo_sgl);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real3_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, 0.0, 360.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 180.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_real3_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    struct kemoviewer_type *kemo_sgl 
            = (struct kemoviewer_type *) g_object_get_data(user_data, "kemoview");
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin3_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real3);
    g_signal_handlers_unblock_by_func(spin, real_spin3_value_change_CB, listitem);

    /* Track user adjustments */
    g_object_set_data(G_OBJECT(spin), "kemoview",   G_OBJECT(kemo_sgl));
    g_object_set_data(G_OBJECT(spin), "list_store", G_OBJECT(store));
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin3_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_real3_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin3_value_change_CB, listitem);
}

/* =========================================================================
 *   Callbacks for buttuns to control list model data
 * ========================================================================= */
static int find_original_index_of_listmodel(GtkColumnView *column_view){
    guint original_index;
    GtkSelectionModel *selected = gtk_column_view_get_model(column_view);
//    guint selected_index = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION (selected));
    TestColItem *item = TESTCOL_ITEM(gtk_single_selection_get_selected_item(GTK_SINGLE_SELECTION(selected)));
    if(item == NULL) {return -1;}
    
    GtkSortListModel *sort_model
            = GTK_SORT_LIST_MODEL(gtk_single_selection_get_model(GTK_SINGLE_SELECTION(selected)));
    GListModel *child_model = gtk_sort_list_model_get_model(sort_model);
    GListStore *original_store = G_LIST_STORE(child_model);
    
    g_list_store_find(original_store, item, &original_index);
    return (int) original_index;
}

static void show_all_button_CB(GtkButton *button, gpointer user_data){
    TestColItem *item;
    GListStore *store = G_LIST_STORE(user_data);
    guint num = g_list_model_get_n_items(G_LIST_MODEL(store));
    printf("Number of items: %d \n", num);
    int i;
    for(i=0;i<num;i++){
        item = TESTCOL_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
        printf("Data at %d: %f %f %f\n", i, 
               item->real1, item->real2, item->real3);
    }
    return;
};

static void show_each_button_CB(GtkButton *button, gpointer user_data){
    TestColItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_listmodel(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        item = TESTCOL_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        printf("Data at %d: %f %f %f \n", original_index, 
               item->real1, item->real2, item->real3);
    }
    return;
};

static void add_item_button_CB(GtkButton *button, gpointer user_data){
    TestColItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(user_data), "kemoview");
    
    int original_index = find_original_index_of_listmodel(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        item = TESTCOL_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        g_list_store_insert(store, original_index, 
                            testcol_item_new(item->real1, item->real2, item->real3));
        kemoview_add_phong_light_list(original_index, item->real1, item->real2, item->real3,
                                      kemo_sgl);
    }
    return;
};


static void delete_item_button_CB(GtkButton *button, gpointer user_data){
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    struct kemoviewer_type *kemo_sgl
            = (struct kemoviewer_type *) g_object_get_data(G_OBJECT(user_data), "kemoview");
    
    int original_index = find_original_index_of_listmodel(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        g_list_store_remove(store, original_index);
        kemoview_delete_phong_light_list(original_index, kemo_sgl);
    }
    return;
};


/* Construct columnview */
GListStore *init_test_list_store(struct kemoviewer_type *kemo_sgl){
    float rtp[3];
    int i;
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    for(i=0;i<kemoview_get_num_light_position(kemo_sgl);i++){
        kemoview_get_each_light_rtp(kemo_sgl, i,
                                    &rtp[0], &rtp[1], &rtp[2]);
        g_list_store_append(store,testcol_item_new(rtp[0], rtp[1], rtp[2]));
    };
    return store;
};

GtkWidget * columnview_init(struct kemoviewer_type *kemo_sgl,
                            GListStore *store){
    GtkWidget *cview = gtk_column_view_new(NULL);
    g_object_set_data(G_OBJECT(store), "kemoview",   G_OBJECT(kemo_sgl));
    
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW(cview)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(cview), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup",  G_CALLBACK(setup_real1_spin_CB),  NULL);
    g_signal_connect(factory1, "bind",   G_CALLBACK(bind_real1_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_real1_spin_CB), NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Radius", factory1);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new (TESTCOL_TYPE_ITEM,NULL,"radius")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup",  G_CALLBACK(setup_real2_spin_CB),  NULL);
    g_signal_connect(factory2, "bind",   G_CALLBACK(bind_real2_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_real2_spin_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Elevation", factory2);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new (TESTCOL_TYPE_ITEM, NULL, "elevation")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_real3_spin_CB),  NULL);
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_real3_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_real3_spin_CB), NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Azimuth", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(TESTCOL_TYPE_ITEM, NULL, "azimuth")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER (sorter));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(cview), column3);
    
    return cview;
}

GtkWidget * init_light_position_list_box(struct kemoviewer_type *kemo_sgl){
    GtkWidget *grid;
    
    // Add column view.  Put in scrolled window.  
    GListStore *store = init_test_list_store(kemo_sgl); 
    GtkWidget *cview = columnview_init(kemo_sgl, store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW (cview),TRUE);
    g_object_set_data(G_OBJECT(cview), "list_store", G_OBJECT(store));
    g_object_set_data(G_OBJECT(cview), "kemoview",   G_OBJECT(kemo_sgl));
    
    // scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(); 
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window),  400);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window), 240);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), cview);
    
    GtkWidget *add_item_button = gtk_button_new_with_label("Add");
    g_signal_connect(G_OBJECT(add_item_button), "clicked",
                     G_CALLBACK(add_item_button_CB), G_OBJECT(cview));
    
    GtkWidget *del_item_button = gtk_button_new_with_label("Delete");
    g_signal_connect(G_OBJECT(del_item_button), "clicked",
                     G_CALLBACK(delete_item_button_CB), G_OBJECT(cview));
    
    
    GtkWidget *show_all_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_all_button), "clicked",
                     G_CALLBACK(show_all_button_CB), G_OBJECT(store));
    
    GtkWidget *show_each_button = gtk_button_new_with_label("Show selected");
    g_signal_connect(G_OBJECT(show_each_button), "clicked",
                     G_CALLBACK(show_each_button_CB), G_OBJECT(cview));
    
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), scrolled_window,  0, 0, 4, 1);
    
    gtk_grid_attach(GTK_GRID(grid), add_item_button,  0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), del_item_button,  2, 1, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), show_all_button,  0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), show_each_button, 2, 2, 2, 1);
    return grid;
};
