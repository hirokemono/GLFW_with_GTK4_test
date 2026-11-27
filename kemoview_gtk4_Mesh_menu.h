/***********************************************************************
 *  kemoview_gtk4_Mesh_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_MENU_
#define KEMOVIEW_GTK4_MESH_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_Mesh_color_sel.h"
#include "kemoview_gtk4_Mesh_polygon_sel.h"
#include "kemoview_gtk4_Mesh_domain.h"

struct mesh_gtk_menu{
    GtkWidget *spin_dist;
    GtkWidget *spin_num_loop;
    
    GtkWidget *spin_node_size;
    GtkWidget *spin_node_digit;
    
    struct mesh_color_view       *mesh_color_vws;
    struct mesh_polygon_selector *polygon_select_vws;
    
    struct domain_gtk_menu       *domain_gmenu;
    struct domain_gtk_menu       *element_grp_gmenu;
    
    GtkWidget *mesh_menu_box;
};

/* prototypes */
struct mesh_gtk_menu * init_gtk_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                                          GtkWidget *window);
void set_gtk_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                       struct mesh_gtk_menu *mesh_gmenu);

void dealloc_mesh_menu(struct mesh_gtk_menu *mesh_gmenu);

#endif /*  KEMOVIEW_GTK4_MESH_MENU_  */
