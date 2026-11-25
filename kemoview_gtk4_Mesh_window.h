/***********************************************************************
 *  kemoview_gtk4_Mesh_window.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_MESH_WINDOW_
#define KEMOVIEW_GTK4_MESH_WINDOW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_Mesh_menu.h"

struct mesh_gtk_window_view{
    int iflag_meshBox;
    
    GtkWidget *closeButton;
    
    GtkWidget *mesh_frame;
    GtkWidget *meshWin;
    
    struct mesh_gtk_menu *mesh_gmenu;
};

/* prototypes */
void init_mesh_window(struct kemoviewer_gl_type *kemo_gl,
                      struct mesh_gtk_window_view *mesh_win_vws,
                      GtkWidget *main_window);

struct mesh_gtk_window_view * alloc_mesh_window_view(void);
void dealloc_mesh_window_view(struct mesh_gtk_window_view *mesh_win_vws);

#endif /*  KEMOVIEW_GTK4_MESH_WINDOW_  */
