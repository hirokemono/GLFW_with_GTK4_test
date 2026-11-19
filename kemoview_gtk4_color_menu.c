/*****************************************************************************************
 *        kemoview_gtk4_color_menu.c
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/

#include "kemoview_gtk4_color_menu.h"

static struct colormap_views * alloc_colormap_views(int iflag_model){
    struct colormap_views *color_vws;
    color_vws = (struct colormap_views *) malloc(sizeof(struct colormap_views));
    if (color_vws == NULL) {
        printf("malloc error for colormap_views\n");
        exit(0);
    }
    color_vws->iflag_current_model = iflag_model;
    return color_vws;
};

static void kemoview_draw_colorabar_CB(GtkDrawingArea *colormap_area, cairo_t *cr, 
                                       int width, int height, 
                                       gpointer user_data)
{ 
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    draw_cairo_colormap(colormap_area, cr, kemo_gl->kemoview_data, *iflag_current_model);
    return;
}

static GtkWidget * init_colormap_draw_area(struct kemoviewer_gl_type *kemo_gl,
                                           int *iflag_current_model){
    GtkWidget *colormap_area = gtk_drawing_area_new();
    
    GtkWidget *dummy_enty = gtk_entry_new();
    g_object_set_data(G_OBJECT(dummy_enty), "kemoview_gl",
                      (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(dummy_enty), "current_model",
                      (gpointer) iflag_current_model);
    
    gtk_drawing_area_set_content_width (GTK_DRAWING_AREA(colormap_area), 320);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(colormap_area), 280);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(colormap_area),
                                   kemoview_draw_colorabar_CB,
                                   dummy_enty, NULL);
    return colormap_area;
}



void dealloc_colormap_views(struct colormap_views *color_vws){
    dealloc_colormode_menu(color_vws->colormode_vws);
    dealloc_kemo_real2_columnview(color_vws->colormap_vws);
    dealloc_kemo_real2_columnview(color_vws->opacitymap_vws);
    dealloc_colormap_IO_widgets(color_vws->colormap_IO);
    g_object_unref(G_OBJECT(color_vws->colormap_area));
    free(color_vws);
    return;
};

struct colormap_views * init_gtk_psf_colormap_expander(struct kemoviewer_gl_type *kemo_gl,
                                                       int iflag_model, GtkWidget *window){
    struct colormap_views *color_vws = alloc_colormap_views(iflag_model);
    
    color_vws->colormap_area = init_colormap_draw_area(kemo_gl, &color_vws->iflag_current_model);
    color_vws->colormap_vws =  init_colormap_list_scroll(kemo_gl, 
                                                         &color_vws->iflag_current_model, 
                                                         color_vws->colormap_area, window);
    color_vws->colormode_vws =  make_gtk4_colormode_menu_box(kemo_gl, 
                                                             &color_vws->iflag_current_model,
                                                             color_vws->colormap_area);
    color_vws->opacitymap_vws = init_opacitymap_list_scroll(kemo_gl, 
                                                            &color_vws->iflag_current_model, 
                                                            color_vws->colormap_area, window);
    color_vws->colormap_IO = init_colormap_save_box(kemo_gl,
                                                    &color_vws->iflag_current_model,
                                                    color_vws->colormap_area,
                                                    color_vws->colormode_vws, 
                                                    color_vws->colormap_vws, 
                                                    color_vws->opacitymap_vws,
                                                    window);
    
    GtkWidget *color_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(color_box), color_vws->colormap_area);
    gtk_box_append(GTK_BOX(color_box), color_vws->colormode_vws->hbox_colormode);
    gtk_box_append(GTK_BOX(color_box), color_vws->colormap_vws->real2_cview_scroll);
    gtk_box_append(GTK_BOX(color_box), color_vws->opacitymap_vws->real2_cview_scroll);
    gtk_box_append(GTK_BOX(color_box), color_vws->colormap_IO->saveButton_box);
    
    color_vws->expander_color = wrap_into_scroll_expansion_gtk4("Color map editor", 420, 450,
                                                                window, color_box);
    return color_vws;
}

