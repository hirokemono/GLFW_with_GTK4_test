/***********************************************************************
 *  kemoview_gtk4_color_select.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_COLOR_SELECT_
#define KEMOVIEW_GTK4_COLOR_SELECT_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_texture_file_IO.h"

/* prototypes */
void init_psf_surf_colmode_dropdown(int id_color_mode,
                                    GtkWidget *color_mode_button);
void init_fline_colmode_dropdown(int id_color_mode,
                                 GtkWidget *color_mode_button);

int find_surface_color_mode(const char *text);
int find_fline_color_mode(const char *text);

void set_psf_surface_color_mode(int index_mode, GtkWidget *parent_window,
                                GtkWidget *color_selector,
                                struct kemoviewer_gl_type *kemo_gl);
void set_fline_surface_color_mode(int index_mode, GtkWidget *parent_window,
                                  GtkWidget *color_selector,
                                  struct kemoviewer_gl_type *kemo_gl,
                                  int id_current_model);

GtkWidget * init_PSF_sfcolor_listbutton(void);
GtkWidget * init_Fline_color_listbutton(void);

#endif /*  KEMOVIEW_GTK4_COLOR_SELECT_  */
