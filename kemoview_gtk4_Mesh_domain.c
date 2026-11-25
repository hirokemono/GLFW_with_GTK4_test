/***********************************************************************
 *  kemoview_gtk4_Mesh_domain.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_domain.h"

static struct domain_gtk_menu * alloc_domain_menu(void){
    struct domain_gtk_menu *domain_gmenu;
    domain_gmenu = (struct domain_gtk_menu *) malloc(sizeof(struct domain_gtk_menu));
    if (domain_gmenu == NULL) {
        printf("malloc error for domain_gtk_menu \n");
        exit(0);
    }
    return domain_gmenu;
};

void set_all_domain_flags(int iflag, int iflag_group, int selected,
                          struct kemoviewer_type *kemoviewer){
	int num = kemoview_get_num_of_mesh_group(kemoviewer, DOMAIN_FLAG);
    int i;
	for(i=0;i<num;i++){
		kemoview_set_draw_mesh_item(iflag_group, selected,
                                    i, iflag, kemoviewer);
	};
    return;
}

static void draw_all_domain_patch_CB(GtkButton *button, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_all_domain_flags(IONE, DOMAIN_FLAG, SURFSOLID_TOGGLE, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void draw_all_domain_grids_CB(GtkButton *button, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_all_domain_flags(IONE, DOMAIN_FLAG, SURFGRID_TOGGLE, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void draw_all_domain_nodes_CB(GtkButton *button, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_all_domain_flags(IONE, DOMAIN_FLAG, SURFNOD_TOGGLE, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}


static void hide_all_domain_patch_CB(GtkButton *button, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_all_domain_flags(IZERO, DOMAIN_FLAG, SURFSOLID_TOGGLE, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void hide_all_domain_grids_CB(GtkButton *button, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_all_domain_flags(IZERO, DOMAIN_FLAG, SURFGRID_TOGGLE, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void hide_all_domain_nodes_CB(GtkButton *button, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_all_domain_flags(IZERO, DOMAIN_FLAG, SURFNOD_TOGGLE, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}


static GtkWidget * initall_subdomain_switch_box(struct kemoviewer_gl_type *kemo_gl,
                                                struct domain_gtk_menu *domain_gmenu){
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    
    domain_gmenu->button_draw_patch = gtk_button_new_with_label("Draw patch");
    g_signal_connect(G_OBJECT(domain_gmenu->button_draw_patch), "clicked",
                     G_CALLBACK(draw_all_domain_patch_CB), (gpointer) entry_buf);
    
    domain_gmenu->button_draw_grid = gtk_button_new_with_label("Draw grids");
    g_signal_connect(G_OBJECT(domain_gmenu->button_draw_grid), "clicked",
                     G_CALLBACK(draw_all_domain_grids_CB), (gpointer) entry_buf);
    
    domain_gmenu->button_draw_node = gtk_button_new_with_label("Draw nodes");
    g_signal_connect(G_OBJECT(domain_gmenu->button_draw_node), "clicked",
                     G_CALLBACK(draw_all_domain_nodes_CB), (gpointer) entry_buf);
    
    
    domain_gmenu->button_hide_patch = gtk_button_new_with_label("Hide patch");
    g_signal_connect(G_OBJECT(domain_gmenu->button_hide_patch), "clicked",
                     G_CALLBACK(hide_all_domain_patch_CB), (gpointer) entry_buf);
    
    domain_gmenu->button_hide_grid = gtk_button_new_with_label("Hide grids");
    g_signal_connect(G_OBJECT(domain_gmenu->button_hide_grid), "clicked",
                     G_CALLBACK(hide_all_domain_grids_CB), (gpointer) entry_buf);
    
    domain_gmenu->button_hide_node = gtk_button_new_with_label("Hide nodes");
    g_signal_connect(G_OBJECT(domain_gmenu->button_hide_node), "clicked",
                     G_CALLBACK(hide_all_domain_nodes_CB), (gpointer) entry_buf);
    
    GtkWidget *grid_all_switch = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Draw all: "),     0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), domain_gmenu->button_draw_patch, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), domain_gmenu->button_draw_grid,  2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), domain_gmenu->button_draw_node,  3, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Hide all: "),     0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), domain_gmenu->button_hide_patch, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), domain_gmenu->button_hide_grid,  2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), domain_gmenu->button_hide_node,  3, 1, 1, 1);
    
    return grid_all_switch;
}

struct domain_gtk_menu * init_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                                              GtkWidget *window){
    struct domain_gtk_menu *domain_gmenu = alloc_domain_menu();
    
    GtkWidget *hbox_all_switch = initall_subdomain_switch_box(kemo_gl, domain_gmenu);
    
    domain_gmenu->domain_menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(domain_gmenu->domain_menu_box), hbox_all_switch);
    
//    set_gtk_domain_menu(kemo_gl, domain_gmenu);
    return domain_gmenu;
}

void set_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                         struct domain_gtk_menu *domain_gmenu){
};

void dealloc_domain_menu(struct domain_gtk_menu *domain_gmenu){
    g_object_unref(domain_gmenu->button_draw_patch);
    g_object_unref(domain_gmenu->button_draw_grid);
    g_object_unref(domain_gmenu->button_draw_node);
    
    g_object_unref(domain_gmenu->button_hide_patch);
    g_object_unref(domain_gmenu->button_hide_grid);
    g_object_unref(domain_gmenu->button_hide_node);
    
    g_object_unref(domain_gmenu->domain_menu_box);
    free(domain_gmenu);
    return;
};
