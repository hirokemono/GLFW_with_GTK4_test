/*****************************************************************************************
 *  kemoview_gtk4_stereo_view.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Nov., 16, 2026.
 *  Copyright 2026
 *  Dept. of Earth and Planetary Sciences, Inst. of Science Tokyo. 
 *  All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_STEREO_VIEW_
#define KEMOVIEW_GTK4_STEREO_VIEW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"

struct stereo_view_widgets{
	int iflag_updated_eye_separation;
    int iflag_updated_eye_sep_angle;
    
	GtkAdjustment *adj_focus;
	GtkWidget *spin_focus;
	GtkWidget *hbox_focus;
	
	GtkAdjustment *adj_eye_sep;
	GtkWidget *spin_eye_sep;
    GtkWidget *hbox_eye_sep;
    
    GtkAdjustment *adj_sep_angle;
    GtkWidget *spin_sep_angle;
    GtkWidget *hbox_sep_angle;
    
    GtkWidget *vbox_stereo;
};

/*  prototypes */
struct stereo_view_widgets * init_stereo_view_widgets(void);
void dealloc_stereo_view_widgets(struct stereo_view_widgets *stereo_menu);

GtkWidget * init_stereo_view_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                        struct stereo_view_widgets *stereo_menu);
void set_gtk4_stereo_viewmat_value(struct kemoviewer_type *kemo_sgl,
                                   struct stereo_view_widgets *stereo_menu);

#endif /* KEMOVIEW_GTK4_STEREO_VIEW_ */
