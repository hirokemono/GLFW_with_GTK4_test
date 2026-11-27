/***********************************************************************
 *  columnview_draw_mesh.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_DRAW_MESH_
#define COLUMNVIEW_DRAW_MESH_

#include "kemoviewer_gl.h"
#include "kemoviewer.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "columnview_sw3_chara_gtk4.h"

/* prototypes */
struct kemo_sw3c_columnview * init_draw_mesh_list_box(struct kemoviewer_gl_type *kemo_gl,
                                                      int *iflag_group,
                                                      GtkWidget *mesh_window);
void update_draw_mesh_list(struct kemoviewer_gl_type *kemo_gl, int iflag_group, 
                           struct kemo_sw3c_columnview *draw_mesh_vws);

#endif /* COLUMNVIEW_DRAW_MESH_ */

