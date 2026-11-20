/***********************************************************************
 *  kemoview_gtk4_fieldline_menu.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_FIELDLINE_MENU_
#define KEMOVIEW_GTK4_FIELDLINE_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"
#include "kemoviewer_gl.h"
#include "kemoview_gtk4_range_menu.h"
#include "kemoview_gtk4_fline_color_sel.h"
#include "kemoview_gtk4_color_menu.h"
// #include "kemoview_gtk_fline_selectors.h"
// #include "kemoview_gtk_PSF_menu.h"

#include "view_modifier_glfw.h"

struct fieldline_gtk_menu{
    int iflag_flinemode;
    
    int iflag_flineBox;
    GtkWidget *flineWin;
    
    GtkWidget *closeButton;
    GtkWidget *switch_tube;
    GtkWidget *fline_switch_bar;
    
    GtkWidget *spin_thick, *spin_digit;
    
    GtkWidget *combobox_field;
    GtkWidget *label_tree_field;
    
    GtkWidget *combobox_comp;
    
    GtkWidget *expander_fline_color;
    
    GtkWidget *fline_frame;
    
    GtkWidget *switch_vect;
    GtkWidget *spin_vect_inc;
    GtkWidget *spin_inc_digit;
    GtkWidget *spin_ref_vect;
    GtkWidget *spin_ref_digit;
    
    struct surface_range_view     *fline_range_vws;
    struct fline_color_sel_view   *fline_color_sel_vws;
    struct colormap_views         *fline_color_vws;
};


/*  prototypes */

void set_gtk_fieldline_menu(struct kemoviewer_gl_type *kemo_gl,
                            struct fieldline_gtk_menu *fline_gmenu);
void init_fieldline_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                              struct fieldline_gtk_menu *fline_gmenu,
                              GtkWidget *parent_window);
GtkWidget * pack_fieldline_menu_frame(struct fieldline_gtk_menu *fline_gmenu);

void init_tracer_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                           struct fieldline_gtk_menu *fline_gmenu,
                           GtkWidget *parent_window);
GtkWidget * pack_tracer_menu_frame(struct fieldline_gtk_menu *fline_gmenu);

struct fieldline_gtk_menu * alloc_gtk_fline_menu();
void dealloc_gtk_fline_menu(struct fieldline_gtk_menu *fline_gmenu);


#endif   /*  KEMOVIEW_GTK4_FIELDLINE_MENU_  */
