/***********************************************************************
 *  columnview_sw3_chara_gtk4.c
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/

#include "columnview_sw3_chara_gtk4.h"

/* 
 * 1. Define private instance data here
 */
struct _Sw3cColumnItemClass
{
    GObjectClass parent_class;
};
/* 
 * 2. Register the type with the GObject library.
 * This wires up my_viewer_get_type() and boilerplate wiring.
 */
G_DEFINE_TYPE(Sw3cColumnItem, sw3c_column_item, G_TYPE_OBJECT)

// Setup custom type - each item has two strings which will be displayed in two columns
static void real3_column_item_get_property(GObject *object,
                                           guint prop_id,
                                           GValue *value,
                                           GParamSpec *pspec){
    Sw3cColumnItem *self = (Sw3cColumnItem *)object;

    switch (prop_id) {
    case PROP_SW3C_SW1:
        g_value_set_float(value, (gfloat) self->real1);
        break;
    case PROP_SW3C_SW2:
        g_value_set_float(value, (gfloat) self->real2);
        break;
    case PROP_SW3C_SW3:
        g_value_set_float(value, (gfloat) self->real3);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void real3_column_item_set_property(GObject *object,
                                           guint prop_id,
                                           const GValue *value,
                                           GParamSpec *pspec){
    Sw3cColumnItem *self = (Sw3cColumnItem *)object;

    switch (prop_id) {
    case PROP_SW3C_SW1:
        self->real1 = (float) g_value_get_float(value);
        break;
    case PROP_SW3C_SW2:
        self->real2 = (float) g_value_get_float(value);
        break;
    case PROP_SW3C_SW3:
        self->real3 = (float) g_value_get_float(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static GParamSpec *obj_properties[N_PROPS_SW3C] = { NULL, };

static void sw3c_column_item_class_init(Sw3cColumnItemClass *class){
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);

    gobject_class->get_property = real3_column_item_get_property;
    gobject_class->set_property = real3_column_item_set_property;

    obj_properties[PROP_SW3C_SW1] = g_param_spec_float("real1",
                                                       "Real1",
                                                       "first column",
                                                       -1.0e+30,
                                                       1.0e+30,
                                                       0.0,
                                                       G_PARAM_READWRITE);

    obj_properties[PROP_SW3C_SW2] = g_param_spec_float("real2",
                                                       "Real2",
                                                       "second column",
                                                       -1.0e+30,
                                                       1.0e+30,
                                                       0.0,
                                                       G_PARAM_READWRITE);

    obj_properties[PROP_SW3C_SW3] = g_param_spec_float("real3",
                                                       "Real3",
                                                       "third real",
                                                       -1.0e+30,
                                                       1.0e+30,
                                                       0.0,
                                                       G_PARAM_READWRITE);
    
    g_object_class_install_properties(gobject_class, N_PROPS_SW3C, obj_properties);
}

static void sw3c_column_item_init(Sw3cColumnItem *item){
    item->real1 = 0.;
    item->real2 = 0.;
    item->real3 = 0.;
}

Sw3cColumnItem * sw3c_column_item_new(const float real1,
                                              const float real2,
                                              const float real3){
    Sw3cColumnItem  *item = g_object_new(SW3C_COLUMN_TYPE_ITEM, NULL);
    item->real1 = real1;
    item->real2 = real2;
    item->real3 = real3;
    return item;
}

int find_original_index_of_sw3c_column(GtkColumnView *column_view){
    guint original_index;
    GtkSelectionModel *selected = gtk_column_view_get_model(column_view);
//    guint selected_index = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION (selected));
    Sw3cColumnItem *item = SW3C_COLUMN_ITEM(gtk_single_selection_get_selected_item(GTK_SINGLE_SELECTION(selected)));
    if(item == NULL) {return -1;}
    
    GtkSortListModel *sort_model
            = GTK_SORT_LIST_MODEL(gtk_single_selection_get_model(GTK_SINGLE_SELECTION(selected)));
    GListModel *child_model = gtk_sort_list_model_get_model(sort_model);
    GListStore *original_store = G_LIST_STORE(child_model);
    
    g_list_store_find(original_store, item, &original_index);
    return (int) original_index;
}

/*  Callbacks  */
/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button 1
 * ========================================================================= */
/*  Float spin button callbacks */
static void real_spin1_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin1 = GTK_SPIN_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->real1 = gtk_spin_button_get_value(spin1);
        g_print("1st Real value updated! New float value: %.2f\n", item->real1);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real1_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, -1000.0, 1000.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 5.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_real1_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin1_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real1);
    g_signal_handlers_unblock_by_func(spin, real_spin1_value_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin1_value_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_real1_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin1_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button 2
 * ========================================================================= */
/*  Float spin button callbacks */
static void real_spin2_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->real2 = gtk_spin_button_get_value(spin);
        g_print("2nd Real value updated! New float value: %.2f\n", item->real2);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real2_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, -1000.0, 1000.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 5.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_real2_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin2_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real2);
    g_signal_handlers_unblock_by_func(spin, real_spin2_value_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin2_value_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_real2_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin2_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button 3
 * ========================================================================= */
/*  Float spin button callbacks */
static void real_spin3_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->real3 = gtk_spin_button_get_value(spin);
        g_print("3rd Real value updated! New float value: %.2f\n", item->real3);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real3_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, -1000.0, 1000.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 5.0, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_real3_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin3_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real3);
    g_signal_handlers_unblock_by_func(spin, real_spin3_value_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin3_value_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_real3_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin3_value_change_CB, listitem);
}

/* =========================================================================
 *   Callbacks for buttuns to control list model data
 * ========================================================================= */
void show_all_sw3c_button_CB(GtkButton *button, gpointer user_data){
    Sw3cColumnItem *item;
    GListStore *store = G_LIST_STORE(user_data);
    guint num = g_list_model_get_n_items(G_LIST_MODEL(store));
    printf("Number of items: %d \n", num);
    int i;
    for(i=0;i<num;i++){
        item = SW3C_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
        printf("Data at %d: %f %f %f\n", i, 
               item->real1, item->real2, item->real3);
    }
    return;
};

void show_each_sw3c_button_CB(GtkButton *button, gpointer user_data){
    Sw3cColumnItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_sw3c_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        item = SW3C_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        printf("Data at %d: %f %f %f \n", original_index, 
               item->real1, item->real2, item->real3);
    }
    return;
};

static void add_item_button_CB(GtkButton *button, gpointer user_data){
    Sw3cColumnItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_sw3c_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        item = SW3C_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        g_list_store_insert(store, original_index, 
                            sw3c_column_item_new(item->real1, item->real2, item->real3));
    }
    return;
};


static void delete_item_button_CB(GtkButton *button, gpointer user_data){
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_sw3c_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        g_list_store_remove(store, original_index);
    }
    return;
};

/* =========================================================================
 *   Construct columnview
 * ========================================================================= */
GListStore *init_sw3c_list_store(int num, float **array3){
    int i;
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    for(i=0;i<num;i++){
        g_list_store_append(store,sw3c_column_item_new(array3[i][0], array3[i][1], array3[i][2]));
    };
    return store;
};

GtkWidget * sw3c_columnview_init(GListStore *store){
    GtkWidget *cview = gtk_column_view_new(NULL);
    
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW(cview)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(cview), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup",  G_CALLBACK(setup_real1_spin_CB),  NULL);
    g_signal_connect(factory1, "bind",   G_CALLBACK(bind_real1_spin_CB),   NULL);
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_real1_spin_CB), NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Data1", factory1);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "real1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup",  G_CALLBACK(setup_real2_spin_CB),  NULL);
    g_signal_connect(factory2, "bind",   G_CALLBACK(bind_real2_spin_CB),   NULL);
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_real2_spin_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Data2", factory2);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "real2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_real3_spin_CB),  NULL);
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_real3_spin_CB),   NULL);
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_real3_spin_CB), NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Data3", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "real3")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER(sorter));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(cview), column3);
    
    return cview;
}

struct kemo_sw3c_columnview * alloc_kemo_sw3c_columnview(){
    struct kemo_sw3c_columnview *sw3c_vws;
    sw3c_vws = (struct kemo_sw3c_columnview *) malloc(sizeof(struct kemo_sw3c_columnview *));
    if (sw3c_vws == NULL){
        printf("malloc error for kemo_sw3c_columnview \n");
        exit(1);
    };
    return sw3c_vws;
};

void dealloc_kemo_sw3c_columnview(struct kemo_sw3c_columnview *sw3c_vws){
    g_object_unref(G_OBJECT(sw3c_vws->real3_cview));
    g_object_unref(G_OBJECT(sw3c_vws->real3_store));
    free(sw3c_vws);
    return;
}

struct kemo_sw3c_columnview * init_sw3c_columnview_scroll(int num, float **array3){
    struct kemo_sw3c_columnview *sw3c_vws = alloc_kemo_sw3c_columnview();
    
    // Add column view.  Put in scrolled window.  
    sw3c_vws->real3_store = init_sw3c_list_store(num, array3); 
    sw3c_vws->real3_cview = sw3c_columnview_init(sw3c_vws->real3_store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(sw3c_vws->real3_cview),TRUE);
    g_object_set_data(G_OBJECT(sw3c_vws->real3_cview), "list_store",
                      G_OBJECT(sw3c_vws->real3_store));
    
    
    GtkWidget *add_item_button = gtk_button_new_with_label("Add");
    g_signal_connect(G_OBJECT(add_item_button), "clicked",
                     G_CALLBACK(add_item_button_CB), G_OBJECT(sw3c_vws->real3_cview));
    
    GtkWidget *del_item_button = gtk_button_new_with_label("Delete");
    g_signal_connect(G_OBJECT(del_item_button), "clicked",
                     G_CALLBACK(delete_item_button_CB), G_OBJECT(sw3c_vws->real3_cview));
    
    
    GtkWidget *show_all_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_all_button), "clicked",
                     G_CALLBACK(show_all_sw3c_button_CB), G_OBJECT(sw3c_vws->real3_store));
    
    GtkWidget *show_each_button = gtk_button_new_with_label("Show selected");
    g_signal_connect(G_OBJECT(show_each_button), "clicked",
                     G_CALLBACK(show_each_sw3c_button_CB), G_OBJECT(sw3c_vws->real3_cview));
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), sw3c_vws->real3_cview, 0, 0, 4, 1);
    
    gtk_grid_attach(GTK_GRID(grid), add_item_button,  0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), del_item_button,  2, 1, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), show_all_button,  0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), show_each_button, 2, 2, 2, 1);
    
    // scrolled window
    sw3c_vws->real3_cview_scroll = gtk_scrolled_window_new(); 
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(sw3c_vws->real3_cview_scroll),  480);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(sw3c_vws->real3_cview_scroll), 240);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw3c_vws->real3_cview_scroll), grid);
    
    return sw3c_vws;
};

