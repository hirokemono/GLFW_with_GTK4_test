/***********************************************************************
 *  columnview_sw3_chara_gtk4.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_SW3_CHARA_GTK4_
#define COLUMNVIEW_SW3_CHARA_GTK4_

#include "calypso_GTK4.h"
/*
 Setup custom type - each item has two strings, one real, and one integer
  which will be displayed in four columns
*/
enum {
    PROP_SW3C_SW1 = 1,
    PROP_SW3C_SW2 = 2,
    PROP_SW3C_SW3 = 3,
    N_PROPS_SW3C
};

struct _Sw3cColumnItem
{
    GObject parent_instance;
    float real1;
    float real2;
    float real3;
};

struct kemo_sw3c_columnview{
    GtkWidget  *real3_cview_scroll;
    
    GListStore *real3_store; 
    GtkWidget  *real3_cview;
};

/*
 * Type declaration for G_OBJECTS
 */
G_BEGIN_DECLS

#define SW3C_COLUMN_TYPE_ITEM (sw3c_column_item_get_type())
G_DECLARE_FINAL_TYPE(Sw3cColumnItem, sw3c_column_item, SW3C_COLUMN, ITEM, GObject)


/* prototypes */
Sw3cColumnItem * sw3c_column_item_new(const float real1,
                                      const float real2,
                                      const float real3);

int find_original_index_of_sw3c_column(GtkColumnView *column_view);

void show_all_sw3c_button_CB(GtkButton *button, gpointer user_data);
void show_each_sw3c_button_CB(GtkButton *button, gpointer user_data);

GListStore *init_sw3c_list_store(int num, float **array3);
GtkWidget * sw3c_columnview_init(GListStore *store);

struct kemo_sw3c_columnview * alloc_kemo_sw3c_columnview();
struct kemo_sw3c_columnview * init_sw3c_columnview_scroll(int num, float **array3);
void dealloc_kemo_sw3c_columnview(struct kemo_sw3c_columnview *sw3c_vws);

G_END_DECLS

#endif /* COLUMNVIEW_SW3_CHARA_GTK4_ */
