/***********************************************************************
 *  kemoview_gtk4_preference_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PREFERENCE_MENU_
#define KEMOVIEW_GTK4_PREFERENCE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "calypso_GTK4.h"
#include "columnview_real3_gtk4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_shading_mode_menu.h"
#include "kemoview_gtk4_tube_pref_menu.h"
#include "kemoview_gtk4_coast_prefs_menu.h"
#include "kemoview_gtk4_axis_prefs_menu.h"
#include "kemoview_gtk4_performance_menu.h"
#include "kemoview_gtk4_image_format_selector.h"
#include "kemoview_gtk4_lighting_menu.h"

#include "view_modifier_glfw.h"

struct preference_view{
    struct kemo_real3_columnview *light_vws;
    int id_def_format;
    
    GtkWidget *pref_window;
};

/*  prototypes */
struct preference_view * init_preference_view(struct kemoviewer_type *kemoviewer_data);
void dealloc_preference_view(struct preference_view *pref_vws);

GtkWidget * init_preference_frame(struct kemoviewer_gl_type *kemo_gl,
                                  struct preference_view *pref_vws,
                                  GtkWidget *window, GLFWwindow *glfw_win);
GtkWidget * init_preference_scrollbox(struct kemoviewer_gl_type *kemo_gl,
                                      struct preference_view *pref_vws,
                                      GtkWidget *window, GLFWwindow *glfw_win);
GtkWidget * init_preference_expander(struct kemoviewer_gl_type *kemo_gl,
                                     struct preference_view *pref_vws,
                                     GtkWidget *window, GLFWwindow *glfw_win);

#endif   /*  KEMOVIEW_GTK4_PREFERENCE_MENU_  */
