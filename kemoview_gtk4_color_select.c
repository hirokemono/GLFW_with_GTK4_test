/***********************************************************************
 *  kemoview_gtk4_color_select.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_color_select.h"

const char * const hd_contour =   "Contour";       //  RAINBOW_PSF_SURF   RAINBOW_LINE
const char * const hd_white =     "White";         //  WHITE_SURFACE      WHITE_LINE
const char * const hd_single =    "Single color";  //  SINGLE_COLOR       ONE_COLOR_LINE
const char * const hd_changeclr = "Change color";  //  CHANGE_PSF_COLOR
const char * const hd_texture =   "Texture";       //  TEXTURE_PSF_SURF

const char * const hd_two_color = "Two Color";     //  TWO_COLOR_LINE
const char * const hd_two_gray =  "Two Grayscale"; //  TWO_GRAY_LINE
const char * const hd_black =     "Black";         //  BLACK_LINE

const char * const psf_sfcolor_list[] = {hd_contour,
                                         hd_white,
                                         hd_single,
                                         hd_texture,
/*                                         hd_changeclr, */
                                         NULL};

const char * const fline_color_list[] = {hd_contour,
                                         hd_two_color,
                                         hd_two_gray,
                                         hd_single,
                                         hd_white,
                                         hd_black,
                                         NULL};

void init_psf_surf_colmode_dropdown(int id_color_mode,
                                    GtkWidget *color_mode_button){
	if       (id_color_mode == RAINBOW_PSF_SURF){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 0);
	} else if(id_color_mode == WHITE_SURFACE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 1);
	} else if(id_color_mode == SINGLE_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 2);
	} else if(id_color_mode == TEXTURE_PSF_SURF){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 3);
	} else if(id_color_mode == CHANGE_PSF_COLOR){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 4);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 0);
	};
    return;
}

void init_fline_colmode_dropdown(int id_color_mode,
                                 GtkWidget *color_mode_button){
	if       (id_color_mode == RAINBOW_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 0);
	} else if(id_color_mode == TWO_COLOR_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 1);
	} else if(id_color_mode == TWO_GRAY_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 2);
	} else if(id_color_mode == ONE_COLOR_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 3);
	} else if(id_color_mode == WHITE_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 4);
	} else if(id_color_mode == BLACK_LINE){
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 5);
	} else {
		gtk_drop_down_set_selected(GTK_DROP_DOWN(color_mode_button), 0);
	};
    return;
}

int find_surface_color_mode(const char *text){
    int index = -100;
    if     (kemoview_compare_string((int) strlen(text), text, hd_contour) > 0)  {index = RAINBOW_PSF_SURF;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_white) > 0)    {index = WHITE_SURFACE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_single) > 0)   {index = SINGLE_COLOR;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_changeclr) > 0){index = CHANGE_PSF_COLOR;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_texture) > 0)  {index = TEXTURE_PSF_SURF;}
    else {index = RAINBOW_PSF_SURF;};
    
    printf("Selected surface color format: %d: %s\n", index, text);
    return index;
}

int find_fline_color_mode(const char *text){
    int index = -100;
    if     (kemoview_compare_string((int) strlen(text), text, hd_contour) > 0)  {index = RAINBOW_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_white) > 0)    {index = WHITE_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_single) > 0)   {index = ONE_COLOR_LINE;}
    
    else if(kemoview_compare_string((int) strlen(text), text, hd_two_color) > 0){index = TWO_COLOR_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_two_gray) > 0) {index = TWO_GRAY_LINE;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_black) > 0)    {index = BLACK_LINE;}
    else {index = RAINBOW_LINE;};
    
    printf("Selected line color format: %d: %s\n", index, text);
    return index;
}

void set_psf_surface_color_mode(int index_mode, GtkWidget *parent_window,
                                GtkWidget *color_selector,
                                struct kemoviewer_gl_type *kemo_gl){
    if(index_mode == TEXTURE_PSF_SURF){
		open_texture_load_dialog(kemo_gl, GTK_WINDOW(parent_window));
/*
	}else if(index_mode == CHANGE_PSF_COLOR){
		gtk_combo_box_set_active(GTK_COMBO_BOX(color_listbutton), 2);
*/
    }else if(index_mode == SINGLE_COLOR){
        kemoview_set_PSF_patch_color_mode(index_mode, kemo_gl->kemoview_data);
    }else{
        kemoview_set_PSF_patch_color_mode(index_mode, kemo_gl->kemoview_data);
    };
    return;
}

void set_fline_surface_color_mode(int index_mode, GtkWidget *parent_window,
                                  GtkWidget *color_selector,
                                  struct kemoviewer_gl_type *kemo_gl,
                                  int id_current_model){
    if(index_mode == ONE_COLOR_LINE){
        kemoview_set_VIZ_patch_color_mode(index_mode, id_current_model,
                                          kemo_gl->kemoview_data);
    }else{
        kemoview_set_VIZ_patch_color_mode(index_mode, id_current_model,
                                          kemo_gl->kemoview_data);
    };
    return;
}

GtkWidget * init_PSF_sfcolor_listbutton(void){
    GtkStringList *psf_sfcolor_model = gtk_string_list_new(psf_sfcolor_list);
    return init_list_button_by_list_model(psf_sfcolor_model);
};

GtkWidget * init_Fline_color_listbutton(void){
    GtkStringList *fline_color_model = gtk_string_list_new(fline_color_list);
    return init_list_button_by_list_model(fline_color_model);
};
