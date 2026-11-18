/***********************************************************************
 *  kemoview_gtk4_colormode_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_colormode_menu.h"

/*
const char * const label_rainbow =      "rainbow";              //   RAINBOW_MODE
const char * const label_grayscale =    "grayscale";            //   GRAYSCALE_MODE
const char * const label_bluered =      "blue_to_red";          //   RED_BLUE_MODE
const char * const label_sym_gray =     "symmetric_grayscale";  //   SYM_GRAY_MODE
const char * const label_orangecyan =   "cyan_to_orange";       //   ORANGE_CYAN_MODE
const char * const label_molten_metal = "molten_metal";         //   MOLTEN_METAL_MODE
const char * const label_space_color  = "space_color";          //   SPACE_COLOR_MODE
*/

struct colormode_menu * alloc_colormode_menu(void){
    struct colormode_menu *cmode_vws;
    cmode_vws = (struct colormode_menu *) malloc(sizeof(struct colormode_menu));
    if (cmode_vws == NULL) {
        printf("malloc error for colormode_menu \n");
        exit(0);
    }
    return cmode_vws;
}
void dealloc_colormode_menu(struct colormode_menu *cmode_vws){
    free(cmode_vws);
    return;
}

void init_colormode_dropdown(struct kemoviewer_gl_type *kemo_gl,
                             int iflag_current_model,
                             GtkWidget *viewmode_button){
    int iflag_mode = kemoview_get_viz_colormap_param(kemo_gl->kemoview_data,
                                                     iflag_current_model,
                                                     ISET_COLORMAP);
    if      (iflag_mode == RAINBOW_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 0);
    }else if(iflag_mode == GRAYSCALE_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 1);
    }else if(iflag_mode == RED_BLUE_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 2);
    }else if(iflag_mode == SYM_GRAY_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 3);
    }else if(iflag_mode == ORANGE_CYAN_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 4);
    }else if(iflag_mode == MOLTEN_METAL_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 5);
    }else if(iflag_mode == SPACE_COLOR_MODE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 6);
    } else {
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 0);
    };
    return;
}

static int find_selected_colormode_id(const char *text){
    int index_mode = -100;
    if     (kemoview_compare_string((int) strlen(text), text, label_rainbow) > 0){
        index_mode = RAINBOW_MODE;
    }else if(kemoview_compare_string((int) strlen(text), text, label_grayscale) > 0){
        index_mode = GRAYSCALE_MODE;
    }else if(kemoview_compare_string((int) strlen(text), text, label_bluered) > 0){
        index_mode = RED_BLUE_MODE;
    }else if(kemoview_compare_string((int) strlen(text), text, label_sym_gray) > 0){
        index_mode = SYM_GRAY_MODE;
    }else if(kemoview_compare_string((int) strlen(text), text, label_orangecyan) > 0){
        index_mode = ORANGE_CYAN_MODE;
    }else if(kemoview_compare_string((int) strlen(text), text, label_molten_metal) > 0){
        index_mode = MOLTEN_METAL_MODE;
    }else if(kemoview_compare_string((int) strlen(text), text, label_space_color) > 0){
        index_mode = SPACE_COLOR_MODE;
    }else {index_mode = RAINBOW_MODE;};
    return index_mode;
}

static void set_GLFW_colormode(int index_mode, int iflag_current_model,
                               struct kemoviewer_gl_type *kemo_gl){
    kemoview_set_colormap_param(iflag_current_model, ISET_COLORMAP,
                                index_mode, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

static void selected_colormode_CB(GtkDropDown *dropdown,
                                  GParamSpec *pspec,
                                  gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string (selected_item);
    int index_mode = find_selected_colormode_id(text);
    set_GLFW_colormode(index_mode, *iflag_current_model, kemo_gl);
}

struct colormode_menu * make_gtk4_colormode_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                                     int *iflag_current_model){
    struct colormode_menu *cmode_vws = alloc_colormode_menu();
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",    (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model",  (gpointer) iflag_current_model);
    /* A dropdown using an expression to obtain strings */
    GtkStringList *viewmode_model = gtk_string_list_new(color_labels);
    cmode_vws->viewmode_button = init_list_button_by_list_model(viewmode_model);
    
    g_signal_connect(cmode_vws->viewmode_button, "notify::selected-item", 
                     G_CALLBACK(selected_colormode_CB), G_OBJECT(entry_buf));
    init_colormode_dropdown(kemo_gl, *iflag_current_model, cmode_vws->viewmode_button);
    
    cmode_vws->hbox_colormode = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(cmode_vws->hbox_colormode), gtk_label_new("View type: "));
    gtk_box_append(GTK_BOX(cmode_vws->hbox_colormode), cmode_vws->viewmode_button);
    return cmode_vws;
}

