/***********************************************************************
 *  column_view_real3_gtk4.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#include "column_view_real3_gtk4.h"

/* Initialize */
static void real3_column_item_init(Real3ColumnItem *item)
{
    printf("real3_column_item_init %p/n", item);
}

static void real3_column_item_class_init(Real3ColumnItemClass *class)
{
    printf("real3_column_item_class_init %p/n", class);
}

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

void real3_column_item_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
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
}

void real3_column_item_set_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
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
}

/*  Construct GListmodel  */
GListModel * real3_column_model_init(void){
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
