/***********************************************************************
 *  kemoview_gtk4_Tracer_menu.h
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_TRACER_MENU_
#define KEMOVIEW_GTK4_TRACER_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_colorbar_switch.h"
#include "kemoview_gtk4_Fline_thickness.h"
#include "kemoview_gtk4_Tracer_increment.h"
#include "kemoview_gtk4_Tracer_ref_vector.h"
#include "kemoview_gtk4_color_menu.h"
#include "kemoview_gtk4_range_menu.h"

struct tracer_gtk_menu{
    int id_tracer_mode;
    
    GtkWidget *vector_switch;
    GtkWidget *vector_hbox;
    
    struct colorbar_sw_view          *tracer_cbar_sw_vws;
    
    struct line_thickness_menu       *thickness_vws;
    struct tracer_ref_vector_menu    *tracer_refvect_vws;
    struct tracer_vec_increment_menu *tracer_increment_vws;
    
    struct surface_range_view        *tracer_range_vws;
    struct psf_surface_color_view    *tracer_sf_color_vws;
    struct colormap_views            *tracer_color_vws;
    
    GtkWidget *tracer_menu_frame;
};


/*  prototypes */
struct tracer_gtk_menu * init_tracer_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                                               GtkWidget *tracerWin);
void set_tracer_menu_parameters(struct kemoviewer_gl_type *kemo_gl,
                                struct tracer_gtk_menu *tracer_gmenu);
void dealloc_tracer_menu_parameters(struct tracer_gtk_menu *tracer_gmenu);

#endif   /*  KEMOVIEW_GTK4_TRACER_MENU_  */
