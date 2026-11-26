/***********************************************************************
 *  kemoview_gtk4_Mesh_domain.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_DOMAIN_
#define KEMOVIEW_GTK4_MESH_DOMAIN_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_Mesh_draw_all.h"
#include "kemoview_gtk4_Mesh_color_menu.h"

struct domain_gtk_menu{
    struct show_all_gtk_menu *all_domain_vws;
    
    GtkWidget *spin_patch_opacity;
    struct mesh_color_menu *domain_patch_color_vws;
    struct mesh_color_menu *domain_grid_color_vws;
    struct mesh_color_menu *domain_node_color_vws;
    
    GtkWidget *domain_menu_expander;
};

/* prototypes */
struct domain_gtk_menu * init_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                                              GtkWidget *window);
void set_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl, 
                         struct domain_gtk_menu *domain_gmenu);
void dealloc_domain_menu(struct domain_gtk_menu *domain_gmenu);

#endif /*  KEMOVIEW_GTK4_MESH_DOMAIN_  */
