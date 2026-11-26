/*****************************************************************************************
 *        kemoview_gtk4_Mesh_color_menu.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_COLOR_MENU_
#define KEMOVIEW_GTK4_MESH_COLOR_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_colorsel.h"

struct mesh_color_menu{
    int iflag_group;
    int selected_type;
    
    GtkWidget *color_sel_dropdown;
    GtkWidget *color_pick_cell;
    GtkWidget *color_sel_box;
};

/* protypes */
struct mesh_color_menu * init_mesh_color_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                                  char *menu_label,
                                                  int iflag_group,
                                                  int selected_type);
void set_mesh_color_menu(struct kemoviewer_gl_type *kemo_gl,
                         struct mesh_color_menu *mesh_color_menu_vws);

void dealloc_mesh_color_menu(struct mesh_color_menu *mesh_color_menu_vws);

#endif  /*  KEMOVIEW_GTK4_MESH_COLOR_MENU_  */