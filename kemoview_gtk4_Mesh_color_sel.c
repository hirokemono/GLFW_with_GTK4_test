/***********************************************************************
 *  kemoview_gtk4_Mesh_color_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_color_sel.h"

/*
const char * const label_rainbow =      "Rainbow";   //  RAINBOW_COLOR
const char * const label_grayscale =    "Grayscale"; //  GRAYSCALE

const char * const mesh_color_list[] = {label_rainbow,
                                        label_grayscale,
                                        NULL};
*/


/*    Buttons to set rotation movie image format  */
static int find_selected_mesh_color(const char *text){
    int index_mode = 0;
    if     (kemoview_compare_string((int) strlen(text), text, label_rainbow) > 0){
        index_mode = RAINBOW_COLOR;
    }else if(kemoview_compare_string((int) strlen(text), text, label_grayscale) > 0){
        index_mode = GRAYSCALE;
    }else{
        index_mode = RAINBOW_COLOR;
    };
    
    printf("Selected mesh color mode %d %s\n", index_mode, text);
    return index_mode;
}

static void selected_mesh_color_CB(GtkDropDown *dropdown,
                                   GParamSpec *pspec,
                                   gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int id_color_mode = find_selected_mesh_color(text);
	kemoview_set_mesh_color_mode(id_color_mode, kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
}


static GtkWidget * init_mesh_color_button(struct kemoviewer_gl_type *kemo_gl,
                                          GtkStringList *mesh_color_model){
    GtkWidget *mesh_color_dropdown;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    mesh_color_dropdown = init_list_button_by_list_model(mesh_color_model);
    g_signal_connect(G_OBJECT(mesh_color_dropdown), "notify::selected-item", 
                     G_CALLBACK (selected_mesh_color_CB), G_OBJECT(entry_buf));
    
    return mesh_color_dropdown;
}

static struct mesh_color_view * alloc_mesh_color_views(void){
    struct mesh_color_view *mesh_color_vws;
    mesh_color_vws = (struct mesh_color_view *) malloc(sizeof(struct mesh_color_view));
    if (mesh_color_vws == NULL) {
        printf("malloc error for mesh_color_view\n");
        exit(0);
    }
    return mesh_color_vws;
}



void set_mesh_color_dropdown(struct kemoviewer_gl_type *kemo_gl,
                             struct mesh_color_view *mesh_color_vws){
	int id_color_mode = kemoview_get_mesh_color_mode(kemo_gl->kemoview_data);
    if(id_color_mode == GRAYSCALE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(mesh_color_vws->mesh_color_dropdown), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(mesh_color_vws->mesh_color_dropdown), 0);
	};
    return;
}

struct mesh_color_view * init_mesh_color_views(struct kemoviewer_gl_type *kemo_gl){
    struct mesh_color_view *mesh_color_vws = alloc_mesh_color_views();
    
    GtkStringList *mesh_color_model = gtk_string_list_new(mesh_color_list);
    mesh_color_vws->mesh_color_dropdown = init_mesh_color_button(kemo_gl,
                                                                 mesh_color_model);
    g_object_unref(mesh_color_model);
    
    mesh_color_vws->mesh_color_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(mesh_color_vws->mesh_color_box), gtk_label_new("Color mode: "));
    gtk_box_append(GTK_BOX(mesh_color_vws->mesh_color_box), mesh_color_vws->mesh_color_dropdown);
    return mesh_color_vws;
}

void dealloc_mesh_color_views(struct mesh_color_view *mesh_color_vws){
    g_object_unref(mesh_color_vws->mesh_color_dropdown);
    g_object_unref(mesh_color_vws->mesh_color_box);
    free(mesh_color_vws);
    return;
}