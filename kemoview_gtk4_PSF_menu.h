/***********************************************************************
 *  kemoview_gtk4_PSF_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_MENU_
#define KEMOVIEW_GTK4_PSF_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "m_kemoview_psf_menu.h"

#include "t_control_chara_IO.h"

#include "calypso_GTK4.h"
#include "columnview_real2_gtk4.h"
#include "kemoview_gtk4_color_menu.h"
#include "kemoview_gtk4_PSF_surface_menu.h"
#include "kemoview_gtk4_PSF_isoline_menu.h"
#include "kemoview_gtk4_PSF_vector_menu.h"

#include "view_modifier_glfw.h"

struct psf_gtk_menu{
    int iflag_PSF_model;
    
    struct psf_isoline_gtk_menu *psf_isoline_menu;
    struct psf_surface_gtk_menu *psf_surface_menu;
    struct psf_vector_gtk_menu *psf_vector_menu;
    
    struct colormap_views *color_vws;
};

/*  prototypes */
struct psf_gtk_menu * alloc_psf_gtk_menu(void);
void dealloc_psf_gtk_menu(struct psf_gtk_menu *psf_gmenu);

void reset_psf_menu_params(struct kemoviewer_gl_type *kemo_gl,
                           struct psf_gtk_menu *psf_gmenu);

GtkWidget * init_psf_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                               GtkWidget *psf_window,
                               struct psf_gtk_menu *psf_gmenu);

#endif   /*  KEMOVIEW_GTK4_PSF_MENU_ */
