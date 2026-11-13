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
#include "skip_comment_c.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

#include "view_modifier_glfw.h"

/*  prototypes */

void reset_psf_list_model(struct kemoviewer_gl_type *kemo_gl,
                          GtkStringList * psf_list_model);
int set_selected_psf_list_address(struct kemoviewer_gl_type *kemo_gl,
                                  GtkStringList *psf_list_model);
int find_selected_psf_id(struct kemoviewer_gl_type *kemo_gl,
                         char *selected_text);


void reset_psf_field_model(struct kemoviewer_gl_type *kemo_gl, int id_model, 
                           GtkStringList *psf_field_model);
int set_selected_psf_field_list(struct kemoviewer_gl_type *kemo_gl,
                                int id_model,
                                GtkStringList *psf_field_model);
int find_selected_field_id(struct kemoviewer_gl_type *kemo_gl, 
                           int id_model,
                           char *selected_text);


void reset_psf_component_model(struct kemoviewer_gl_type *kemo_gl, int id_model,
                               GtkStringList * psf_component_model);
int set_selected_psf_component_list(struct kemoviewer_gl_type *kemo_gl,
                                    int id_model,
                                    GtkStringList *psf_component_model);
int find_selected_component_id(struct kemoviewer_gl_type *kemo_gl, 
                               int id_model,
                               char *selected_text);

#endif /* KEMOVIEW_GTK4_PSF_SELECTORS_ */
