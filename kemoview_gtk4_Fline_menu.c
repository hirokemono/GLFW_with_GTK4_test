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


static void psf_fieldtube_switch_CB(GObject *switch_1, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int istate = gtk_switch_get_state(GTK_SWITCH(switch_1));
    kemoview_set_line_type_flag(istate, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static void fline_colorbar_switch_CB(GObject *switch_bar, GParamSpec *pspec, gpointer user_data){
    struct fieldline_gtk_menu *fline_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "fline_view");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_bar));
    kemoview_set_colorbar_draw_flag(iflag, fline_gmenu->iflag_flinemode,
                                    kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
};





void set_gtk_fieldline_menu(struct kemoviewer_gl_type *kemo_gl,
                            struct fieldline_gtk_menu *fline_gmenu){
    int icolor_mode = kemoview_get_VIZ_patch_color_mode(kemo_gl->kemoview_data,
                                                        fline_gmenu->iflag_flinemode);
	
    set_line_thickness_menu(fline_gmenu->thickness_vws);
    return;
};

void init_fieldline_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                              struct fieldline_gtk_menu *fline_gmenu){
    int iflag = 0;
    int index = 0;

    fline_gmenu->iflag_flinemode = FIELDLINE_RENDERING;
    GtkWidget *dummy_entry = gtk_entry_new();
    g_object_set_data(G_OBJECT(dummy_entry),
                      "fline_view", (gpointer) fline_gmenu);
    g_object_set_data(G_OBJECT(dummy_entry),
                      "kemoview_gl", (gpointer) kemo_gl);
    
    fline_gmenu->thickness_vws = init_line_thickness_hbox(kemo_gl, 
                                                          fline_gmenu->iflag_flinemode);

    
    fline_gmenu->switch_tube = gtk_switch_new();
    iflag = kemoview_get_line_type_flag(kemo_gl->kemoview_data);
    gtk_switch_set_state(GTK_SWITCH(fline_gmenu->switch_tube), iflag);
    gtk_switch_set_active(GTK_SWITCH(fline_gmenu->switch_tube), FALSE);
    g_signal_connect(G_OBJECT(fline_gmenu->switch_tube), "notify::active",
                     G_CALLBACK(psf_fieldtube_switch_CB), (gpointer) kemo_gl);
    return;
}

GtkWidget * pack_fieldline_menu_frame(struct fieldline_gtk_menu *fline_gmenu){
    GtkWidget *frame;
    
    GtkWidget *hbox_tube = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_tube), gtk_label_new("Draw tube: "));
	gtk_box_append(GTK_BOX(hbox_tube), fline_gmenu->switch_tube);
	
    GtkWidget *hbox_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_bar), gtk_label_new("Draw color bar: "));
    gtk_box_append(GTK_BOX(hbox_bar), fline_gmenu->fline_switch_bar);
    

    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(menu_box), hbox_bar);
    gtk_box_append(GTK_BOX(menu_box), hbox_tube);
	gtk_box_append(GTK_BOX(menu_box), fline_gmenu->thickness_vws->hbox_thickness);
    
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->expander_fline_color);
    frame = wrap_into_frame_gtk4("Fieldline", menu_box);
    return frame;
}




void set_fieldline_menu_parameters(struct kemoviewer_gl_type *kemo_gl,
                                struct fieldline_gtk_menu *fline_gmenu){
    set_colorbar_switch(kemo_gl, FIELDLINE_RENDERING, 
                        fline_gmenu->tracer_cbar_sw_vws);
    set_line_thickness_menu(kemo_gl, FIELDLINE_RENDERING, 
                            fline_gmenu->thickness_vws);
    return;
};
