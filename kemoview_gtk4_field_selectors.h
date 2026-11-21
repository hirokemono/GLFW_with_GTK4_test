/***********************************************************************
 *  kemoview_gtk4_field_selectors.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#ifndef KEMOVIEW_GTK4_FIELD_SELECTORS_
#define KEMOVIEW_GTK4_FIELD_SELECTORS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"
#include "m_kemoview_psf_menu.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_PSF_selectors.h"

/*  prototypes */

int kemoview_psf_select_CB(struct kemoviewer_gl_type *kemo_gl,
                           int id_model, GtkDropDown *dropdown);
int kemoview_field_select_CB(struct kemoviewer_gl_type *kemo_gl,
                             int id_model, GtkDropDown *dropdown);
int kemoview_component_select(struct kemoviewer_gl_type *kemo_gl,
                              int id_model, GtkDropDown *dropdown);

#endif /* KEMOVIEW_GTK4_FIELD_SELECTORS_ */
