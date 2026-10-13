/***********************************************************************
 *  kemoview_gtk4_pref_window.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PREFERENCE_WINDOW_
#define KEMOVIEW_GTK4_PREFERENCE_WINDOW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_light_menu.h"
#include "kemoview_gtk4_preference_menu.h"

struct preference_view{
    struct lightparams_view *light_vws;
    
    GtkWidget *pref_window;
};

/*  prototypes */

struct preference_view * init_preference_view(struct kemoviewer_type *kemoviewer_data);
void dealloc_preference_view(struct preference_view *pref_vws);

void init_preference_window(struct kemoviewer_gl_type *kemo_gl,
                            struct preference_view *pref_vws,
                            GtkWidget *main_window);

#endif   /*  KEMOVIEW_GTK4_PREFERENCE_MENU_  */
