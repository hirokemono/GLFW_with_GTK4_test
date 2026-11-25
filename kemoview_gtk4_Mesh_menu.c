/***********************************************************************
 *  kemoview_gtk4_Mesh_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_menu.h"

static struct mesh_gtk_menu * alloc_mesh_menu(void){
    struct mesh_gtk_menu *mesh_gmenu;
    mesh_gmenu = (struct mesh_gtk_menu *) malloc(sizeof(struct mesh_gtk_menu));
    if (mesh_gmenu == NULL) {
        printf("malloc error for mesh_gtk_menu \n");
        exit(0);
    }
    return mesh_gmenu;
};

static void subdoain_distance_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double dist = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	if(dist >= 0.0) kemoview_set_domain_distance(dist, kemo_gl->kemoview_data);
	kemoview_draw_with_modified_domain_distance(kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
}


struct mesh_gtk_menu * init_gtk_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                                          GtkWidget *window){
    struct mesh_gtk_menu *mesh_gmenu = alloc_mesh_menu();
    
    GtkAdjustment *adj_dist = gtk_adjustment_new(0.0, 0.0, 10.0, 0.005, 0.005, 0.0);
    mesh_gmenu->spin_dist = gtk_spin_button_new(GTK_ADJUSTMENT(adj_dist), 0, 3);
    g_signal_connect(mesh_gmenu->spin_dist, "value-changed",
                     G_CALLBACK(subdoain_distance_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_distance = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(hbox_distance), gtk_label_new("Subdomain distance: "));
    gtk_box_append(GTK_BOX(hbox_distance), mesh_gmenu->spin_dist);
    
    mesh_gmenu->mesh_menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(mesh_gmenu->mesh_menu_box), hbox_distance);
    return mesh_gmenu;
}

void dealloc_mesh_menu(struct mesh_gtk_menu *mesh_gmenu){
    g_object_unref(mesh_gmenu->mesh_menu_box);
    free(mesh_gmenu);
    return;
};
