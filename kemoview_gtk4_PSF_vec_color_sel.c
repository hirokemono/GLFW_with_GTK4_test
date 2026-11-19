/***********************************************************************
 *  kemoview_gtk4_PSF_vec_color_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_vec_color_sel.h"

const char * const hd_Color = "Colored";  //  COLORED_BY_DATA
const char * const hd_White = "White";    //  WHITE_SURFACE

const char * const vector_color_list[] = {hd_Color,
                                          hd_White,
                                          NULL};


/*    Buttons to set vector mode  */
static int find_selected_vector_color(const char *text){
    int index_mode = 0;
    if     (kemoview_compare_string((int) strlen(text), text, hd_Color) > 0){
        index_mode = COLORED_BY_DATA;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_White) > 0){
        index_mode = WHITE_SURFACE;
    }else{
        index_mode = COLORED_BY_DATA;
    };
    
    printf("Selected vector color %d %s\n", index_mode, text);
    return index_mode;
}

static void selected_vector_color_CB(GtkDropDown *dropdown,
                                     GParamSpec *pspec,
                                     gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int id_color = find_selected_vector_color(text);
	kemoview_set_PSF_color_param(ISET_VECTOR_COLOR, id_color, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

static GtkWidget * init_vector_color_button(struct kemoviewer_gl_type *kemo_gl,
                                            GtkStringList *vector_mode_model){
    GtkWidget *vector_color_button;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    vector_color_button = init_list_button_by_list_model(vector_mode_model);
    g_signal_connect(G_OBJECT(vector_color_button), "notify::selected-item", 
                     G_CALLBACK (selected_vector_color_CB), G_OBJECT(entry_buf));
    
    return vector_color_button;
}

static struct psf_vector_color_view * alloc_psf_vector_color_view(void){
    struct psf_vector_color_view *vector_color_vws;
    vector_color_vws = (struct psf_vector_color_view *) malloc(sizeof(struct psf_vector_color_view));
    if (vector_color_vws == NULL) {
        printf("malloc error for psf_vector_color_view\n");
        exit(0);
    }
    return vector_color_vws;
}



void set_psf_vector_color_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                   struct psf_vector_color_view *vector_color_vws){
    int iflag_vec_mode = kemoview_get_PSF_color_param(kemo_gl->kemoview_data, ISET_VECTOR_COLOR);
    if(iflag_vec_mode == WHITE_SURFACE){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(vector_color_vws->vector_color_button), 1);
    } else {
        gtk_drop_down_set_selected(GTK_DROP_DOWN(vector_color_vws->vector_color_button), 0);
    };
    return;
}

GtkWidget * init_psf_vector_color_views(struct kemoviewer_gl_type *kemo_gl){
    struct psf_vector_color_view *vector_color_vws = alloc_psf_vector_color_view();
    
    GtkStringList *vecotor_color_model = gtk_string_list_new(vector_color_list);
    vector_color_vws->vector_color_button = init_vector_color_button(kemo_gl,
                                                                     vecotor_color_model);
    g_object_unref(vecotor_color_model);
    set_psf_vector_color_dropdown(kemo_gl, vector_color_vws);
    
    vector_color_vws->hbox_vec_color = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(vector_color_vws->hbox_vec_color), gtk_label_new("Color: "));
    gtk_box_append(GTK_BOX(vector_color_vws->hbox_vec_color), vector_color_vws->vector_color_button);
    return vector_color_vws;
}

void dealloc_psf_vector_color_view(struct psf_vector_color_view *vector_color_vws){
    g_object_unref(vector_color_vws->vector_color_button);
    g_object_unref(vector_color_vws->hbox_vec_color);
    free(vector_color_vws);
    return;
}