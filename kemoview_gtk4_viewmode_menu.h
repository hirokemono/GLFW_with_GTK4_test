/*
 *  kemoview_gtk4_viewmode_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */
#ifndef KEMOVIEW_GTK4_VIEWMODE_MENU_
#define KEMOVIEW_GTK4_VIEWMODE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "skip_comment_c.h"
#include "kemoview_gtk4_viewmatrix_menu.h"

#include "view_modifier_glfw.h"

/* prototypes */

GtkWidget * make_gtk4_viewmode_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                        struct view_widgets *view_menu);
#endif /*  KEMOVIEW_GTK4_VIEWMODE_MENU_  */
