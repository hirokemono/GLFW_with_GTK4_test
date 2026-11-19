/*
 *  kemoview_gtk4_PSF_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_PSF_menu.h"


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

GtkWidget * init_colormap_draw_area(struct kemoviewer_gl_type *kemo_gl,
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

struct psf_gtk_menu * alloc_psf_gtk_menu(){
	struct psf_gtk_menu *psf_gmenu = (struct psf_gtk_menu *) malloc(sizeof(struct psf_gtk_menu));
    if (psf_gmenu == NULL) {
        printf("malloc error for psf_gtk_menu\n");
        exit(0);
    }
    
    psf_gmenu->psf_surface_menu = alloc_psf_surface_gtk_menu();
    /*
    psf_gmenu->psf_vector_menu =  (struct psf_vector_gtk_menu *) malloc(sizeof(struct psf_vector_gtk_menu));
    if (psf_gmenu->psf_vector_menu == NULL) {
        printf("malloc error for psf_gmenu->psf_vector_menu\n");
        exit(0);
    }
*/
    return psf_gmenu;
};

void dealloc_psf_gtk_menu(struct psf_gtk_menu *psf_gmenu){
    dealloc_colormap_views(psf_gmenu->color_vws);
//  free(psf_gmenu->psf_isoline_menu);
//  free(psf_gmenu->psf_surface_menu);
//  free(psf_gmenu->psf_vector_menu);
    free(psf_gmenu);
    return;
};

void set_vector_plot_availablity(int iflag_current_model,
                                 struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_gtk_menu *psf_gmenu){
    int if_psf = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              iflag_current_model,
                                              FIELD_SEL_FLAG);
    int ncomp = (int) kemoview_get_VIZ_num_component(kemo_gl->kemoview_data,
                                                     iflag_current_model, if_psf);
    if(ncomp == 3){
        gtk_widget_set_sensitive(psf_gmenu->expander_vect, TRUE);
    } else {
        gtk_widget_set_sensitive(psf_gmenu->expander_vect, FALSE);
    };
    return;
};

void init_psf_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                        struct psf_gtk_menu *psf_gmenu){
/*
    psf_gmenu->expander_iso = init_isoline_menu_expander(kemo_gl, psf_gmenu->psfWin,
                                                         psf_gmenu->psf_isoline_menu);
*/
    psf_gmenu->expander_surf = init_gtk4_psf_surface_menu_expander(kemo_gl, psf_gmenu->psfWin,
                                                                   psf_gmenu->psf_surface_menu);
    set_gtk4_surface_menu_values(kemo_gl, psf_gmenu->psf_surface_menu);
    
    psf_gmenu->color_vws = init_gtk_psf_colormap_expander(kemo_gl, SURFACE_RENDERING,
                                                          psf_gmenu->psfWin);
    
    /*    
    set_gtk_isoline_menu_values(kemo_gl, psf_gmenu->psf_isoline_menu);

    psf_gmenu->expander_vect = make_gtk_psf_vector_menu(kemo_gl, psf_gmenu->psfWin,
                                                        psf_gmenu->psf_vector_menu);
    set_gtk_psf_vector_menu(kemo_gl, psf_gmenu->psf_vector_menu);
*/
//    set_vector_plot_availablity(SURFACE_RENDERING, kemo_gl, psf_gmenu);
    return;
}

void append_psf_menus_to_hbox(GtkWidget *psf_vbox,
                              struct psf_gtk_menu *psf_gmenu){
//    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_iso);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_surf);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->color_vws->expander_color);
//    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_vect);
    return;
};
