/***********************************************************************
 *  columnview_light_positions.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_LIGHT_POSITIONS_
#define COLUMNVIEW_LIGHT_POSITIONS_

#include "kemoviewer.h"

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

struct _TestColItem
{
    GObject parent_instance;
    float real1;
    float real2;
    float real3;
};


/*
 * Type declaration for G_OBJECTS
 */
G_BEGIN_DECLS

#define TESTCOL_TYPE_ITEM (testcol_item_get_type())
G_DECLARE_FINAL_TYPE(TestColItem, testcol_item, TESTCOL, ITEM, GObject)


/* prototypes */
GtkWidget *init_light_position_list_box(struct kemoviewer_type *kemo_sgl);

G_END_DECLS

#endif /* COLUMNVIEW_LIGHT_POSITIONS_ */

