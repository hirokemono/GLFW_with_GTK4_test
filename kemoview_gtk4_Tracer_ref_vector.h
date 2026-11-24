/***********************************************************************
 *  kemoview_gtk4_Tracer_ref_vector.h
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_TRACER_REF_VECTOR_
#define KEMOVIEW_GTK4_TRACER_REF_VECTOR_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_PSF_menu.h"

struct tracer_ref_vector_menu{
    GtkWidget *spin_ref_vect;
    GtkWidget *spin_ref_digit;
    GtkWidget *hbox_ref_vect;
};

/*  prototypes */
struct tracer_ref_vector_menu * init_tracer_ref_vector_hbox(struct kemoviewer_gl_type *kemo_gl);
void set_tracer_ref_vector_menu(struct kemoviewer_gl_type *kemo_gl,
                                struct tracer_ref_vector_menu *tracer_refvect_vws);
void dealloc_tracer_ref_vector_menu(struct tracer_ref_vector_menu *tracer_refvect_vws);

#endif   /*  KEMOVIEW_GTK4_TRACER_REF_VECTOR_  */
