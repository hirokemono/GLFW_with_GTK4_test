/***********************************************************************
 *  kemoview_gtk4_PSF_vec_param_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_VEC_PARAM_MENU_
#define KEMOVIEW_GTK4_PSF_VEC_PARAM_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

struct psf_vector_param_views{
    GtkWidget *spin_ref_vect;
    GtkWidget *spin_ref_digit;
    GtkWidget *spin_vect_width;
    GtkWidget *spin_width_digit;
    GtkWidget *spin_vect_inc;
    GtkWidget *spin_inc_digit;
    
    GtkWidget *hbox_ref_vect;
    GtkWidget *hbox_vect_width;
    GtkWidget *hbox_vect_inc;
};


/*  prototypes */

void set_psf_vector_param_menus(struct kemoviewer_gl_type *kemo_gl,
                                struct psf_vector_param_views *psf_vec_vws);
struct psf_vector_param_views * init_psf_vector_param_menus(struct kemoviewer_gl_type *kemo_gl);
void deallc_psf_vector_param_views(struct psf_vector_param_views *psf_vec_vws);

#endif  /*  KEMOVIEW_GTK4_PSF_VEC_PARAM_MENU_  */
