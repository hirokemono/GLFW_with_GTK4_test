/***********************************************************************
 *  kemoview_gtk4_PSF_window.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#ifndef KEMOVIEW_GTK4_PSF_WINDOW_
#define KEMOVIEW_GTK4_PSF_WINDOW_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_PSF_selectors.h"
#include "kemoview_gtk4_PSF_menu.h"
#include "kemoview_gtk4_evolution_menu.h"
#include "kemoview_gtk4_field_selectors.h"

#include "view_modifier_glfw.h"

struct psf_gtk_window_view{
    int iflag_psfBox;
    int num_psfs;
    
    GtkWidget *closeButton;
    
    GtkWidget *psf_frame;
    GtkWidget *psfWin;
    
    struct psf_gtk_menu *psf_gmenu;
    
    GtkWidget *combobox_field;
    GtkStringList *psf_field_model;
    
    GtkWidget *combobox_comp;
    GtkStringList *psf_component_model;
    
    GtkWidget *combobox_psfs;
    GtkStringList *psf_list_model;
};

/*  prototypes */
void init_psf_window(struct kemoviewer_gl_type *kemo_gl,
                     struct psf_gtk_window_view *psf_win_vws,
                     GtkWidget *main_window, GtkWidget *itemTEvo);

struct psf_gtk_window_view * alloc_psf_gtk_window_view(void);
void dealloc_psf_gtk_window(struct psf_gtk_window_view *psf_win_vws);

#endif /*  KEMOVIEW_GTK_PSF4_WINDOW_ */
