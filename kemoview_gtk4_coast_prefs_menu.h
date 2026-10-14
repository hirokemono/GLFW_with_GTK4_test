/***********************************************************************
 *  kemoview_gtk4_coast_prefs_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/

#ifndef KEMOVIEW_GTK4_COAST_PREFS_MENU_
#define KEMOVIEW_GTK4_COAST_PREFS_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_axis_posi_sel.h"

/* protytypes */

GtkWidget * init_coastline_pref_menu(struct kemoviewer_gl_type *kemo_gl);

#endif /* KEMOVIEW_GTK4_COAST_PREFS_MENU_ */
