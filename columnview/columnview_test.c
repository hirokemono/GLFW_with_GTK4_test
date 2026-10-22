/***********************************************************************
 *  columnview_test.c
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/

#include "columnview_test.h"

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
    case PROP_STR1:
        g_value_set_string(value, self->str1);
        break;
    case PROP_STR2:
        g_value_set_string(value, self->str2);
        break;
    case PROP_REAL3:
        g_value_set_float(value, (gfloat) self->real3);
        break;
    case PROP_INT4:
        g_value_set_int(value, (gint) self->int4);
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
    case PROP_STR1:
        g_free(self->str1);
        self->str1 = g_value_dup_string(value);
        break;
    case PROP_STR2:
        g_free(self->str2);
        self->str2 = g_value_dup_string(value);
        break;
    case PROP_REAL3:
        self->real3 = (float) g_value_get_float(value);
        break;
    case PROP_INT4:
        self->int4 = (int) g_value_get_int(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void testcol_item_init(TestColItem *item)
{
    item->str1 = NULL;
    item->str2 = NULL;
    item->real3 = 0.;
    item->int4 =  0;
}


static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void testcol_item_class_init(TestColItemClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);

    gobject_class->get_property = testcol_item_get_property;
    gobject_class->set_property = testcol_item_set_property;

    obj_properties[PROP_STR1] = g_param_spec_string("str1",
                                                    "Str1",
                                                    "first column",
                                                    NULL,
                                                    G_PARAM_READWRITE);

    obj_properties[PROP_STR2] = g_param_spec_string("str2",
                                                    "Str2",
                                                    "second column",
                                                    NULL,
                                                    G_PARAM_READWRITE);

    obj_properties[PROP_REAL3] = g_param_spec_float("real3",
                                                    "Real3",
                                                    "third real",
                                                    -1.0e+30,
                                                     1.0e+30,
                                                     0.0,
                                                    G_PARAM_READWRITE);

    obj_properties[PROP_INT4] = g_param_spec_int("int4",
                                                 "Int4",
                                                 "forth integer",
                                                 -32768,
                                                  32768,
                                                      0,
                                                 G_PARAM_READWRITE);
    
    g_object_class_install_properties(gobject_class, N_PROPERTIES, obj_properties);
}


// Create the custom object
static TestColItem * testcol_item_new(const char *str1,
                                      const char *str2,
                                      const float real3,
                                      const int int4){
    TestColItem  *item = g_object_new(TESTCOL_TYPE_ITEM, NULL);
    item->str1 =  g_strdup(str1);
    item->str2 =  g_strdup(str2);
    item->real3 = real3;
    item->int4 =  int4;
    return item;
}   

/*  Callbacks  */
/* =========================================================================
 *   Factory Callbacks for editable label
 * ========================================================================= */

/*  string editable button callbacks */
static void str1_editing_CB(GObject *object, GParamSpec *pspec, gpointer user_data) {
    GtkEditable *editable = GTK_EDITABLE(object);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("str1_editing_CB %p %p \n", listitem, item);
    
    gboolean is_editing;
    g_object_get(G_OBJECT(editable), "editing", &is_editing, NULL);
    if(!is_editing) {
        const char *new_text = gtk_editable_get_text(GTK_EDITABLE(editable));
        g_free(item->str1);
        item->str1 = g_strdup(new_text);
        g_print("str1 data updated to: %s\n", item->str1);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_str1_label_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *editable_label = gtk_editable_label_new("");
    
    gtk_list_item_set_child(listitem, editable_label);
}

// 4. Factory bind: Populate the widget with data from the row object
static void bind_str1_label_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *editable_label = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("bind_str1_label_CB %p %p \n", listitem, item);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(editable_label, str1_editing_CB, listitem);
    gtk_editable_set_text(GTK_EDITABLE(editable_label), item->str1);
    g_signal_handlers_unblock_by_func(editable_label, str1_editing_CB, listitem);
    
    /* Connect the notify::editing signal to catch when editing finishes */
    g_object_set_data(G_OBJECT(editable_label), "list_store", G_OBJECT(store));
    g_signal_connect(editable_label, "notify::editing", 
                     G_CALLBACK(str1_editing_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_str1_label_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *editable_label = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(editable_label, str1_editing_CB, listitem);
}

/* =========================================================================
 *   Factory Callbacks for GtkEntry
 * ========================================================================= */

/*  string editable button callbacks */
static void str2_activate_CB(GObject *object, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(object);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("str2_activate_CB %p %p \n", listitem, item);
    
    const char *new_text = gtk_editable_get_text(GTK_EDITABLE(entry));
    g_free(item->str2);
    item->str2 = g_strdup(new_text);
    g_print("str2 data updated to: %s\n", item->str2);
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_str2_label_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *entry = gtk_entry_new();
    
    gtk_list_item_set_child(listitem, entry);
}

// 4. Factory bind: Populate the widget with data from the row object
static void bind_str2_label_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *entry = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("bind_str2_label_CB %p %p \n", listitem, item);
    
    /* Block previous signal handler if recycled, or set text */
    gtk_editable_set_text(GTK_EDITABLE(entry), item->str2 ? item->str2 : "");
    
    /* Disconnect old handler if stored, or connect new change listener */
    g_signal_handlers_block_by_func(entry, str2_activate_CB, listitem);
    gtk_editable_set_text(GTK_EDITABLE(entry), item->str2);
    g_signal_handlers_unblock_by_func(entry, str2_activate_CB, listitem);
    
    /* Connect the activate signal to catch when editing finishes */
    g_object_set_data(G_OBJECT(entry), "list_store", G_OBJECT(store));
    g_signal_connect(entry, "activate", 
                     G_CALLBACK(str2_activate_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_str2_label_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *entry = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(entry, str2_activate_CB, listitem);
}


/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void real_spin_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("real_spin_value_change_CB %p %p \n", listitem, item);
    
    if(item){
        item->real3 = gtk_spin_button_get_value(spin);
        g_print("Real value updated! New float value: %.2f\n", item->real3);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real3_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, 0.0, 360.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 0.25, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_real3_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("bind_real3_spin_CB %p %p \n", listitem, item);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real3);
    g_signal_handlers_unblock_by_func(spin, real_spin_value_change_CB, listitem);

    /* Track user adjustments */
    g_object_set_data(G_OBJECT(spin), "list_store", G_OBJECT(store));
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_real3_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for integer spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void int_spin_value_change_CB(GObject *object, gpointer user_data) {
    GtkSpinButton *spin = GTK_SPIN_BUTTON(object);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(object), "list_store"));
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("int_spin_value_change_CB %p %p \n", listitem, item);
    
    if(item){
        item->int4 = gtk_spin_button_get_value_as_int(spin);
        g_print("Integer updated! New integer: %d\n", item->int4);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_int4_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0, -64, 64, 1, 1, 0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 1, 0);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_int4_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
//    printf("bind_int4_spin_CB %p %p \n", listitem, item);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, int_spin_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->int4);
    g_signal_handlers_unblock_by_func(spin, int_spin_value_change_CB, listitem);

    /* Track user adjustments */
    g_object_set_data(G_OBJECT(spin), "list_store", G_OBJECT(store));
    g_signal_connect(spin, "value-changed", 
                     G_CALLBACK(int_spin_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_int4_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, int_spin_value_change_CB, listitem);
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
        printf("Data at %d: %s %s %f %d\n", i, 
               item->str1, item->str2, item->real3, item->int4);
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
        printf("Data at %d: %s %s %f %d\n", original_index, 
               item->str1, item->str2, item->real3, item->int4);
    }
    return;
};

static void add_item_button_CB(GtkButton *button, gpointer user_data){
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_listmodel(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        g_list_store_insert(store, original_index, testcol_item_new("New","new2", 0.0, 0));
    }
    return;
};


static void delete_item_button_CB(GtkButton *button, gpointer user_data){
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_listmodel(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        g_list_store_remove(store, original_index);
    }
    return;
};


/* Construct columnview */
GListStore *init_test_list_store(){
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT); 
    g_list_store_append(store,testcol_item_new("abc1","ghi1",  8.60,  33));
    g_list_store_append(store,testcol_item_new("def2","aaa2", -10.0, -87));
    g_list_store_append(store,testcol_item_new("aaa3","jkl3", 240.0,  10));
    return store;
};

GtkWidget * columnview_init(GListStore *store){
    GtkWidget *cview = gtk_column_view_new(NULL);

    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW(cview)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(cview), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup", G_CALLBACK(setup_str1_label_CB),    G_OBJECT(store));
    g_signal_connect(factory1, "bind", G_CALLBACK(bind_str1_label_CB),      G_OBJECT(store));
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_str1_label_CB),  G_OBJECT(store));
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Col1", factory1);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM,NULL,"str1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup", G_CALLBACK(setup_str2_label_CB),   G_OBJECT(store));
    g_signal_connect(factory2, "bind", G_CALLBACK(bind_str2_label_CB),     G_OBJECT(store));
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_str2_label_CB), G_OBJECT(store));
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Col2", factory2);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM, NULL, "str2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_real3_spin_CB),  G_OBJECT(store));
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_real3_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_real3_spin_CB), G_OBJECT(store));
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Real3", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(TESTCOL_TYPE_ITEM, NULL, "real3")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column3);

    // Factory setup and bind for column 4
    GtkListItemFactory *factory4 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory4, "setup",  G_CALLBACK(setup_int4_spin_CB),  G_OBJECT(store));
    g_signal_connect(factory4, "bind",   G_CALLBACK(bind_int4_spin_CB),   G_OBJECT(store));
    g_signal_connect(factory4, "unbind", G_CALLBACK(unbind_int4_spin_CB), G_OBJECT(store));
    GtkColumnViewColumn *column4 = gtk_column_view_column_new("Int4", factory4);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(TESTCOL_TYPE_ITEM, NULL, "int4")));
    gtk_column_view_column_set_sorter (column4, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column4);
    
    return cview;
}

GtkWidget * init_list_store_test_grid(){
    GtkWidget *grid;
    
    // Add column view.  Put in scrolled window.  
    GListStore *store = init_test_list_store(); 
    GtkWidget *cview = columnview_init(store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW (cview),TRUE);
    g_object_set_data(G_OBJECT(cview), "list_store", G_OBJECT(store));
    
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
