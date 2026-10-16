/***********************************************************************
 *  column_view_real3_gtk4.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "column_view_real3_gtk4.h"

/* Create the custom object for each column*/
static Real3ColumnItem * real3_column_item_new(int id,
                                               float real1,
                                               float real2,
                                               float real3){
    Real3ColumnItem  *item = g_object_new(REAL3_COLUMN_TYPE_ITEM, NULL);
    item->id = id;
    item->real1 = real1;
    item->real2 = real2;
    item->real3 = real3;
    return item;
}

void real3_column_item_get_property(GObject *object,
                                    guint prop_id,
                                    GValue *value,
                                    GParamSpec *pspec){
    Real3ColumnItem *item = (Real3ColumnItem *)object;
    
    switch (prop_id){
    case PROP_ID:
        g_value_set_int(value, item->id);
        break;
    case PROP_REAL1:
        g_value_set_float(value, item->real1);
        break;
    case PROP_REAL2:
        g_value_set_float(value, item->real2);
        break;
    case PROP_REAL3:
        g_value_set_float(value, item->real3);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
    return;
}


static int real3_column_item_get_id(Real3ColumnItem *item){
    return item->id;
}

static float real3_column_item_get_real1(Real3ColumnItem *item){
    return item->real1;
}

static float real3_column_item_get_real2(Real3ColumnItem *item){
    return item->real2;
}

static float real3_column_item_get_real3(Real3ColumnItem *item){
    return item->real3;
}

void real3_column_item_set_property(GObject *object,
                                    guint prop_id,
                                    const GValue *value,
                                    GParamSpec *pspec){
    Real3ColumnItem *item = (Real3ColumnItem *)object;
    
    switch (prop_id){
    case PROP_ID:
        item->id = (int) g_value_get_int(value);
        break;
    case PROP_REAL1:
        item->real1 = (float) g_value_get_float(value);
        break;
    case PROP_REAL2:
        item->real2 = (float) g_value_get_float(value);
        break;
    case PROP_REAL3:
        item->real3 = (float) g_value_get_float(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
    return;
}

/*  Construct GListmodel  */
GListModel * real3_column_model_new(void){
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    return G_LIST_MODEL(store);
}

int append_real3_column_model(int icou, struct real3_clist *r3_clist_gtk, 
                              GListStore *store){
    double data[3];
    copy_from_real3_clist(r3_clist_gtk, icou,
                          &data[0], &data[1], &data[2]);
    g_list_store_append(store, real3_column_item_new(icou, data[0], data[1], data[2]));
    return (icou + 1);
};

GListModel * real3_column_model_init(struct real3_clist *r3_clist_gtk){
    GListModel *store = real3_column_model_new();
    
    double data[3];
    int num = 0;
    struct real3_ctl_list *tako = &r3_clist_gtk->r3_item_head;
    struct real3_ctl_list *head = tako->_next;
    while (head != NULL){
        set_from_real3_ctl_item_c(head->r3_item, &data[0], &data[1], &data[2]);
        g_list_store_append(G_LIST_STORE(store), 
                            real3_column_item_new(num, data[0], data[1], data[2]));
        head = head->_next;
        num = num + 1;
    };
    printf(" %d of columns are appended \n", num);
    return G_LIST_MODEL(store);
}


/* Initialize */

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

void real3_column_item_init(Real3ColumnItem *item){
    printf("real3_column_item_init %p/n", item);
    item->id = 0;
    item->real1 = 0.;
    item->real2 = 0.;
    item->real3 = 0.;
    return;
}

void real3_column_item_class_init(Real3ColumnItemClass *class){
    printf("real3_column_item_class_init %p/n", class);
    
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);
    
    gobject_class->get_property = real3_column_item_get_property;
    gobject_class->set_property = real3_column_item_set_property;
    
    obj_properties[PROP_ID] =    g_param_spec_string("id",
                                                     "Id",
                                                     "ID",
                                                     NULL,
                                                     G_PARAM_READWRITE);
    obj_properties[PROP_REAL1] = g_param_spec_string("real1",
                                                     "Real1",
                                                     "first_value",
                                                     NULL,
                                                     G_PARAM_READWRITE);
    obj_properties[PROP_REAL3] = g_param_spec_string("real2",
                                                     "Real2",
                                                     "second_value",
                                                     NULL,
                                                     G_PARAM_READWRITE);
    obj_properties[PROP_REAL3] = g_param_spec_string("real3",
                                                     "Real3",
                                                     "thired_value",
                                                     NULL,
                                                     G_PARAM_READWRITE);
    
    g_object_class_install_properties(gobject_class, N_PROPERTIES, obj_properties);
    return;
}

/*  Callbacks */
static void setup_CB(GtkSignalListItemFactory *factory,GObject  *listitem)
{
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(GTK_LIST_ITEM(listitem),label);
    printf("setup_CB %p %p \n", factory, listitem);
}

static void bind_real1_CB(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
//    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    float real1 = real3_column_item_get_real1(REAL3_COLUMN_ITEM(item));
//    gtk_label_set_text(GTK_LABEL (label), string);
    printf("bind_real1_CB %f \n", real1);
}

static void bind_real2_CB(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
//    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    float real2 = real3_column_item_get_real2(REAL3_COLUMN_ITEM(item));
//    gtk_label_set_text(GTK_LABEL (label), string);
    printf("bind_real2_CB %f \n", real2);
}

static void bind_real3_CB(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
//    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    float real3 = real3_column_item_get_real3(REAL3_COLUMN_ITEM(item));
//    gtk_label_set_text(GTK_LABEL (label), string);
    printf("bind_real3_CB %f \n", real3);
}

/* Initialize table with three real columns */
GtkWidget * real3_columnview_init(struct real3_clist *r3_clist_gtk){
    GtkWidget *columnview;
    
    /* Create and populate custom list model */
    GListModel *store = real3_column_model_init(r3_clist_gtk);
    
    // Add column view.  Put in scrolled window.  
    columnview = gtk_column_view_new(NULL); // Null for now want to add sorter
    gtk_column_view_set_show_column_separators (GTK_COLUMN_VIEW (columnview), TRUE);
     
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref (gtk_column_view_get_sorter (GTK_COLUMN_VIEW (columnview)));
    GtkSortListModel *model = gtk_sort_list_model_new (G_LIST_MODEL (store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new (G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model (GTK_COLUMN_VIEW (columnview), GTK_SELECTION_MODEL (selection));
    
    // Factory setup and bind for column 1 
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_CB),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_real1_CB),NULL);
    GtkColumnViewColumn *column = gtk_column_view_column_new("real1", factory);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (REAL3_COLUMN_TYPE_ITEM,NULL,"real1")));
    gtk_column_view_column_set_sorter (column, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (columnview), column);
    
    // Factory setup and bind for column 2
    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_CB),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_real2_CB),NULL);
    column = gtk_column_view_column_new("real2", factory);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (REAL3_COLUMN_TYPE_ITEM, NULL, "real2")));
    gtk_column_view_column_set_sorter (column, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (columnview), column);
    
    // Factory setup and bind for column 3
    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_CB),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_real3_CB),NULL);
    column = gtk_column_view_column_new("real3", factory);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (REAL3_COLUMN_TYPE_ITEM, NULL, "real3")));
    gtk_column_view_column_set_sorter (column, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (columnview), column);
    
    return columnview;
}
