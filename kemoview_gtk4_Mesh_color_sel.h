/*****************************************************************************************
 *        kemoview_gtk4_Mesh_color_sel.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_COLOR_SEL_
#define KEMOVIEW_GTK4_MESH_COLOR_SEL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "set_rgba_table_c.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

struct mesh_color_view{
    GtkWidget *mesh_color_dropdown;
    GtkWidget *mesh_color_box;
};

/* protypes */
void set_mesh_color_dropdown(struct kemoviewer_gl_type *kemo_gl,
                             struct mesh_color_view *mesh_color_vws);
struct mesh_color_view * init_mesh_color_views(struct kemoviewer_gl_type *kemo_gl);
void dealloc_mesh_color_views(struct mesh_color_view *mesh_color_vws);

#endif  /*  KEMOVIEW_GTK4_MESH_COLOR_SEL_  */