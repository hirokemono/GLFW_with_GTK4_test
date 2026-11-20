/***********************************************************************
 *  kemoview_gtk4_Fline_window.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_FLINE_WINDOW_
#define KEMOVIEW_GTK4_FLINE_WINDOW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
// #include "kemoview_gtk4_fline_selectors.h"
// #include "kemoview_gtk4_fileselector.h"
#include "kemoview_gtk4_fieldline_menu.h"
#include "kemoview_gtk4_evolution_menu.h"

/* prototypes */

void init_fline_window(struct kemoviewer_gl_type *kemo_gl,
                       struct fieldline_gtk_menu *fline_gmenu,
                       GtkWidget *main_window, GtkWidget *itemTEvo);
void replace_fline_frame(struct kemoviewer_gl_type *kemo_gl,
                         struct fieldline_gtk_menu *fline_gmenu,
                         GtkWidget *itemTEvo);
 
void init_tracer_window(struct kemoviewer_gl_type *kemo_gl,
                        struct fieldline_gtk_menu *fline_gmenu,
                        GtkWidget *main_window, GtkWidget *itemTEvo);
void replace_tracer_frame(struct kemoviewer_gl_type *kemo_gl,
                          struct fieldline_gtk_menu *tracer_gmenu,
                          GtkWidget *itemTEvo);

#endif /*  KEMOVIEW_GTK4_FLINE_WINDOW_  */
