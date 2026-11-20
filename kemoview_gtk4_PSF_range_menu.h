/***********************************************************************
 *  kemoview_gtk4_PSF_range_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_RANGE_MENU_
#define KEMOVIEW_GTK4_PSF_RANGE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_texture_file_IO.h"

struct psf_surface_range_view{
    GtkWidget *spin_opacity1;
    
    GtkWidget *label_range_min;
    GtkWidget *spin_range_min;
    GtkWidget *spin_digit_min; 
    
    GtkWidget *label_range_max;
    GtkWidget *spin_range_max;
    GtkWidget *spin_digit_max;
    
    GtkWidget *hbox_psf_range;
};


/*  prototypes */
void set_gtk4_surf_range_values(struct kemoviewer_gl_type *kemo_gl,
                                struct psf_surface_range_view *psf_range_vws);
struct psf_surface_range_view * init_gtk4_psf_surf_range_view(struct kemoviewer_gl_type *kemo_gl,
                                                              GtkWidget *window);
void dealloc_psf_surf_range_view(struct psf_surface_range_view *psf_range_vws);

#endif   /*  KEMOVIEW_GTK4_PSF_RANGE_MENU_  */
