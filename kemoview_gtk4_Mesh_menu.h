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

struct mesh_gtk_menu{
    GtkWidget *spin_dist;
    
    GtkWidget *mesh_menu_box;
};

/* prototypes */
struct mesh_gtk_menu * init_gtk_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                                          GtkWidget *window);

void dealloc_mesh_menu(struct mesh_gtk_menu *mesh_gmenu);

#endif /*  KEMOVIEW_GTK4_MESH_MENU_  */
