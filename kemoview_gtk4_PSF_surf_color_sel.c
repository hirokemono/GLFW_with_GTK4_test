/***********************************************************************
 *  kemoview_gtk4_PSF_surf_color_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_surf_color_sel.h"

static struct psf_surface_color_view * alloc_psf_surface_gtk_menu(void){
    struct psf_surface_color_view *psf_sf_color_vws;
    psf_sf_color_vws = (struct psf_surface_color_view *) malloc(sizeof(struct psf_surface_color_view));
    if(psf_sf_color_vws == NULL) {
        printf("malloc error for psf_surface_color_view\n");
        exit(0);
    }
    return psf_sf_color_vws;
}

static void surface_colormode_CB(GtkDropDown *dropdown,
                                 GParamSpec *pspec,
                                 gpointer user_data){
    GtkWidget *psf_color_selector = GTK_WIDGET(user_data);
    GtkWidget *parent_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model =         (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    
    if(*id_current_model == SURFACE_RENDERING){
        int index_mode = find_surface_color_mode(text);
        set_psf_surface_color_mode(index_mode, parent_window, 
                                   psf_color_selector, kemo_gl);
    }else{
        int index_mode = find_fline_color_mode(text);
        set_fline_surface_color_mode(index_mode, parent_window, 
                                     psf_color_selector,
                                     kemo_gl, *id_current_model);
    };
    draw_double_gl(kemo_gl);
    return;
};

static void surface_colorsel_CB(GObject    *object,
                                GParamSpec *pspec,
                                gpointer    user_data){
    GtkWidget *psf_color_selector = GTK_WIDGET(object);
    GtkWidget *parent_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model =         (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    float fcolor[4];
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON(psf_color_selector);
    const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
    
    set_RGB_from_GTK(rgba, fcolor);
    fcolor[3] = (float) kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                       *id_current_model,
                                                       ISET_OPACITY_MAX);
    kemoview_set_VIZ_single_color(fcolor,
                                  *id_current_model,
                                  kemo_gl->kemoview_data);
    
    draw_double_gl(kemo_gl);
    gtk_widget_queue_draw(parent_window);
    return;
};

void set_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_surface_color_view *psf_sf_color_vws){
    int iflag_sfcolor = kemoview_get_VIZ_patch_color_mode(kemo_gl->kemoview_data,
                                                          SURFACE_RENDERING);
    init_psf_surf_colmode_dropdown(iflag_sfcolor, psf_sf_color_vws->listbutton_sfcolor);
};
void set_gtk4_fline_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                   int id_color_mode,
                                   struct psf_surface_color_view *psf_sf_color_vws){
    int iflag_sfcolor = kemoview_get_VIZ_patch_color_mode(kemo_gl->kemoview_data,
                                                          id_color_mode);
    init_fline_colmode_dropdown(iflag_sfcolor, psf_sf_color_vws->listbutton_sfcolor);
};

struct psf_surface_color_view * init_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                             int *id_current_model,
                                                             GtkWidget *parent_window){
    struct psf_surface_color_view *psf_sf_color_vws = alloc_psf_surface_gtk_menu();
    
    GdkRGBA bg_rgba;
    float color[4];
    
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha(color_dialog, TRUE); // Allow transparency adjustments
    
    if(*id_current_model == SURFACE_RENDERING){
        psf_sf_color_vws->listbutton_sfcolor = init_PSF_sfcolor_listbutton();
    }else{
        psf_sf_color_vws->listbutton_sfcolor = init_Fline_color_listbutton();
    };
    psf_sf_color_vws->psf_color_selector = gtk_color_dialog_button_new(color_dialog);
    
    kemoview_get_background_color(kemo_gl->kemoview_data, color);
    set_RGB_to_GTK(color, &bg_rgba);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(psf_sf_color_vws->psf_color_selector),
                                     &bg_rgba);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "current_model", (gpointer)  id_current_model);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  parent_window);
    
    g_signal_connect(G_OBJECT(psf_sf_color_vws->listbutton_sfcolor), "notify::selected-item", 
                     G_CALLBACK (surface_colormode_CB), G_OBJECT(entry_buf));
    g_signal_connect (psf_sf_color_vws->psf_color_selector, "notify::rgba", 
                      G_CALLBACK(surface_colorsel_CB), G_OBJECT(entry_buf));
    
    psf_sf_color_vws->hbox_sf_color = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(psf_sf_color_vws->hbox_sf_color), gtk_label_new("Color: "));
    gtk_box_append(GTK_BOX(psf_sf_color_vws->hbox_sf_color), psf_sf_color_vws->listbutton_sfcolor);
    gtk_box_append(GTK_BOX(psf_sf_color_vws->hbox_sf_color), psf_sf_color_vws->psf_color_selector);
    
    return psf_sf_color_vws;
}

void dealloc_psf_surf_color_views(struct psf_surface_color_view *psf_sf_color_vws){
    g_object_unref(psf_sf_color_vws->listbutton_sfcolor);
    g_object_unref(psf_sf_color_vws->psf_color_selector);
    g_object_unref(psf_sf_color_vws->hbox_sf_color);
    free(psf_sf_color_vws);
    return;
};