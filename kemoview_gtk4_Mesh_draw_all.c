/***********************************************************************
 *  kemoview_gtk4_Mesh_draw_all.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_draw_all.h"

static struct show_all_gtk_menu * alloc_show_all_mesh_menu(){
    struct show_all_gtk_menu *show_all_vws;
    show_all_vws = (struct show_all_gtk_menu *) malloc(sizeof(struct show_all_gtk_menu));
    if (show_all_vws == NULL) {
        printf("malloc error for show_all_gtk_menu \n");
        exit(0);
    }
    return show_all_vws;
};

void set_draw_all_mesh_flags(int iflag, int iflag_group, int selected,
                             struct kemoviewer_type *kemoviewer){
	int num = kemoview_get_num_of_mesh_group(kemoviewer, iflag_group);
    int i;
	for(i=0;i<num;i++){
		kemoview_set_draw_mesh_item(iflag_group, selected,
                                    i, iflag, kemoviewer);
	};
    return;
}

static void draw_all_mesh_patch_CB(GtkButton *button, gpointer user_data){
    struct kemo_sw3c_columnview *draw_mesh_vws
            = (struct kemo_sw3c_columnview *) g_object_get_data(G_OBJECT(user_data), "mesh_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group = (int *) g_object_get_data(G_OBJECT(user_data), "group_flag");
    set_draw_all_mesh_flags(IONE, *iflag_group, SURFSOLID_TOGGLE, kemo_gl->kemoview_data);
    update_draw_mesh_list(kemo_gl, *iflag_group, draw_mesh_vws);
    draw_full_gl(kemo_gl);
}

static void draw_all_mesh_grids_CB(GtkButton *button, gpointer user_data){
    struct kemo_sw3c_columnview *draw_mesh_vws
            = (struct kemo_sw3c_columnview *) g_object_get_data(G_OBJECT(user_data), "mesh_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group = (int *) g_object_get_data(G_OBJECT(user_data), "group_flag");
    set_draw_all_mesh_flags(IONE, *iflag_group, SURFGRID_TOGGLE, kemo_gl->kemoview_data);
    update_draw_mesh_list(kemo_gl, *iflag_group, draw_mesh_vws);
    draw_full_gl(kemo_gl);
}

static void draw_all_mesh_nodes_CB(GtkButton *button, gpointer user_data){
    struct kemo_sw3c_columnview *draw_mesh_vws
            = (struct kemo_sw3c_columnview *) g_object_get_data(G_OBJECT(user_data), "mesh_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group = (int *) g_object_get_data(G_OBJECT(user_data), "group_flag");
    set_draw_all_mesh_flags(IONE, *iflag_group, SURFNOD_TOGGLE, kemo_gl->kemoview_data);
    update_draw_mesh_list(kemo_gl, *iflag_group, draw_mesh_vws);
    draw_full_gl(kemo_gl);
}


static void hide_all_mesh_patch_CB(GtkButton *button, gpointer user_data)
{
    struct kemo_sw3c_columnview *draw_mesh_vws
            = (struct kemo_sw3c_columnview *) g_object_get_data(G_OBJECT(user_data), "mesh_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group = (int *) g_object_get_data(G_OBJECT(user_data), "group_flag");
    set_draw_all_mesh_flags(IZERO, *iflag_group, SURFSOLID_TOGGLE, kemo_gl->kemoview_data);
    update_draw_mesh_list(kemo_gl, *iflag_group, draw_mesh_vws);
    draw_full_gl(kemo_gl);
}

static void hide_all_mesh_grids_CB(GtkButton *button, gpointer user_data)
{
    struct kemo_sw3c_columnview *draw_mesh_vws
            = (struct kemo_sw3c_columnview *) g_object_get_data(G_OBJECT(user_data), "mesh_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group = (int *) g_object_get_data(G_OBJECT(user_data), "group_flag");
    set_draw_all_mesh_flags(IZERO, *iflag_group, SURFGRID_TOGGLE, kemo_gl->kemoview_data);
    update_draw_mesh_list(kemo_gl, *iflag_group, draw_mesh_vws);
    draw_full_gl(kemo_gl);
}

static void hide_all_mesh_nodes_CB(GtkButton *button, gpointer user_data)
{
    struct kemo_sw3c_columnview *draw_mesh_vws
            = (struct kemo_sw3c_columnview *) g_object_get_data(G_OBJECT(user_data), "mesh_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group = (int *) g_object_get_data(G_OBJECT(user_data), "group_flag");
    set_draw_all_mesh_flags(IZERO, *iflag_group, SURFNOD_TOGGLE, kemo_gl->kemoview_data);
    update_draw_mesh_list(kemo_gl, *iflag_group, draw_mesh_vws);
    draw_full_gl(kemo_gl);
}


struct show_all_gtk_menu * init_show_all_mesh_menu(struct kemoviewer_gl_type *kemo_gl,
                                                   int *iflag_group,
                                                   struct kemo_sw3c_columnview *draw_mesh_vws){
    struct show_all_gtk_menu *show_all_vws = alloc_show_all_mesh_menu();
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "group_flag",  (gpointer) iflag_group);
    g_object_set_data(G_OBJECT(entry_buf), "mesh_menu",   (gpointer) draw_mesh_vws);
    
    show_all_vws->button_draw_patch = gtk_button_new_with_label("Draw");
    g_signal_connect(G_OBJECT(show_all_vws->button_draw_patch), "clicked",
                     G_CALLBACK(draw_all_mesh_patch_CB), (gpointer) entry_buf);
    
    show_all_vws->button_draw_grid = gtk_button_new_with_label("Draw");
    g_signal_connect(G_OBJECT(show_all_vws->button_draw_grid), "clicked",
                     G_CALLBACK(draw_all_mesh_grids_CB), (gpointer) entry_buf);
    
    show_all_vws->button_draw_node = gtk_button_new_with_label("Draw");
    g_signal_connect(G_OBJECT(show_all_vws->button_draw_node), "clicked",
                     G_CALLBACK(draw_all_mesh_nodes_CB), (gpointer) entry_buf);
    
    
    show_all_vws->button_hide_patch = gtk_button_new_with_label("Hide");
    g_signal_connect(G_OBJECT(show_all_vws->button_hide_patch), "clicked",
                     G_CALLBACK(hide_all_mesh_patch_CB), (gpointer) entry_buf);
    
    show_all_vws->button_hide_grid = gtk_button_new_with_label("Hide");
    g_signal_connect(G_OBJECT(show_all_vws->button_hide_grid), "clicked",
                     G_CALLBACK(hide_all_mesh_grids_CB), (gpointer) entry_buf);
    
    show_all_vws->button_hide_node = gtk_button_new_with_label("Hide");
    g_signal_connect(G_OBJECT(show_all_vws->button_hide_node), "clicked",
                     G_CALLBACK(hide_all_mesh_nodes_CB), (gpointer) entry_buf);
    
    GtkWidget *grid_all_switch = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new(" "),              0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Patch"),          1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Grid"),           2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Node"),           3, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Draw all: "),     0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), show_all_vws->button_draw_patch, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), show_all_vws->button_draw_grid,  2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), show_all_vws->button_draw_node,  3, 1, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid_all_switch), gtk_label_new("Hide all: "),     0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), show_all_vws->button_hide_patch, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), show_all_vws->button_hide_grid,  2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_all_switch), show_all_vws->button_hide_node,  3, 2, 1, 1);
    
    show_all_vws->show_all_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(show_all_vws->show_all_box), grid_all_switch);
    return show_all_vws;
}

struct show_all_gtk_menu * init_show_all_node_menu(struct kemoviewer_gl_type *kemo_gl,
                                                   int *iflag_group,
                                                   struct kemo_sw3c_columnview *draw_mesh_vws){
    struct show_all_gtk_menu *show_all_vws = init_show_all_mesh_menu(kemo_gl, iflag_group,
                                                                     draw_mesh_vws);
    
    gtk_widget_set_visible(show_all_vws->button_draw_patch, FALSE);
    gtk_widget_set_visible(show_all_vws->button_draw_grid,  FALSE);
    gtk_widget_set_visible(show_all_vws->button_hide_patch, FALSE);
    gtk_widget_set_visible(show_all_vws->button_hide_grid,  FALSE);
    return show_all_vws;
}

void dealloc_show_all_mesh_menu(struct show_all_gtk_menu *show_all_vws){
    g_object_unref(show_all_vws->button_draw_patch);
    g_object_unref(show_all_vws->button_draw_grid);
    g_object_unref(show_all_vws->button_draw_node);
    
    g_object_unref(show_all_vws->button_hide_patch);
    g_object_unref(show_all_vws->button_hide_grid);
    g_object_unref(show_all_vws->button_hide_node);
    
    g_object_unref(show_all_vws->show_all_box);
    free(show_all_vws);
    return;
};
