/***********************************************************************
 *  kemoview_gtk4_rotation_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_ROTATION_MENU_
#define KEMOVIEW_GTK4_ROTATION_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "kemoview_gtk4_fileselector.h"
#include "kemoview_gtk4_routines.h"

#include "view_modifier_glfw.h"

struct rotation_gtk_menu{
    int id_fmt_rot;
    
    int i_FPS;
    int inc_deg;
    int iaxis_rot;
    
    GtkWidget *spin_rot_increment;
    GtkWidget *spin_rot_FPS;

    GtkWidget *combobox_rotation_fileformat;
    GtkWidget *rotView_Button;
    GtkWidget *rotSave_Button;
};


/*  prototypes */

struct rotation_gtk_menu * init_rotation_menu_box(void);
GtkWidget * init_rotation_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                        struct rotation_gtk_menu *rot_gmenu,
                                        GtkWidget *window);

#endif  /* KEMOVIEW_GTK4_ROTATION_MENU_ */
