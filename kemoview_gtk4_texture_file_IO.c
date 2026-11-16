/*****************************************************************************************
 *        kemoview_gtk4_texture_file_IO.c
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *****************************************************************************************/

#include "kemoview_gtk4_texture_file_IO.h"

static void kemoview_open_texture_CB(GObject *source,
                                     GAsyncResult *result,
                                     gpointer data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    
    GFile *file = gfile_from_kemoview_open_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    g_object_unref(file);
    
    struct kv_string *image_prefix = kemoview_alloc_kvstring();
    struct kv_string *stripped_ext = kemoview_alloc_kvstring();
	kemoview_get_ext_from_file_name(filename, image_prefix, stripped_ext);
	int id_image = kemoview_set_image_file_format_id(stripped_ext);
    
    if(id_image == SAVE_PNG || id_image != SAVE_BMP){
        kemoview_release_PSF_gl_texture(kemo_gl->kemoview_data, kemo_gl);
        kemoview_update_PSF_textured_id(kemo_gl->kemoview_data);
		kemoview_set_texture_to_PSF(id_image, image_prefix, kemo_gl->kemoview_data, kemo_gl);
        kemoview_set_PSF_patch_color_mode(TEXTURED_SURFACE, kemo_gl->kemoview_data);
    };
    kemoview_free_kvstring(stripped_ext);
    kemoview_free_kvstring(image_prefix);
    kemoview_free_kvstring(filename);
    
    draw_full_gl(kemo_gl);
    return;
}

void open_texture_load_dialog(struct kemoviewer_gl_type *kemo_gl, GtkWindow *window){
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    gtk_file_dialog_open(dialog, window, cancellable, 
                         kemoview_open_texture_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
    return;
};
