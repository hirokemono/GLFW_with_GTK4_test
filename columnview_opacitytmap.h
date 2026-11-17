/***********************************************************************
 *  columnview_opacitymap.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_OPACITYMAP_
#define COLUMNVIEW_OPACITYMAP_

#include "kemoviewer_gl.h"
#include "kemoviewer.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "columnview_real2_gtk4.h"
#include "kemoview_gtk4_routines.h"

/* prototypes */
struct kemo_real2_columnview * init_opacitymap_list_scroll(struct kemoviewer_gl_type *kemo_gl,
                                                           GtkWidget *psf_window);

Real2ColumnItem * real2_column_item_new(const float real1,
                                        const float real2);

int find_original_index_of_real2_column(GtkColumnView *column_view);

void show_all_real2_button_CB(GtkButton *button, gpointer user_data);
void show_each_real2_button_CB(GtkButton *button, gpointer user_data);

GListStore *init_real2_list_store(int num, float **array2);
GtkWidget * real2_columnview_init(GListStore *store);

struct kemo_real2_columnview * alloc_kemo_real3_columnview();
struct kemo_real2_columnview * init_real2_columnview_scroll(int num, float **array2);
void dealloc_kemo_real3_columnview(struct kemo_real2_columnview *real2_vws);

G_END_DECLS

#endif /* COLUMNVIEW_OPACITYMAP_ */
