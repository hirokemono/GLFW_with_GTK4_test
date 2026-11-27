/***********************************************************************
 *  kemoview_gtk4_Mesh_draw_all.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_DRAW_ALL_
#define KEMOVIEW_GTK4_MESH_DRAW_ALL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "columnview_sw3_chara_gtk4.h"
#include "columnview_draw_mesh.h"

struct show_all_gtk_menu{
    int iflag_group;
    
    GtkWidget *button_draw_patch;
    GtkWidget *button_draw_grid;
    GtkWidget *button_draw_node;
    
    GtkWidget *button_hide_patch;
    GtkWidget *button_hide_grid;
    GtkWidget *button_hide_node;
    
    GtkWidget *show_all_box;
};

/* prototypes */
struct show_all_gtk_menu * init_show_all_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                                                   int iflag_group,
                                                   struct kemo_sw3c_columnview *draw_mesh_vws);
struct show_all_gtk_menu * init_show_all_node_menu(struct kemoviewer_gl_type *kemo_gl,
                                                   struct kemo_sw3c_columnview *draw_mesh_vws);

void dealloc_show_all_mesh_menu(struct show_all_gtk_menu *show_all_vws);

#endif /*  KEMOVIEW_GTK4_MESH_DRAW_ALL_  */
