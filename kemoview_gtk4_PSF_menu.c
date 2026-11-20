/***********************************************************************
 *  kemoview_gtk4_PSF_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_menu.h"



struct psf_gtk_menu * alloc_psf_gtk_menu(){
	struct psf_gtk_menu *psf_gmenu = (struct psf_gtk_menu *) malloc(sizeof(struct psf_gtk_menu));
    if (psf_gmenu == NULL) {
        printf("malloc error for psf_gtk_menu\n");
        exit(0);
    }
    return psf_gmenu;
};

void dealloc_psf_gtk_menu(struct psf_gtk_menu *psf_gmenu){
    dealloc_colormap_views(psf_gmenu->color_vws);
    dealloc_psf_isoline_menu(psf_gmenu->psf_isoline_menu);
    dealloc_psf_surface_gtk_menu(psf_gmenu->psf_surface_menu);
    deallc_psf_vector_gtk_menu(psf_gmenu->psf_vector_menu);
    free(psf_gmenu);
    return;
};

void reset_psf_menu_params(struct kemoviewer_gl_type *kemo_gl,
                           struct psf_gtk_menu *psf_gmenu){
    set_gtk_isoline_menu_values(kemo_gl, psf_gmenu->psf_isoline_menu);
    set_gtk4_surface_menu_values(kemo_gl, psf_gmenu->psf_surface_menu);
    reset_colormap_menu_views(kemo_gl, psf_gmenu->color_vws);
    
    set_gtk_psf_vector_menu(kemo_gl, SURFACE_RENDERING, 
                            psf_gmenu->psf_vector_menu);
    set_vector_plot_availablity(kemo_gl, SURFACE_RENDERING,
                                psf_gmenu->psf_vector_menu);
    return;
};

GtkWidget * init_psf_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                               GtkWidget *psf_window,
                               struct psf_gtk_menu *psf_gmenu){
    GtkWidget *gmenu_vbox;
    
    psf_gmenu->psf_isoline_menu = init_isoline_menu_expander(kemo_gl, psf_window);
    psf_gmenu->psf_surface_menu = init_gtk4_psf_surface_menu_expander(kemo_gl, psf_window);
    psf_gmenu->color_vws = init_gtk_psf_colormap_expander(kemo_gl, SURFACE_RENDERING,
                                                          psf_window);
    psf_gmenu->psf_vector_menu = init_gtk_psf_vector_menu(kemo_gl, SURFACE_RENDERING,
                                                          psf_window);
    set_vector_plot_availablity(kemo_gl, SURFACE_RENDERING, psf_gmenu->psf_vector_menu);
    
    gmenu_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(gmenu_vbox), psf_gmenu->psf_isoline_menu->expander_iso);
    gtk_box_append(GTK_BOX(gmenu_vbox), psf_gmenu->psf_surface_menu->expander_surf);
    gtk_box_append(GTK_BOX(gmenu_vbox), psf_gmenu->color_vws->expander_color);
    gtk_box_append(GTK_BOX(gmenu_vbox), psf_gmenu->psf_vector_menu->expander_vect);
    return gmenu_vbox;
}
