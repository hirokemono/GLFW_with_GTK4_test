/***********************************************************************
 *  kemoview_gtk4_colorbar_switch.h
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_COLORBAR_SWITCH_
#define KEMOVIEW_GTK4_COLORBAR_SWITCH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"

struct colorbar_sw_view{
    GtkWidget *colorbar_switch;
    GtkWidget *cbar_sw_hbox;
};

/*  prototypes */

struct colorbar_sw_view * init_colorbar_switch_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                    int *id_current_model);
void set_colorbar_switch(struct kemoviewer_gl_type *kemo_gl,
                         int id_current_model,
                         struct colorbar_sw_view *colorbar_sw_vws);
void dealloc_colorbar_sw_view(struct colorbar_sw_view *colorbar_sw_vws);

#endif /*  KEMOVIEW_GTK4_COLORBAR_SWITCH_  */
