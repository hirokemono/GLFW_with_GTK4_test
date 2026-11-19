/***********************************************************************
 *  kemoview_gtk4_PSF_vec_mode_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_vec_mode_sel.h"

const char * const hd_Full_vect =  "Full";        //  FULL_COMPONENT
const char * const hd_Tangential = "Tangential";  //  TANGENTIAL_COMPONENT

const char * const vector_mode_list[] = {hd_Full_vect,
                                        hd_Tangential,
                                        NULL};


/*    Buttons to set vector mode  */
static int find_selected_vector_mode(const char *text){
    int index_mode = 0;
    if     (kemoview_compare_string((int) strlen(text), text, hd_Full_vect) > 0){
        index_mode = FULL_COMPONENT;
    }else if(kemoview_compare_string((int) strlen(text), text, hd_Tangential) > 0){
        index_mode = TANGENTIAL_COMPONENT;
    }else{
        index_mode = FULL_COMPONENT;
    };
    
    printf("Selected vector mode %d %s\n", index_mode, text);
    return index_mode;
}

static void selected_vector_mode_CB(GtkDropDown *dropdown,
                                    GParamSpec *pspec,
                                    gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int id_vector_mode = find_selected_vector_mode(text);
	kemoview_set_PSF_tangential_vec_mode(id_vector_mode, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
}

static GtkWidget * init_vector_mode_button(struct kemoviewer_gl_type *kemo_gl,
                                           GtkStringList *vector_mode_model){
    GtkWidget *vector_mode_button;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    vector_mode_button = init_list_button_by_list_model(vector_mode_model);
    g_signal_connect(G_OBJECT(vector_mode_button), "notify::selected-item", 
                     G_CALLBACK (selected_vector_mode_CB), G_OBJECT(entry_buf));
    
    return vector_mode_button;
}

static struct psf_vector_mode_view * alloc_psf_vector_mode_view(void){
    struct psf_vector_mode_view *vector_mode_vws;
    vector_mode_vws = (struct psf_vector_mode_view *) malloc(sizeof(struct psf_vector_mode_view));
    if (vector_mode_vws == NULL) {
        printf("malloc error for psf_vector_mode_view\n");
        exit(0);
    }
    return vector_mode_vws;
}



void set_psf_vector_mode_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                  struct psf_vector_mode_view *vector_mode_vws){
    int iflag_vec_mode = kemoview_get_PSF_draw_flags(kemo_gl->kemoview_data, PSFTANVEC_TOGGLE);
	if(iflag_vec_mode == TANGENTIAL_COMPONENT){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(vector_mode_vws->vector_mode_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(vector_mode_vws->vector_mode_button), 0);
	};
    return;
}

GtkWidget * init_psf_vector_mode_views(struct kemoviewer_gl_type *kemo_gl){
    struct psf_vector_mode_view *vector_mode_vws = alloc_psf_vector_mode_view();
    
    GtkStringList *vecotor_mode_model = gtk_string_list_new(vector_mode_list);
    vector_mode_vws->vector_mode_button = init_vector_mode_button(kemo_gl,
                                                                  vecotor_mode_model);
    g_object_unref(vecotor_mode_model);
    set_psf_vector_mode_dropdown(kemo_gl, vector_mode_vws);
    
    vector_mode_vws->hbox_vec_mode = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(vector_mode_vws->hbox_vec_mode), gtk_label_new("Direction: "));
    gtk_box_append(GTK_BOX(vector_mode_vws->hbox_vec_mode), vector_mode_vws->vector_mode_button);
    return vector_mode_vws;
}

void dealloc_psf_vector_mode_view(struct psf_vector_mode_view *vector_mode_vws){
    g_object_unref(vector_mode_vws->vector_mode_button);
    g_object_unref(vector_mode_vws->hbox_vec_mode);
    free(vector_mode_vws);
    return;
}