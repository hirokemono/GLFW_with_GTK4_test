/***********************************************************************
 *  kemoview_gtk4_Mesh_polygon_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_Mesh_polygon_sel.h"

const char * const hd_normal =     "Normal";   //  NORMAL_POLYGON
const char * const hd_reverse =    "Reverse"; //   REVERSE_POLYGON

const char * const polygon_side_list[] = {hd_normal,
                                          hd_reverse,
                                          NULL};

/*    Buttons to set Polygon direction  */
static int find_selected_polygon_side(const char *text){
    int id_mode = 0;
    if     (kemoview_compare_string((int) strlen(text), text, hd_normal) > 0){
        id_mode = NORMAL_POLYGON;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_reverse) > 0){
        id_mode = REVERSE_POLYGON;
    }else{
        id_mode = NORMAL_POLYGON;
    };
    
    printf("Selected mesh polygon mode %d %s\n", id_mode, text);
    return id_mode;
}

static void selected_polygon_side_CB(GtkDropDown *dropdown,
                                     GParamSpec *pspec,
                                     gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int id_mode = find_selected_polygon_side(text);
    kemoview_set_object_property_flags(POLYGON_SWITCH, id_mode,
                                       kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
}


static GtkWidget * init_polygon_select_button(struct kemoviewer_gl_type *kemo_gl,
                                              GtkStringList *mesh_color_model){
    GtkWidget *polygon_dropdown;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    polygon_dropdown = init_list_button_by_list_model(mesh_color_model);
    g_signal_connect(G_OBJECT(polygon_dropdown), "notify::selected-item", 
                     G_CALLBACK (selected_polygon_side_CB), G_OBJECT(entry_buf));
    
    return polygon_dropdown;
}

static struct mesh_polygon_selector * alloc_mesh_polygon_selector(void){
    struct mesh_polygon_selector *polygon_select_vws;
    polygon_select_vws = (struct mesh_polygon_selector *) malloc(sizeof(struct mesh_polygon_selector));
    if (polygon_select_vws == NULL) {
        printf("malloc error for mesh_polygon_selector\n");
        exit(0);
    }
    return polygon_select_vws;
}



void set_polygon_select_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                 struct mesh_polygon_selector *polygon_select_vws){
	int id_mode = kemoview_get_object_property_flags(kemo_gl->kemoview_data, POLYGON_SWITCH);
    if(id_mode == REVERSE_POLYGON){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(polygon_select_vws->polygon_dropdown), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(polygon_select_vws->polygon_dropdown), 0);
	};
    return;
}

struct mesh_polygon_selector * init_polygon_select_views(struct kemoviewer_gl_type *kemo_gl){
    struct mesh_polygon_selector *polygon_select_vws = alloc_mesh_polygon_selector();
    
    GtkStringList *polygon_side_model = gtk_string_list_new(polygon_side_list);
    polygon_select_vws->polygon_dropdown = init_polygon_select_button(kemo_gl,
                                                                      polygon_side_model);
    g_object_unref(polygon_side_model);
    
    polygon_select_vws->polygon_sel_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(polygon_select_vws->polygon_sel_box), gtk_label_new("Polygon_type: "));
    gtk_box_append(GTK_BOX(polygon_select_vws->polygon_sel_box), polygon_select_vws->polygon_dropdown);
    return polygon_select_vws;
}

void dealloc_mesh_polygon_selector(struct mesh_polygon_selector *polygon_select_vws){
    g_object_unref(polygon_select_vws->polygon_dropdown);
    g_object_unref(polygon_select_vws->polygon_sel_box);
    free(polygon_select_vws);
    return;
}