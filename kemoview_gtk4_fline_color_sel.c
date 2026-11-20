/***********************************************************************
 *  kemoview_gtk4_fline_color_sel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_fline_color_sel.h"

const char * const hd_fl_contour = "Contour";        //  RAINBOW_LINE
const char * const hd_two_color =  "Two color";      //  TWO_COLOR_LINE
const char * const hd_two_gray =   "Two grayscale";  //  TWO_GRAY_LINE
const char * const hd_fl_single =  "Single color";   //  ONE_COLOR_LINE
const char * const hd_fl_white =   "White";          //  WHITE_LINE

const char * const psf_fline_color_list[] = {hd_fl_contour,
                                             hd_two_color,
                                             hd_two_gray,
                                             hd_fl_single,
                                             hd_fl_white,
                                             NULL};

static struct fline_color_sel_view * alloc_fline_color_sel_view(void){
    struct fline_color_sel_view *fline_color_sel_vws;
    fline_color_sel_vws = (struct fline_color_sel_view *) malloc(sizeof(struct fline_color_sel_view));
    if(fline_color_sel_vws == NULL) {
        printf("malloc error for fline_color_sel_view\n");
        exit(0);
    }
    return fline_color_sel_vws;
}

static void init_fline_colormode_dropdown(int id_color_mode,
                                          GtkWidget *surf_color_mode_button){
	if       (id_color_mode == TWO_COLOR_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 1);
	} else if(id_color_mode == TWO_GRAY_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 2);
	} else if(id_color_mode == ONE_COLOR_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 3);
	} else if(id_color_mode == WHITE_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 4);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(surf_color_mode_button), 0);
	};
    return;
}

static void set_Fline_color_GTK(struct kemoviewer_gl_type *kemo_gl,
                                int id_current_model,
                                GtkWidget *button_fline_color,
                                GtkWidget *fline_color_selector){
    float fcolor[4];
    GtkColorDialogButton* color_button
            = GTK_COLOR_DIALOG_BUTTON(fline_color_selector);
    const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(button_fline_color), 2);
    
    set_RGB_from_GTK(rgba, fcolor);
    fcolor[3] = (float) kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                       id_current_model,
                                                       ISET_OPACITY_MAX);
    kemoview_set_VIZ_single_color(fcolor,
                                  id_current_model,
                                  kemo_gl->kemoview_data);
    kemoview_set_PSF_patch_color_mode(ONE_COLOR_LINE, kemo_gl->kemoview_data);
	return;
}

static int find_fieldline_color_mode(const char *text){
    int index = -100;
    if     (kemoview_compare_string((int) strlen(text), text, hd_fl_contour) > 0){index = RAINBOW_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_two_color) > 0){index =   TWO_COLOR_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_two_gray) > 0){index =   TWO_GRAY_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_fl_single) > 0){index =  ONE_COLOR_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_fl_white) > 0){index = WHITE_LINE;}
    else {index = RAINBOW_LINE;};
    
    printf("Selected format %d %s\n", index, text);
    return index;
}

static void set_fieldline_color_mode(struct kemoviewer_gl_type *kemo_gl,
                                     int id_current_model, int index_mode,
                                     GtkWidget *parent_window,
                                     GtkWidget *button_fline_color,
                                     GtkWidget *fline_color_selector){
    kemoview_set_VIZ_patch_color_mode(index_mode, id_current_model,
                                      kemo_gl->kemoview_data);
    if(index_mode == SINGLE_COLOR){
        set_Fline_color_GTK(kemo_gl, id_current_model,
                            button_fline_color, fline_color_selector);
        draw_double_gl(kemo_gl);
	};
    draw_full_gl(kemo_gl);
    return;
}

static void psf_fline_colormode_CB(GtkDropDown *dropdown,
                                  GParamSpec *pspec,
                                  gpointer user_data){
    GtkWidget *button_fline_color = GTK_WIDGET(dropdown);
    GtkWidget *fline_color_selector = GTK_WIDGET(user_data);
    GtkWidget *parent_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int index_mode = find_fieldline_color_mode(text);
    set_fieldline_color_mode(kemo_gl, *id_current_model, 
                             index_mode, parent_window, 
                             button_fline_color, fline_color_selector);
    return;
};

static void set_fline_colorsel_CB(GObject    *object,
                                  GParamSpec *pspec,
                                  gpointer    user_data){
    GtkWidget *fline_color_selector = GTK_WIDGET(object);
    GtkWidget *button_fline_color = GTK_WIDGET(user_data);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int *id_current_model = (int *) g_object_get_data(G_OBJECT(user_data), "current_model");
    
    set_Fline_color_GTK(kemo_gl, *id_current_model,
                        button_fline_color, fline_color_selector);
    draw_triple_gl(kemo_gl);
    return;
};

void set_gtk4_fline_color_sel(struct kemoviewer_gl_type *kemo_gl,
                              int id_current_model,
                              struct fline_color_sel_view *fline_color_sel_vws){
    int iflag_sfcolor = kemoview_get_VIZ_patch_color_mode(kemo_gl->kemoview_data,
                                                          id_current_model);
    init_fline_colormode_dropdown(iflag_sfcolor, fline_color_sel_vws->button_fline_color);
};

struct fline_color_sel_view * init_gtk4_fline_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                        int *id_current_model, 
                                                        GtkWidget *parent_window){
    struct fline_color_sel_view *fline_color_sel_vws = alloc_fline_color_sel_view();
    
    GdkRGBA bg_rgba;
    float color[4];
    
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha(color_dialog, TRUE); // Allow transparency adjustments
    
    GtkStringList *psf_sfcolor_model = gtk_string_list_new(psf_fline_color_list);
    fline_color_sel_vws->button_fline_color = init_list_button_by_list_model(psf_sfcolor_model);
    fline_color_sel_vws->fline_color_selector = gtk_color_dialog_button_new(color_dialog);
    
    kemoview_get_background_color(kemo_gl->kemoview_data, color);
    set_RGB_to_GTK(color, &bg_rgba);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(fline_color_sel_vws->fline_color_selector),
                                     &bg_rgba);
    
    g_object_set_data(G_OBJECT(fline_color_sel_vws->button_fline_color), 
                      "kemoview_gl",   (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(fline_color_sel_vws->button_fline_color), 
                      "current_model", (gpointer) id_current_model);
    g_signal_connect (fline_color_sel_vws->fline_color_selector, "notify::rgba", 
                      G_CALLBACK(set_fline_colorsel_CB), 
                      G_OBJECT(fline_color_sel_vws->button_fline_color));
    
    g_object_set_data(G_OBJECT(fline_color_sel_vws->fline_color_selector), 
                      "kemoview_gl",   (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(fline_color_sel_vws->fline_color_selector), 
                      "current_model", (gpointer) id_current_model);
    g_object_set_data(G_OBJECT(fline_color_sel_vws->fline_color_selector),
                      "parents",       (gpointer) parent_window);
    g_signal_connect(G_OBJECT(fline_color_sel_vws->button_fline_color), "notify::selected-item", 
                     G_CALLBACK (psf_fline_colormode_CB), fline_color_sel_vws->fline_color_selector);
    
    
    fline_color_sel_vws->hbox_fline_color = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(fline_color_sel_vws->hbox_fline_color), gtk_label_new("Color: "));
    gtk_box_append(GTK_BOX(fline_color_sel_vws->hbox_fline_color), fline_color_sel_vws->button_fline_color);
    gtk_box_append(GTK_BOX(fline_color_sel_vws->hbox_fline_color), fline_color_sel_vws->fline_color_selector);
    
    return fline_color_sel_vws;
}

void dealloc_fline_color_views(struct fline_color_sel_view *fline_color_sel_vws){
    g_object_unref(fline_color_sel_vws->button_fline_color);
    g_object_unref(fline_color_sel_vws->fline_color_selector);
    g_object_unref(fline_color_sel_vws->hbox_fline_color);
    free(fline_color_sel_vws);
    return;
};