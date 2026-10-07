/*
 *  kemoview_gtk4_fileselector.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_fileselector.h"
#include "kemoviewer.h"
#include "kemoviewer_gl.h"
//#include "tree_view_4_colormap.h"
#include "view_modifier_glfw.h"
//#include "kemoview_gtk_PSF_surface_menu.h"
#include "kemoview_gtk4_main_menu.h"

static const gchar *gtk_selected_filename;

/*
   Constract input windows
*/
void open_kemoviewer_file_glfw(struct kemoviewer_gl_type *kemo_gl,
                               struct kv_string *filename,
                               struct main_buttons *mbot,
                               GtkWidget *main_window){
    struct kv_string *file_prefix = kemoview_alloc_kvstring();
    struct kv_string *stripped_ext = kemoview_alloc_kvstring();
	int iflag_datatype = kemoview_set_data_format_flag(filename, file_prefix, stripped_ext);
	
	printf("file name: %s\n", filename->string);
	printf("file_prefix %s\n", file_prefix->string);
	printf("stripped_ext %s\n", stripped_ext->string);
    kemoview_free_kvstring(stripped_ext);
    kemoview_free_kvstring(file_prefix);
	
	iflag_datatype = kemoview_open_data(filename, kemo_gl->kemoview_data);
    kemoview_free_kvstring(filename);
    
    /*
    init_psf_window(kemo_gl, mbot->psf_gmenu, main_window,    mbot->itemTEvo);
    init_tracer_window(kemo_gl, mbot->tracer_gmenu, main_window, mbot->itemTEvo);
    init_fline_window(kemo_gl, mbot->fline_gmenu,  main_window, mbot->itemTEvo);
    init_mesh_window(kemo_gl, mbot->mesh_vws,  main_window, mbot->meshWin);
    
    if(iflag_datatype == 0 || iflag_datatype == IFLAG_MESH){
        mbot->evo_gmenu->istart_evo = 1;
        mbot->evo_gmenu->iend_evo =   1;
    }else{
        mbot->evo_gmenu->istart_evo = kemo_gl->kemoview_data->istep_evo;
        mbot->evo_gmenu->iend_evo =   kemo_gl->kemoview_data->istep_evo;
    }
    printf("kemo_gl->kemoview_data->istep_evo %d\n", kemo_gl->kemoview_data->istep_evo);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(mbot->evo_gmenu->spin_evo_start), (double) mbot->evo_gmenu->istart_evo);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(mbot->evo_gmenu->spin_evo_end),   (double) mbot->evo_gmenu->iend_evo);
    activate_evolution_menu(kemo_gl->kemoview_data, mbot->itemTEvo);
    */
    draw_full_gl(kemo_gl);
	return;
};

void save_kemoview_image_file(struct kemoviewer_gl_type *kemo_gl, 
                              struct kv_string *filename){
    int i_quilt;
    int iflag_quilt = kemoview_get_quilt_nums(kemo_gl->kemoview_data, ISET_QUILT_MODE);
    int npix_x = kemoview_get_view_integer(kemo_gl->kemoview_data, ISET_PIXEL_X);
    int npix_y = kemoview_get_view_integer(kemo_gl->kemoview_data, ISET_PIXEL_Y);
    unsigned char *image = kemoview_alloc_RGB_buffer_to_bmp(npix_x, npix_y);

    struct kv_string *stripped_ext = kemoview_alloc_kvstring();
    struct kv_string *file_prefix = kemoview_alloc_kvstring();
    
    kemoview_get_ext_from_file_name(filename, file_prefix, stripped_ext);
    int id_imagefmt_by_input = kemoview_set_image_file_format_id(stripped_ext);
    if(id_imagefmt_by_input < 0) {
        id_imagefmt_by_input = kemoview_get_view_integer(kemo_gl->kemoview_data,
                                                         IMAGE_FORMAT_FLAG);;
        kemoview_free_kvstring(file_prefix);
        file_prefix = kemoview_init_kvstring_by_string(filename->string);
    };
    if(id_imagefmt_by_input == 0) return;
    kemoview_free_kvstring(filename);
    kemoview_free_kvstring(stripped_ext);
    
    printf("header: %s\n", file_prefix->string);
    if(iflag_quilt == 0){
        struct gl_texure_image *image_t = sel_draw_GLFW_buffer_to_rgb(kemo_gl);
        kemoview_write_window_to_file(id_imagefmt_by_input, file_prefix,
                                      image_t->nipxel_xy[0], image_t->nipxel_xy[1],
                                      image_t->texure_rgba);
        dealloc_kemoview_gl_texure(image_t);
    } else {
        int nimg_column = kemoview_get_quilt_nums(kemo_gl->kemoview_data,
                                                  ISET_QUILT_COLUMN);
        int nimg_raw =    kemoview_get_quilt_nums(kemo_gl->kemoview_data,
                                                  ISET_QUILT_RAW);
        unsigned char *quilt_image = kemoview_alloc_RGB_buffer_to_bmp((nimg_column * npix_x),
                                                                      (nimg_raw * npix_y));
        for(i_quilt=0;i_quilt<(nimg_column*nimg_raw);i_quilt++){
            draw_quilt(i_quilt, kemo_gl);
            kemoview_add_quilt_img(i_quilt, kemo_gl->kemoview_data,
                                   kemo_gl->kemo_VAOs, kemo_gl->kemo_shaders,
                                   quilt_image);
       };
        kemoview_write_window_to_file(id_imagefmt_by_input, file_prefix,
                                      (nimg_column * npix_x),
                                      (nimg_raw * npix_y), quilt_image);
        free(quilt_image);
        printf("quilt! %d x %d\n", nimg_column, nimg_raw);
        draw_full_gl(kemo_gl);
    }
    free(image);
    kemoview_free_kvstring(file_prefix);
    
    return;
};

int load_texture_file_gtk4(GtkWidget *window, struct kv_string *file_prefix){
    struct kv_string *stripped_ext;
	int id_img;
	struct kv_string *filename= kemoview_read_file_panel(window);
	
	if(filename->string[0] == '\0') return 0;
	
    stripped_ext = kemoview_alloc_kvstring();
	kemoview_get_ext_from_file_name(filename, file_prefix, stripped_ext);
	
	id_img = kemoview_set_image_file_format_id(stripped_ext);
    kemoview_free_kvstring(stripped_ext);
    kemoview_free_kvstring(filename);
	return id_img;
}


GFile * gfile_from_kemoview_open_dialog(GtkFileDialog *dialog,
                                        GAsyncResult *result){
    GError *error = NULL;
    GFile *file = gtk_file_dialog_open_finish(dialog, result, &error);
    if(!file){
        g_print ("%s\n", error->message);
        g_error_free(error);
    }
    return file;
}

GFile * gfile_from_kemoview_save_dialog(GtkFileDialog *dialog,
                                        GAsyncResult *result){
    GError *error = NULL;
    GFile *file = gtk_file_dialog_save_finish(dialog, result, &error);
    if(!file){
        g_print ("%s\n", error->message);
        g_error_free(error);
    }
    return file;
}


void save_viewmatrix_CB(GObject *source,
                        GAsyncResult *result,
                        void *data){
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
};


/*
static void load_colormap_file_CB(GObject *source,
                                  GAsyncResult *result,
                                  gpointer data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    struct colormap_view *color_vws
            = (struct colormap_view *) g_object_get_data(G_OBJECT(data), "colormap_view");
    
    GFile *file = gfile_from_kemoview_open_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    kemoview_read_colormap_file(filename, color_vws->iflag_current_model,
                                kemo_gl->kemoview_data);
	kemoview_free_kvstring(filename);
    g_object_unref(file);
	
	gtk_widget_queue_draw(main_window);
    draw_full_gl(kemo_gl);
    return;
};
*/
void load_viewmatrix_CB(GObject *source,
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
};

void load_texture_file_CB(GObject *source,
                          GAsyncResult *result,
                          gpointer data){
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    
    GFile *file = gfile_from_kemoview_open_dialog(GTK_FILE_DIALOG (source), result);
    
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    struct kv_string *image_prefix = kemoview_alloc_kvstring();
    struct kv_string *stripped_ext = kemoview_alloc_kvstring();
	kemoview_get_ext_from_file_name(filename, image_prefix, stripped_ext);
	
	int id_img = kemoview_set_image_file_format_id(stripped_ext);
    kemoview_free_kvstring(stripped_ext);
    kemoview_free_kvstring(filename);
	
    if(!file) return;
	int id_image = load_texture_file_gtk4(main_window, image_prefix);
	if(id_image == SAVE_PNG || id_image == SAVE_BMP){
        kemoview_release_PSF_gl_texture(kemo_gl->kemoview_data, kemo_gl);
        kemoview_update_PSF_textured_id(kemo_gl->kemoview_data);
		kemoview_set_texture_to_PSF(id_image, image_prefix, kemo_gl->kemoview_data, kemo_gl);
        kemoview_set_PSF_patch_color_mode(TEXTURED_SURFACE, kemo_gl->kemoview_data);
	};
    kemoview_free_kvstring(image_prefix);
	
    g_object_unref(file);
    draw_full_gl(kemo_gl);
    return;
};

static void kemoview_file_open_CB(GObject *source,
                                  GAsyncResult *result,
                                  gpointer data){
	GtkEntryBuffer *entry_buf = GTK_ENTRY_BUFFER(data);
	GtkEntryBuffer *full_path_buf = GTK_ENTRY_BUFFER(g_object_get_data(G_OBJECT(data), "full_path"));
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    struct main_buttons *mbot = (struct main_buttons *) g_object_get_data(G_OBJECT(data), "buttons");
    
    GFile *file = gfile_from_kemoview_open_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    gtk_entry_buffer_set_text(full_path_buf, g_file_get_path(file), 
                              strlen(g_file_get_path(file))+1);
    gtk_entry_buffer_set_text(entry_buf, g_file_get_basename(file), 
                              strlen(g_file_get_basename(file))+1);
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    open_kemoviewer_file_glfw(kemo_gl, filename, mbot, main_window);
	kemoview_free_kvstring(filename);
    g_object_unref(file);
    draw_full_gl(kemo_gl);
    printf("full path: %s \n", gtk_entry_buffer_get_text(entry_buf));
    printf("Open file: %s \n", gtk_entry_buffer_get_text(full_path_buf));
    return;
}

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

static void kemoview_file_save_CB(GObject *source,
                                  GAsyncResult *result,
                                  gpointer data){
	GtkEntryBuffer *entry_buf = GTK_ENTRY_BUFFER(data);
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    struct main_buttons *mbot = (struct main_buttons *) g_object_get_data(G_OBJECT(data), "buttons");
    
    GFile *file = gfile_from_kemoview_save_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    gtk_entry_buffer_set_text(entry_buf, g_file_get_basename(file), 
                              strlen(g_file_get_basename(file))+1);
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    open_kemoviewer_file_glfw(kemo_gl, filename, mbot, main_window);
	kemoview_free_kvstring(filename);
    g_object_unref(file);
    return;
}

void kemoview_gtk4_read_file_select(GtkButton *button, GtkWindow *window,
                                    GtkEntryBuffer *entry_buf,
                                    GtkEntryBuffer *full_path_buf){
	/* generate file selection widget*/
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    g_object_set_data(G_OBJECT(entry_buf), "full_path", G_OBJECT(full_path_buf));
    gtk_file_dialog_open(dialog, window, cancellable, 
                         kemoview_file_open_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
	return;
}

static void kemoview_save_image_CB(GObject *source,
                                  GAsyncResult *result,
                                  gpointer data){
	GtkEntryBuffer *entry_buf = GTK_ENTRY_BUFFER(data);
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(data), "kemoview_gl");
    struct main_buttons *mbot = (struct main_buttons *) g_object_get_data(G_OBJECT(data), "buttons");
    
    GFile *file = gfile_from_kemoview_save_dialog(GTK_FILE_DIALOG (source), result);
    
    if(!file) return;
    gtk_entry_buffer_set_text(entry_buf, g_file_get_basename(file), 
                              strlen(g_file_get_basename(file))+1);
    
    struct kv_string *filename = kemoview_init_kvstring_by_string(g_file_get_path(file));
    save_kemoview_image_file(kemo_gl, filename);
    kemoview_free_kvstring(filename);
    g_object_unref(file);
    return;
};

void kemoview_gtk4_save_image_select(struct kemoviewer_gl_type *kemo_gl, 
                                     GtkWindow *window){
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    gtk_file_dialog_save(dialog, window, cancellable, kemoview_save_image_CB, entry_buf);
    g_object_unref(filter);
    g_object_unref(cancellable);
	return;
}


void kemoview_gtk4_read_viewmatrix_select(struct kemoviewer_gl_type *kemo_gl, 
                                          GtkWindow *window){
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
}

void kemoview_gtk4_save_viewmatrix_select(struct kemoviewer_gl_type *kemo_gl, 
                                          GtkWindow *window){
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    gtk_file_dialog_save(dialog, window, cancellable, kemoview_save_viewmat_CB, G_OBJECT(entry_buf));
    g_object_unref(filter);
    g_object_unref(cancellable);
	return;
}

void kemoview_gtk4_save_file_select(GtkButton *button, gpointer data){
    GtkWindow *window = GTK_WINDOW(g_object_get_data(G_OBJECT(data), "parent"));
	
	/* generate file selection widget*/
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    GCancellable *cancellable = g_cancellable_new();
    g_cancellable_cancel(cancellable);
    
    gtk_file_dialog_save(dialog, window, cancellable, kemoview_file_save_CB, data);
    g_object_unref(filter);
    g_object_unref(cancellable);
	return;
}

struct kv_string * kemoview_read_file_panel(GtkWidget *window_cmap){
    GtkEntryBuffer *entry_buf =     gtk_entry_buffer_new("", -1);
    GtkEntryBuffer *full_path_buf = gtk_entry_buffer_new("", -1);
	GtkWidget *entry = gtk_entry_new_with_buffer(entry_buf);
	kemoview_gtk4_read_file_select(NULL, window_cmap, entry_buf, full_path_buf);
	struct kv_string *filename = kemoview_init_kvstring_by_string(gtk_selected_filename);
	return filename;
};
struct kv_string * kemoview_save_file_panel(GtkWidget *window_cmap){
	GtkWidget *entry = gtk_entry_new();
	g_object_set_data(G_OBJECT(entry), "parent", (gpointer) window_cmap);
	kemoview_gtk4_save_file_select(NULL, G_OBJECT(entry));
	struct kv_string *filename = kemoview_init_kvstring_by_string(gtk_selected_filename);
	return filename;
};
