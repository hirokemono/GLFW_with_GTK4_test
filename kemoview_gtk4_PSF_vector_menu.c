/***********************************************************************
 *  kemoview_gtk4_PSF_vector_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_vector_menu.h"

static struct psf_vector_gtk_menu * allc_psf_vector_gtk_menu(void){
    struct psf_vector_gtk_menu *psf_vector_menu;
    psf_vector_menu =  (struct psf_vector_gtk_menu *) malloc(sizeof(struct psf_vector_gtk_menu));
    if (psf_vector_menu == NULL) {
        printf("malloc error for psf_vector_gtk_menu\n");
        exit(0);
    }
    return psf_vector_menu;
}

static void psf_vector_switch_CB(GObject *switch_vect, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_vect));
    kemoview_set_VIZ_vector_draw_flags(iflag, SURFACE_RENDERING,
                                       kemo_gl->kemoview_data);

    draw_full_gl(kemo_gl);
	return;
};

static void set_ref_vector_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  SURFACE_RENDERING, ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                       gtk_floatvalue, i_digit,
                                       SURFACE_RENDERING,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}


void set_vector_plot_availablity(struct kemoviewer_gl_type *kemo_gl,
                                 int iflag_current_model,
                                 struct psf_vector_gtk_menu *psf_vector_menu){
    int if_psf = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              iflag_current_model,
                                              FIELD_SEL_FLAG);
    int ncomp = (int) kemoview_get_VIZ_num_component(kemo_gl->kemoview_data,
                                                     iflag_current_model, if_psf);
    if(ncomp == 3){
        gtk_widget_set_sensitive(psf_vector_menu->expander_vect, TRUE);
    } else {
        gtk_widget_set_sensitive(psf_vector_menu->expander_vect, FALSE);
    };
    return;
};

void set_gtk_psf_vector_menu(struct kemoviewer_gl_type *kemo_gl,
                             int iflag_current_model,
                             struct psf_vector_gtk_menu *psf_vector_menu){
    int if_psf = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              iflag_current_model,
                                              FIELD_SEL_FLAG);
    int ncomp = (int) kemoview_get_VIZ_num_component(kemo_gl->kemoview_data,
                                                     iflag_current_model, if_psf);
    if(ncomp != 3) return;
    
    int iflag = kemoview_get_VIZ_vector_draw_flags(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(psf_vector_menu->switch_vect), iflag);
    
    set_psf_vector_mode_dropdown(kemo_gl,  psf_vector_menu->vector_mode_vws);
    set_psf_vector_color_dropdown(kemo_gl, psf_vector_menu->vector_color_vws);
    set_psf_vector_param_menus(kemo_gl, psf_vector_menu->psf_vec_vws);
	return;
};

struct psf_vector_gtk_menu * init_gtk_psf_vector_menu(struct kemoviewer_gl_type *kemo_gl,
                                                      int iflag_current_model,
                                                      GtkWidget *window){
    struct psf_vector_gtk_menu *psf_vector_menu = allc_psf_vector_gtk_menu();
    
    psf_vector_menu->switch_vect = gtk_switch_new();
    g_signal_connect(G_OBJECT(psf_vector_menu->switch_vect), "notify::active",
                     G_CALLBACK(psf_vector_switch_CB), (gpointer) kemo_gl);
    
    psf_vector_menu->vector_mode_vws =  init_psf_vector_mode_views(kemo_gl);
    psf_vector_menu->vector_color_vws = init_psf_vector_color_views(kemo_gl);
    psf_vector_menu->psf_vec_vws =      init_psf_vector_param_menus(kemo_gl);
    
    GtkWidget *hbox_draw = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(hbox_draw), gtk_label_new("Draw vector: "));
    gtk_box_append(GTK_BOX(hbox_draw), psf_vector_menu->switch_vect);
    
    GtkWidget *vbox_vec = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(vbox_vec), hbox_draw);
    gtk_box_append(GTK_BOX(vbox_vec), psf_vector_menu->vector_mode_vws->hbox_vec_mode);
    gtk_box_append(GTK_BOX(vbox_vec), psf_vector_menu->vector_color_vws->hbox_vec_color);
    gtk_box_append(GTK_BOX(vbox_vec), psf_vector_menu->psf_vec_vws->hbox_ref_vect);
    gtk_box_append(GTK_BOX(vbox_vec), psf_vector_menu->psf_vec_vws->hbox_vect_width);
    gtk_box_append(GTK_BOX(vbox_vec), psf_vector_menu->psf_vec_vws->hbox_vect_inc);
    
    set_gtk_psf_vector_menu(kemo_gl, iflag_current_model, psf_vector_menu);
    psf_vector_menu->expander_vect = wrap_into_scroll_expansion_gtk4("Vector", 425, 300,
                                                                     window, vbox_vec);
    return psf_vector_menu;
}

void deallc_psf_vector_gtk_menu(struct psf_vector_gtk_menu *psf_vector_menu){
    dealloc_psf_vector_mode_view(psf_vector_menu->vector_mode_vws);
    dealloc_psf_vector_color_view(psf_vector_menu->vector_color_vws);
    deallc_psf_vector_param_views(psf_vector_menu->psf_vec_vws);
    g_object_unref(psf_vector_menu->switch_vect);
    g_object_unref(psf_vector_menu->expander_vect);
    free(psf_vector_menu);
    return;
}

