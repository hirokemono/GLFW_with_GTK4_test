/***********************************************************************
 *  kemoview_gtk4_Fline_menu.h
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_FLINE_MENU_
#define KEMOVIEW_GTK4_FLINE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_colorbar_switch.h"
#include "kemoview_gtk4_Fline_thickness.h"
#include "kemoview_gtk4_Tracer_increment.h"
#include "kemoview_gtk4_Tracer_ref_vector.h"

struct fieldline_gtk_menu{
    int iflag_flinemode;
    
    GtkWidget *switch_tube;
    GtkWidget *fline_switch_bar;
    
    GtkWidget *switch_vect;
    
    GtkWidget *expander_fline_color;
    
    struct line_thickness_menu       *thickness_vws;
    struct tracer_ref_vector_menu    *tracer_refvect_vws;
    struct tracer_vec_increment_menu *tracer_increment_vws;
};


/*  prototypes */

void set_gtk_fieldline_menu(struct kemoviewer_gl_type *kemo_gl,
                            struct fieldline_gtk_menu *fline_gmenu);
void init_fieldline_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                              struct fieldline_gtk_menu *fline_gmenu);
GtkWidget * pack_fieldline_menu_frame(struct fieldline_gtk_menu *fline_gmenu);

void init_tracer_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                           struct fieldline_gtk_menu *fline_gmenu);
#endif   /*  KEMOVIEW_GTK4_FLINE_MENU_  */
