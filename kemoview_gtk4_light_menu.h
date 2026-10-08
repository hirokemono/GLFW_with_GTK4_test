/***********************************************************************
 *  kemoview_gtk4_light_menu.h
 *  
 *
 *  Created by Hiroaki Matsui on 2018/08/19.
***********************************************************************/

#ifndef KEMOVIEW_GTK4_LIGHT_MENU_
#define KEMOVIEW_GTK4_LIGHT_MENU_

#include "calypso_GTK4.h"
#include "t_control_real_IO.h"
#include "t_control_real3_IO.h"
#include "m_phong_light_table_c.h"
#include "tree_views_4_fixed_lists_GTK.h"
#include "tree_view_real3_GTK.h"
#include "quicksort_c.h"

struct lightparams_view{
    struct r3_clist_view *light_rtp_vws;
    struct phong_lights *lights_gtk;
    
    GtkWidget *scrolled_window;
    GtkWidget *light_vbox;
};

/* prototypes */

struct lightparams_view * init_light_views_4_ctl(struct real3_clist *light_list);
struct lightparams_view * init_light_views_4_viewer(struct phong_lights *lights);
void dealloc_light_views_4_viewer(struct lightparams_view *light_vws);

GtkWidget *init_lightposition_expander(struct kemoviewer_type *kemo_sgl,
                                       struct lightparams_view *light_vws);


#endif /* KEMOVIEW_GTK4_LIGHT_MENU_ */
