/*****************************************************************************************
 *        kemoview_gtk4_texture_file_IO.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_TEXTURE_FILE_IO_
#define KEMOVIEW_GTK4_TEXTURE_FILE_IO_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"

/*  prototypes */
void open_texture_load_dialog(struct kemoviewer_gl_type *kemo_gl, GtkWindow *window);

#endif /*  KEMOVIEW_GTK4_TEXTURE_FILE_IO_  */