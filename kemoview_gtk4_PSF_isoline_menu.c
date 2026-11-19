/***********************************************************************
 *  kemoview_gtk4_PSF_isoline_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_isoline_menu.h"

static struct psf_isoline_switches * alloc_psf_isoline_switches(void){
    struct psf_isoline_switches *isoline_sw;
	isoline_sw = (struct psf_isoline_switches *) malloc(sizeof(struct psf_isoline_switches));
    if(isoline_sw == NULL){
        printf("malloc error for psf_isoline_switches\n");
        exit(0);
    }
    return isoline_sw;
};

static void dealloc_psf_isoline_switches(struct psf_isoline_switches *isoline_sw){
    g_object_unref(isoline_sw->switch_1);
    g_object_unref(isoline_sw->hbox_draw);
    g_object_unref(isoline_sw->switch_zero);
    g_object_unref(isoline_sw->hbox_zero);
    free(isoline_sw);
    return;
};

static void psf_grid_switch_CB(GObject *switch_1, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_1));
    kemoview_set_PSF_draw_flags(iflag, PSFGRID_TOGGLE, kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
	return;
};

static void psf_zero_switch_CB(GObject *switch_1, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_1));
    kemoview_set_PSF_draw_flags(iflag, ZEROGRID_TOGGLE, kemo_gl->kemoview_data);
    
    draw_full_gl(kemo_gl);
    return;
};

static void set_gtk_isoline_switches(struct kemoviewer_gl_type *kemo_gl,
                                     struct psf_isoline_switches *isoline_sw){
    if(kemoview_get_PSF_draw_flags(kemo_gl->kemoview_data, PSFGRID_TOGGLE) == 0){
        gtk_switch_set_active(GTK_SWITCH(isoline_sw->switch_1), FALSE);
    } else {
        gtk_switch_set_active(GTK_SWITCH(isoline_sw->switch_1), TRUE);
    };
    if(kemoview_get_PSF_draw_flags(kemo_gl->kemoview_data, ZEROGRID_TOGGLE) == 0){
        gtk_switch_set_active(GTK_SWITCH(isoline_sw->switch_zero), FALSE);
    } else {
        gtk_switch_set_active(GTK_SWITCH(isoline_sw->switch_zero), TRUE);
    };
    return;
};

static struct psf_isoline_switches * init_isoline_switches(struct kemoviewer_gl_type *kemo_gl){
    struct psf_isoline_switches *isoline_sw = alloc_psf_isoline_switches();
    int iflag;
    
    GtkEntryBuffer *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",  (gpointer) kemo_gl);
    
	isoline_sw->switch_1 = gtk_switch_new();
    iflag = kemoview_get_PSF_draw_flags(kemo_gl->kemoview_data, PSFGRID_TOGGLE);
    gtk_switch_set_state(GTK_SWITCH(isoline_sw->switch_1), iflag);
    gtk_switch_set_active(GTK_SWITCH(isoline_sw->switch_1), FALSE);
    g_signal_connect(G_OBJECT(isoline_sw->switch_1), "notify::active",
                     G_CALLBACK(psf_grid_switch_CB), (gpointer) entry_buf);
    
    isoline_sw->switch_zero = gtk_switch_new();
    iflag = kemoview_get_PSF_draw_flags(kemo_gl->kemoview_data, ZEROGRID_TOGGLE);
    gtk_switch_set_state(GTK_SWITCH(isoline_sw->switch_zero), iflag);
	gtk_switch_set_active(GTK_SWITCH(isoline_sw->switch_zero), FALSE);
    g_signal_connect(G_OBJECT(isoline_sw->switch_zero), "notify::active",
                     G_CALLBACK(psf_zero_switch_CB), (gpointer) entry_buf);
    
    isoline_sw->hbox_draw = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(isoline_sw->hbox_draw), gtk_label_new("Draw isolines: "));
    gtk_box_append(GTK_BOX(isoline_sw->hbox_draw), isoline_sw->switch_1);
    
    isoline_sw->hbox_zero = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(isoline_sw->hbox_zero), gtk_label_new("Draw zero line: "));
    gtk_box_append(GTK_BOX(isoline_sw->hbox_zero), isoline_sw->switch_zero);
    
    set_gtk_isoline_switches(kemo_gl, isoline_sw);
    return isoline_sw;
}



static struct psf_isoline_param_menu * alloc_psf_isoline_param_menu(void){
    struct psf_isoline_param_menu *isoline_param_vws;
	isoline_param_vws = (struct psf_isoline_param_menu *) malloc(sizeof(struct psf_isoline_param_menu));
    if(isoline_param_vws == NULL){
        printf("malloc error for psf_isoline_param_menu\n");
        exit(0);
    }
    return isoline_param_vws;
};

static void dealloc_psf_isoline_param_menu(struct psf_isoline_param_menu *isoline_param_vws){
    g_object_unref(isoline_param_vws->spin_nline);
    g_object_unref(isoline_param_vws->spin_width);
    g_object_unref(isoline_param_vws->spin_digit);
    g_object_unref(isoline_param_vws->hbox_nline);
    g_object_unref(isoline_param_vws->hbox_width);
    free(isoline_param_vws);
    return;
};

static void set_nline_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = ( struct kemoviewer_gl_type *) user_data;
	int gtk_intvalue = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	kemoview_set_PSF_color_param(ISET_NLINE, gtk_intvalue, kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
	return;
}

static void set_width_CB(GtkWidget *entry, gpointer user_data)
{
	double current_width;
	int i_digit;
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_WIDTH,
                                 &current_width, &i_digit);
    kemoview_set_VIZ_color_value_w_exp(SURFACE_RENDERING, ISET_WIDTH,
                                       gtk_value, i_digit,
                                       kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
	return;
}

static void set_digit_CB(GtkWidget *entry, gpointer user_data)
{
	double current_width;
	int i_digit;
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	int gtk_value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_WIDTH,
                                 &current_width, &i_digit);
    kemoview_set_VIZ_color_value_w_exp(SURFACE_RENDERING, ISET_WIDTH,
                                       current_width, gtk_value,
                                       kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
	return;
}

void set_gtk_isoline_params_menu(struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_isoline_param_menu *isoline_param_vws){
    double current_width;
    int i_digit;
    int current_nline = kemoview_get_PSF_color_param(kemo_gl->kemoview_data, ISET_NLINE);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(isoline_param_vws->spin_nline), (double) current_nline);
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 SURFACE_RENDERING, ISET_WIDTH,
                                 &current_width, &i_digit);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(isoline_param_vws->spin_width), current_width);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(isoline_param_vws->spin_digit), (double) i_digit);
    return;
};

struct psf_isoline_param_menu * init_isoline_line_params(struct kemoviewer_gl_type *kemo_gl){
    struct psf_isoline_param_menu *isoline_param_vws = alloc_psf_isoline_param_menu();
    
    GtkAdjustment *adj_nline = gtk_adjustment_new (10, 0, 200, 1, 1, 0.0);
	isoline_param_vws->spin_nline = gtk_spin_button_new(GTK_ADJUSTMENT(adj_nline), 0, 0);
	g_signal_connect(isoline_param_vws->spin_nline, "value-changed",
                     G_CALLBACK(set_nline_CB), (gpointer) kemo_gl);
	
	GtkAdjustment *adj_width = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
	isoline_param_vws->spin_width = gtk_spin_button_new(GTK_ADJUSTMENT(adj_width), 0, 0);
	g_signal_connect(isoline_param_vws->spin_width, "value-changed", 
                     G_CALLBACK(set_width_CB), (gpointer) kemo_gl);
	
	GtkAdjustment *adj_digit = gtk_adjustment_new(0, -10, 10, 1, 1, 0);
	isoline_param_vws->spin_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
	g_signal_connect(isoline_param_vws->spin_digit, "value-changed",
                     G_CALLBACK(set_digit_CB), (gpointer) kemo_gl);
    
    isoline_param_vws->hbox_nline = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_append(GTK_BOX(isoline_param_vws->hbox_nline), gtk_label_new("Num. of lines: "));
	gtk_box_append(GTK_BOX(isoline_param_vws->hbox_nline), isoline_param_vws->spin_nline);
	
	isoline_param_vws->hbox_width = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_append(GTK_BOX(isoline_param_vws->hbox_width), gtk_label_new("Line Width: "));
	gtk_box_append(GTK_BOX(isoline_param_vws->hbox_width), isoline_param_vws->spin_width);
	gtk_box_append(GTK_BOX(isoline_param_vws->hbox_width), gtk_label_new("X 10^"));
    gtk_box_append(GTK_BOX(isoline_param_vws->hbox_width), isoline_param_vws->spin_digit);
    
    set_gtk_isoline_params_menu(kemo_gl, isoline_param_vws);
    return isoline_param_vws;
}

static struct psf_isoline_gtk_menu * alloc_psf_isoline_menu(void){
    struct psf_isoline_gtk_menu *psf_isoline_menu;
	psf_isoline_menu = (struct psf_isoline_gtk_menu *) malloc(sizeof(struct psf_isoline_gtk_menu));
    if(psf_isoline_menu == NULL) {
        printf("malloc error for psf_isoline_gtk_menu\n");
        exit(0);
    }
    return psf_isoline_menu;
};

void dealloc_psf_isoline_menu(struct psf_isoline_gtk_menu *psf_isoline_menu){
    dealloc_psf_isoline_switches(psf_isoline_menu->isoline_sw);
    dealloc_psf_isoline_param_menu(psf_isoline_menu->isoline_param_vws);
    dealloc_isoline_color_views(psf_isoline_menu->isoline_color_vws);
    g_object_unref(psf_isoline_menu->expander_iso);
    free(psf_isoline_menu);
    return;
}

void set_gtk_isoline_menu_values(struct kemoviewer_gl_type *kemo_gl,
                                 struct psf_isoline_gtk_menu *psf_isoline_menu){
    set_gtk_isoline_switches(kemo_gl,    psf_isoline_menu->isoline_sw);
    set_gtk_isoline_params_menu(kemo_gl, psf_isoline_menu->isoline_param_vws);
    set_isoline_color_dropdown(kemo_gl,  psf_isoline_menu->isoline_color_vws);
};

struct psf_isoline_gtk_menu * init_isoline_menu_expander(struct kemoviewer_gl_type *kemo_gl,
                                                         GtkWidget *window){
    struct psf_isoline_gtk_menu *psf_isoline_menu = alloc_psf_isoline_menu();
    psf_isoline_menu->isoline_sw =        init_isoline_switches(kemo_gl);
    psf_isoline_menu->isoline_param_vws = init_isoline_line_params(kemo_gl);
    psf_isoline_menu->isoline_color_vws = init_isoline_color_views(kemo_gl);
    
    GtkWidget *isoline_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(isoline_box), psf_isoline_menu->isoline_sw->hbox_draw);
    gtk_box_append(GTK_BOX(isoline_box), psf_isoline_menu->isoline_sw->hbox_zero);
    gtk_box_append(GTK_BOX(isoline_box), psf_isoline_menu->isoline_color_vws->hbox_color);
    gtk_box_append(GTK_BOX(isoline_box), psf_isoline_menu->isoline_param_vws->hbox_nline);
    gtk_box_append(GTK_BOX(isoline_box), psf_isoline_menu->isoline_param_vws->hbox_width);
    
    psf_isoline_menu->expander_iso = wrap_into_scroll_expansion_gtk4("Isolines", 360, 220,
                                                                     window, isoline_box);
    return psf_isoline_menu;
}
