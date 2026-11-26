/***********************************************************************
 *  columnview_chara_sw3_gtk4.c
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/

#include "columnview_chara_sw3_gtk4.h"

/* 
 * 1. Define private instance data here
 */
struct _CharaSwitch3ColumnItemClass
{
    GObjectClass parent_class;
};
/* 
 * 2. Register the type with the GObject library.
 * This wires up my_viewer_get_type() and boilerplate wiring.
 */
G_DEFINE_TYPE(CharaSwitch3ColumnItem, chara_sw3_column_item, G_TYPE_OBJECT)

// Setup custom type - each item has two strings which will be displayed in two columns
static void chara_sw3_column_item_get_property(GObject *object,
                                               guint prop_id,
                                               GValue *value,
                                               GParamSpec *pspec){
    CharaSwitch3ColumnItem *self = (CharaSwitch3ColumnItem *)object;

    switch (prop_id) {
    case PROP_CHARA_SW3_C0:
        g_value_set_string(value, self->string);
        break;
    case PROP_CHARA_SW3_I1:
        g_value_set_int(value, (gint) self->int1);
        break;
    case PROP_CHARA_SW3_I2:
        g_value_set_int(value, (gint) self->int2);
        break;
    case PROP_CHARA_SW3_I3:
        g_value_set_int(value, (gint) self->int3);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void chara_sw3_column_item_set_property(GObject *object,
                                               guint prop_id,
                                               const GValue *value,
                                               GParamSpec *pspec){
    CharaSwitch3ColumnItem *self = (CharaSwitch3ColumnItem *)object;

    switch (prop_id) {
    case PROP_CHARA_SW3_C0:
        g_free(self->string);
        self->string = g_value_dup_string(value);
        break;
    case PROP_CHARA_SW3_I1:
        self->int1 = g_value_get_int(value);
        break;
    case PROP_CHARA_SW3_I2:
        self->int2 = g_value_get_int(value);
        break;
    case PROP_CHARA_SW3_I3:
        self->int3 = g_value_get_int(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static GParamSpec *obj_properties[N_PROPS_CHARA_SW3] = { NULL, };

static void chara_sw3_column_item_class_init(CharaSwitch3ColumnItemClass *class){
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);

    gobject_class->get_property = chara_sw3_column_item_get_property;
    gobject_class->set_property = chara_sw3_column_item_set_property;

    obj_properties[PROP_CHARA_SW3_C0] = g_param_spec_string("str1",
                                                            "Str1",
                                                            "first column",
                                                            NULL,
                                                            G_PARAM_READWRITE);
    
    obj_properties[PROP_CHARA_SW3_I1] = g_param_spec_float("flag1",
                                                           "Flag1",
                                                           "second column",
                                                           0,
                                                           1,
                                                           0,
                                                           G_PARAM_READWRITE);

    obj_properties[PROP_CHARA_SW3_I2] = g_param_spec_float("flag2",
                                                           "Flag2",
                                                           "third column",
                                                           0,
                                                           1,
                                                           0,
                                                           G_PARAM_READWRITE);
    obj_properties[PROP_CHARA_SW3_I2] = g_param_spec_float("flag3",
                                                           "Flag3",
                                                           "forth column",
                                                           0,
                                                           1,
                                                           0,
                                                           G_PARAM_READWRITE);
    g_object_class_install_properties(gobject_class, N_PROPS_CHARA_SW3, obj_properties);
}

static void chara_sw3_column_item_init(CharaSwitch3ColumnItem *item){
    item->string = NULL;
    item->int1 = 0;
    item->int2 = 0;
    item->int3 = 0;
}

CharaSwitch3ColumnItem * chara_sw3_column_item_new(const char *str1, 
                                                   const int int1,
                                                   const int int2,
                                                   const int int3){
    CharaSwitch3ColumnItem  *item = g_object_new(CHARA_SW3_COLUMN_TYPE_ITEM, NULL);
    item->int1 = int1;
    item->int2 = int2;
    item->int3 = int3;
    return item;
}

int find_original_index_of_csw3_column(GtkColumnView *column_view){
    guint original_index;
    GtkSelectionModel *selected = gtk_column_view_get_model(column_view);
//    guint selected_index = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION (selected));
    CharaSwitch3ColumnItem *item = CHARA_SW3_COLUMN_ITEM(gtk_single_selection_get_selected_item(GTK_SINGLE_SELECTION(selected)));
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
 *   GtkColumnView Factory Callbacks for string box
 * ========================================================================= */
/*  Text box callbacks */
static void str1_activate_CB(GObject *object, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    printf("str1_activate_CB %p %p \n", listitem, item);
    
    const char *new_text = gtk_editable_get_text(GTK_EDITABLE(entry));
    /*
    g_free(item->str2);
    item->str2 = g_strdup(new_text);
    g_print("str1 data updated to: %s\n", item->str2);
    */
}

/* Setup: Allocate and configure the float-capable GtkCheckButton */
static void setup_str1_label_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GtkWidget *editable_label = gtk_editable_label_new("");
    gtk_list_item_set_child(listitem, editable_label);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_str1_label_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *editable_label = gtk_list_item_get_child(listitem);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(editable_label, str1_activate_CB, listitem);
    gtk_editable_set_text(GTK_EDITABLE(editable_label), item->string);
    g_signal_handlers_unblock_by_func(editable_label, str1_activate_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(editable_label, "notify::editing", 
                     G_CALLBACK(str1_activate_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_str1_label_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *editable_label = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(editable_label, str1_activate_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for check button 1
 * ========================================================================= */
/*  Float check button callbacks */
static void button1_value_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *check = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int1 = (int) gtk_check_button_get_active(check);
        g_print("1st switch value updated! New value: %d \n", item->int1);
    }
}

/* Setup: Allocate and configure the float-capable GtkCheckButton */
static void setup_int1_button_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), FALSE);
    gtk_list_item_set_child(listitem, button);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_int1_button_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, button1_value_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int1);
    g_signal_handlers_unblock_by_func(button, button1_value_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(button, "value-changed", G_CALLBACK(button1_value_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_int1_button_CB(GtkSignalListItemFactory *factory,
                                  GtkListItem *listitem,
                                  gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, button1_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for check button 2
 * ========================================================================= */
/*  Float check button callbacks */
static void button2_value_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *check = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int2 = (int) gtk_check_button_get_active(check);
        g_print("2nd switch value updated! New value: %d \n", item->int2);
    }
}

/* Setup: Allocate and configure the float-capable GtkCheckButton */
static void setup_int2_button_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), FALSE);
    gtk_list_item_set_child(listitem, button);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_int2_button_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, button2_value_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int2);
    g_signal_handlers_unblock_by_func(button, button2_value_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(button, "value-changed", G_CALLBACK(button2_value_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_int2_button_CB(GtkSignalListItemFactory *factory,
                                  GtkListItem *listitem,
                                  gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, button2_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for check button 3
 * ========================================================================= */
/*  Float check button callbacks */
static void button3_value_change_CB(GObject *object, gpointer user_data) {
    GtkCheckButton *check = GTK_CHECK_BUTTON(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    if(item){
        item->int3 = (int) gtk_check_button_get_active(check);
        g_print("3rd switch value updated! New value: %d \n", item->int3);
    }
}

/* Setup: Allocate and configure the float-capable GtkCheckButton */
static void setup_int3_button_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *button = gtk_check_button_new();
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), FALSE);
    gtk_list_item_set_child(listitem, button);
}

/* Bind: Populate widget data from model and listen for changes */
static void bind_int3_button_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    CharaSwitch3ColumnItem *item = gtk_list_item_get_item(listitem);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(button, button3_value_change_CB, listitem);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(button), (gboolean) item->int1);
    g_signal_handlers_unblock_by_func(button, button3_value_change_CB, listitem);
    
    /* Track user adjustments */
    g_signal_connect(button, "value-changed", G_CALLBACK(button3_value_change_CB), listitem);
}

/* Unbind: Clean up connection when cell is ready for recycling */
static void unbind_int3_button_CB(GtkSignalListItemFactory *factory,
                                  GtkListItem *listitem,
                                  gpointer user_data){
    GtkWidget *button = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(button, button3_value_change_CB, listitem);
}

/* =========================================================================
 *   Callbacks for buttuns to control list model data
 * ========================================================================= */
void show_all_real2_button_CB(GtkButton *button, gpointer user_data){
    CharaSwitch3ColumnItem *item;
    GListStore *store = G_LIST_STORE(user_data);
    guint num = g_list_model_get_n_items(G_LIST_MODEL(store));
    printf("Number of items: %d \n", num);
    int i;
    for(i=0;i<num;i++){
        item = CHARA_SW3_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
        printf("Data at %d: %s: %d %d %d\n", i, item->string,
               item->int1, item->int2, item->int3);
    }
    return;
};

void show_each_real2_button_CB(GtkButton *button, gpointer user_data){
    CharaSwitch3ColumnItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_csw3_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        item = CHARA_SW3_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        printf("Data at %d: %s: %d %d %d\n", original_index, item->string,
               item->int1, item->int2, item->int3);
    }
    return;
};

static void add_item_button_CB(GtkButton *button, gpointer user_data){
    CharaSwitch3ColumnItem *item;
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
	GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_csw3_column(column_view);
    if(original_index < 0) {
        printf("No item selected.\n");
        return;
    }else if(original_index < g_list_model_get_n_items(G_LIST_MODEL(store))){
        item = CHARA_SW3_COLUMN_ITEM(g_list_model_get_object(G_LIST_MODEL(store), original_index));
        g_list_store_insert(store, original_index, 
                            chara_sw3_column_item_new(item->string, item->int1, 
                                                      item->int2, item->int3));
    }
    return;
};

static void delete_item_button_CB(GtkButton *button, gpointer user_data){
    GtkColumnView *column_view = GTK_COLUMN_VIEW(user_data);
    GListStore *store = G_LIST_STORE(g_object_get_data(G_OBJECT(user_data), "list_store"));
    
    int original_index = find_original_index_of_csw3_column(column_view);
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
GListStore *init_csw3_list_store(int num, char *string, int **iarray3){
    int i;
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    for(i=0;i<num;i++){
        g_list_store_append(store,chara_sw3_column_item_new(string, iarray3[i][0], 
                                                            iarray3[i][1], iarray3[i][2]));
    };
    return store;
};

GtkWidget * csw3_columnview_init(GListStore *store){
    GtkWidget *cview = gtk_column_view_new(NULL);
    
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW(cview)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(cview), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup", G_CALLBACK(setup_str1_label_CB),    NULL);
    g_signal_connect(factory1, "bind", G_CALLBACK(bind_str1_label_CB),      NULL);
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_str1_label_CB),  NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("str1", factory1);
    sorter = GTK_SORTER(gtk_string_sorter_new(gtk_property_expression_new(CHARA_SW3_COLUMN_TYPE_ITEM,NULL,"Str1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup",  G_CALLBACK(setup_int1_button_CB),  NULL);
    g_signal_connect(factory2, "bind",   G_CALLBACK(bind_int1_button_CB),   NULL);
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_int1_button_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("flag1", factory2);
    sorter = GTK_SORTER(gtk_numeric_sorter_new(gtk_property_expression_new (CHARA_SW3_COLUMN_TYPE_ITEM,
                                                                             NULL, "Flag1")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column2);
    
    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_int2_button_CB),  NULL);
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_int2_button_CB),   NULL);
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_int2_button_CB), NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("flag2", factory3);
    sorter = GTK_SORTER(gtk_numeric_sorter_new(gtk_property_expression_new (CHARA_SW3_COLUMN_TYPE_ITEM,
                                                                             NULL, "Flag2")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column3);
    
    // Factory setup and bind for column 4
    GtkListItemFactory *factory4 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory4, "setup",  G_CALLBACK(setup_int3_button_CB),  NULL);
    g_signal_connect(factory4, "bind",   G_CALLBACK(bind_int3_button_CB),   NULL);
    g_signal_connect(factory4, "unbind", G_CALLBACK(unbind_int3_button_CB), NULL);
    GtkColumnViewColumn *column4 = gtk_column_view_column_new("Flag3", factory4);
    sorter = GTK_SORTER(gtk_numeric_sorter_new(gtk_property_expression_new (CHARA_SW3_COLUMN_TYPE_ITEM,
                                                                             NULL, "Flag3")));
    gtk_column_view_column_set_sorter (column4, GTK_SORTER(sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW(cview), column4);
    
    return cview;
}

struct kemo_chara_sw3_columnview * alloc_kemo_csw3_columnview(){
    struct kemo_chara_sw3_columnview *sw3_vws;
    sw3_vws = (struct kemo_chara_sw3_columnview *) malloc(sizeof(struct kemo_chara_sw3_columnview *));
    if (sw3_vws == NULL){
        printf("malloc error for kemo_chara_sw3_columnview \n");
        exit(1);
    };
    return sw3_vws;
};

struct kemo_chara_sw3_columnview * init_csw3_columnview_scroll(int num, char *string, int **iarray3){
    struct kemo_chara_sw3_columnview *sw3_vws = alloc_kemo_csw3_columnview();
    
    // Add column view.  Put in scrolled window.  
    sw3_vws->ci3_store = init_csw3_list_store(num, string, iarray3); 
    sw3_vws->ci3_cview = csw3_columnview_init(sw3_vws->ci3_store);
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(sw3_vws->ci3_cview),TRUE);
    g_object_set_data(G_OBJECT(sw3_vws->ci3_cview), "list_store",
                      G_OBJECT(sw3_vws->ci3_store));
    
    
    GtkWidget *add_item_button = gtk_button_new_with_label("Add");
    g_signal_connect(G_OBJECT(add_item_button), "clicked",
                     G_CALLBACK(add_item_button_CB), G_OBJECT(sw3_vws->ci3_cview));
    
    GtkWidget *del_item_button = gtk_button_new_with_label("Delete");
    g_signal_connect(G_OBJECT(del_item_button), "clicked",
                     G_CALLBACK(delete_item_button_CB), G_OBJECT(sw3_vws->ci3_cview));
    
    
    GtkWidget *show_all_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_all_button), "clicked",
                     G_CALLBACK(show_all_real2_button_CB), G_OBJECT(sw3_vws->ci3_store));
    
    GtkWidget *show_each_button = gtk_button_new_with_label("Show selected");
    g_signal_connect(G_OBJECT(show_each_button), "clicked",
                     G_CALLBACK(show_each_real2_button_CB), G_OBJECT(sw3_vws->ci3_cview));
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), sw3_vws->ci3_cview, 0, 0, 4, 1);
    
    gtk_grid_attach(GTK_GRID(grid), add_item_button,  0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), del_item_button,  2, 1, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), show_all_button,  0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), show_each_button, 2, 2, 2, 1);
    
    // scrolled window
    sw3_vws->ci3_cview_scroll = gtk_scrolled_window_new(); 
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(sw3_vws->ci3_cview_scroll),  360);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(sw3_vws->ci3_cview_scroll), 240);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw3_vws->ci3_cview_scroll), grid);
    
    return sw3_vws;
};

void dealloc_kemo_csw3_columnview(struct kemo_chara_sw3_columnview *sw3_vws){
    g_object_unref(G_OBJECT(sw3_vws->ci3_store));
    g_object_unref(G_OBJECT(sw3_vws->ci3_store));
    g_object_unref(G_OBJECT(sw3_vws->ci3_cview_scroll));
    free(sw3_vws);
    return;
}
