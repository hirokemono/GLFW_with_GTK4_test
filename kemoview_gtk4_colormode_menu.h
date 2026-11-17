/***********************************************************************
 *  kemoview_gtk4_colormode_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#ifndef KEMOVIEW_GTK4_COLORMODE_MENU_
#define KEMOVIEW_GTK4_COLORMODE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "skip_comment_c.h"
#include "set_rgba_table_c.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

/* prototypes */

GtkWidget * make_gtk4_colormode_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                         int *iflag_current_model);

#endif /*  KEMOVIEW_GTK4_COLORMODE_MENU_  */
