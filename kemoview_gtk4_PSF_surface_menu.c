/***********************************************************************
 *  kemoview_gtk4_PSF_surface_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_surface_menu.h"

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

static struct psf_surface_gtk_menu * alloc_psf_surface_gtk_menu(void){
    struct psf_surface_gtk_menu *gtk_psf_surface;
    gtk_psf_surface = (struct psf_surface_gtk_menu *) malloc(sizeof(struct psf_surface_gtk_menu));
    if(gtk_psf_surface == NULL) {
        printf("malloc error for psf_surface_gtk_menu\n");
        exit(0);
    }
    return gtk_psf_surface;
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

static void psf_surface_switch_CB(GObject *switch_draw, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_draw));
    kemoview_set_VIZ_draw_flag(SURFACE_RENDERING, iflag,
                               kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static GtkWidget * init_gtk4_psf_surface_switch_box(struct kemoviewer_gl_type *kemo_gl,
                                                    struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_draw;
    
    psf_surface_menu->switch_draw = gtk_switch_new();
    int iflag = kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data, SURFACE_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(psf_surface_menu->switch_draw), iflag);
    gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_draw), TRUE);
    g_signal_connect(G_OBJECT(psf_surface_menu->switch_draw), "notify::active",
                     G_CALLBACK(psf_surface_switch_CB), (gpointer) kemo_gl);
    
    hbox_draw = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_draw), gtk_label_new("Draw surface: "));
    gtk_box_append(GTK_BOX(hbox_draw), psf_surface_menu->switch_draw);
    
    return hbox_draw;
};


static void psf_colorbar_switch_CB(GObject *switch_bar, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_bar));
    kemoview_set_colorbar_draw_flag(iflag, SURFACE_RENDERING,
                                    kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static GtkWidget * init_gtk4_psf_colorbar_switch_box(struct kemoviewer_gl_type *kemo_gl,
                                                     struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_cbar;
    
    psf_surface_menu->switch_bar = gtk_switch_new();
    int iflag = kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data, 
                                                SURFACE_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(psf_surface_menu->switch_bar), iflag);
    gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_bar), FALSE);
    g_signal_connect(G_OBJECT(psf_surface_menu->switch_bar), "notify::active",
                     G_CALLBACK(psf_colorbar_switch_CB), (gpointer) kemo_gl);
    
    hbox_cbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_cbar), gtk_label_new("Draw color bar: "));
    gtk_box_append(GTK_BOX(hbox_cbar), psf_surface_menu->switch_bar);
    return hbox_cbar;
};


static void set_PSFcolor_GTK(GtkWidget *listbutton_sfcolor,
                             GtkWidget *psf_color_selector,
                             struct kemoviewer_gl_type *kemo_gl){
    float fcolor[4];
    GtkColorDialogButton* color_button
            = GTK_COLOR_DIALOG_BUTTON(psf_color_selector);
    GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
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
		open_texture_load_dialog(kemo_gl, parent_window);
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

static void set_psf_opacity_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_set_constant_opacity(gtk_floatvalue,
                                  SURFACE_RENDERING,
                                  kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void MinRangeValueChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(gtk_floatvalue, i_min_digit,
                                 value_max, i_max_digit,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void MinRangeDigitChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	int gtk_intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, gtk_intvalue,
                                 value_max, i_max_digit,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

void update_psf_minimum_label(struct kemoviewer_gl_type *kemo_gl, 
                              GtkWidget *label_range_min){
	char min_text[30];
    double value_min = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_COLOR_MIN);
	sprintf(min_text, "Min(%1.2e): ", value_min);
	gtk_label_set_text(GTK_LABEL(label_range_min), min_text);
    return;
}

static GtkWidget * init_gtk4_psf_set_minimum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_min_range;
	char min_text[30];
    
    GtkAdjustment *adj_range_min = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_min = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    psf_surface_menu->spin_range_min = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_min),0,2);
    psf_surface_menu->spin_digit_min = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_min),0,0);
    g_signal_connect(psf_surface_menu->spin_range_min, "value-changed", 
                     G_CALLBACK(MinRangeValueChange_CB), (gpointer) kemo_gl);
    g_signal_connect(psf_surface_menu->spin_digit_min, "value-changed",
                     G_CALLBACK(MinRangeDigitChange_CB), (gpointer) kemo_gl);
    
    psf_surface_menu->label_range_min = gtk_label_new(" ");
    update_psf_minimum_label(kemo_gl, psf_surface_menu->label_range_min);
    
    hbox_min_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_min_range), psf_surface_menu->label_range_min);
    gtk_box_append(GTK_BOX(hbox_min_range), psf_surface_menu->spin_range_min);
    gtk_box_append(GTK_BOX(hbox_min_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_min_range), psf_surface_menu->spin_digit_min);
    return hbox_min_range;
}

static void MaxRangeValueChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, i_min_digit,
                                 gtk_floatvalue, i_max_digit,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

static void MaxRangeDigitChange_CB(GtkWidget *entry, gpointer data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) data;
	int gtk_intvalue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	double value_min, value_max;
	int i_min_digit, i_max_digit;
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &value_min, &i_min_digit);
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &value_max, &i_max_digit);
    kemoview_set_linear_colormap(value_min, i_min_digit,
                                 value_max, gtk_intvalue,
                                 SURFACE_RENDERING,
                                 kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
}

void update_psf_maximum_label(struct kemoviewer_gl_type *kemo_gl, 
                              GtkWidget *label_range_max){
	char max_text[30];
    double value_max = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_COLOR_MAX);
    sprintf(max_text, "Max(%1.2e): ", value_max);
    gtk_label_set_text(GTK_LABEL(label_range_max), max_text);
    return;
}

static GtkWidget * init_gtk4_psf_set_maximum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_max_range;
    
    GtkAdjustment *adj_range_max = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_max = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    psf_surface_menu->spin_range_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_max),0,2);
    psf_surface_menu->spin_digit_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_max),0,0);
    g_signal_connect(psf_surface_menu->spin_range_max, "value-changed",
                     G_CALLBACK(MaxRangeValueChange_CB), (gpointer) kemo_gl);
    g_signal_connect(psf_surface_menu->spin_digit_max, "value-changed",
                     G_CALLBACK(MaxRangeDigitChange_CB), (gpointer) kemo_gl);
    
    psf_surface_menu->label_range_max = gtk_label_new(" ");
    update_psf_maximum_label(kemo_gl, psf_surface_menu->label_range_max);
    
    hbox_max_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_max_range), psf_surface_menu->label_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), psf_surface_menu->spin_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_max_range), psf_surface_menu->spin_digit_max);
    return hbox_max_range;
}

static GtkWidget * init_gtk4_psf_set_opacity_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_one_opacity;
    
    GtkAdjustment *adj_opacity1 = gtk_adjustment_new(1.0, 0.0, 1.0, 0.01, 0.01, 0.0);
    psf_surface_menu->spin_opacity1 = gtk_spin_button_new(GTK_ADJUSTMENT(adj_opacity1), 0, 2);
    g_signal_connect(psf_surface_menu->spin_opacity1, "value-changed",
                     G_CALLBACK(set_psf_opacity_CB), (gpointer) kemo_gl);
    
    hbox_one_opacity = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_one_opacity), gtk_label_new("Opacity"));
    gtk_box_append(GTK_BOX(hbox_one_opacity), psf_surface_menu->spin_opacity1);
    return hbox_one_opacity;
}


void set_gtk4_surface_menu_values(struct kemoviewer_gl_type *kemo_gl,
                                  struct psf_surface_gtk_menu *psf_surface_menu){
	int iflag_sfcolor;
	int i_digit;
	double current_value;
    
    return;
	if(kemoview_get_VIZ_draw_flags(kemo_gl->kemoview_data, SURFACE_RENDERING) == 0){
		gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_draw), FALSE);
	} else {
		gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_draw), TRUE);
	};
	if(kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                       SURFACE_RENDERING) == 0){
		gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_bar), FALSE);
	} else {
		gtk_switch_set_active(GTK_SWITCH(psf_surface_menu->switch_bar), TRUE);
	};

	iflag_sfcolor = kemoview_get_VIZ_patch_color_mode(kemo_gl->kemoview_data,
                                                      SURFACE_RENDERING);
    init_psf_surf_colmode_dropdown(iflag_sfcolor, psf_surface_menu->listbutton_sfcolor);
    
	current_value = kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_OPACITY_MAX);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_surface_menu->spin_opacity1), current_value);
	
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MIN,
                                 &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_surface_menu->spin_range_min), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_surface_menu->spin_digit_min), i_digit);
	
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_COLOR_MAX,
                                 &current_value, &i_digit);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_surface_menu->spin_range_max), current_value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(psf_surface_menu->spin_digit_max), i_digit);
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

static GtkWidget * init_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                struct GtkWidget *window,
                                                struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *color_hbox;
    
    GdkRGBA bg_rgba;
    float color[4];
    
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha(color_dialog, TRUE); // Allow transparency adjustments
    
    GtkStringList *psf_sfcolor_model = gtk_string_list_new(psf_sfcolor_list);
    psf_surface_menu->listbutton_sfcolor = init_list_button_by_list_model(psf_sfcolor_model);
    psf_surface_menu->psf_color_selector = gtk_color_dialog_button_new(color_dialog);
    
    kemoview_get_background_color(kemo_gl->kemoview_data, color);
    set_RGB_to_GTK(color, &bg_rgba);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(psf_surface_menu->psf_color_selector),
                                     &bg_rgba);
    
    g_object_set_data(G_OBJECT(psf_surface_menu->listbutton_sfcolor), 
                      "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(psf_surface_menu->psf_color_selector), 
                      "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(psf_surface_menu->psf_color_selector),
                      "parents", (gpointer) window);
    
    
    g_signal_connect(G_OBJECT(psf_surface_menu->listbutton_sfcolor), "notify::selected-item", 
                     G_CALLBACK (psf_surf_colormode_CB), psf_surface_menu->psf_color_selector);
    g_signal_connect (psf_surface_menu->psf_color_selector, "notify::rgba", 
                      G_CALLBACK(set_PSF_colorsel_CB), 
                      G_OBJECT(psf_surface_menu->listbutton_sfcolor));
    
    
    color_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(color_hbox), gtk_label_new("Color: "));
    gtk_box_append(GTK_BOX(color_hbox), psf_surface_menu->listbutton_sfcolor);
    gtk_box_append(GTK_BOX(color_hbox), psf_surface_menu->psf_color_selector);
    
    return color_hbox;
}

struct psf_surface_gtk_menu * init_gtk4_psf_surface_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                                  GtkWidget *window){
    struct psf_surface_gtk_menu *psf_surface_menu = alloc_psf_surface_gtk_menu();
    
    GtkWidget *hbox_draw = init_gtk4_psf_surface_switch_box(kemo_gl, psf_surface_menu);
    GtkWidget *hbox_cbar = init_gtk4_psf_colorbar_switch_box(kemo_gl, psf_surface_menu);
    
    GtkWidget *hbox_min_range =   init_gtk4_psf_set_minimum_box(kemo_gl, psf_surface_menu);
    GtkWidget *hbox_max_range =   init_gtk4_psf_set_maximum_box(kemo_gl, psf_surface_menu);
    GtkWidget *hbox_one_opacity = init_gtk4_psf_set_opacity_box(kemo_gl, psf_surface_menu);
    
    GtkWidget *hbox_colorsel = init_gtk4_psf_surf_color_sel(kemo_gl, window, psf_surface_menu);
    
    GtkWidget *patch_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(patch_box), hbox_draw);
    gtk_box_append(GTK_BOX(patch_box), hbox_cbar);
    gtk_box_append(GTK_BOX(patch_box), hbox_colorsel);
    
    gtk_box_append(GTK_BOX(patch_box), gtk_label_new("Range"));
    gtk_box_append(GTK_BOX(patch_box), hbox_min_range);
    gtk_box_append(GTK_BOX(patch_box), hbox_max_range);
    gtk_box_append(GTK_BOX(patch_box), hbox_one_opacity);
    
    psf_surface_menu->expander_surf = wrap_into_scroll_expansion_gtk4("Surface", 420, 320,
                                                                      window, patch_box);
    return psf_surface_menu;
}

void dealloc_psf_surface_gtk_menu(struct psf_surface_gtk_menu *psf_surface_menu){
    g_object_unref(psf_surface_menu->switch_draw);
    g_object_unref(psf_surface_menu->switch_bar);
    g_object_unref(psf_surface_menu->listbutton_sfcolor);
    g_object_unref(psf_surface_menu->psf_color_selector);
    g_object_unref(psf_surface_menu->label_range_min);
    g_object_unref(psf_surface_menu->label_range_max);
    g_object_unref(psf_surface_menu->spin_opacity1);
    g_object_unref(psf_surface_menu->spin_range_min);
    g_object_unref(psf_surface_menu->spin_digit_min);
    g_object_unref(psf_surface_menu->spin_range_max);
    g_object_unref(psf_surface_menu->spin_digit_max);
    g_object_unref(psf_surface_menu->expander_surf);
    free(psf_surface_menu);
    return;
};