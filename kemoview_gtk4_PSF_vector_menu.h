/***********************************************************************
 *  kemoview_gtk4_PSF_vector_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_VECTOR_MENU_
#define KEMOVIEW_GTK4_PSF_VECTOR_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_PSF_vec_mode_sel.h"
#include "kemoview_gtk4_PSF_vec_color_sel.h"
#include "kemoview_gtk4_PSF_vec_param_menu.h"

struct psf_vector_gtk_menu{
	GtkWidget *switch_vect;
	
    struct psf_vector_mode_view  *vector_mode_vws;
    struct psf_vector_color_view *vector_color_vws;
    struct psf_vector_param_views *psf_vec_vws;
    
    GtkWidget *expander_vect;
};


/*  prototypes */

void set_vector_plot_availablity(int iflag_current_model,
                                 struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_vector_gtk_menu *psf_vector_menu);
void set_gtk_psf_vector_menu(struct kemoviewer_gl_type *kemo_gl,
                             struct psf_vector_gtk_menu *psf_vector_menu);

struct psf_vector_gtk_menu * init_gtk_psf_vector_menu(struct kemoviewer_gl_type *kemo_gl,
                                                      GtkWidget *window);
void deallc_psf_vector_gtk_menu(struct psf_vector_gtk_menu *psf_vector_menu);

#endif  /*  KEMOVIEW_GTK4_PSF_VECTOR_MENU_  */
