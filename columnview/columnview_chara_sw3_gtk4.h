/***********************************************************************
 *  columnview_chara_sw3_gtk4.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_CHARA_SW3_GTK4_
#define COLUMNVIEW_CHARA_SW3_GTK4_

#include "calypso_GTK4.h"
/*
 Setup custom type - each item has two strings, one real, and one integer
  which will be displayed in four columns
*/
enum {
    PROP_CHARA_SW3_C0 = 1,
    PROP_CHARA_SW3_I1 = 2,
    PROP_CHARA_SW3_I2 = 3,
    PROP_CHARA_SW3_I3 = 4,
    N_PROPS_CHARA_SW3
};

struct _CharaSwitch3ColumnItem
{
    GObject parent_instance;
    char *string;
    int   int1;
    int   int2;
    int   int3;
};

struct kemo_chara_sw3_columnview
{
    GtkWidget  *ci3_cview_scroll;
    
    GListStore *ci3_store; 
    GtkWidget  *ci3_cview;
};

/*
 * Type declaration for G_OBJECTS
 */
G_BEGIN_DECLS

#define CHARA_SW3_COLUMN_TYPE_ITEM (chara_sw3_column_item_get_type())
G_DECLARE_FINAL_TYPE(CharaSwitch3ColumnItem, chara_sw3_column_item, CHARA_SW3_COLUMN, ITEM, GObject)


/* prototypes */
CharaSwitch3ColumnItem * csw3_column_item_new(const float real1,
                                            const float real2);

int find_original_index_of_csw3_column(GtkColumnView *column_view);

void show_all_ci3_button_CB(GtkButton *button, gpointer user_data);
void show_each_ci3_button_CB(GtkButton *button, gpointer user_data);

GListStore *init_ci3_list_store(int num, char *carray, int **iarray3);
GtkWidget * ci3_columnview_init(GListStore *store);

struct kemo_chara_sw3_columnview * alloc_kemo_csw3_columnview();
struct kemo_chara_sw3_columnview * init_csw3_columnview_scroll(int num, char *string, int **iarray3);
void dealloc_kemo_csw3_columnview(struct kemo_chara_sw3_columnview *ci3_vws);

G_END_DECLS

#endif /* COLUMNVIEW_CHARA_SW3_GTK4_ */
