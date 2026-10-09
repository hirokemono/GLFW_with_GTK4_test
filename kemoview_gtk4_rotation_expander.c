/***********************************************************************
 *  kemoview_gtk4_rotation_expander.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_rotation_expander.h"

/*    Buttons to invoke rotation view or to save rotation movie    */
static void kemoview_save_rot_images_CB(GObject *source,
                                        GAsyncResult *result,
                                        gpointer data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    struct rotation_gtk_menu *rot_gmenu 
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(data), "rotation_menu");
    
    GFile *file = gfile_from_kemoview_save_dialog(GTK_FILE_DIALOG(source), result);
    if(!file) return;
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    struct kv_string *stripped_ext = kemoview_alloc_kvstring();
    struct kv_string *file_prefix = kemoview_alloc_kvstring();
    kemoview_get_ext_from_file_name(filename, file_prefix, stripped_ext);
    
    int id_image = kemoview_set_image_file_format_id(stripped_ext);
    printf("id_format %d %d \n", rot_gmenu->id_fmt_rot, id_image);
    if(id_image < 0) {id_image = rot_gmenu->id_fmt_rot;};
	kemoview_free_kvstring(stripped_ext);
	kemoview_free_kvstring(filename);
    g_object_unref(file);
    if(id_image == 0) return;
    
    sel_write_rotate_views(kemo_gl, id_image, file_prefix,
                           rot_gmenu->i_FPS, rot_gmenu->iaxis_rot, rot_gmenu->inc_deg);
    kemoview_free_kvstring(file_prefix);
    return;
};

static void rotation_save_CB(GtkButton *button, gpointer user_data){
    GtkWidget *window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct rotation_gtk_menu *rot_gmenu 
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)   kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "rotation_menu", (gpointer) rot_gmenu);
    gtk_file_dialog_save(dialog, window, cancellable, 
                         kemoview_save_rot_images_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
    return;
};

static void rotation_view_CB(GtkButton *button, gpointer user_data){
    GtkWidget *window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parent"));
    struct rotation_gtk_menu *rot_gmenu 
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
    draw_rotate_views(kemo_gl, rot_gmenu->iaxis_rot, rot_gmenu->inc_deg, IONE);
    return;
};

GtkWidget * init_rotation_image_save_hbox(struct kemoviewer_gl_type *kemo_gl,
                                          struct rotation_gtk_menu *rot_gmenu,
                                          GtkWidget *window){
    GtkWidget *hbox_rotation_save;
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "parent",        (gpointer) window);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",   (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "rotation_menu", (gpointer) rot_gmenu);
    
    GtkWidget *rotView_Button = gtk_button_new_with_label("View Rotation");
    g_signal_connect(G_OBJECT(rotView_Button), "clicked", 
                     G_CALLBACK(rotation_view_CB), G_OBJECT(entry_buf));
    GtkWidget *rotSave_Button = gtk_button_new_with_label("Save Rotation");
    g_signal_connect(G_OBJECT(rotSave_Button), "clicked", 
                     G_CALLBACK(rotation_save_CB), G_OBJECT(entry_buf));
    
    hbox_rotation_save = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_rotation_save), rotView_Button);
    gtk_box_append(GTK_BOX(hbox_rotation_save), rotSave_Button);
    return hbox_rotation_save;
}



/*   Construct rotation menu expander    */
GtkWidget * init_rotation_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                        struct rotation_gtk_menu *rot_gmenu,
                                        GtkWidget *window){
    GtkWidget *expander_rot;
    
    GtkWidget *entry_rotation_file = gtk_entry_new();
    g_object_set_data(G_OBJECT(entry_rotation_file), "kemoview_gl",  (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_rotation_file), "parent", (gpointer) window);
    g_object_set_data(G_OBJECT(entry_rotation_file), "rotation_menu", (gpointer) rot_gmenu);
    
    
    GtkWidget *hbox_rot_increment =       init_rotation_increment_hbox(rot_gmenu);
    GtkWidget *hbox_rotation_dir =        init_rotation_direction_hbox(kemo_gl, rot_gmenu);
    GtkWidget *hbox_rot_FPS =             init_movie_FPS_hbox(&rot_gmenu->i_FPS);
    GtkWidget *hbox_rotation_fileformat = init_movie_format_hbox(kemo_gl, &rot_gmenu->id_fmt_rot);
    GtkWidget *hbox_rotation_save =       init_rotation_image_save_hbox(kemo_gl, rot_gmenu, window);
    
    GtkWidget *rot_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(rot_box), hbox_rotation_dir);
    gtk_box_append(GTK_BOX(rot_box), hbox_rot_increment);
    gtk_box_append(GTK_BOX(rot_box), hbox_rot_FPS);
    gtk_box_append(GTK_BOX(rot_box), hbox_rotation_fileformat);
    gtk_box_append(GTK_BOX(rot_box), hbox_rotation_save);
    
    expander_rot = wrap_into_scroll_expansion_gtk4("Rotation", 360, 240, window, rot_box);
    return expander_rot;
}
