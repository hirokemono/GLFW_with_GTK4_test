/***********************************************************************
 *  kemoview_gtk4_PSF_surface_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_SURFACE_MENU_
#define KEMOVIEW_GTK4_PSF_SURFACE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_PSF_surf_color_sel.h"
#include "kemoview_gtk4_range_menu.h"

struct psf_surface_gtk_menu{
    GtkWidget *switch_draw;
    GtkWidget *switch_bar;
	
    GtkWidget *spin_opacity1;
    
    struct surface_range_view     *psf_range_vws;
    struct psf_surface_color_view *psf_sf_color_vws;
    
    GtkWidget *expander_surf;
};


/*  prototypes */
void set_gtk4_surface_menu_values(struct kemoviewer_gl_type *kemo_gl,
                                  struct psf_surface_gtk_menu *gtk_psf_surface);
struct psf_surface_gtk_menu * init_gtk4_psf_surface_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                                  GtkWidget *window);
void dealloc_psf_surface_gtk_menu(struct psf_surface_gtk_menu *psf_surface_menu);

#endif   /*  KEMOVIEW_GTK4_PSF_SURFACE_MENU_  */
