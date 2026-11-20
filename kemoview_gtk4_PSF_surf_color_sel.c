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

const char * const hd_contour =   "Contour";       //  RAINBOW_PSF_SURF
const char * const hd_white =     "White";         //  WHITE_SURFACE
const char * const hd_single =    "Single color";  //  SINGLE_COLOR
const char * const hd_changeclr = "Change color";  //  CHANGE_PSF_COLOR
const char * const hd_texture =   "Texture";       //  TEXTURE_PSF_SURF

const char * const psf_sfcolor_list[] = {hd_contour,
                                         hd_white,
                                         hd_single,
/*                                         hd_changeclr, */
                                         hd_texture,
                                         NULL};

GtkWidget *window_csel;

static struct psf_surface_color_view * alloc_psf_surface_gtk_menu(void){
    struct psf_surface_color_view *psf_sf_color_vws;
    psf_sf_color_vws = (struct psf_surface_color_view *) malloc(sizeof(struct psf_surface_color_view));
    if(psf_sf_color_vws == NULL) {
        printf("malloc error for psf_surface_color_view\n");
        exit(0);
    }
    return psf_sf_color_vws;
}

static void init_psf_surf_colmode_dropdown(int id_color_mode,
                                           GtkWidget *surf_color_mode_button){
	if       (id_color_mode == WHITE_SURFACE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 1);
	} else if(id_color_mode == SINGLE_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 2);
	} else if(id_color_mode == CHANGE_PSF_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 3);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 0);
	};
    return;
}

static void set_PSFcolor_GTK(GtkWidget *listbutton_sfcolor,
                             GtkWidget *psf_color_selector,
                             struct kemoviewer_gl_type *kemo_gl){
    float fcolor[4];
    GtkColorDialogButton* color_button
            = GTK_COLOR_DIALOG_BUTTON(psf_color_selector);
    const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(listbutton_sfcolor), 2);
    
    set_RGB_from_GTK(rgba, fcolor);
    fcolor[3] = (float) kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                       SURFACE_RENDERING,
                                                       ISET_OPACITY_MAX);
    kemoview_set_VIZ_single_color(fcolor,
                                  SURFACE_RENDERING,
                                  kemo_gl->kemoview_data);
    kemoview_set_PSF_patch_color_mode(SINGLE_COLOR, kemo_gl->kemoview_data);
	return;
}

static int find_surface_color_mode(const char *text){
    int index = -100;
    if     (kemoview_compare_string((int) strlen(text), text, hd_contour) > 0){index = RAINBOW_PSF_SURF;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_white) > 0){index =   WHITE_SURFACE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_single) > 0){index =  SINGLE_COLOR;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_texture) > 0){index = TEXTURE_PSF_SURF;}
    else {index = RAINBOW_PSF_SURF;};
    
    printf("Selected format %d %s\n", index, text);
    return index;
}

static void set_psf_surface_color_mode(int index_mode, GtkWidget *parent_window,
                                       GtkWidget *listbutton_sfcolor,
                                       GtkWidget *psf_color_selector,
                                       struct kemoviewer_gl_type *kemo_gl){
	if      (index_mode == WHITE_SURFACE){
        kemoview_set_PSF_patch_color_mode(WHITE_SURFACE, kemo_gl->kemoview_data);
        draw_full_gl(kemo_gl);
    }else if(index_mode == SINGLE_COLOR){
        set_PSFcolor_GTK(listbutton_sfcolor, psf_color_selector, kemo_gl);
        draw_triple_gl(kemo_gl);
/*
	}else if(index_mode == CHANGE_PSF_COLOR){
		gtk_combo_box_set_active(GTK_COMBO_BOX(listbutton_sfcolor), 2);
        draw_full_gl(kemo_gl);
*/
	}else if(index_mode == RAINBOW_PSF_SURF){
        kemoview_set_PSF_patch_color_mode(COLORED_BY_DATA, kemo_gl->kemoview_data);
        draw_full_gl(kemo_gl);
	}else if(index_mode == TEXTURE_PSF_SURF){
		open_texture_load_dialog(kemo_gl, GTK_WINDOW(parent_window));
        draw_full_gl(kemo_gl);
	};
    return;
}

static void psf_surf_colormode_CB(GtkDropDown *dropdown,
                                  GParamSpec *pspec,
                                  gpointer user_data){
    GtkWidget *listbutton_sfcolor = GTK_WIDGET(dropdown);
    GtkWidget *psf_color_selector = GTK_WIDGET(user_data);
    GtkWidget *parent_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int index_mode = find_surface_color_mode(text);
    set_psf_surface_color_mode(index_mode, parent_window, 
                               listbutton_sfcolor, psf_color_selector,
                               kemo_gl);
    return;
};

static void set_PSF_colorsel_CB(GObject    *object,
                                GParamSpec *pspec,
                                gpointer    user_data){
    GtkWidget *psf_color_selector = GTK_WIDGET(object);
    GtkWidget *listbutton_sfcolor = GTK_WIDGET(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    set_PSFcolor_GTK(listbutton_sfcolor, psf_color_selector, kemo_gl);
    draw_triple_gl(kemo_gl);
    return;
};

void set_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_surface_color_view *psf_sf_color_vws){
    int iflag_sfcolor = kemoview_get_VIZ_patch_color_mode(kemo_gl->kemoview_data,
                                                      SURFACE_RENDERING);
    init_psf_surf_colmode_dropdown(iflag_sfcolor, psf_sf_color_vws->listbutton_sfcolor);
};

struct psf_surface_color_view * init_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                             GtkWidget *parent_window){
    struct psf_surface_color_view *psf_sf_color_vws = alloc_psf_surface_gtk_menu();
    
    GdkRGBA bg_rgba;
    float color[4];
    
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha(color_dialog, TRUE); // Allow transparency adjustments
    
    GtkStringList *psf_sfcolor_model = gtk_string_list_new(psf_sfcolor_list);
    psf_sf_color_vws->listbutton_sfcolor = init_list_button_by_list_model(psf_sfcolor_model);
    psf_sf_color_vws->psf_color_selector = gtk_color_dialog_button_new(color_dialog);
    
    kemoview_get_background_color(kemo_gl->kemoview_data, color);
    set_RGB_to_GTK(color, &bg_rgba);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(psf_sf_color_vws->psf_color_selector),
                                     &bg_rgba);
    
    g_object_set_data(G_OBJECT(psf_sf_color_vws->listbutton_sfcolor), 
                      "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(psf_sf_color_vws->psf_color_selector), 
                      "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(psf_sf_color_vws->psf_color_selector),
                      "parents", (gpointer) parent_window);
    
    g_signal_connect(G_OBJECT(psf_sf_color_vws->listbutton_sfcolor), "notify::selected-item", 
                     G_CALLBACK (psf_surf_colormode_CB), psf_sf_color_vws->psf_color_selector);
    g_signal_connect (psf_sf_color_vws->psf_color_selector, "notify::rgba", 
                      G_CALLBACK(set_PSF_colorsel_CB), 
                      G_OBJECT(psf_sf_color_vws->listbutton_sfcolor));
    
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