/***********************************************************************
 *  kemoview_gtk4_fileselector.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ************************************************************************/
#ifndef KEMOVIEW_GTK4_FILESELECTOR_
#define KEMOVIEW_GTK4_FILESELECTOR_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer.h"
#include "kemoviewer_gl.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_PSF_window.h"
#include "kemoview_gtk4_main_menu.h"

/*  prototypes */
/* Routines for inout from console */
void open_window_for_opened_file(int iflag_datatype,
                                 struct kemoviewer_gl_type *kemo_gl,
                                 struct main_buttons *mbot,
                                 GtkWidget *main_window);


void kemoview_gtk4_read_file_select(GtkButton *button, GtkWindow *window,
                                    GtkEntryBuffer *entry_buf,
                                    GtkEntryBuffer *full_path_buf);
void kemoview_gtk4_save_image_select(struct kemoviewer_gl_type *kemo_gl, 
                                     GtkWindow *window);

#endif /* KEMOVIEW_GTK4_FILESELECTOR_ */
