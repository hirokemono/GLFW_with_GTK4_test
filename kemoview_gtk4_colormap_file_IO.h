/*****************************************************************************************
 *        kemoview_gtk4_colormap_file_IO.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_COLORMAP_FILE_IO_
#define KEMOVIEW_GTK4_COLORMAP_FILE_IO_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_fileselector.h"
#include "kemoview_gtk4_colormode_menu.h"
#include "columnview_colormap.h"
#include "columnview_opacitymap.h"

struct colormap_file_IO_widgets{
/**    Dummy buffer to send required pointers  **/
    GtkEntryBuffer *entry_buf_colormap_file;
/**    Save button  **/
    GtkWidget *saveColormap_Button;
/**    Load button  **/
    GtkWidget *loadColormap_Button;
    
    /**  Box for IO buttons **/
    GtkWidget *saveButton_box;
};

struct colormap_views{
/**    Draw area for colorbar example  **/
    GtkWidget *colormap_area;
/**    Draw area for color mode menu  **/
    struct colormode_menu *colormode_vws;
/**    Structure for coloemap edit menu  **/
    struct kemo_real2_columnview *colormap_vws;
/**    Structure for opacity edit menu  **/
    struct kemo_real2_columnview *opacitymap_vws;
/**    Structure for colormap file IO menu  **/
    struct colormap_file_IO_widgets *colormap_IO;
    
/**    Expander for colormap menu  **/
    GtkWidget *expander_color;
};

/*  prototypes   */
struct colormap_file_IO_widgets * alloc_colormap_IO_widgets(void);
void dealloc_colormap_IO_widgets(struct colormap_file_IO_widgets *colormap_IO);

GtkWidget * init_colormap_save_box(struct kemoviewer_gl_type *kemo_gl,
                                   int *iflag_current_model,
                                   struct colormode_menu *colormode_vws,
                                   struct kemo_real2_columnview *colormap_vws,
                                   struct kemo_real2_columnview *opacitymap_vws,
                                   struct colormap_file_IO_widgets *colormap_IO,
                                   GtkWidget *window);

#endif /*  KEMOVIEW_GTK4_COLORMAP_FILE_IO_  */
