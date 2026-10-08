/***********************************************************************
 *  kemoview_gtk4_evolution_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_EVOLUTION_MENU_
#define KEMOVIEW_GTK4_EVOLUTION_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "calypso_GTK4.h"
// #include "tree_view_chara_int_GTK.h"
// #include "tree_views_4_fixed_lists_GTK.h"
#include "kemoview_gtk4_image_format_selector.h"
#include "kemoview_gtk4_fileselector.h"
#include "kemoview_gtk4_routines.h"

#include "view_modifier_glfw.h"

struct evolution_gtk_menu{
    int id_fmt_evo;
    
    int istart_evo;
    int iend_evo;
    int inc_evo;
    int i_FPS;
};

/*  prototypes */

struct evolution_gtk_menu * init_evoluaiton_menu_box(struct kemoviewer_type *kemo_sgl);

GtkWidget * init_evolution_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                         struct evolution_gtk_menu *evo_gmenu,
                                         GtkWidget *window);

void activate_evolution_menu(struct kemoviewer_type *kemo_sgl,
                             GtkWidget *evo_widget);

#endif   /* KEMOVIEW_GTK4_EVOLUTION_MENU_ */
