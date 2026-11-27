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
static void sw3c_column_item_get_property(GObject *object,
                                          guint prop_id,
                                          GValue *value,
                                          GParamSpec *pspec){
    Sw3cColumnItem *self = (Sw3cColumnItem *)object;

    switch (prop_id) {
    case PROP_SW3C_SW1:
        g_value_set_int(value, self->int1);
        break;
    case PROP_SW3C_SW2:
        g_value_set_int(value, self->int2);
        break;
    case PROP_SW3C_SW3:
        g_value_set_int(value, self->int3);
        break;
    case PROP_SW3C_STR:
        g_value_set_string(value, self->str);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void sw3c_column_item_set_property(GObject *object,
                                          guint prop_id,
                                          const GValue *value,
                                          GParamSpec *pspec){
    Sw3cColumnItem *self = (Sw3cColumnItem *)object;
    
    switch (prop_id) {
    case PROP_SW3C_SW1:
        self->int1 = g_value_get_int(value);
        break;
    case PROP_SW3C_SW2:
        self->int2 = g_value_get_int(value);
        break;
    case PROP_SW3C_SW3:
        self->int3 = g_value_get_int(value);
        break;
    case PROP_SW3C_STR:
        g_free(self->str);
        self->str = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static GParamSpec *obj_properties[N_PROPS_SW3C] = { NULL, };

static void sw3c_column_item_class_init(Sw3cColumnItemClass *class){
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);

    gobject_class->get_property = sw3c_column_item_get_property;
    gobject_class->set_property = sw3c_column_item_set_property;

    obj_properties[PROP_SW3C_SW1] = g_param_spec_int("int1",
                                                     "Int1",
                                                     "first column",
                                                      0,
                                                      1,
                                                      0,
                                                      G_PARAM_READWRITE);

    obj_properties[PROP_SW3C_SW2] = g_param_spec_int("int2",
                                                     "Int2",
                                                     "second column",
                                                      0,
                                                      1,
                                                      0,
                                                      G_PARAM_READWRITE);

    obj_properties[PROP_SW3C_SW3] = g_param_spec_int("int3",
                                                     "Int3",
                                                     "third real",
                                                      0,
                                                      1,
                                                      0,
                                                      G_PARAM_READWRITE);
    
    obj_properties[PROP_SW3C_STR] = g_param_spec_string("string",
                                                       "String",
                                                       "forth string",
                                                       NULL,
                                                       G_PARAM_READWRITE);
    
    g_object_class_install_properties(gobject_class, N_PROPS_SW3C, obj_properties);
}

static void sw3c_column_item_init(Sw3cColumnItem *item){
    item->int1 = 0.;
    item->int2 = 0.;
    item->int3 = 0.;
    item->str = "";
}

Sw3cColumnItem * sw3c_column_item_new(const int int1,
                                      const int int2,
                                      const int int3,
                                      const char *str){
    Sw3cColumnItem  *item = g_object_new(SW3C_COLUMN_TYPE_ITEM, NULL);
    item->int1 = int1;
    item->int2 = int2;
    item->int3 = int3;
    item->str =   g_strdup(str);
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
 *   GtkColumnView Factory Callbacks for button 1
 * ========================================================================= */
/*  check button callbacks */
static void first_check_button_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *button = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int1 = (int) gtk_check_button_get_active(button);
        g_print("1st value updated! New integer value: %d\n", item->int1);
    }
}

/* Setup: Allocate and configure the check button */
static void setup_1st_check_button_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_list_item_set_child(listitem, button);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_1st_check_button_CB(GtkSignalListItemFactory *factory,
                                     GtkListItem *listitem,
                                     gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, first_check_button_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int1);
    g_signal_handlers_unblock_by_func(button, first_check_button_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(button, "value-changed", G_CALLBACK(first_check_button_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_1st_check_button_CB(GtkSignalListItemFactory *factory,
                                       GtkListItem *listitem,
                                       gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, first_check_button_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for button 2
 * ========================================================================= */
/*  check button callbacks */
static void second_check_button_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *button = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int2 = (int) gtk_check_button_get_active(button);
        g_print("2nd value updated! New integer value: %d\n", item->int2);
    }
}

/* Setup: Allocate and configure the check button */
static void setup_2nd_check_button_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_list_item_set_child(listitem, button);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_2nd_check_button_CB(GtkSignalListItemFactory *factory,
                                     GtkListItem *listitem,
                                     gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, second_check_button_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int2);
    g_signal_handlers_unblock_by_func(button, second_check_button_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(button, "value-changed", G_CALLBACK(second_check_button_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_2nd_check_button_CB(GtkSignalListItemFactory *factory,
                                       GtkListItem *listitem,
                                       gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, second_check_button_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for button 3
 * ========================================================================= */
/*  check button callbacks */
static void third_check_button_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *button = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int3 = (int) gtk_check_button_get_active(button);
        g_print("3rd value updated! New inteegr value: %d\n", item->int3);
    }
}

/* Setup: Allocate and configure the check button */
static void setup_3rd_check_button_CB(GtkSignalListItemFactory *factory,
                                      GtkListItem *listitem,
                                      gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_list_item_set_child(listitem, button);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_3rd_check_button_CB(GtkSignalListItemFactory *factory,
                                     GtkListItem *listitem,
                                     gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    Sw3cColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, third_check_button_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int3);
    g_signal_handlers_unblock_by_func(button, third_check_button_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(button, "value-changed", G_CALLBACK(third_check_button_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_3rd_check_button_CB(GtkSignalListItemFactory *factory,
                                       GtkListItem *listitem,
                                       gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, third_check_button_change_CB, listitem);
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

/* Unbind: Clean up connection when cell is ready for recycling

static void unbind_str_label_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
}
*/

/* =========================================================================
 *   Callbacks for label box to control list model data
 * ========================================================================= */
void show_all_sw3c_button_CB(GtkButton *button, gpointer user_data){
    Sw3cColumnItem *item;
    GListStore *store = G_LIST_STORE(user_data);
    guint num = g_list_model_get_n_items(G_LIST_MODEL(store));
    printf("Number of items: %d \n", num);
    int i;
    for(i=0;i<num;i++){
        item = SW3C_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
        printf("Data at %d: %s: %d %d %d\n", i, 
               item->str, item->int1, item->int2, item->int3);
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
        printf("Data at %d: %s: %d %d %d \n", original_index, 
               item->str, item->int1, item->int2, item->int3);
    }
    return;
};

static void add_sw3c_item_button_CB(GtkButton *button, gpointer user_data){
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
                            sw3c_column_item_new(item->int1, item->int2, item->int3, item->str));
    }
    return;
};


static void delete_sw3c_item_button_CB(GtkButton *button, gpointer user_data){
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
GListStore *init_sw3c_list_store(int num, int **iarray3, char **str){
    int i;
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    for(i=0;i<num;i++){
        g_list_store_append(store,sw3c_column_item_new(iarray3[i][0], iarray3[i][1], 
                                                       iarray3[i][2], str[i]));
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
    g_signal_connect(factory1, "setup",  G_CALLBACK(setup_1st_check_button_CB),  NULL);
    g_signal_connect(factory1, "bind",   G_CALLBACK(bind_1st_check_button_CB),   NULL);
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_1st_check_button_CB), NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Data1", factory1);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "int1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup",  G_CALLBACK(setup_2nd_check_button_CB),  NULL);
    g_signal_connect(factory2, "bind",   G_CALLBACK(bind_2nd_check_button_CB),   NULL);
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_2nd_check_button_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Data2", factory2);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "int2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_3rd_check_button_CB),  NULL);
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_3rd_check_button_CB),   NULL);
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_3rd_check_button_CB), NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Data3", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                            NULL, "int3")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER(sorter));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(cview), column3);

    // Factory setup and bind for column 4
    GtkListItemFactory *factory4 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory4, "setup",  G_CALLBACK(setup_str_label_CB),  NULL);
    g_signal_connect(factory4, "bind",   G_CALLBACK(bind_str_label_CB),   NULL);
//    g_signal_connect(factory4, "unbind", G_CALLBACK(unbind_str_label_CB), NULL);
    GtkColumnViewColumn *column4 = gtk_column_view_column_new("Label", factory4);
    sorter = GTK_SORTER (gtk_string_sorter_new(gtk_property_expression_new(SW3C_COLUMN_TYPE_ITEM,
                                                                           NULL, "string")));
    gtk_column_view_column_set_sorter (column4, GTK_SORTER(sorter));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(cview), column4);
    
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

struct kemo_sw3c_columnview * init_sw3c_columnview_scroll(int num, int **iarray3, char **str){
    struct kemo_sw3c_columnview *sw3c_vws = alloc_kemo_sw3c_columnview();
    
    // Add column view.  Put in scrolled window.  
    sw3c_vws->real3_store = init_sw3c_list_store(num, iarray3, str); 
    sw3c_vws->real3_cview = sw3c_columnview_init(sw3c_vws->real3_store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(sw3c_vws->real3_cview),TRUE);
    g_object_set_data(G_OBJECT(sw3c_vws->real3_cview), "list_store",
                      G_OBJECT(sw3c_vws->real3_store));
    
    
    GtkWidget *add_item_button = gtk_button_new_with_label("Add");
    g_signal_connect(G_OBJECT(add_item_button), "clicked",
                     G_CALLBACK(add_sw3c_item_button_CB), G_OBJECT(sw3c_vws->real3_cview));
    
    GtkWidget *del_item_button = gtk_button_new_with_label("Delete");
    g_signal_connect(G_OBJECT(del_item_button), "clicked",
                     G_CALLBACK(delete_sw3c_item_button_CB), G_OBJECT(sw3c_vws->real3_cview));
    
    
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

