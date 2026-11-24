/***********************************************************************
 *  kemoview_gtk4_Tracer_increment.h
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_TRACER_INCREMENT_
#define KEMOVIEW_GTK4_TRACER_INCREMENT_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_PSF_menu.h"

struct tracer_vec_increment_menu{
    GtkWidget *spin_vect_inc;
    GtkWidget *spin_inc_digit;
    GtkWidget *hbox_vect_inc;
};

/*  prototypes */
struct tracer_vec_increment_menu * init_tracer_vec_increment_hbox(struct kemoviewer_gl_type *kemo_gl);
void set_tracer_vec_increment_menu(struct kemoviewer_gl_type *kemo_gl,
                                   struct tracer_vec_increment_menu *tracer_increment_vws);
void dealloc_tracer_vec_increment_menu(struct tracer_vec_increment_menu *tracer_increment_vws);

#endif   /*  KEMOVIEW_GTK4_TRACER_INCREMENT_  */
