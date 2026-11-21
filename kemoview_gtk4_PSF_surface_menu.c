/***********************************************************************
 *  kemoview_gtk4_PSF_surface_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_surface_menu.h"

static struct psf_surface_gtk_menu * alloc_psf_surface_gtk_menu(void){
    struct psf_surface_gtk_menu *gtk_psf_surface;
    gtk_psf_surface = (struct psf_surface_gtk_menu *) malloc(sizeof(struct psf_surface_gtk_menu));
    if(gtk_psf_surface == NULL) {
        printf("malloc error for psf_surface_gtk_menu\n");
        exit(0);
    }
    return gtk_psf_surface;
}

static void psf_surface_switch_CB(GObject *switch_draw, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_draw));
    kemoview_set_VIZ_draw_flag(SURFACE_RENDERING, iflag,
                               kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static void set_gtk4_surf_draw_switch(struct kemoviewer_gl_type *kemo_gl,
                                      GtkWidget *switch_draw){
	if(kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data, SURFACE_RENDERING) == 0){
		gtk_switch_set_active(GTK_SWITCH(switch_draw), FALSE);
	} else {
		gtk_switch_set_active(GTK_SWITCH(switch_draw), TRUE);
	};
    return;
};

static GtkWidget * init_gtk4_psf_surface_switch_box(struct kemoviewer_gl_type *kemo_gl,
                                                    struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_draw;
    
    psf_surface_menu->switch_draw = gtk_switch_new();
    int iflag = kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data, SURFACE_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(psf_surface_menu->switch_draw), iflag);
    gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_draw), TRUE);
    g_signal_connect(G_OBJECT(psf_surface_menu->switch_draw), "notify::active",
                     G_CALLBACK(psf_surface_switch_CB), (gpointer) kemo_gl);
    
    hbox_draw = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_draw), gtk_label_new("Draw surface: "));
    gtk_box_append(GTK_BOX(hbox_draw), psf_surface_menu->switch_draw);
    
    return hbox_draw;
};


static void psf_colorbar_switch_CB(GObject *switch_bar, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_bar));
    kemoview_set_colorbar_draw_flag(iflag, SURFACE_RENDERING,
                                    kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static void set_gtk4_colorbar_draw_switch(struct kemoviewer_gl_type *kemo_gl,
                                          GtkWidget *switch_bar){
	if(kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                       SURFACE_RENDERING) == 0){
		gtk_switch_set_active(GTK_SWITCH(switch_bar), FALSE);
	} else {
		gtk_switch_set_active(GTK_SWITCH(switch_bar), TRUE);
	};
    return;
};

static GtkWidget * init_gtk4_psf_colorbar_switch_box(struct kemoviewer_gl_type *kemo_gl,
                                                     struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_cbar;
    
    psf_surface_menu->switch_bar = gtk_switch_new();
    int iflag = kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data, 
                                                SURFACE_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(psf_surface_menu->switch_bar), iflag);
    gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_bar), FALSE);
    g_signal_connect(G_OBJECT(psf_surface_menu->switch_bar), "notify::active",
                     G_CALLBACK(psf_colorbar_switch_CB), (gpointer) kemo_gl);
    
    hbox_cbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_cbar), gtk_label_new("Draw color bar: "));
    gtk_box_append(GTK_BOX(hbox_cbar), psf_surface_menu->switch_bar);
    return hbox_cbar;
};

static void set_psf_surface_opacity(struct kemoviewer_gl_type *kemo_gl,
                                    GtkWidget *spin_opacity1){
    double current_value = kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                          SURFACE_RENDERING,
                                                          ISET_OPACITY_MAX);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_opacity1), current_value);
	
};

static void set_psf_opacity_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_constant_opacity(gtk_floatvalue,
                                  SURFACE_RENDERING,
                                  kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}



void set_gtk4_surface_menu_values(struct kemoviewer_gl_type *kemo_gl,
                                  int id_current_model,
                                  struct psf_surface_gtk_menu *psf_surface_menu){
    set_gtk4_surf_draw_switch(kemo_gl, psf_surface_menu->switch_draw);
    set_gtk4_colorbar_draw_switch(kemo_gl, psf_surface_menu->switch_bar);
    set_psf_surface_opacity(kemo_gl, psf_surface_menu->spin_opacity1);
    
    set_gtk4_psf_surf_color_sel(kemo_gl, psf_surface_menu->psf_sf_color_vws);
    set_gtk4_surf_range_values(kemo_gl, id_current_model, 
                               psf_surface_menu->psf_range_vws);
    return;
};

struct psf_surface_gtk_menu * init_gtk4_psf_surface_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                                  int *id_current_model,
                                                                  GtkWidget *window){
    struct psf_surface_gtk_menu *psf_surface_menu = alloc_psf_surface_gtk_menu();
    
    GtkWidget *hbox_draw = init_gtk4_psf_surface_switch_box(kemo_gl, psf_surface_menu);
    GtkWidget *hbox_cbar = init_gtk4_psf_colorbar_switch_box(kemo_gl, psf_surface_menu);
    
    psf_surface_menu->psf_range_vws =    init_gtk_range_view(kemo_gl, id_current_model, window);
    psf_surface_menu->psf_sf_color_vws = init_gtk4_psf_surf_color_sel(kemo_gl, 
                                                                      id_current_model, 
                                                                      window);
    
    
    GtkAdjustment *adj_opacity1 = gtk_adjustment_new(1.0, 0.0, 1.0, 0.01, 0.01, 0.0);
    psf_surface_menu->spin_opacity1 = gtk_spin_button_new(GTK_ADJUSTMENT(adj_opacity1), 0, 2);
    g_signal_connect(psf_surface_menu->spin_opacity1, "value-changed",
                     G_CALLBACK(set_psf_opacity_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_one_opacity = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_one_opacity), gtk_label_new("Opacity"));
    gtk_box_append(GTK_BOX(hbox_one_opacity), psf_surface_menu->spin_opacity1);
    
    
    set_gtk4_surface_menu_values(kemo_gl, *id_current_model, psf_surface_menu);
    
    GtkWidget *patch_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(patch_box), hbox_draw);
    gtk_box_append(GTK_BOX(patch_box), hbox_cbar);
    gtk_box_append(GTK_BOX(patch_box), hbox_one_opacity);
    gtk_box_append(GTK_BOX(patch_box), psf_surface_menu->psf_sf_color_vws->hbox_sf_color);
    gtk_box_append(GTK_BOX(patch_box), psf_surface_menu->psf_range_vws->hbox_psf_range);
    
    psf_surface_menu->expander_surf = wrap_into_scroll_expansion_gtk4("Surface", 420, 320,
                                                                      window, patch_box);
    return psf_surface_menu;
}

void dealloc_psf_surface_gtk_menu(struct psf_surface_gtk_menu *psf_surface_menu){
    dealloc_gtk_range_view(psf_surface_menu->psf_range_vws);
    dealloc_psf_surf_color_views(psf_surface_menu->psf_sf_color_vws);
    g_object_unref(psf_surface_menu->spin_opacity1);
    g_object_unref(psf_surface_menu->switch_draw);
    g_object_unref(psf_surface_menu->switch_bar);
    g_object_unref(psf_surface_menu->expander_surf);
    free(psf_surface_menu);
    return;
};
