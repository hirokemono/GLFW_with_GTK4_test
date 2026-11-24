/***********************************************************************
 *  kemoview_gtk4_Fline_thickness.h
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_FLINE_THICKNESS_
#define KEMOVIEW_GTK4_FLINE_THICKNESS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_PSF_menu.h"

struct line_thickness_menu{
    GtkWidget *spin_thick;
    GtkWidget *spin_digit;
    GtkWidget *hbox_thickness;
};

/*  prototypes */
struct line_thickness_menu * init_line_thickness_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                      int *id_current_model);
void set_line_thickness_menu(struct kemoviewer_gl_type *kemo_gl,
                             int id_current_model,
                             struct line_thickness_menu *thickness_vws);
void dealloc_line_thickness_menu(struct line_thickness_menu *thickness_vws);

#endif   /*  KEMOVIEW_GTK4_FLINE_THICKNESS_  */
