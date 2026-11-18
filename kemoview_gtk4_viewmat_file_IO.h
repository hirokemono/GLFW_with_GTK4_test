/*****************************************************************************************
 *        kemoview_gtk4_viewmat_file_IO.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_VIEWMAT_FILE_IO_
#define KEMOVIEW_GTK4_VIEWMAT_FILE_IO_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_fileselector.h"

struct viewmat_file_IO_widgets{
    GtkEntryBuffer *entry_buf_viewmat_file;
    GtkWidget *saveView_Button;
    GtkWidget *loadView_Button;
};

/*  prototypes */
struct viewmat_file_IO_widgets * init_viewmat_IO_widgets(void);
void dealloc_viewmat_IO_widgets(struct viewmat_file_IO_widgets *viewmat_IO);

GtkWidget * init_viewmatrix_save_box(struct kemoviewer_gl_type *kemo_gl,
                                     struct viewmat_file_IO_widgets *viewmat_IO,
                                     GtkWidget *window);

#endif /*  KEMOVIEW_GTK4_VIEWMAT_FILE_IO_  */
