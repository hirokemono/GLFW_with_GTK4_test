/*****************************************************************************************
 *        kemoview_gtk4_Mesh_polygon_sel.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_POLYGON_SEL_
#define KEMOVIEW_GTK4_MESH_POLYGON_SEL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

struct mesh_polygon_selector{
    GtkWidget *polygon_dropdown;
    GtkWidget *polygon_sel_box;
};

/* protypes */
void set_polygon_select_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                 struct mesh_polygon_selector *polygon_select_vws);
struct mesh_polygon_selector * init_polygon_select_views(struct kemoviewer_gl_type *kemo_gl);
void dealloc_mesh_polygon_selector(struct mesh_polygon_selector *polygon_select_vws);

#endif  /*  KEMOVIEW_GTK4_MESH_POLYGON_SEL_  */