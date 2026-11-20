/***********************************************************************
 *  kemoview_gtk4_fline_color_sel.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_FLINE_COLOR_SEL_
#define KEMOVIEW_GTK4_FLINE_COLOR_SEL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_routines.h"

struct fline_color_sel_view{
	GtkWidget *button_fline_color;
    GtkWidget *fline_color_selector;
    GtkWidget *hbox_fline_color;
};


/*  prototypes */
void set_gtk4_fline_color_sel(struct kemoviewer_gl_type *kemo_gl,
                              int id_current_model,
                              struct fline_color_sel_view *fline_color_sel_vws);
struct fline_color_sel_view * init_gtk4_fline_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                        int *id_current_model, 
                                                        GtkWidget *parent_window);
void dealloc_fline_color_views(struct fline_color_sel_view *fline_color_sel_vws);

#endif   /*  KEMOVIEW_GTK4_FLINE_COLOR_SEL_  */
