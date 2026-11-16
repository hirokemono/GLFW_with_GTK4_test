/*****************************************************************************************
 *  kemoview_gtk4_rotation_view.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Nov., 16, 2026.
 *  Copyright 2026
 *  Dept. of Earth and Planetary Sciences, Inst. of Science Tokyo. 
 *  All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_ROTATION_VIEW_
#define KEMOVIEW_GTK4_ROTATION_VIEW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"

struct rot_view_widgets{
    GtkAdjustment *adj_rotation_x;
    GtkWidget *spin_rotation_x;
    GtkWidget *hbox_rotation_x;
    
    GtkAdjustment *adj_rotation_y;
    GtkWidget *spin_rotation_y;
    GtkWidget *hbox_rotation_y;
    
    GtkAdjustment *adj_rotation_z;
    GtkWidget *spin_rotation_z;
    GtkWidget *hbox_rotation_z;
	
	GtkAdjustment *adj_rotation_deg;
	GtkWidget *spin_rotation_deg;
	GtkWidget *hbox_rotation_deg;
    
    GtkWidget *vbox_rotation;
};

/*  prototypes */
struct rot_view_widgets * init_rot_view_widgets(void);
void dealloc_rot_view_widgets(struct rot_view_widgets *rot_view_menu);

GtkWidget * init_rotation_viewmat_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                             struct rot_view_widgets *rot_view_menu);
void set_gtk4_rotation_viewmat_value(struct kemoviewer_type *kemo_sgl,
                                     struct rot_view_widgets *rot_view_menu);

#endif /* KEMOVIEW_GTK4_ROTATION_VIEW_ */
