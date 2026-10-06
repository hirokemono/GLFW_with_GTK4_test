/*
 *  kemoview_gtk4_main_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_main_menu.h"


struct main_buttons * init_main_buttons(struct kemoviewer_type *kemoviewer_data){
	struct main_buttons *mbot = (struct main_buttons *) malloc(sizeof(struct main_buttons));
    if (mbot == NULL) {
        printf("malloc error for main_buttons\n");
        exit(0);
    }

//    mbot->psf_gmenu = alloc_psf_gtk_menu();
//    mbot->fline_gmenu =  (struct fieldline_gtk_menu *) malloc(sizeof(struct fieldline_gtk_menu));
//    mbot->tracer_gmenu = (struct fieldline_gtk_menu *) malloc(sizeof(struct fieldline_gtk_menu));
//    mbot->mesh_vws = (struct kemoview_mesh_view *) malloc(sizeof(struct kemoview_mesh_view));
//    mbot->evo_gmenu = init_evoluaiton_menu_box(kemoviewer_data);

    mbot->view_menu = (struct view_widgets *) malloc(sizeof(struct view_widgets));

    mbot->rot_gmenu = init_rotation_menu_box();
//    mbot->quilt_gmenu = init_quilt_menu_box();
//    mbot->lightparams_vws = init_light_views_4_viewer(kemoviewer_data->kemo_buffers->kemo_lights);
	return mbot;
};

void dealloc_main_buttons(struct main_buttons *mbot){
//    dealloc_psf_gtk_menu(mbot->psf_gmenu);
//    free(mbot->tracer_gmenu);
//    free(mbot->fline_gmenu);
//    free(mbot->mesh_vws);
//    free(mbot->evo_gmenu);

//    dealloc_light_views_4_viewer(mbot->lightparams_vws);
	
	free(mbot->rot_gmenu);
	free(mbot->view_menu);
	
	free(mbot);
	return;
};

static void open_file_CB(GtkButton *button, gpointer *user_data){
    GtkEntryBuffer *entry_buf = GTK_ENTRY_BUFFER(user_data);
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(user_data, "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(user_data, "kemoview_gl");
    
    GtkEntryBuffer *full_path_buf = gtk_entry_buffer_new("", -1);
    kemoview_gtk4_read_file_select(button, main_window, entry_buf, full_path_buf);
    return;
}

static void gtkCopyToClipboard_CB(GtkButton *button, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    struct gl_texure_image *render_image = sel_draw_GLFW_anaglyph_to_rgb(kemo_gl);
    
    struct gl_texure_image *fliped_img = alloc_kemoview_gl_texure();
    alloc_draw_psf_texture(render_image->nipxel_xy[0],
                           render_image->nipxel_xy[1],
                           fliped_img);
    flip_gl_bitmap(render_image->nipxel_xy[0], render_image->nipxel_xy[1],
                   render_image->texure_rgba, fliped_img->texure_rgba);
    
    unsigned int isize = render_image->nipxel_xy[0]*render_image->nipxel_xy[0] * 8 * 3;
    GBytes *bytes = g_bytes_new_static(fliped_img->texure_rgba, isize);
    GdkTexture *texture;
    texture = gdk_memory_texture_new (render_image->nipxel_xy[0],
                                      render_image->nipxel_xy[1],
                                      GDK_MEMORY_B8G8R8,
                                      bytes,
                                      (3*render_image->nipxel_xy[0]));
    
    
    GdkClipboard *clipboard = gtk_widget_get_clipboard (GTK_WIDGET (button));
    gdk_clipboard_set(clipboard, GDK_TYPE_TEXTURE, texture);
    g_bytes_unref(bytes);
    g_object_unref(texture);
    dealloc_kemoview_gl_texure(fliped_img);
    dealloc_kemoview_gl_texure(render_image);
    return;
}

static void image_save_CB(GtkButton *button, gpointer user_data){
    GtkWidget *window = GTK_WIDGET(g_object_get_data(user_data, "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    kemoview_gtk4_save_image_select(kemo_gl, window);
    return;
};


static GtkWidget * make_gtk4_open_file_box(struct kemoviewer_gl_type *kemo_gl,
                                           GtkWidget *main_window,
                                           struct main_buttons *mbot){
    GtkWidget *hbox_open;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    GtkWidget *entry_file = gtk_entry_new_with_buffer(entry_buf);
    g_object_set_data(G_OBJECT(entry_buf), "buttons", (gpointer)  mbot);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "window", (gpointer) main_window);

//    GtkWidget *menuGrid = make_gtk_menu_button(kemo_gl, main_window,
//                                               mbot->lightparams_vws,
//                                               mbot->evo_gmenu);
    
    GtkWidget *open_Button = gtk_button_new_with_label("Open...");
    g_signal_connect(G_OBJECT(open_Button), "clicked",
                     G_CALLBACK(open_file_CB), (gpointer) entry_buf);
    
    hbox_open = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
//    gtk_box_append(GTK_BOX(hbox_open), menuGrid, TRUE, TRUE, 0);
    gtk_box_append(GTK_BOX(hbox_open), gtk_label_new("File: "));
    gtk_box_append(GTK_BOX(hbox_open), entry_file);
    gtk_box_append(GTK_BOX(hbox_open), open_Button);
    return hbox_open;
}


static GtkWidget * make_gtk4_save_file_box(GtkWidget *quitButton,
                                           GtkWidget *main_window,
                                           struct kemoviewer_gl_type *kemo_gl){
    GtkWidget *savebox;
    GtkEntryBuffer *entry_buf_save_file = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf_save_file), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf_save_file), "window", (gpointer) main_window);
    
    GtkWidget *imageSave_Button = gtk_button_new_with_label("Save Image...");
    g_signal_connect(G_OBJECT(imageSave_Button), "clicked",
                     G_CALLBACK(image_save_CB), (gpointer) entry_buf_save_file);
    
    GtkEntryBuffer *entry_buf_Copy = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf_Copy), "kemoview_gl", (gpointer) kemo_gl);
    GtkWidget *copyButton = gtk_button_new_with_label("Copy");
    g_signal_connect(G_OBJECT(copyButton), "clicked",
                     G_CALLBACK(gtkCopyToClipboard_CB), entry_buf_Copy);
    
    savebox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(savebox), imageSave_Button);
    gtk_box_append(GTK_BOX(savebox), copyButton);
    gtk_box_append(GTK_BOX(savebox), quitButton);
    return savebox;
};

GtkWidget * make_gtk4_main_menu_box(struct main_buttons *mbot,
                                    GtkWidget *quitButton, GtkWidget *main_window,
                                    struct kemoviewer_gl_type *kemo_gl){
    GtkWidget *vbox_menu;
    
    GtkWidget *hbox_open = make_gtk4_open_file_box(kemo_gl, main_window, mbot);
    GtkWidget *savebox = make_gtk4_save_file_box(quitButton, main_window, kemo_gl);
    
    GtkWidget *hbox_viewtype = make_gtk4_viewmode_menu_box(kemo_gl, mbot->view_menu);
//    GtkWidget *hbox_axis = make_axis_menu_box(kemo_gl, main_window);
    GtkWidget *expander_rot = init_rotation_menu_expander(kemo_gl, mbot->rot_gmenu,
                                                          main_window);
//    mbot->itemTEvo = init_evolution_menu_expander(kemo_gl, mbot->evo_gmenu, main_window);
    
    mbot->expander_view = init_viewmatrix_menu_expander(kemo_gl, mbot->view_menu,
                                                        main_window);
//    mbot->expander_quilt = init_quilt_menu_expander(kemo_gl, mbot->quilt_gmenu,
//                                                    mbot->view_menu, main_window);
    
    vbox_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox_menu), hbox_open);
    gtk_box_append(GTK_BOX(vbox_menu), savebox);
    gtk_box_append(GTK_BOX(vbox_menu), hbox_viewtype);
//    gtk_box_append(GTK_BOX(vbox_menu), hbox_axis);
    gtk_box_append(GTK_BOX(vbox_menu), expander_rot);
//    gtk_box_append(GTK_BOX(vbox_menu), mbot->itemTEvo);
//    gtk_box_append(GTK_BOX(vbox_menu), mbot->expander_quilt);
    gtk_box_append(GTK_BOX(vbox_menu), mbot->expander_view);
    return vbox_menu;
}

