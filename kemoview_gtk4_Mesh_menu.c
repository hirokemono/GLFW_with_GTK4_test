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

static void subdomain_distance_CB(GtkWidget *entry, gpointer data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
    double dist = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    if(dist >= 0.0) kemoview_set_domain_distance(dist, kemo_gl->kemoview_data);
    kemoview_draw_with_modified_domain_distance(kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
}

static void set_subdomain_distance_menu(struct kemoviewer_gl_type *kemo_gl,
                                        struct mesh_gtk_menu *mesh_gmenu){
    double current_distance = kemoview_get_domain_distance(kemo_gl->kemoview_data);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(mesh_gmenu->spin_dist), current_distance);
    return;
};

static GtkWidget * init_subdomain_distance_box(struct kemoviewer_gl_type *kemo_gl,
                                               struct mesh_gtk_menu *mesh_gmenu){
    GtkAdjustment *adj_dist = gtk_adjustment_new(0.0, 0.0, 10.0, 0.005, 0.005, 0.0);
    mesh_gmenu->spin_dist = gtk_spin_button_new(GTK_ADJUSTMENT(adj_dist), 0, 3);
    g_signal_connect(mesh_gmenu->spin_dist, "value-changed",
                     G_CALLBACK(subdomain_distance_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_distance = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(hbox_distance), gtk_label_new("Subdomain distance: "));
    gtk_box_append(GTK_BOX(hbox_distance), mesh_gmenu->spin_dist);
    return hbox_distance;
}


static void num_color_loop_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
    int nloop = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
    kemoview_set_num_of_color_loop(nloop, kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
}

static void set_subdomain_color_loop_menu(struct kemoviewer_gl_type *kemo_gl,
                                          struct mesh_gtk_menu *mesh_gmenu){
    int current_num_loop =    kemoview_get_num_of_color_loop(kemo_gl->kemoview_data);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(mesh_gmenu->spin_num_loop), current_num_loop);
    return;
};


static GtkWidget * init_subdomain_color_loop_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct mesh_gtk_menu *mesh_gmenu){
    GtkAdjustment *adj_num_loop = gtk_adjustment_new(5, 0, 100, 1, 1, 0.0);
    mesh_gmenu->spin_num_loop = gtk_spin_button_new(GTK_ADJUSTMENT(adj_num_loop), 0, 0);
    g_signal_connect(mesh_gmenu->spin_num_loop, "value-changed",
                     G_CALLBACK(num_color_loop_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_num_loop = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(hbox_num_loop), gtk_label_new("Number of color loop: "));
    gtk_box_append(GTK_BOX(hbox_num_loop), mesh_gmenu->spin_num_loop);
    return hbox_num_loop;
}


static void node_size_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double current_size;
	int i_digit;
	double floatvalue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));

	kemoview_get_node_diamater(kemo_gl->kemoview_data, &current_size, &i_digit);
	kemoview_set_node_diamater(floatvalue, i_digit, kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
}

static void node_digit_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double current_size;
	int i_digit;
	int intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	
	kemoview_get_node_diamater(kemo_gl->kemoview_data,
                               &current_size, &i_digit);
	kemoview_set_node_diamater(current_size, intvalue,
                               kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
}

static void set_mesh_node_size(struct kemoviewer_gl_type *kemo_gl,
                               struct mesh_gtk_menu *mesh_gmenu){
	double current_size;
    int i_digit;
    kemoview_get_node_diamater(kemo_gl->kemoview_data, &current_size, &i_digit);
    
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(mesh_gmenu->spin_node_size), current_size);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(mesh_gmenu->spin_node_digit), i_digit);
    return;
};


static GtkWidget * init_mesh_node_size_box(struct kemoviewer_gl_type *kemo_gl,
                                           struct mesh_gtk_menu *mesh_gmenu){
    GtkAdjustment *adj_node_size = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    mesh_gmenu->spin_node_size = gtk_spin_button_new(GTK_ADJUSTMENT(adj_node_size), 0, 3);
    g_signal_connect(mesh_gmenu->spin_node_size, "value-changed",
                     G_CALLBACK(node_size_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_node_digit = gtk_adjustment_new(-3, -10, 10, 1, 1, 0);
    mesh_gmenu->spin_node_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_node_digit), 0, 0);
    g_signal_connect(mesh_gmenu->spin_node_digit, "value-changed",
                     G_CALLBACK(node_digit_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_node_size = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(hbox_node_size), gtk_label_new("Node size: "));
    gtk_box_append(GTK_BOX(hbox_node_size), mesh_gmenu->spin_node_size);
    gtk_box_append(GTK_BOX(hbox_node_size), gtk_label_new("x 10^ "));
    gtk_box_append(GTK_BOX(hbox_node_size), mesh_gmenu->spin_node_digit);
    return hbox_node_size;
}



struct mesh_gtk_menu * init_gtk_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                                          GtkWidget *window){
    struct mesh_gtk_menu *mesh_gmenu = alloc_mesh_menu();
    
    GtkWidget *hbox_distance =  init_subdomain_distance_box(kemo_gl, mesh_gmenu);
    GtkWidget *hbox_num_loop =  init_subdomain_color_loop_box(kemo_gl, mesh_gmenu);
    GtkWidget *hbox_node_size = init_mesh_node_size_box(kemo_gl, mesh_gmenu);
    
    mesh_gmenu->mesh_color_vws = init_mesh_color_views(kemo_gl);
    mesh_gmenu->domain_gmenu =   init_gtk_domain_menu(kemo_gl, window);
    
    mesh_gmenu->mesh_menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(mesh_gmenu->mesh_menu_box), mesh_gmenu->domain_gmenu->domain_menu_expander);
    gtk_box_append(GTK_BOX(mesh_gmenu->mesh_menu_box), hbox_distance);
    gtk_box_append(GTK_BOX(mesh_gmenu->mesh_menu_box), hbox_node_size);
    gtk_box_append(GTK_BOX(mesh_gmenu->mesh_menu_box), mesh_gmenu->mesh_color_vws->mesh_color_box);
    gtk_box_append(GTK_BOX(mesh_gmenu->mesh_menu_box), hbox_num_loop);
    
    
    set_gtk_mesh_menu(kemo_gl, mesh_gmenu);
    return mesh_gmenu;
}

void set_gtk_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                       struct mesh_gtk_menu *mesh_gmenu){
    set_subdomain_distance_menu(kemo_gl, mesh_gmenu);
    set_subdomain_color_loop_menu(kemo_gl, mesh_gmenu);
    set_mesh_node_size(kemo_gl, mesh_gmenu);
    
    set_mesh_color_dropdown(kemo_gl, mesh_gmenu->mesh_color_vws);
//    set_gtk_domain_menu(kemo_gl, mesh_gmenu->domain_gmenu);
};

void dealloc_mesh_menu(struct mesh_gtk_menu *mesh_gmenu){
    dealloc_mesh_color_views(mesh_gmenu->mesh_color_vws);
    dealloc_domain_menu(mesh_gmenu->domain_gmenu);
    
    g_object_unref(mesh_gmenu->spin_dist);
    g_object_unref(mesh_gmenu->spin_num_loop);
    g_object_unref(mesh_gmenu->mesh_menu_box);
    g_object_unref(mesh_gmenu->spin_node_size);
    g_object_unref(mesh_gmenu->spin_node_digit);
    
    free(mesh_gmenu);
    return;
};
