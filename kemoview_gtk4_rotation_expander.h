/***********************************************************************
 *  kemoview_gtk4_rotation_expander.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_ROTATION_EXPANDER_
#define KEMOVIEW_GTK4_ROTATION_EXPANDER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer.h"
#include "kemoviewer_gl.h"

#include "kemoview_gtk4_image_format_selector.h"
#include "kemoview_gtk4_rotation_menu.h"

/*  prototypes */

GtkWidget * init_rotation_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                        struct rotation_gtk_menu *rot_gmenu,
                                        GtkWidget *window);

#endif  /* KEMOVIEW_GTK4_ROTATION_EXPANDER_ */

