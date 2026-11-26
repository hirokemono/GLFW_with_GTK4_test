/***********************************************************************
 *  kemoview_gtk4_Mesh_color_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_color_menu.h"

const char * const hd_domain =               "Color by domain"; //   DOMAIN_COLOR
const char * const hd_group =                "Color by group";  //   REVERSE_POLYGON
const char * const hd_single_meshcolor =     "Single color";    //   SINGLE_COLOR
const char * const hd_white_meshcolor =      "White";           //   WHITE_SURFACE

const char * const mesh_color_mode_list[] = {hd_domain,
                                             hd_group,
                                             hd_white_meshcolor,
                                             hd_single_meshcolor,
                                             NULL};


static struct mesh_color_menu * alloc_mesh_color_menu(int iflag_group, int selected_type){
    struct mesh_color_menu *mesh_color_menu_vws;
    mesh_color_menu_vws = (struct mesh_color_menu *) malloc(sizeof(struct mesh_color_menu));
    if (mesh_color_menu_vws == NULL) {
        printf("malloc error for mesh_color_menu\n");
        exit(0);
    }
    
    mesh_color_menu_vws->iflag_group =   iflag_group;
    mesh_color_menu_vws->selected_type = selected_type;
    return mesh_color_menu_vws;
}

/*    Buttons to set Polygon direction  */
static int find_selected_mesh_color_mode(const char *text){
    int id_mode = 0;
    if      (kemoview_compare_string((int) strlen(text), text, hd_domain) > 0){
        id_mode = DOMAIN_COLOR;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_group) >  0){
        id_mode = GROUP_COLOR;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_single_meshcolor) > 0){
        id_mode = SINGLE_COLOR;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_white_meshcolor) >  0){
        id_mode = WHITE_SURFACE;
    }else{
        id_mode = DOMAIN_COLOR;
    };
    
    printf("Selected mesh color mode %d %s\n", id_mode, text);
    return id_mode;
}

static void set_mesh_color_mode_button(int id_color_mode, 
                                       struct kemoviewer_gl_type *kemo_gl,
                                       int iflag_group,
                                       int selected_type, 
                                       GtkWidget *color_sel_dropdown){
    if      (id_color_mode == DOMAIN_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_sel_dropdown), 0);
    }else if(id_color_mode == GROUP_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_sel_dropdown), 1);
    }else if(id_color_mode == WHITE_SURFACE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_sel_dropdown), 2);
    }else if(id_color_mode == SINGLE_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_sel_dropdown), 3);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_sel_dropdown), 0);
	};
    return;
}

static void selected_mesh_color_mode_CB(GtkDropDown *dropdown,
                                        GParamSpec *pspec,
                                        gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_group =   (int *) g_object_get_data(G_OBJECT(user_data), "mesh_group");
    int *selected_type = (int *) g_object_get_data(G_OBJECT(user_data), "mesh_type");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int id_mode = find_selected_mesh_color_mode(text);
    kemoview_set_mesh_color_flag(*iflag_group, *selected_type, id_mode,
                                 kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
}


static GtkWidget * init_mesh_color_mode_button(struct kemoviewer_gl_type *kemo_gl,
                                               int *iflag_group,
                                               int *selected_type, 
                                               GtkStringList *mesh_color_model){
    GtkWidget *color_sel_dropdown;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "mesh_group", (gpointer) iflag_group);
    g_object_set_data(G_OBJECT(entry_buf), "mesh_type", (gpointer) selected_type);
    
    color_sel_dropdown = init_list_button_by_list_model(mesh_color_model);
    g_signal_connect(G_OBJECT(color_sel_dropdown), "notify::selected-item", 
                     G_CALLBACK (selected_mesh_color_mode_CB), G_OBJECT(entry_buf));
    
    return color_sel_dropdown;
}



static void set_mesh_single_color_CB(GObject    *object,
                                     GParamSpec *pspec,
                                     gpointer    user_data){
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON(object);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(user_data, "kemoview_gl");
    int *iflag_group =   (int *) g_object_get_data(G_OBJECT(user_data), "mesh_group");
    int *selected_type = (int *) g_object_get_data(G_OBJECT(user_data), "mesh_type");
    
    const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
    float color[4];
    set_RGB_from_GTK(rgba, color);
//    printf("updated %f %f %f %f\n", color[0], color[1], color[2], color[3]);
    kemoview_set_mesh_color_code(*iflag_group, *selected_type, color,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    
    return;
};

static GtkWidget * init_mesh_color_cell(struct kemoviewer_gl_type *kemo_gl,
                                        int *iflag_group,
                                        int *selected_type){
    GtkWidget *color_pick_cell;
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha (color_dialog, TRUE); // Allow transparency adjustments
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "mesh_group", (gpointer) iflag_group);
    g_object_set_data(G_OBJECT(entry_buf), "mesh_type", (gpointer) selected_type);
    
    color_pick_cell = gtk_color_dialog_button_new(color_dialog);
    g_signal_connect(color_pick_cell, "notify::rgba", G_CALLBACK(set_mesh_single_color_CB), entry_buf);
    return color_pick_cell;
}

struct mesh_color_menu * init_mesh_color_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                                  char *menu_label,
                                                  int iflag_group,
                                                  int selected_type){
    struct mesh_color_menu *mesh_color_menu_vws = alloc_mesh_color_menu(iflag_group, selected_type);
    
    GtkStringList *polygon_side_model = gtk_string_list_new(mesh_color_mode_list);
    mesh_color_menu_vws->color_sel_dropdown
            = init_mesh_color_mode_button(kemo_gl,
                                          &mesh_color_menu_vws->iflag_group, 
                                          &mesh_color_menu_vws->selected_type,
                                          polygon_side_model);
    g_object_unref(polygon_side_model);
    
    mesh_color_menu_vws->color_pick_cell = init_mesh_color_cell(kemo_gl,
                                                                &mesh_color_menu_vws->iflag_group, 
                                                                &mesh_color_menu_vws->selected_type);
    
    mesh_color_menu_vws->color_sel_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(mesh_color_menu_vws->color_sel_box), gtk_label_new(menu_label));
    gtk_box_append(GTK_BOX(mesh_color_menu_vws->color_sel_box), mesh_color_menu_vws->color_sel_dropdown);
    gtk_box_append(GTK_BOX(mesh_color_menu_vws->color_sel_box), mesh_color_menu_vws->color_pick_cell);
    return mesh_color_menu_vws;
}

void set_mesh_color_menu(struct kemoviewer_gl_type *kemo_gl,
                         struct mesh_color_menu *mesh_color_menu_vws){
    int id_mode = kemoview_get_mesh_color_flag(kemo_gl->kemoview_data, 
                                               mesh_color_menu_vws->iflag_group, 
                                               mesh_color_menu_vws->selected_type);
    set_mesh_color_mode_button(id_mode, kemo_gl, 
                               mesh_color_menu_vws->iflag_group, 
                               mesh_color_menu_vws->selected_type,
                               mesh_color_menu_vws->color_pick_cell);
    
    float color[4];
    GdkRGBA mesh_rgba;
    kemoview_get_mesh_color_code(kemo_gl->kemoview_data,
                                 mesh_color_menu_vws->iflag_group, 
                                 mesh_color_menu_vws->selected_type, 
                                 color);
    set_RGB_to_GTK(color, &mesh_rgba);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(mesh_color_menu_vws->color_sel_dropdown),
                                     &mesh_rgba);
    return;
}

void dealloc_mesh_color_menu(struct mesh_color_menu *mesh_color_menu_vws){
    g_object_unref(mesh_color_menu_vws->color_sel_dropdown);
    g_object_unref(mesh_color_menu_vws->color_pick_cell);
    g_object_unref(mesh_color_menu_vws->color_sel_box);
    free(mesh_color_menu_vws);
    return;
}