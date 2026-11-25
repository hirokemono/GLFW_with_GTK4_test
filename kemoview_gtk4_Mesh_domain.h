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

struct domain_gtk_menu{
    GtkWidget *button_draw_patch;
    GtkWidget *button_draw_grid;
    GtkWidget *button_draw_node;
    
    GtkWidget *button_hide_patch;
    GtkWidget *button_hide_grid;
    GtkWidget *button_hide_node;
    
    GtkWidget *domain_menu_box;
};

/* prototypes */
struct domain_gtk_menu * init_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                                              GtkWidget *window);
void set_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                         struct domain_gtk_menu *domain_gmenu);

void dealloc_domain_menu(struct domain_gtk_menu *domain_gmenu);

#endif /*  KEMOVIEW_GTK4_MESH_DOMAIN_  */
