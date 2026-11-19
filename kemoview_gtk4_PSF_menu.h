/***********************************************************************
 *  kemoview_gtk4_PSF_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_MENU_
#define KEMOVIEW_GTK4_PSF_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "m_kemoview_psf_menu.h"

#include "t_control_chara_IO.h"

#include "columnview_real2_gtk4.h"
#include "columnview_colormap.h"
#include "columnview_opacitymap.h"
#include "kemoview_cairo_colormap.h"
#include "kemoview_gtk4_colormode_menu.h"
#include "kemoview_gtk4_colormap_file_IO.h"
#include "kemoview_gtk4_fileselector.h"
#include "kemoview_gtk4_PSF_surface_menu.h"
//#include "kemoview_gtk_PSF_isoline_menu.h"
//#include "kemoview_gtk_PSF_vector_menu.h"

#include "view_modifier_glfw.h"

struct psf_gtk_menu{
    int iflag_psfBox;
    GtkWidget *psfWin;

    GtkWidget *psf_frame;

    GtkWidget *closeButton;
    
    GtkWidget *hbox_field;
    GtkWidget *combobox_field;
    GtkStringList *psf_field_model;
    
    GtkWidget *hbox_component;
    GtkWidget *combobox_comp;
    GtkStringList *psf_component_model;
    
    int num_psfs;
    GtkWidget *hbox_psfs;
    GtkWidget *combobox_psfs;
    GtkStringList *psf_list_model;

//    struct psf_isoline_gtk_menu *psf_isoline_menu;
    GtkWidget *expander_iso;
    
    struct psf_surface_gtk_menu *psf_surface_menu;
    GtkWidget *expander_surf;

//  struct psf_vector_gtk_menu *psf_vector_menu;
    GtkWidget *expander_vect;
    
    struct colormap_views *color_vws;
};

/*  prototypes */
struct psf_gtk_menu * alloc_psf_gtk_menu(void);
void dealloc_psf_gtk_menu(struct psf_gtk_menu *psf_gmenu);

void set_vector_plot_availablity(int iflag_current_model,
                                 struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_gtk_menu *psf_gmenu);
void init_psf_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                        struct psf_gtk_menu *psf_gmenu);
void append_psf_menus_to_hbox(GtkWidget *psf_vbox,
                              struct psf_gtk_menu *psf_gmenu);

#endif   /*  KEMOVIEW_GTK4_PSF_MENU_ */
