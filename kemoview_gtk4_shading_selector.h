/***********************************************************************
 *  kemoview_gtk4_shading_selector.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/

#ifndef KEMOVIEW_GTK4_SHADING_SELECTOR_
#define KEMOVIEW_GTK4_SHADING_SELECTOR_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"

/* protytypes */

GtkWidget * init_shading_mode_button(struct kemoviewer_gl_type *kemo_gl,
                                     int *iflag_shading);

#endif /* KEMOVIEW_GTK4_SHADING_SELECTOR_*/
