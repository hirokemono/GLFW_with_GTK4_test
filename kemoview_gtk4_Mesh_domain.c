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


struct domain_gtk_menu * init_gtk_domain_menu(struct kemoviewer_gl_type *kemo_gl,
                                              GtkWidget *window){
    struct domain_gtk_menu *domain_gmenu = alloc_domain_menu();
    domain_gmenu->all_domain_vws = init_show_all_mesh_menu(kemo_gl, DOMAIN_FLAG);
    
    GtkWidget *domain_menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(domain_menu_box), domain_gmenu->all_domain_vws->show_all_box);
    
    domain_gmenu->domain_menu_expander = wrap_into_scroll_expansion_gtk4("Subdomains", 360, 420,
                                                           window, domain_menu_box);
    return domain_gmenu;
}

void dealloc_domain_menu(struct domain_gtk_menu *domain_gmenu){
    dealloc_show_all_mesh_menu(domain_gmenu->all_domain_vws);
    
    g_object_unref(domain_gmenu->domain_menu_expander);
    free(domain_gmenu);
    return;
};
