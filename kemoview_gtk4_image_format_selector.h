/***********************************************************************
 *  kemoview_gtk4_image_format_selector.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_IMAGE_FORMAT_SELECTOR_
#define KEMOVIEW_GTK4_IMAGE_FORMAT_SELECTOR_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"

#include "calypso_GTK4.h"

/*  prototypes */

GtkWidget * init_image_format_hbox(int *id_image_format);
GtkWidget * init_movie_FPS_hbox(int *i_FPS);

#endif  /* KEMOVIEW_GTK4_IMAGE_FORMAT_SELECTOR_ */
