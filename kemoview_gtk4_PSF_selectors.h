/***********************************************************************
 *  kemoview_gtk4_PSF_selectors.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/
#ifndef KEMOVIEW_GTK4_PSF_SELECTORS_
#define KEMOVIEW_GTK4_PSF_SELECTORS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

#include "view_modifier_glfw.h"

/*  prototypes */

void kemoview_psf_select_CB(GtkComboBox *combobox_psfs, int id_model,
                            struct kemoviewer_gl_type *kemo_gl);
void kemoview_field_select_CB(GtkComboBox *combobox_field, int id_model,
                              struct kemoviewer_gl_type *kemo_gl);
void kemoview_component_select_CB(GtkComboBox *combobox_comp, int id_model,
                                  struct kemoviewer_gl_type *kemo_gl);

GtkWidget * draw_current_psf_set_hbox(int id_current_psf,
                                      struct kemoviewer_gl_type *kemo_gl,
                                      int *index);
GtkWidget * draw_viz_field_gtk_box(struct kemoviewer_gl_type *kemo_gl, int id_model);
GtkWidget * draw_viz_component_gtk_box(struct kemoviewer_gl_type *kemo_gl, int id_model);

GtkStringList * init_psf_list_model(struct kemoviewer_gl_type *kemo_gl,
                                    int *index_current);


#endif /* KEMOVIEW_GTK4_PSF_SELECTORS_ */
