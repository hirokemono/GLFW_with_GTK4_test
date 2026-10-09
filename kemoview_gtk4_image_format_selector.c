/***********************************************************************
 *  kemoview_gtk4_image_format_selector.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_image_format_selector.h"

const char * const hd_NoImage =   "No Image";  //  NO_SAVE_FILE
const char * const hd_PNGimage =  "PNG";       //  SAVE_PNG
const char * const hd_BMPimage =  "BMP";       //  SAVE_BMP
const char * const hd_Movie =     "Movie";     //  SAVE_QT_MOVIE

const char * const image_fmt_list[] = {hd_NoImage,
                                       hd_PNGimage,
                                       hd_BMPimage,
                                       NULL};

const char * const movie_fmt_list[] = {hd_NoImage,
                                       hd_PNGimage,
                                       hd_BMPimage,
#ifdef FFMPEG
                                       hd_Movie,
#endif
                                       NULL};


/*    Buttons to set rotation movie image format  */
static void init_image_format_dropdown(int id_image_format,
                                       GtkWidget *file_format_button){
	if(id_image_format == SAVE_BMP){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(file_format_button), 2);
	} else if(id_image_format == SAVE_PNG){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(file_format_button), 1);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(file_format_button), 0);
	};
    return;
}

static int find_selected_movie_format(const char *text){
    int index_mode = -100;
    if(kemoview_compare_string((int) strlen(text), text, hd_Movie) > 0){index_mode = SAVE_QT_MOVIE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_PNGimage) > 0){index_mode = SAVE_PNG;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_BMPimage) > 0){index_mode = SAVE_BMP;}
    else {index_mode = NO_SAVE_FILE;};
    return index_mode;
}

static void selected_movie_format_CB(GtkDropDown *dropdown,
                                     GParamSpec *pspec,
                                     gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_image_format = (int *) g_object_get_data(G_OBJECT(user_data), "image_format");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    *id_image_format = find_selected_movie_format(text);
    kemoview_set_view_integer(IMAGE_FORMAT_FLAG, *id_image_format,
                              kemo_gl->kemoview_data);
    return;
}


static GtkWidget * init_image_format_button(struct kemoviewer_gl_type *kemo_gl,
                                            GtkStringList *image_format_model, 
                                            int *id_image_format){
    GtkWidget *image_format_button;
    
    GListStore *image_format_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(image_format_store, image_format_model);
    GtkFlattenListModel *image_format_flat = gtk_flatten_list_model_new(G_LIST_MODEL(image_format_store));
    GtkExpression *image_format_expression = gtk_property_expression_new(GTK_TYPE_STRING_OBJECT,
                                                                         NULL,
                                                                         "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "image_format", (gpointer) id_image_format);
    
    image_format_button = gtk_drop_down_new(G_LIST_MODEL(image_format_flat),
                                            image_format_expression);
    g_signal_connect(G_OBJECT(image_format_button), "notify::selected-item", 
                     G_CALLBACK (selected_movie_format_CB), G_OBJECT(entry_buf));
    
    GtkListItemFactory *rot_file_fmt_factory = gtk_signal_list_item_factory_new ();
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN(image_format_button), rot_file_fmt_factory);
//    g_object_unref(image_format_expression);       Do not release GtkExpression!!
    g_object_unref(rot_file_fmt_factory);
    g_object_unref(image_format_flat);
    init_image_format_dropdown(*id_image_format, image_format_button);
    
    return image_format_button;
}


GtkWidget * init_image_format_hbox(struct kemoviewer_gl_type *kemo_gl,
                                   int *id_image_format){
    GtkWidget *hbox_image_format;
    
    GtkStringList *movie_format_model = gtk_string_list_new(image_fmt_list);
    GtkWidget *image_format_button = init_image_format_button(kemo_gl,
                                                              movie_format_model, 
                                                              id_image_format);
    g_object_unref(movie_format_model);
    
    hbox_image_format = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_image_format), gtk_label_new("Image Format: "));
    gtk_box_append(GTK_BOX(hbox_image_format), image_format_button);
    return hbox_image_format;
}

GtkWidget * init_movie_format_hbox(struct kemoviewer_gl_type *kemo_gl,
                                   int *id_image_format){
    GtkWidget *hbox_image_format;
    
    GtkStringList *movie_format_model = gtk_string_list_new(movie_fmt_list);
    GtkWidget *image_format_button = init_image_format_button(kemo_gl,
                                                              movie_format_model, 
                                                              id_image_format);
    g_object_unref(movie_format_model);
    
    hbox_image_format = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_image_format), gtk_label_new("Movie Format: "));
    gtk_box_append(GTK_BOX(hbox_image_format), image_format_button);
    return hbox_image_format;
}


/*    box to set movie FPS   */

static void movie_FPS_CB(GtkWidget *entry, gpointer user_data){
    int *i_FPS = (int *) g_object_get_data(G_OBJECT(user_data), "movie_FPS");
    *i_FPS = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
/*    printf("FPS %d\n", *i_FPS);*/
}

GtkWidget * init_movie_FPS_hbox(int *i_FPS){
    GtkWidget *hbox_rot_FPS;
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "movie_FPS", (gpointer) i_FPS);
    
    GtkAdjustment *adj_rot_FPS = gtk_adjustment_new(*i_FPS, 1, 180, 1, 1, 0.0);
    GtkWidget *spin_rot_FPS = gtk_spin_button_new(GTK_ADJUSTMENT(adj_rot_FPS), 0, 1);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(spin_rot_FPS), 0);
    g_signal_connect(G_OBJECT(spin_rot_FPS), "value-changed",
                     G_CALLBACK(movie_FPS_CB), G_OBJECT(entry_buf));
    
    hbox_rot_FPS = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_rot_FPS), gtk_label_new("FPS for movie: "));
    gtk_box_append(GTK_BOX(hbox_rot_FPS), spin_rot_FPS);
    return hbox_rot_FPS;
}

