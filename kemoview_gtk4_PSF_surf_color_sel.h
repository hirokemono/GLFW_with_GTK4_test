/***********************************************************************
 *  kemoview_gtk4_PSF_surf_color_sel.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_SURF_COLOR_SEL_
#define KEMOVIEW_GTK4_PSF_SURF_COLOR_SEL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_color_select.h"

struct psf_surface_color_view{
	GtkWidget *listbutton_sfcolor;
    GtkWidget *psf_color_selector;
    GtkWidget *hbox_sf_color;
};


/*  prototypes */
void set_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_surface_color_view *gtk_psf_surface);
void set_gtk4_fline_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                   int id_color_mode,
                                   struct psf_surface_color_view *psf_sf_color_vws);

struct psf_surface_color_view * init_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                             int *id_current_model,
                                                             GtkWidget *parent_window);
void dealloc_psf_surf_color_views(struct psf_surface_color_view *psf_sf_color_vws);

#endif   /*  KEMOVIEW_GTK4_PSF_SURF_COLOR_SEL_  */
