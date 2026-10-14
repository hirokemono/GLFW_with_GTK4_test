/***********************************************************************
 *  kemoview_gtk4_shading_mode_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_SHADING_MODE_MENU_
#define KEMOVIEW_GTK4_SHADING_MODE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_shading_selector.h"
#include "kemoview_gtk4_side_selector.h"

GtkWidget * shading_mode_menu_frame(struct kemoviewer_gl_type *kemo_gl);

#endif /*KEMOVIEW_GTK4_SHADING_MODE_MENU_ */
