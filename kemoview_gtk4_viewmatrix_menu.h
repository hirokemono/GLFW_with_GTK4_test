/*****************************************************************************************
 *        kemoview_gtk4_viewmatrix_menu.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_VIEWMATRIX_MENU_
#define KEMOVIEW_GTK4_VIEWMATRIX_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_viewmat_file_IO.h"
#include "kemoview_gtk4_rotation_view.h"
#include "kemoview_gtk4_stereo_view.h"

#include "view_modifier_glfw.h"



struct view_widgets{
    GtkWidget *spin_win_x;
    GtkWidget *spin_win_y;
    GtkWidget *hbox_win_x;
    GtkWidget *hbox_win_y;
	
    GtkAdjustment *adj_eye_x;
    GtkAdjustment *adj_eye_y;
    GtkAdjustment *adj_eye_z;
    GtkWidget *spin_eye_x;
    GtkWidget *spin_eye_y;
    GtkWidget *spin_eye_z;
    
    GtkWidget *hbox_eye_x;
    GtkWidget *hbox_eye_y;
    GtkWidget *hbox_eye_z;
	
    GtkWidget *hbox_looking_x;
    GtkWidget *hbox_looking_y;
    GtkWidget *hbox_looking_z;
	
	GtkAdjustment *adj_scale;
	GtkWidget *spin_scale;
	GtkWidget *hbox_scale;
	
	GtkAdjustment *adj_aperture;
	GtkWidget *spin_aperture;
	GtkWidget *hbox_aperture;
	
	
    GtkWidget *vbox_win;
    GtkWidget *Frame_win;
    GtkWidget *vbox_eye;
    GtkWidget *Frame_eye;
    GtkWidget *vbox_looking;
    GtkWidget *Frame_looking;
    GtkWidget *vbox_scale;
    GtkWidget *Frame_scale;
    GtkWidget *vbox_aperture;
    GtkWidget *Frame_aperture;
    
    
    struct stereo_view_widgets *stereo_menu;
    GtkWidget *Frame_stereo;
    
    struct rot_view_widgets *rot_view_menu;
    GtkWidget *Frame_rotation;
    
    struct viewmat_file_IO_widgets *viewmat_IO;
    GtkWidget *hbox_viewmatrix_save;
};

/*  prototypes */
struct view_widgets * init_viewmat_widgets(void);
void dealloc_viewmat_widgets(struct view_widgets *view_menu);

void update_windowsize_menu(struct kemoviewer_gl_type *kemo_gl,
                            struct view_widgets *view_menu,
                            GtkWidget *window);
void set_viewmatrix_value(struct kemoviewer_type *kemo_sgl,
                          struct view_widgets *view_menu,
                          GtkWidget *window);

GtkWidget * init_viewmatrix_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                          struct view_widgets *view_menu,
                                          GtkWidget *window);
#endif
