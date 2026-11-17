/***********************************************************************
 *  columnview_colormap.h
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ************************************************************************/
#ifndef COLUMNVIEW_COLORMAP_
#define COLUMNVIEW_COLORMAP_

#include "kemoviewer_gl.h"
#include "kemoviewer.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "columnview_real2_gtk4.h"
#include "kemoview_gtk4_routines.h"

/* prototypes */
struct kemo_real2_columnview * init_colormap_list_scroll(struct kemoviewer_gl_type *kemo_gl,
                                                         int *iflag_current_model,
                                                         GtkWidget *psf_window);

#endif /* COLUMNVIEW_COLORMAP_ */
