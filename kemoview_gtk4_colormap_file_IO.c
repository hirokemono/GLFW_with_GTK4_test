/*****************************************************************************************
 *        kemoview_gtk4_colormap_file_IO.c
 *                           Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 *****************************************************************************************/

#include "kemoview_gtk4_colormap_file_IO.h"

static void kemoview_open_colormap_CB(GObject *source,
                                      GAsyncResult *result,
                                      gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *)    g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    struct colormode_menu *cmode_vws
            = (struct colormode_menu *) g_object_get_data(G_OBJECT(user_data), "color_mode");
    struct kemo_real2_columnview *colormap_vws
            = (struct kemo_real2_columnview *) g_object_get_data(G_OBJECT(user_data), "color_map");
    struct kemo_real2_columnview *opacitymap_vws
            = (struct kemo_real2_columnview *) g_object_get_data(G_OBJECT(user_data), "opacity_map");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(user_data), 
                                                                       "cbar_area"));
    
    GFile *file = gfile_from_kemoview_open_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    kemoview_read_colormap_file(filename, *iflag_current_model, kemo_gl->kemoview_data);
    
    init_colormode_dropdown(kemo_gl, *iflag_current_model, cmode_vws);
    reset_colormap_list_store(kemo_gl, *iflag_current_model, 
                              colormap_area, colormap_vws);
    reset_opacitymap_list_store(kemo_gl, iflag_current_model, opacitymap_vws);
	kemoview_free_kvstring(filename);
    g_object_unref(file);
    draw_full_gl(kemo_gl);
    return;
}

static void kemoview_save_colormap_CB(GObject *source,
                                      GAsyncResult *result,
                                      gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    GFile *file = gfile_from_kemoview_save_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    kemoview_write_colormap_file(filename, *iflag_current_model, kemo_gl->kemoview_data);
    kemoview_free_kvstring(filename);
    g_object_unref(file);
    return;
};

static void load_colormap_CB(GtkButton *button, gpointer user_data){
    GtkWindow *window = GTK_WINDOW(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    struct colormode_menu *cmode_vws
            = (struct colormode_menu *) g_object_get_data(G_OBJECT(user_data), "color_mode");
    struct kemo_real2_columnview *colormap_vws
            = (struct kemo_real2_columnview *) g_object_get_data(G_OBJECT(user_data), "color_map");
    struct kemo_real2_columnview *opacitymap_vws
            = (struct kemo_real2_columnview *) g_object_get_data(G_OBJECT(user_data), "opacity_map");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    GtkDrawingArea *colormap_area = GTK_DRAWING_AREA(g_object_get_data(G_OBJECT(user_data), 
                                                                       "cbar_area"));
    
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",   (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "color_mode",    (gpointer) cmode_vws);
    g_object_set_data(G_OBJECT(entry_buf), "color_map",     (gpointer) colormap_vws);
    g_object_set_data(G_OBJECT(entry_buf), "opacity_map",   (gpointer) opacitymap_vws);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer) iflag_current_model);
    g_object_set_data(G_OBJECT(entry_buf), "cbar_area",     (gpointer) colormap_area);
    gtk_file_dialog_open(dialog, window, cancellable, 
                         kemoview_open_colormap_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
    return;
};

static void save_colormap_CB(GtkButton *button, gpointer user_data){
    GtkWindow *window = GTK_WINDOW(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *iflag_current_model =      (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",   (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer) iflag_current_model);
    gtk_file_dialog_save(dialog, window, cancellable,
                         kemoview_save_colormap_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
    return;
};


static struct colormap_file_IO_views * alloc_colormap_IO_widgets(void){
    struct colormap_file_IO_views *colormap_IO;
    colormap_IO = (struct colormap_file_IO_views *) malloc(sizeof(struct colormap_file_IO_views));
    if (colormap_IO == NULL) {
        printf("malloc error for colormap_file_IO_views\n");
        exit(0);
    }
    return colormap_IO;
};

void dealloc_colormap_IO_widgets(struct colormap_file_IO_views *colormap_IO){
    free(colormap_IO);
    return;
};

struct colormap_file_IO_views * init_colormap_save_box(struct kemoviewer_gl_type *kemo_gl,
                                                       int *iflag_current_model,
                                                       GtkDrawingArea *colormap_area,
                                                       struct colormode_menu *colormode_vws,
                                                       struct kemo_real2_columnview *colormap_vws,
                                                       struct kemo_real2_columnview *opacitymap_vws,
                                                       GtkWidget *window){
    struct colormap_file_IO_views *colormap_IO = alloc_colormap_IO_widgets();
    
    colormap_IO->entry_buf_colormap_file = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file),
                      "parent", (gpointer) window);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file),
                      "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file),
                      "color_mode", (gpointer) colormode_vws);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file),
                      "color_map", (gpointer) colormap_vws);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file),
                      "opacity_map", (gpointer) opacitymap_vws);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file),
                      "current_model", (gpointer) iflag_current_model);
    g_object_set_data(G_OBJECT(colormap_IO->entry_buf_colormap_file), 
                      "cbar_area",     (gpointer) colormap_area);
    
    colormap_IO->saveColormap_Button = gtk_button_new_with_label("Save...");
    g_signal_connect(G_OBJECT(colormap_IO->saveColormap_Button), "clicked", 
                     G_CALLBACK(save_colormap_CB), 
                     G_OBJECT(colormap_IO->entry_buf_colormap_file));
    colormap_IO->loadColormap_Button = gtk_button_new_with_label("Load...");
    g_signal_connect(G_OBJECT(colormap_IO->loadColormap_Button), "clicked", 
                     G_CALLBACK(load_colormap_CB), 
                     G_OBJECT(colormap_IO->entry_buf_colormap_file));
    
    colormap_IO->saveButton_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(colormap_IO->saveButton_box), 
                   colormap_IO->saveColormap_Button);
    gtk_box_append(GTK_BOX(colormap_IO->saveButton_box), 
                   colormap_IO->loadColormap_Button);
    
    return colormap_IO;
}
