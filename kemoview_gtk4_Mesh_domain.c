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

static void set_domain_opacity_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    float colorcode4[4];
    kemoview_get_mesh_color_code(kemo_gl->kemoview_data, DOMAIN_FLAG,
                                 SURFSOLID_TOGGLE, colorcode4);
    colorcode4[3] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    
	kemoview_set_mesh_color_code(DOMAIN_FLAG, SURFSOLID_TOGGLE,
                                 colorcode4, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}


struct domain_gtk_menu * init_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                                              GtkWidget *window){
    struct domain_gtk_menu *domain_gmenu = alloc_domain_menu();
    
    domain_gmenu->all_domain_vws = init_show_all_mesh_menu(kemo_gl, DOMAIN_FLAG);
    
    domain_gmenu->draw_domain_vws = init_draw_mesh_list_scroll(kemo_gl, window);
    
    domain_gmenu->domain_patch_color_vws = init_mesh_color_menu_box(kemo_gl, "Patch color",
                                                                    DOMAIN_FLAG, SURFSOLID_TOGGLE);
    domain_gmenu->domain_grid_color_vws =  init_mesh_color_menu_box(kemo_gl, "Grid color",
                                                                    DOMAIN_FLAG, SURFGRID_TOGGLE);
    domain_gmenu->domain_node_color_vws =  init_mesh_color_menu_box(kemo_gl, "Node color",
                                                                    DOMAIN_FLAG, SURFNOD_TOGGLE);
    
    GtkAdjustment *adj_opacity1 = gtk_adjustment_new(1.0, 0.0, 1.0, 0.01, 0.01, 0.0);
    domain_gmenu->spin_patch_opacity = gtk_spin_button_new(GTK_ADJUSTMENT(adj_opacity1), 0, 2);
    g_signal_connect(domain_gmenu->spin_patch_opacity, "value-changed",
                     G_CALLBACK(set_domain_opacity_CB), (gpointer) kemo_gl);
    
    GtkWidget *hbox_opacity = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_opacity), gtk_label_new("Patch opacity"));
    gtk_box_append(GTK_BOX(hbox_opacity), domain_gmenu->spin_patch_opacity);
    
    GtkWidget *domain_menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(domain_menu_box), domain_gmenu->draw_domain_vws->real3_cview_scroll);
    gtk_box_append(GTK_BOX(domain_menu_box), domain_gmenu->all_domain_vws->show_all_box);
    
    gtk_box_append(GTK_BOX(domain_menu_box), hbox_opacity);
    gtk_box_append(GTK_BOX(domain_menu_box), domain_gmenu->domain_patch_color_vws->color_sel_box);
    gtk_box_append(GTK_BOX(domain_menu_box), domain_gmenu->domain_grid_color_vws->color_sel_box);
    gtk_box_append(GTK_BOX(domain_menu_box), domain_gmenu->domain_node_color_vws->color_sel_box);
    
    domain_gmenu->domain_menu_expander = wrap_into_scroll_expansion_gtk4("Subdomains", 360, 420,
                                                           window, domain_menu_box);
    return domain_gmenu;
}

void set_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl, 
                         struct domain_gtk_menu *domain_gmenu){
    set_mesh_color_menu(kemo_gl, domain_gmenu->domain_patch_color_vws);
    set_mesh_color_menu(kemo_gl, domain_gmenu->domain_grid_color_vws);
    set_mesh_color_menu(kemo_gl, domain_gmenu->domain_node_color_vws);
    
    float color4[4];
    kemoview_get_mesh_color_code(kemo_gl->kemoview_data, DOMAIN_FLAG,
                                 SURFSOLID_TOGGLE, color4);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(domain_gmenu->spin_patch_opacity),
                              (double) color4[3]);
}

void dealloc_domain_menu(struct domain_gtk_menu *domain_gmenu){
    dealloc_show_all_mesh_menu(domain_gmenu->all_domain_vws);
    
    g_object_unref(domain_gmenu->spin_patch_opacity);
    dealloc_mesh_color_menu(domain_gmenu->domain_patch_color_vws);
    dealloc_mesh_color_menu(domain_gmenu->domain_grid_color_vws);
    dealloc_mesh_color_menu(domain_gmenu->domain_node_color_vws);
    
    g_object_unref(domain_gmenu->domain_menu_expander);
    free(domain_gmenu);
    return;
};
