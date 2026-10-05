/*
 *  kemoview_gtk4_fileselector.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */
#ifndef KEMOVIEW_GTK4_FILESELECTOR_
#define KEMOVIEW_GTK4_FILESELECTOR_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"

#include "kemoviewer.h"

/*  prototypes */
/* Routines for inout from console */

void kemoview_gtk4_read_file_select(GtkButton *button, GtkWindow *window,
                                    GtkEntryBuffer *entry_buf,
                                    GtkEntryBuffer *full_path_buf);
void kemoview_gtk4_save_image_select(GtkButton *button, gpointer data);
void kemoview_gtk4_save_file_select(GtkButton *button, gpointer data);

struct kv_string * kemoview_read_file_panel(GtkWidget *window_cmap);
struct kv_string * kemoview_save_file_panel(GtkWidget *window_cmap);

#endif /* KEMOVIEW_GTK4_FILESELECTOR_ */
