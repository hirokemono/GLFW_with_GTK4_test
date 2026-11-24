/***********************************************************************
 *  kemoview_gtk4_Tracer_menu.c
 *  Kemoview_Cocoa
 *
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Tracer_menu.h"

static struct tracer_gtk_menu * alloc_tracer_gtk_menu(void){
    struct tracer_gtk_menu *tracer_gmenu;
    tracer_gmenu = (struct tracer_gtk_menu *) malloc(sizeof(struct tracer_gtk_menu));
    if(tracer_gmenu == NULL) {
        printf("malloc error for tracer_gtk_menu\n");
        exit(0);
    }
    
    tracer_gmenu->id_tracer_mode = TRACER_RENDERING;
    return tracer_gmenu;
}

static void psf_vector_switch_CB(GObject *vector_switch, 
                                 GParamSpec *pspec,
                                 gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(vector_switch));
    kemoview_set_VIZ_vector_draw_flags(iflag, TRACER_RENDERING,
                                       kemo_gl->kemoview_data);

    draw_full_gl(kemo_gl);
	return;
};

static void init_tracer_vector_mode_switch(struct kemoviewer_gl_type *kemo_gl,
                                           struct tracer_gtk_menu *tracer_gmenu){
    tracer_gmenu->vector_switch = gtk_switch_new();
	g_signal_connect(G_OBJECT(tracer_gmenu->vector_switch), "notify::active",
				G_CALLBACK(psf_vector_switch_CB), (gpointer) kemo_gl);
    
    tracer_gmenu->vector_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(tracer_gmenu->vector_hbox), gtk_label_new("Draw vector: "));
    gtk_box_append(GTK_BOX(tracer_gmenu->vector_hbox), tracer_gmenu->vector_switch);
    return;
};

static void set_tracer_vector_mode_switch(struct kemoviewer_gl_type *kemo_gl,
                                          struct tracer_gtk_menu *tracer_gmenu){
    int iflag = kemoview_get_VIZ_vector_draw_flags(kemo_gl->kemoview_data,
                                                   TRACER_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(tracer_gmenu->vector_switch), iflag);
    
	if(kemoview_get_VIZ_vector_draw_flags(kemo_gl->kemoview_data,
                                          TRACER_RENDERING) == 0){
		gtk_switch_set_active(GTK_SWITCH(tracer_gmenu->vector_switch), FALSE);
	} else {
		gtk_switch_set_active(GTK_SWITCH(tracer_gmenu->vector_switch), TRUE);
	};
    return;
};


struct tracer_gtk_menu * init_tracer_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                                               GtkWidget *tracerWin){
    struct tracer_gtk_menu *tracer_gmenu = alloc_tracer_gtk_menu();
    init_tracer_vector_mode_switch(kemo_gl, tracer_gmenu);
    
    tracer_gmenu->tracer_cbar_sw_vws = init_colorbar_switch_hbox(kemo_gl,
                                                                 &tracer_gmenu->id_tracer_mode);
    tracer_gmenu->thickness_vws =      init_line_thickness_hbox(kemo_gl, 
                                                                &tracer_gmenu->id_tracer_mode);
    tracer_gmenu->tracer_refvect_vws =   init_tracer_ref_vector_hbox(kemo_gl);
    tracer_gmenu->tracer_increment_vws = init_tracer_vec_increment_hbox(kemo_gl);
    
    tracer_gmenu->tracer_range_vws = init_gtk_range_view(kemo_gl,
                                                         &tracer_gmenu->id_tracer_mode, 
                                                         tracerWin);
    tracer_gmenu->tracer_sf_color_vws = init_gtk4_psf_surf_color_sel(kemo_gl, 
                                                                     &tracer_gmenu->id_tracer_mode, 
                                                                     tracerWin);
    /*
    tracer_gmenu->tracer_color_vws = init_gtk_psf_colormap_expander(kemo_gl,
                                                                    &tracer_gmenu->id_tracer_mode, 
                                                                    tracerWin);
    */
    
    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->tracer_cbar_sw_vws->cbar_sw_hbox);
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->thickness_vws->hbox_thickness);
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->vector_hbox);
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->tracer_refvect_vws->hbox_ref_vect);
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->tracer_increment_vws->hbox_vect_inc);
    
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->tracer_sf_color_vws->hbox_sf_color);
    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->tracer_range_vws->hbox_psf_range);
//    gtk_box_append(GTK_BOX(menu_box), tracer_gmenu->tracer_color_vws->expander_color);
    tracer_gmenu->tracer_menu_frame = wrap_into_frame_gtk4("Fieldline", menu_box);
    return tracer_gmenu;
}

void set_tracer_menu_parameters(struct kemoviewer_gl_type *kemo_gl,
                                struct tracer_gtk_menu *tracer_gmenu){
    set_colorbar_switch(kemo_gl, tracer_gmenu->id_tracer_mode, 
                        tracer_gmenu->tracer_cbar_sw_vws);
    set_tracer_vector_mode_switch(kemo_gl, tracer_gmenu);
    set_line_thickness_menu(kemo_gl, tracer_gmenu->id_tracer_mode,
                            tracer_gmenu->thickness_vws);
    set_tracer_ref_vector_menu(kemo_gl, tracer_gmenu->tracer_refvect_vws);
    set_tracer_vec_increment_menu(kemo_gl, tracer_gmenu->tracer_increment_vws);
    
    set_gtk4_fline_surf_color_sel(kemo_gl, tracer_gmenu->id_tracer_mode,
                                  tracer_gmenu->tracer_color_vws);
    reset_colormap_menu_views(kemo_gl, tracer_gmenu->id_tracer_mode, 
                              tracer_gmenu->tracer_sf_color_vws);
    set_gtk4_surf_range_values(kemo_gl, tracer_gmenu->id_tracer_mode, 
                               tracer_gmenu->tracer_range_vws);
    return;
};

void dealloc_tracer_menu_parameters(struct tracer_gtk_menu *tracer_gmenu){
    dealloc_gtk_range_view(tracer_gmenu->tracer_range_vws);
    dealloc_psf_surf_color_views(tracer_gmenu->tracer_sf_color_vws);
    dealloc_colormap_views(tracer_gmenu->tracer_color_vws);
    
    dealloc_colorbar_sw_view(tracer_gmenu->tracer_cbar_sw_vws);
    
    dealloc_line_thickness_menu(tracer_gmenu->thickness_vws);
    dealloc_tracer_ref_vector_menu(tracer_gmenu->tracer_refvect_vws);
    dealloc_tracer_vec_increment_menu(tracer_gmenu->tracer_increment_vws);
    
    g_object_unref(tracer_gmenu->tracer_menu_frame);
    g_object_unref(tracer_gmenu->vector_hbox);
    g_object_unref(tracer_gmenu->vector_switch);
    free(tracer_gmenu);
    return;
};
