/***********************************************************************
 *  column_view_real3_gtk4.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#ifndef COLUMN_VIEW_REAL3_GTK4_
#define COLUMN_VIEW_REAL3_GTK4_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "t_control_real3_IO.h"

#include "calypso_GTK4.h"

/* Setup custom type */
#define REAL3_COLUMN_TYPE_ITEM (real3_column_item_get_type())
G_DECLARE_FINAL_TYPE(Real3ColumnItem, real3_column_item, REAL3_COLUMN, ITEM, GObject)

struct _Real3ColumnItem{
    GObject parent_instance;
    int   id;
    float real1;
    float real2;
    float real3;
};

struct _Real3ColumnItemClass{
    GObjectClass parent_class;
};

G_DEFINE_TYPE(Real3ColumnItem, real3_column_item, G_TYPE_OBJECT);

enum{
    PROP_ID =    1,
    PROP_REAL1 = 2,
    PROP_REAL2 = 3,
    PROP_REAL3 = 4,
    N_PROPERTIES
};


/*  prototypes */

void real3_column_item_init(Real3ColumnItem *item);
void real3_column_item_class_init(Real3ColumnItemClass *class);

GtkWidget * real3_columnview_init(struct real3_clist *r3_clist_gtk);


#endif  /* COLUMN_VIEW_REAL3_GTK4_ */ 
