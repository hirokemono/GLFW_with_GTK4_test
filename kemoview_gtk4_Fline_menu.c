/***********************************************************************
 *  kemoview_gtk4_Fline_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Fline_menu.h"

static struct fieldline_gtk_menu * alloc_fieldline_gtk_menu(void){
    struct fieldline_gtk_menu *fline_gmenu;
    fline_gmenu = (struct fieldline_gtk_menu *) malloc(sizeof(struct fieldline_gtk_menu));
    if(fline_gmenu == NULL) {
        printf("malloc error for fieldline_gtk_menu\n");
        exit(0);
    }
    fline_gmenu->id_fline_mode = FIELDLINE_RENDERING;
    return fline_gmenu;
}


static void psf_fieldtube_switch_CB(GObject *tb_switch, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int istate = gtk_switch_get_state(GTK_SWITCH(tb_switch));
    kemoview_set_line_type_flag(istate, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static void init_fieldtube_switch_hbox(struct kemoviewer_gl_type *kemo_gl,
                                       struct fieldline_gtk_menu *fline_gmenu){
    fline_gmenu->switch_tube = gtk_switch_new();
    g_signal_connect(G_OBJECT(fline_gmenu->switch_tube), "notify::active",
                     G_CALLBACK(psf_fieldtube_switch_CB), (gpointer) kemo_gl);
    
    fline_gmenu->hbox_fline_tube = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(fline_gmenu->hbox_fline_tube), gtk_label_new("Draw tube: "));
	gtk_box_append(GTK_BOX(fline_gmenu->hbox_fline_tube), fline_gmenu->switch_tube);
};

static void set_fieldtube_switch(struct kemoviewer_gl_type *kemo_gl,
                                 struct fieldline_gtk_menu *fline_gmenu){
    int iflag = kemoview_get_line_type_flag(kemo_gl->kemoview_data);
    gtk_switch_set_state(GTK_SWITCH(fline_gmenu->switch_tube), iflag);
    return;
};


struct fieldline_gtk_menu * init_fieldline_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                     GtkWidget *flineWin){
    struct fieldline_gtk_menu *fline_gmenu = alloc_fieldline_gtk_menu();
    init_fieldtube_switch_hbox(kemo_gl, fline_gmenu);
    
    fline_gmenu->fline_cbar_sw_vws = init_colorbar_switch_hbox(kemo_gl, 
                                                               &fline_gmenu->id_fline_mode);
    fline_gmenu->thickness_vws = init_line_thickness_hbox(kemo_gl, 
                                                          &fline_gmenu->id_fline_mode);
    
    
    fline_gmenu->fline_range_vws = init_gtk_range_view(kemo_gl,
                                                         &fline_gmenu->id_fline_mode, 
                                                         flineWin);
    fline_gmenu->fline_sf_color_vws = init_gtk4_psf_surf_color_sel(kemo_gl, 
                                                                   &fline_gmenu->id_fline_mode, 
                                                                   flineWin);
    fline_gmenu->fline_color_vws = init_gtk_psf_colormap_expander(kemo_gl,
                                                                  &fline_gmenu->id_fline_mode, 
                                                                  flineWin);
    
    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->hbox_fline_tube);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->thickness_vws->hbox_thickness);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_cbar_sw_vws->cbar_sw_hbox);
    
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_sf_color_vws->hbox_sf_color);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_range_vws->hbox_psf_range);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_color_vws->expander_color);
    
    fline_gmenu->fline_color_frame = wrap_into_frame_gtk4("Fieldline", menu_box);
    return fline_gmenu;
}

void set_fieldline_menu_parameters(struct kemoviewer_gl_type *kemo_gl,
                                   struct fieldline_gtk_menu *fline_gmenu){
    set_fieldtube_switch(kemo_gl, fline_gmenu);
    set_colorbar_switch(kemo_gl, fline_gmenu->id_fline_mode, 
                        fline_gmenu->fline_cbar_sw_vws);
    set_line_thickness_menu(kemo_gl, fline_gmenu->id_fline_mode, 
                            fline_gmenu->thickness_vws);
    
    set_gtk4_fline_surf_color_sel(kemo_gl, fline_gmenu->id_fline_mode,
                                  fline_gmenu->fline_sf_color_vws);
    reset_colormap_menu_views(kemo_gl, fline_gmenu->id_fline_mode, 
                              fline_gmenu->fline_color_vws);
    set_gtk4_surf_range_values(kemo_gl, fline_gmenu->id_fline_mode, 
                               fline_gmenu->fline_range_vws);
    return;
};


void dealloc_fieldline_gtk_menu(struct fieldline_gtk_menu *fline_gmenu){
    dealloc_gtk_range_view(fline_gmenu->fline_range_vws);
    dealloc_psf_surf_color_views(fline_gmenu->fline_sf_color_vws);
    dealloc_colormap_views(fline_gmenu->fline_color_vws);
    dealloc_colorbar_sw_view(fline_gmenu->fline_cbar_sw_vws);
    dealloc_line_thickness_menu(fline_gmenu->thickness_vws);
    g_object_unref(fline_gmenu->switch_tube);
    g_object_unref(fline_gmenu->hbox_fline_tube);
    g_object_unref(fline_gmenu->fline_color_frame);
    free(fline_gmenu);
    return;
};