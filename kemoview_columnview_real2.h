/***********************************************************************
 *  kemoview_columnview_real2.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef KEMOVIEW_COLUMNVIEW_REAL2_
#define KEMOVIEW_COLUMNVIEW_REAL2_

#include "calypso_GTK4.h"
#include "columnview_real2_gtk4.h"

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

#endif /* KEMOVIEW_COLUMNVIEW_REAL2_ */
