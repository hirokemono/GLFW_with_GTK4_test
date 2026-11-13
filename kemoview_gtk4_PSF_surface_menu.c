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


struct psf_surface_gtk_menu * alloc_psf_surface_gtk_menu(void){
    struct psf_surface_gtk_menu *gtk_psf_surface;
    gtk_psf_surface = (struct psf_surface_gtk_menu *) malloc(sizeof(struct psf_surface_gtk_menu));
    if(gtk_psf_surface == NULL) {
        printf("malloc error for psf_surface_gtk_menu\n");
        exit(0);
    }
    return gtk_psf_surface;
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


static void set_PSFcolor_GTK(struct kemoviewer_gl_type *kemo_gl,
                             GtkColorChooser *colordialog)
{
	GdkRGBA gcolor;
	gdouble dcolor[4];
	
	dcolor[0] = gcolor.red;
	dcolor[1] = gcolor.green;
	dcolor[2] = gcolor.blue;
	dcolor[3] = (gdouble) kemoview_get_VIZ_opacity_range(kemo_gl->kemoview_data,
                                                         SURFACE_RENDERING,
                                                         ISET_OPACITY_MAX);
    kemoview_set_VIZ_single_color(dcolor,
                                  SURFACE_RENDERING,
                                  kemo_gl->kemoview_data);
    kemoview_set_PSF_patch_color_mode(SINGLE_COLOR, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}
/*
static void kemoview_gtk_surfcolorsel(struct kemoviewer_gl_type *kemo_gl,
                                      GtkWidget *parent_window){
	int response;
	GtkColorChooser *chooser;
	
	window_csel = gtk_color_chooser_dialog_new("Choose surface color",
                                               GTK_WINDOW(parent_window));
	gtk_widget_show_all(window_csel);
	
	response = gtk_dialog_run(GTK_DIALOG(window_csel));
	if (response == GTK_RESPONSE_OK){
		chooser = GTK_COLOR_CHOOSER(window_csel);
		set_PSFcolor_GTK(kemo_gl, chooser);
		g_print ("color selected \n");
	}
	else if( response == GTK_RESPONSE_CANCEL ){
		g_print( "Cancel button was pressed.\n" );
		gtk_window_destroy(window_csel);
	}
	return;
}

static int load_texture_file_gtk3(GtkWidget *window, 
                                  struct kv_string *file_prefix){
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
*/

static void load_texture_handler(struct kemoviewer_gl_type *kemo_gl,
                                 GtkWidget *window){
	int id_image;
    struct kv_string *image_prefix = kemoview_alloc_kvstring();
//	id_image = load_texture_file_gtk3(window, image_prefix);
	
	if(id_image == SAVE_PNG || id_image == SAVE_BMP){
        kemoview_release_PSF_gl_texture(kemo_gl->kemoview_data, kemo_gl);
        kemoview_update_PSF_textured_id(kemo_gl->kemoview_data);
		kemoview_set_texture_to_PSF(id_image, image_prefix, kemo_gl->kemoview_data, kemo_gl);
        kemoview_set_PSF_patch_color_mode(TEXTURED_SURFACE, kemo_gl->kemoview_data);
        kemoview_free_kvstring(image_prefix);
	};
	
	return;
};

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
                                       struct kemoviewer_gl_type *kemo_gl){
	if      (index_mode == WHITE_SURFACE){
        kemoview_set_PSF_patch_color_mode(WHITE_SURFACE, kemo_gl->kemoview_data);
	}else if(index_mode == SINGLE_COLOR){
//		kemoview_gtk_surfcolorsel(kemo_gl, parent_window);
/*
	}else if(index_mode == CHANGE_PSF_COLOR){
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_sfcolor), 2);
*/
	}else if(index_mode == RAINBOW_PSF_SURF){
        kemoview_set_PSF_patch_color_mode(COLORED_BY_DATA, kemo_gl->kemoview_data);
	}else if(index_mode == TEXTURE_PSF_SURF){
		load_texture_handler(kemo_gl, parent_window);
	};
    return;
}

static void psf_surf_colormode_CB(GtkDropDown *dropdown,
                                  GParamSpec *pspec,
                                  gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    GtkWidget *parent_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parents"));
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string(selected_item);
    int index_mode = find_surface_color_mode(text);
    set_psf_surface_color_mode(index_mode, parent_window, kemo_gl);
    draw_full_gl(kemo_gl);
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
    
    double value_min = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_COLOR_MIN);
	sprintf(min_text, "Min(%1.2e): ", value_min);
    psf_surface_menu->label_range_min = gtk_label_new(" ");
	gtk_label_set_text(GTK_LABEL(psf_surface_menu->label_range_min), min_text);
    
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

static GtkWidget * init_gtk4_psf_set_maximum_box(struct kemoviewer_gl_type *kemo_gl,
                                                 struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *hbox_max_range;
	char max_text[30];
    
    GtkAdjustment *adj_range_max = gtk_adjustment_new (1, -9.999, 9.999, 0.1, 0.1, 0.0);
    GtkAdjustment *adj_digit_max = gtk_adjustment_new (0, -20, 20, 1, 1, 0);
    psf_surface_menu->spin_range_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_range_max),0,2);
    psf_surface_menu->spin_digit_max = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit_max),0,0);
    g_signal_connect(psf_surface_menu->spin_range_max, "value-changed",
                     G_CALLBACK(MaxRangeValueChange_CB), (gpointer) kemo_gl);
    g_signal_connect(psf_surface_menu->spin_digit_max, "value-changed",
                     G_CALLBACK(MaxRangeDigitChange_CB), (gpointer) kemo_gl);
    
    double value_max = kemoview_get_VIZ_data_range(kemo_gl->kemoview_data,
                                                   SURFACE_RENDERING,
                                                   ISET_COLOR_MAX);
    sprintf(max_text, "Max(%1.2e): ", value_max);
    psf_surface_menu->label_range_max = gtk_label_new(" ");
    gtk_label_set_text(GTK_LABEL(psf_surface_menu->label_range_max), max_text);
    
    hbox_max_range = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_max_range), psf_surface_menu->label_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), psf_surface_menu->spin_range_max);
    gtk_box_append(GTK_BOX(hbox_max_range), gtk_label_new("x10^"));
    gtk_box_append(GTK_BOX(hbox_max_range), psf_surface_menu->spin_digit_max);
    return hbox_max_range;
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
	if(iflag_sfcolor == TEXTURE_PSF_SURF){
		gtk_combo_box_set_active(GTK_COMBO_BOX(psf_surface_menu->combobox_sfcolor), 4);
	} else 	if(iflag_sfcolor == SINGLE_COLOR){
		gtk_combo_box_set_active(GTK_COMBO_BOX(psf_surface_menu->combobox_sfcolor), 2);
	} else 	if(iflag_sfcolor == WHITE_SURFACE){
		gtk_combo_box_set_active(GTK_COMBO_BOX(psf_surface_menu->combobox_sfcolor), 1);
	} else {
		gtk_combo_box_set_active(GTK_COMBO_BOX(psf_surface_menu->combobox_sfcolor), 0);
	};

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
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON(object);
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
    float color[4];
    set_RGB_from_GTK(rgba, color);
    kemoview_set_background_color(color, kemo_gl->kemoview_data);
//    printf("updated %f %f %f %f\n", color[0], color[1], color[2], color[3]);
    
    draw_full_gl(kemo_gl);
    return;
};

static GtkWidget * init_gtk4_psf_surf_color_sel(struct kemoviewer_gl_type *kemo_gl,
                                                struct GtkWidget *window,
                                         GLFWwindow *glfw_win){
    GtkWidget *color_hbox;
    
    GdkRGBA bg_rgba;
    float color[4];
    kemoview_get_background_color(kemo_gl->kemoview_data, color);
    set_RGB_to_GTK(color, &bg_rgba);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "window", (gpointer) window);
    
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha (color_dialog, TRUE); // Allow transparency adjustments
    
    GtkWidget *color_button = gtk_color_dialog_button_new(color_dialog);
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON(color_button), &bg_rgba);
    g_signal_connect (color_button, "notify::rgba", 
                      G_CALLBACK(set_PSF_colorsel_CB), G_OBJECT(entry_buf));
    
    
    color_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(color_hbox), gtk_label_new("Color: "));
    gtk_box_append(GTK_BOX(color_hbox), color_button);
    
    return color_hbox;
}

GtkWidget * init_gtk4_psf_surface_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                GtkWidget *window, 
                                                struct psf_surface_gtk_menu *psf_surface_menu){
    GtkWidget *expander_surf;
    int iflag;
    
    GtkWidget *hbox_draw = init_gtk4_psf_surface_switch_box(kemo_gl, psf_surface_menu);
    GtkWidget *hbox_cbar = init_gtk4_psf_colorbar_switch_box(kemo_gl, psf_surface_menu);
    
    GtkWidget *hbox_min_range = init_gtk4_psf_set_minimum_box(kemo_gl, psf_surface_menu);
    GtkWidget *hbox_max_range = init_gtk4_psf_set_maximum_box(kemo_gl, psf_surface_menu);
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl", (gpointer)  kemo_gl);
    g_object_set_data(G_OBJECT(entry_buf), "parents", (gpointer)  window);
    
    GtkStringList *psf_sfcolor_model = gtk_string_list_new(psf_sfcolor_list);
    psf_surface_menu->combobox_sfcolor = init_list_button_by_list_model(psf_sfcolor_model);
    g_signal_connect(G_OBJECT(psf_surface_menu->combobox_sfcolor), "notify::selected-item", 
                     G_CALLBACK (psf_surf_colormode_CB), entry_buf);
//    init_image_format_dropdown(*id_image_format, psf_surface_menu->combobox_sfcolor);
    
    GtkWidget *hbox_color = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_color), gtk_label_new("Color mode: "));
    gtk_box_append(GTK_BOX(hbox_color), psf_surface_menu->combobox_sfcolor);
    
    
    GtkWidget *hbox_colorsel = init_gtk4_psf_surf_color_sel(kemo_gl, psf_sfcolor_model);
    /*
    
    GtkAdjustment *adj_opacity1 = gtk_adjustment_new(1.0, 0.0, 1.0, 0.01, 0.01, 0.0);
    psf_surface_menu->spin_opacity1 = gtk_spin_button_new(GTK_ADJUSTMENT(adj_opacity1), 0, 2);
    g_signal_connect(psf_surface_menu->spin_opacity1, "value-changed",
                     G_CALLBACK(set_psf_opacity_CB), (gpointer) kemo_gl);
    
    */
    
//    GtkWidget *hbox_one_opacity = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
//    gtk_box_append(GTK_BOX(hbox_one_opacity), gtk_label_new("Opacity"));
//    gtk_box_append(GTK_BOX(hbox_one_opacity), psf_surface_menu->spin_opacity1);
//    
    GtkWidget *patch_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(patch_box), hbox_draw);
    gtk_box_append(GTK_BOX(patch_box), hbox_cbar);
    gtk_box_append(GTK_BOX(patch_box), hbox_color);
    gtk_box_append(GTK_BOX(patch_box), hbox_colorsel);
//    gtk_box_append(GTK_BOX(patch_box), hbox_one_opacity);
//    
    gtk_box_append(GTK_BOX(patch_box), gtk_label_new("Range"));
    gtk_box_append(GTK_BOX(patch_box), hbox_min_range);
    gtk_box_append(GTK_BOX(patch_box), hbox_max_range);
    
    expander_surf = wrap_into_scroll_expansion_gtk4("Surface", 420, 320, window, patch_box);
    return expander_surf;
}
