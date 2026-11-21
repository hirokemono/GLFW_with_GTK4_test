/***********************************************************************
 *  kemoview_gtk4_Tracer_window.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_TRACER_WINDOW_
#define KEMOVIEW_GTK4_TRACER_WINDOW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "calypso_GTK4.h"
// #include "kemoview_gtk4_fline_selectors.h"
// #include "kemoview_gtk4_fileselector.h"
// #include "kemoview_gtk4_fieldline_menu.h"
#include "kemoview_gtk4_PSF_selectors.h"
#include "kemoview_gtk4_field_selectors.h"
#include "kemoview_gtk4_evolution_menu.h"

struct tracer_gtk_window_view{
    int iflag_tracerBox;
    
    GtkWidget *closeButton;
    
    GtkWidget *tracer_field_model;
    GtkWidget *combobox_field;
    
    GtkWidget *tracer_component_model;
    GtkWidget *combobox_comp;
    
    GtkWidget *tracer_frame;
    GtkWidget *tracerWin;
};

/* prototypes */

void replace_tracer_frame(struct kemoviewer_gl_type *kemo_gl,
                          struct tracer_gtk_window_view *tracer_win_vws,
//                          struct fieldline_gtk_menu *fline_gmenu,
                          GtkWidget *itemTEvo);

void init_tracer_window(struct kemoviewer_gl_type *kemo_gl,
                        struct tracer_gtk_window_view *tracer_win_vws,
//                        struct fieldline_gtk_menu *fline_gmenu,
                        GtkWidget *main_window, GtkWidget *itemTEvo);

struct tracer_gtk_window_view * alloc_tracer_gtk_window_view(void);
void dealloc_tracer_gtk_window(struct tracer_gtk_window_view *tracer_win_vws);

#endif /*  KEMOVIEW_GTK4_TRACER_WINDOW_  */
