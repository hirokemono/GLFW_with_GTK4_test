/***********************************************************************
 *  kemoview_gtk4_colorbar_switch.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_colorbar_switch.h"

static struct colorbar_sw_view * alloc_colorbar_sw_view(void){
    struct colorbar_sw_view *colorbar_sw_vws;
    colorbar_sw_vws = (struct colorbar_sw_view *) malloc(sizeof(struct colorbar_sw_view));
    if(colorbar_sw_vws == NULL) {
        printf("malloc error for colorbar_sw_view\n");
        exit(0);
    }
    return colorbar_sw_vws;
}

static void colorbar_switch_CB(GObject *switch_bar, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_bar));
    kemoview_set_colorbar_draw_flag(iflag, *id_current_model,
                                    kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
};

struct colorbar_sw_view * init_colorbar_switch_hbox(struct kemoviewer_gl_type *kemo_gl,
                                                    int *id_current_model){
    struct colorbar_sw_view *colorbar_sw_vws = alloc_colorbar_sw_view();
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer)  id_current_model);
    
    colorbar_sw_vws->colorbar_switch = gtk_switch_new();
    gtk_switch_set_active(GTK_SWITCH(colorbar_sw_vws->colorbar_switch), TRUE);
    g_signal_connect(G_OBJECT(colorbar_sw_vws->colorbar_switch), "notify::active",
                     G_CALLBACK(colorbar_switch_CB), (gpointer) entry_buf);
    
    colorbar_sw_vws->cbar_sw_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(colorbar_sw_vws->cbar_sw_hbox), gtk_label_new("Draw color bar: "));
    gtk_box_append(GTK_BOX(colorbar_sw_vws->cbar_sw_hbox), colorbar_sw_vws->colorbar_switch);
    return colorbar_sw_vws;
};

void set_colorbar_switch(struct kemoviewer_gl_type *kemo_gl,
                         int id_current_model,
                         struct colorbar_sw_view *colorbar_sw_vws){
    int iflag = kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                                id_current_model);
    gtk_switch_set_state(GTK_SWITCH(colorbar_sw_vws->colorbar_switch), iflag);
    return;
};

void dealloc_colorbar_sw_view(struct colorbar_sw_view *colorbar_sw_vws){
    g_object_unref(colorbar_sw_vws->colorbar_switch);
    g_object_unref(colorbar_sw_vws->cbar_sw_hbox);
    free(colorbar_sw_vws);
    return;
}

