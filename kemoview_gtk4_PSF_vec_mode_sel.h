/*****************************************************************************************
 *        kemoview_gtk4_PSF_vec_mode_sel.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_VEC_MODE_SEL_
#define KEMOVIEW_GTK4_PSF_VEC_MODE_SEL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

struct psf_vector_mode_view{
    GtkWidget *vector_mode_button;
    GtkWidget *hbox_vec_mode;
};

/* protypes */
void set_psf_vector_mode_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                  struct psf_vector_mode_view *vector_mode_vws);
struct psf_vector_mode_view * init_psf_vector_mode_views(struct kemoviewer_gl_type *kemo_gl);
void dealloc_psf_vector_mode_view(struct psf_vector_mode_view *vector_mode_vws);

#endif  /*  KEMOVIEW_GTK4_PSF_VEC_MODE_SEL_  */