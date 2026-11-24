/*****************************************************************************************
 *        kemoview_gtk4_color_menu.h
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/
#ifndef KEMOVIEW_GTK4_COLOR_MENU_
#define KEMOVIEW_GTK4_COLOR_MENU_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kemoviewer_gl.h"
#include "m_kemoviewer_data.h"

#include "view_modifier_glfw.h"

#include "calypso_GTK4.h"
#include "kemoview_cairo_colormap.h"
#include "kemoview_gtk4_routines.h"
#include "kemoview_gtk4_colormap_file_IO.h"
#include "kemoview_gtk4_colormode_menu.h"
#include "columnview_colormap.h"
#include "columnview_opacitymap.h"

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
    struct colormap_file_IO_views *colormap_IO;
    
/**    Expander for colormap menu  **/
    GtkWidget *expander_color;
};

/*  prototypes   */

void reset_colormap_menu_views(struct kemoviewer_gl_type *kemo_gl,
                               int iflag_current_model,
                               struct colormap_views *color_vws);
struct colormap_views * init_gtk_psf_colormap_expander(struct kemoviewer_gl_type *kemo_gl,
                                                       int *iflag_current_model,
                                                       GtkWidget *window);
void dealloc_colormap_views(struct colormap_views *color_vws);

#endif /*  KEMOVIEW_GTK4_COLOR_MENU_  */
