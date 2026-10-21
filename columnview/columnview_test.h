/***********************************************************************
 *  columnview_test.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_TEST_
#define COLUMNVIEW_TEST_

#include <gtk/gtk.h>



/*
 Setup custom type - each item has two strings, one real, and one integer
  which will be displayed in four columns
*/
enum {
    PROP_STR1 =  1,
    PROP_STR2 =  2,
    PROP_REAL3 = 3,
    PROP_INT4 =  4,
    N_PROPERTIES
};

struct _TestColItem
{
    GObject parent_instance;
    char  *str1;
    char  *str2;
    float real3;
    int    int4;
};


/*
 * Type declaration for G_OBJECTS
 */
G_BEGIN_DECLS

#define TESTCOL_TYPE_ITEM (testcol_item_get_type())
G_DECLARE_FINAL_TYPE(TestColItem, testcol_item, TESTCOL, ITEM, GObject)


/* prototypes */
GtkWidget * init_list_store_test_grid();

G_END_DECLS

#endif /* COLUMNVIEW_TEST_ */

