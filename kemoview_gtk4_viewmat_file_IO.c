/*****************************************************************************************
 *        kemoview_gtk4_viewmat_file_IO.c
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *****************************************************************************************/

#include "kemoview_gtk4_viewmat_file_IO.h"

struct viewmat_file_IO_widgets * init_viewmat_IO_widgets(void){
    struct viewmat_file_IO_widgets *viewmat_IO;
    viewmat_IO = (struct viewmat_file_IO_widgets *) malloc(sizeof(struct viewmat_file_IO_widgets));
    if (viewmat_IO == NULL) {
        printf("malloc error for viewmat_file_IO_widgets\n");
        exit(0);
    }
    return viewmat_IO;
};

void dealloc_viewmat_IO_widgets(struct viewmat_file_IO_widgets *viewmat_IO){
    free(viewmat_IO);
    return;
};


static void kemoview_open_viewmat_CB(GObject *source,
                                     GAsyncResult *result,
                                     gpointer data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    
    GFile *file = gfile_from_kemoview_open_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    kemoview_load_modelview_file(filename, kemo_gl->kemoview_data);
	kemoview_free_kvstring(filename);
    g_object_unref(file);
    draw_full_gl(kemo_gl);
    return;
}

static void kemoview_save_viewmat_CB(GObject *source,
                                     GAsyncResult *result,
                                     gpointer data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    
    GFile *file = gfile_from_kemoview_save_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    kemoview_write_modelview_file(filename, kemo_gl->kemoview_data);
    kemoview_free_kvstring(filename);
    g_object_unref(file);
    return;
};

static void load_viewmatrix_CB(GtkButton *button, gpointer user_data){
    GtkWindow *window = GTK_WINDOW(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    gtk_file_dialog_open(dialog, window, cancellable, 
                         kemoview_open_viewmat_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
    return;
};

static void save_viewmatrix_CB(GtkButton *button, gpointer user_data){
    GtkWindow *window = GTK_WINDOW(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    gtk_file_dialog_save(dialog, window, cancellable,
                         kemoview_save_viewmat_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
    return;
};

GtkWidget * init_viewmatrix_save_box(struct kemoviewer_gl_type *kemo_gl,
                                     struct viewmat_file_IO_widgets *viewmat_IO,
                                     GtkWidget *window){
    GtkWidget *hbox_viewmat_save;
    
    viewmat_IO->entry_buf_viewmat_file = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(viewmat_IO->entry_buf_viewmat_file),
                      "parent", (gpointer) window);
    g_object_set_data(G_OBJECT(viewmat_IO->entry_buf_viewmat_file),
                      "kemoview_gl", (gpointer) kemo_gl);
    
    viewmat_IO->saveView_Button = gtk_button_new_with_label("Save View...");
    g_signal_connect(G_OBJECT(viewmat_IO->saveView_Button), "clicked", 
                     G_CALLBACK(save_viewmatrix_CB), 
                     G_OBJECT(viewmat_IO->entry_buf_viewmat_file));
    viewmat_IO->loadView_Button = gtk_button_new_with_label("Load View...");
    g_signal_connect(G_OBJECT(viewmat_IO->loadView_Button), "clicked", 
                     G_CALLBACK(load_viewmatrix_CB), 
                     G_OBJECT(viewmat_IO->entry_buf_viewmat_file));
    
    hbox_viewmat_save = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(hbox_viewmat_save), 
                   viewmat_IO->saveView_Button);
    gtk_box_append(GTK_BOX(hbox_viewmat_save), 
                   viewmat_IO->loadView_Button);
    
    return hbox_viewmat_save;
}