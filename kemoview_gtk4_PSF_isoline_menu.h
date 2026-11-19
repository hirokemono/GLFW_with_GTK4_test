/***********************************************************************
 *  kemoview_gtk4_PSF_isoline_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_ISOLINE_MENU_
#define KEMOVIEW_GTK4_PSF_ISOLINE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_isoline_color_sel.h"

struct psf_isoline_switches{
    GtkWidget *switch_1;
    GtkWidget *hbox_draw;
    
    GtkWidget *switch_zero;
    GtkWidget *hbox_zero;
};

struct psf_isoline_param_menu{
    GtkWidget *spin_nline;
    GtkWidget *spin_width;
    GtkWidget *spin_digit;
    
    GtkWidget *hbox_nline;
    GtkWidget *hbox_width;
};


struct psf_isoline_gtk_menu{
    struct psf_isoline_switches   *isoline_sw;
    struct psf_isoline_param_menu *isoline_param_vws;
    struct isoline_color_view     *isoline_color_vws;
    
    GtkWidget *expander_iso;
};


/*  prototypes */
void set_gtk_isoline_menu_values(struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_isoline_gtk_menu *psf_isoline_menu);
struct psf_isoline_gtk_menu * init_isoline_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                         GtkWidget *window);
void dealloc_psf_isoline_menu(struct psf_isoline_gtk_menu *psf_isoline_menu);

#endif   /*  KEMOVIEW_GTK4_PSF_ISOLINE_MENU_  */
