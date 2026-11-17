/***********************************************************************
 *  kemoview_cairo_colormap.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_CAIRO_COLORMAP_
#define KEMOVIEW_CAIRO_COLORMAP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "m_kemoview_psf_menu.h"

/*  prototypes */
gboolean expose_event_CB(cairo_t *cr, struct colormap_view *color_vws);

#endif   /*  KEMOVIEW_CAIRO_COLORMAP_ */
