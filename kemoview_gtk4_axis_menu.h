/***********************************************************************
 *  kemoview_gtk4_axis_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_AXIS_MENU_
#define KEMOVIEW_GTK4_AXIS_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_fileselector.h"
// #include "tree_views_4_fixed_lists_GTK.h"
// #include "tree_view_chara_int_GTK.h"

#include "view_modifier_glfw.h"

/*  prototypes */

GtkWidget * make_axis_menu_box(struct kemoviewer_gl_type *kemo_gl,
                               GtkWidget *window);

#endif  /* KEMOVIEW_GTK4_AXIS_MENU_ */ 
