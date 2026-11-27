/***********************************************************************
 *  kemoview_gtk4_isoline_color_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_isoline_color_sel.h"

const char * const hd_ColorLine =  "Color lines";  //  RAINBOW_LINE
const char * const hd_BlackLine =  "Black lines";  //  BLACK_LINE
const char * const hd_WhiteLine =  "White lines";  //  WHITE_LINE

const char * const line_color_list[] = {hd_ColorLine,
                                        hd_BlackLine,
                                        hd_WhiteLine,
                                        NULL};


/*    Buttons to set rotation movie image format  */
static int find_selected_isoline_color(const char *text){
    int index_mode = 0;
    if     (kemoview_compare_string((int) strlen(text), text, hd_ColorLine) > 0){
        index_mode = RAINBOW_LINE;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_WhiteLine) > 0){
        index_mode = WHITE_LINE;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_BlackLine) > 0){
        index_mode = BLACK_LINE;
    }else{
        index_mode = RAINBOW_LINE;
    };
    
    printf("Selected isoline mode %d %s\n", index_mode, text);
    return index_mode;
}

static void selected_isoline_color_CB(GtkDropDown *dropdown,
                                      GParamSpec *pspec,
                                      gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int id_color_mode = find_selected_isoline_color(text);
    kemoview_set_PSF_color_param(PSFGRID_TOGGLE, id_color_mode,
                                 kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
}


static GtkWidget * init_isoline_color_button(struct kemoviewer_gl_type *kemo_gl,
                                             GtkStringList *isoline_color_model){
    GtkWidget *isoline_color_button;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    isoline_color_button = init_list_button_by_list_model(isoline_color_model);
    g_signal_connect(G_OBJECT(isoline_color_button), "notify::selected-item", 
                     G_CALLBACK (selected_isoline_color_CB), G_OBJECT(entry_buf));
    
    return isoline_color_button;
}

static struct isoline_color_view * alloc_isoline_color_views(void){
    struct isoline_color_view *isoline_color_vws;
    isoline_color_vws = (struct isoline_color_view *) malloc(sizeof(struct isoline_color_view));
    if (isoline_color_vws == NULL) {
        printf("malloc error for isoline_color_view\n");
        exit(0);
    }
    return isoline_color_vws;
}



void set_isoline_color_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                struct isoline_color_view *isoline_color_vws){
    int id_color_mode = kemoview_get_PSF_color_param(kemo_gl->kemoview_data, PSFGRID_TOGGLE);
	if(id_color_mode == WHITE_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(isoline_color_vws->isoline_color_button), 2);
	} else if(id_color_mode == BLACK_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(isoline_color_vws->isoline_color_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(isoline_color_vws->isoline_color_button), 0);
	};
    return;
}

struct isoline_color_view * init_isoline_color_views(struct kemoviewer_gl_type *kemo_gl){
    struct isoline_color_view *isoline_color_vws = alloc_isoline_color_views();
    
    GtkStringList *isoline_color_model = gtk_string_list_new(line_color_list);
    isoline_color_vws->isoline_color_button = init_isoline_color_button(kemo_gl,
                                                                        isoline_color_model);
    set_isoline_color_dropdown(kemo_gl, isoline_color_vws);
    g_object_unref(isoline_color_model);
    
    isoline_color_vws->hbox_color = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(isoline_color_vws->hbox_color), gtk_label_new("Isoline color: "));
    gtk_box_append(GTK_BOX(isoline_color_vws->hbox_color), isoline_color_vws->isoline_color_button);
    return isoline_color_vws;
}

void dealloc_isoline_color_views(struct isoline_color_view *isoline_color_vws){
    g_object_unref(isoline_color_vws->isoline_color_button);
    g_object_unref(isoline_color_vws->hbox_color);
    free(isoline_color_vws);
    return;
}