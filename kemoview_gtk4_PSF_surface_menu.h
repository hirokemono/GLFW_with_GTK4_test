/***********************************************************************
 *  kemoview_gtk4_PSF_surface_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_SURFACE_MENU_
#define KEMOVIEW_GTK4_PSF_SURFACE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_colorsel.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_texture_file_IO.h"

struct psf_surface_gtk_menu{
    GtkWidget *switch_draw;
    GtkWidget *switch_bar;
	
	GtkWidget *listbutton_sfcolor;
    GtkWidget *psf_color_selector;
	
    GtkWidget *label_range_min;
    GtkWidget *label_range_max;
	GtkWidget *spin_opacity1;
    GtkWidget *spin_range_min;
    GtkWidget *spin_digit_min; 
    GtkWidget *spin_range_max;
    GtkWidget *spin_digit_max;
    
    GtkWidget *expander_surf;
};


/*  prototypes */
void set_gtk4_surface_menu_values(struct kemoviewer_gl_type *kemo_gl,
                                  struct psf_surface_gtk_menu *gtk_psf_surface);
struct psf_surface_gtk_menu * init_gtk4_psf_surface_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                                  GtkWidget *window);
void dealloc_psf_surface_gtk_menu(struct psf_surface_gtk_menu *psf_surface_menu);

#endif   /*  KEMOVIEW_GTK4_PSF_SURFACE_MENU_  */
