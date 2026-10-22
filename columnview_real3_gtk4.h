/***********************************************************************
 *  columnview_real3_gtk4.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_REAL3_GTK4_
#define COLUMNVIEW_REAL3_GTK4_

#include "calypso_GTK4.h"
/*
 Setup custom type - each item has two strings, one real, and one integer
  which will be displayed in four columns
*/
enum {
    PROP_REAL1 = 1,
    PROP_REAL2 = 2,
    PROP_REAL3 = 3,
    N_PROPERTIES
};

struct _Real3ColumnItem
{
    GObject parent_instance;
    float real1;
    float real2;
    float real3;
};

struct kemo_real3_columnview{
    GtkWidget  *real3_cview_scroll;
    
    GListStore *real3_store; 
    GtkWidget  *real3_cview;
};

/*
 * Type declaration for G_OBJECTS
 */
G_BEGIN_DECLS

#define TESTCOL_TYPE_ITEM (real3_column_item_get_type())
G_DECLARE_FINAL_TYPE(Real3ColumnItem, real3_column_item, REAL3_COLUMN, ITEM, GObject)


/* prototypes */
Real3ColumnItem * real3_column_item_new(const float real1,
                                        const float real2,
                                        const float real3);

int find_original_index_of_real3_column(GtkColumnView *column_view);

void show_all_real3_button_CB(GtkButton *button, gpointer user_data);
void show_each_real3_button_CB(GtkButton *button, gpointer user_data);

GListStore *init_real3_list_store(int num, float **real3);
GtkWidget * real3_columnview_init(GListStore *store);

struct kemo_real3_columnview * alloc_kemo_real3_columnview();
struct kemo_real3_columnview * init_real_columnview_scroll(int num, float **real3);
void dealloc_kemo_real3_columnview(struct kemo_real3_columnview *real3_vws);

G_END_DECLS

#endif /* COLUMNVIEW_REAL3_GTK4_ */
