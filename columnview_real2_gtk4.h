/***********************************************************************
 *  columnview_real2_gtk4.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_REAL2_GTK4_
#define COLUMNVIEW_REAL2_GTK4_

#include "calypso_GTK4.h"
/*
 Setup custom type - each item has two strings, one real, and one integer
  which will be displayed in four columns
*/
enum {
    PROP_REAL2_R1 = 1,
    PROP_REAL2_R2 = 2,
    N_PROPS_REAL2
};

struct _Real2ColumnItem
{
    GObject parent_instance;
    float real1;
    float real2;
};

struct kemo_real2_columnview
{
    GtkWidget  *real2_cview_scroll;
    
    GListStore *real2_store; 
    GtkWidget  *real2_cview;
};

/*
 * Type declaration for G_OBJECTS
 */
G_BEGIN_DECLS

#define REAL2_COLUMN_TYPE_ITEM (real2_column_item_get_type())
G_DECLARE_FINAL_TYPE(Real2ColumnItem, real2_column_item, REAL2_COLUMN, ITEM, GObject)


/* prototypes */
Real2ColumnItem * real2_column_item_new(const float real1,
                                        const float real2);

int find_original_index_of_real2_column(GtkColumnView *column_view);

void show_all_real2_button_CB(GtkButton *button, gpointer user_data);
void show_each_real2_button_CB(GtkButton *button, gpointer user_data);

GListStore *init_real2_list_store(int num, float **array2);
GtkWidget * real2_columnview_init(GListStore *store);

struct kemo_real2_columnview * alloc_kemo_real2_columnview();
struct kemo_real2_columnview * init_real2_columnview_scroll(int num, float **array2);
void dealloc_kemo_real2_columnview(struct kemo_real2_columnview *real2_vws);

G_END_DECLS

#endif /* COLUMNVIEW_REAL2_GTK4_ */
