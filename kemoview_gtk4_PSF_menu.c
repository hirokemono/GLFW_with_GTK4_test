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
    dealloc_psf_isoline_menu(psf_gmenu->psf_isoline_menu);
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
    psf_gmenu->psf_isoline_menu = init_isoline_menu_expander(kemo_gl, psf_gmenu->psfWin);
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
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->psf_isoline_menu->expander_iso);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_surf);
    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->color_vws->expander_color);
//    gtk_box_append(GTK_BOX(psf_vbox), psf_gmenu->expander_vect);
    return;
};
