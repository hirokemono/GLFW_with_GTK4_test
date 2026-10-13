/***********************************************************************
 *  kemoview_gtk4_pref_window.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_pref_window.h"

struct preference_view * init_preference_view(struct kemoviewer_type *kemoviewer_data){
    struct preference_view *pref_vws
            = (struct preference_view *) malloc(sizeof(struct preference_view));
    if (pref_vws == NULL) {
        printf("malloc error for preference_view\n");
        exit(0);
    }
    
    pref_vws->light_vws = init_light_views_4_viewer(kemoviewer_data->kemo_buffers->kemo_lights);
    return pref_vws;
};


void dealloc_preference_view(struct preference_view *pref_vws){
    dealloc_light_views_4_viewer(pref_vws->light_vws);
    free(pref_vws);
    return;
};


static gboolean dummy_close_CB(GtkWidget *widget, GdkEvent *event, gpointer data){
        gtk_window_destroy(GTK_WINDOW(widget));
//    gtk_window_close(GTK_WINDOW(widget));
//    g_object_unref(widget);
//    gtk_widget_set_visible(widget, FALSE);
    return TRUE; 
}

void init_preference_window(struct kemoviewer_gl_type *kemo_gl,
                            struct preference_view *pref_vws,
                            GtkWidget *main_window){
//    if(psf_gmenu->iflag_psfBox > 0){gtk_window_destroy(psf_gmenu->psfWin);};
//    psf_gmenu->iflag_psfBox = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data,
//                                                             NUM_LOADED);
//    if(psf_gmenu->iflag_psfBox == 0){return;}
    
    pref_vws->pref_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(pref_vws->pref_window), "Preferences");
    gtk_widget_set_size_request(pref_vws->pref_window, 450, -1);
    
    g_signal_connect(pref_vws->pref_window, "close-request",
                     G_CALLBACK(dummy_close_CB), NULL);
    
    GtkWidget *pref_frame = init_preference_scrollbox(kemo_gl, pref_vws->light_vws, main_window);
    gtk_window_set_child(GTK_WINDOW(pref_vws->pref_window), pref_frame);
    gtk_widget_set_visible(pref_vws->pref_window, TRUE);
    return;
}

