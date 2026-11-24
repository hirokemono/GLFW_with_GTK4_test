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
#include "kemoview_gtk4_color_menu.h"
#include "kemoview_gtk4_range_menu.h"

struct fieldline_gtk_menu{
    int id_fline_mode;
    
    GtkWidget *switch_tube;
    GtkWidget *hbox_fline_tube;
    
    struct colorbar_sw_view    *fline_cbar_sw_vws;
    struct line_thickness_menu *thickness_vws;
    
    struct surface_range_view     *fline_range_vws;
    struct psf_surface_color_view *fline_sf_color_vws;
    struct colormap_views         *fline_color_vws;
    
    GtkWidget *fline_color_frame;
};


/*  prototypes */
struct fieldline_gtk_menu * init_fieldline_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                     GtkWidget *flineWin);
void set_fieldline_menu_parameters(struct kemoviewer_gl_type *kemo_gl,
                                   struct fieldline_gtk_menu *fline_gmenu);
void dealloc_fieldline_gtk_menu(struct fieldline_gtk_menu *fline_gmenu);


#endif   /*  KEMOVIEW_GTK4_FLINE_MENU_  */
