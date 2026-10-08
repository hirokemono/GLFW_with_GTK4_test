/****************************************************************
 *  kemoview_gtk4_routines.h
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ****************************************************************/
#ifndef KEMOVIEW_GTK4_ROUTINES_
#define KEMOVIEW_GTK4_ROUTINES_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calypso_GTK4.h"


/*  prototypes */

GtkWidget * wrap_into_frame_gtk4(const char *title, GtkWidget *box_in);
GtkWidget * wrap_into_scrollbox_gtk4(int width, int height, 
                                     GtkWidget *box_in);
GtkWidget * wrap_into_expanded_frame_gtk4(const char *title, GtkWidget *window,
                                          GtkWidget *box_in);
GtkWidget * wrap_into_scroll_expansion_gtk4(const char *title, 
                                            int width, int height,
                                            GtkWidget *window, 
                                            GtkWidget *box_in);

// int gtk4_selected_combobox_index(GtkComboBox *combobox);


#endif
