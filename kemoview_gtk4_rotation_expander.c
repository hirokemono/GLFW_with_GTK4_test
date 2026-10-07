/***********************************************************************
 *  kemoview_gtk4_rotation_expander.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_rotation_expander.h"


const char * const hd_NoImage =   "No Image";  //  NO_SAVE_FILE
const char * const hd_PNGimage =  "PNG";       //  SAVE_PNG
const char * const hd_BMPimage =  "BMP";       //  SAVE_BMP
const char * const hd_Movie =     "Movie";     //  SAVE_QT_MOVIE

const char * const image_fmt_list[] = {hd_NoImage,
                                       hd_PNGimage,
                                       hd_BMPimage,
#ifdef FFMPEG
                                       hd_Movie,
#endif
                                       NULL};



/*    Buttons to set rotation movie image format  */
static void init_rotation_file_fmt_dropdown(struct rotation_gtk_menu *rot_gmenu,
                                            GtkWidget *rot_file_fmt_button){
	if(rot_gmenu->id_fmt_rot == SAVE_BMP){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(rot_file_fmt_button), 2);
	} else if(rot_gmenu->id_fmt_rot == SAVE_PNG){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(rot_file_fmt_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(rot_file_fmt_button), 0);
	};
    return;
}

static int find_selected_rot_file_format(const char *text){
    int index_mode = -100;
    if(kemoview_compare_string((int) strlen(text), text, hd_Movie) > 0){index_mode = SAVE_QT_MOVIE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_PNGimage) > 0){index_mode = SAVE_PNG;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_BMPimage) > 0){index_mode = SAVE_BMP;}
    else {index_mode = NO_SAVE_FILE;};
    return index_mode;
}

static void selected_rot_file_fmt_CB(GtkDropDown *dropdown,
                                     GParamSpec *pspec,
                                     gpointer user_data)
{
    struct rotation_gtk_menu *rot_gmenu
            = (struct rotation_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "rotation_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    rot_gmenu->id_fmt_rot = find_selected_rot_file_format(text);
    
    return;
}


GtkWidget * init_rotation_image_format_hbox(struct kemoviewer_gl_type *kemo_gl,
                                            struct rotation_gtk_menu *rot_gmenu){
    GtkWidget *hbox_rotation_fileformat;
    
    GtkStringList *rot_file_fmt_model = gtk_string_list_new(image_fmt_list);
    GListStore *rot_file_fmt_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(rot_file_fmt_store, rot_file_fmt_model);
    g_object_unref(rot_file_fmt_model);
    GtkFlattenListModel *rot_file_fmt_flat = gtk_flatten_list_model_new(G_LIST_MODEL(rot_file_fmt_store));
    GtkExpression *rot_file_fmt_expression = gtk_property_expression_new (GTK_TYPE_STRING_OBJECT,
                                                                          NULL,
                                                                          "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "rotation_menu", (gpointer) rot_gmenu);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    
    GtkWidget *rot_file_fmt_button = gtk_drop_down_new(G_LIST_MODEL(rot_file_fmt_flat),
                                                       rot_file_fmt_expression);
    g_signal_connect(G_OBJECT(rot_file_fmt_button), "notify::selected-item", 
                     G_CALLBACK (selected_rot_file_fmt_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *rot_file_fmt_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(rot_file_fmt_button), rot_file_fmt_factory);
//    g_object_unref(rot_file_fmt_expression);       Do not release GtkExpression!!
    g_object_unref(rot_file_fmt_factory);
    g_object_unref(rot_file_fmt_flat);
    init_rotation_file_fmt_dropdown(rot_gmenu, rot_file_fmt_button);
    
    hbox_rotation_fileformat = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_rotation_fileformat), gtk_label_new("File format: "));
	gtk_box_append(GTK_BOX(hbox_rotation_fileformat), rot_file_fmt_button);
    return hbox_rotation_fileformat;
}


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
    
    rot_gmenu->rotView_Button = gtk_button_new_with_label("View Rotation");
    g_signal_connect(G_OBJECT(rot_gmenu->rotView_Button), "clicked", 
                     G_CALLBACK(rotation_view_CB), G_OBJECT(entry_buf));
    rot_gmenu->rotSave_Button = gtk_button_new_with_label("Save Rotation");
    g_signal_connect(G_OBJECT(rot_gmenu->rotSave_Button), "clicked", 
                     G_CALLBACK(rotation_save_CB), G_OBJECT(entry_buf));
    
    hbox_rotation_save = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_rotation_save), rot_gmenu->rotView_Button);
    gtk_box_append(GTK_BOX(hbox_rotation_save), rot_gmenu->rotSave_Button);
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
    GtkWidget *hbox_rot_FPS =             init_rotation_movie_FPS_hbox(rot_gmenu);
    GtkWidget *hbox_rotation_dir =        init_rotation_direction_hbox(kemo_gl, rot_gmenu);
    GtkWidget *hbox_rotation_fileformat = init_rotation_image_format_hbox(kemo_gl, rot_gmenu);
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
