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

#include "calypso_GTK4.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_fileselector.h"

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
	
    GtkAdjustment *adj_rotation_x;
    GtkAdjustment *adj_rotation_y;
    GtkAdjustment *adj_rotation_z;
    GtkWidget *spin_rotation_x;
    GtkWidget *spin_rotation_y;
    GtkWidget *spin_rotation_z;
    
    GtkWidget *hbox_rotation_x;
    GtkWidget *hbox_rotation_y;
    GtkWidget *hbox_rotation_z;
	
	GtkAdjustment *adj_rotation_deg;
	GtkWidget *spin_rotation_deg;
	GtkWidget *hbox_rotation_deg;
	
	GtkAdjustment *adj_aperture;
	GtkWidget *spin_aperture;
	GtkWidget *hbox_aperture;
	
	GtkAdjustment *adj_focus;
	GtkWidget *spin_focus;
	GtkWidget *hbox_focus;
	
	int iflag_updated_eye_separation;
    int iflag_updated_eye_sep_angle;
	GtkAdjustment *adj_eye_sep;
    GtkAdjustment *adj_sep_angle;
	GtkWidget *spin_eye_sep;
	GtkWidget *hbox_eye_sep;
    GtkWidget *spin_sep_angle;
    GtkWidget *hbox_sep_angle;
	
    GtkWidget *vbox_win;
    GtkWidget *Frame_win;
    GtkWidget *vbox_eye;
    GtkWidget *Frame_eye;
    GtkWidget *vbox_looking;
    GtkWidget *Frame_looking;
    GtkWidget *vbox_scale;
    GtkWidget *Frame_scale;
    GtkWidget *vbox_rotation;
    GtkWidget *Frame_rotation;
    GtkWidget *vbox_aperture;
    GtkWidget *Frame_aperture;
    GtkWidget *vbox_streo;
    GtkWidget *Frame_stereo;
    
    GtkWidget *vbox_viewmatrix_save;
    GtkWidget *hbox_viewmatrix_save;
    GtkEntryBuffer *entry_buf_viewmat_file;
    GtkWidget *saveView_Button;
    GtkWidget *loadView_Button;
};

/*  prototypes */

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
