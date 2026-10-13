/*
 *  kemoview_gtk4_performance_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */
#ifndef KEMOVIEW_GTK4_PERFORMANCE_MENU_
#define KEMOVIEW_GTK4_PERFORMANCE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"


/*  prototypes */

GtkWidget * init_num_threads_menu_frame(struct kemoviewer_gl_type *kemo_gl);
GtkWidget * init_FPS_test_menu_frame(struct kemoviewer_gl_type *kemo_gl,
                                     GtkWidget *window);

#endif /*  KEMOVIEW_GTK4_PERFORMANCE_MENU_  */
