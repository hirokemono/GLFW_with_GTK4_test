/***********************************************************************
 *  kemoview_gtk4_fieldline_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_fieldline_menu.h"

static void fline_thickness_CB(GtkWidget *entry, gpointer user_data)
{
    struct fieldline_gtk_menu *fline_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "fline_view");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
	double current_thick;
	int current_digit;
    
	double thick_in = gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	if(thick_in < 0) return;
	
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 fline_gmenu->iflag_flinemode, ISET_WIDTH,
                                 &current_thick, &current_digit);
    kemoview_set_VIZ_color_value_w_exp(fline_gmenu->iflag_flinemode, ISET_WIDTH,
                                       thick_in, current_digit,
                                       kemo_gl->kemoview_data);
    
    
    draw_full_gl(kemo_gl);
}
static void fline_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct fieldline_gtk_menu *fline_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "fline_view");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
	double current_thick;
	int current_digit;
    
	int in_digit = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 fline_gmenu->iflag_flinemode, ISET_WIDTH,
                                 &current_thick, &current_digit);
    kemoview_set_VIZ_color_value_w_exp(fline_gmenu->iflag_flinemode, ISET_WIDTH,
                                       current_thick, in_digit,
                                       kemo_gl->kemoview_data);
	
    draw_full_gl(kemo_gl);
}


static void psf_fieldtube_switch_CB(GObject *switch_1, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int istate = gtk_switch_get_state(GTK_SWITCH(switch_1));
    kemoview_set_line_type_flag(istate, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
};

static void fline_colorbar_switch_CB(GObject *switch_bar, GParamSpec *pspec, gpointer user_data){
    struct fieldline_gtk_menu *fline_gmenu
            = (struct fieldline_gtk_menu *) g_object_get_data(G_OBJECT(user_data), "fline_view");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_bar));
    kemoview_set_colorbar_draw_flag(iflag, fline_gmenu->iflag_flinemode,
                                    kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    return;
};



static void set_ref_vector_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING, ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                       gtk_floatvalue, i_digit,
                                       TRACER_RENDERING,
                                       kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_ref_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_intvalue = (double) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_PSF_REFVECT,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_PSF_REFVECT,
                                      current_value, gtk_intvalue,
                                      TRACER_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_vect_increment_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	double gtk_floatvalue = (double) gtk_spin_button_get_value(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_VECTOR_INC,
                                      gtk_floatvalue, i_digit,
                                      TRACER_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}

static void set_increment_digit_CB(GtkWidget *entry, gpointer user_data)
{
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
	int gtk_intvalue = (int) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry));
	int i_digit;
	double current_value;
    kemoview_get_VIZ_vector_w_exp(kemo_gl->kemoview_data,
                                  TRACER_RENDERING,
                                  ISET_VECTOR_INC,
                                  &current_value, &i_digit);
    kemoview_set_each_VIZ_vector_w_exp(ISET_VECTOR_INC,
                                      current_value, gtk_intvalue,
                                      TRACER_RENDERING,
                                      kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
	return;
}


static void psf_vector_switch_CB(GObject *switch_vect, GParamSpec *pspec, gpointer user_data){
    struct kemoviewer_gl_type *kemo_gl = (struct kemoviewer_gl_type *) user_data;
    int iflag = gtk_switch_get_state(GTK_SWITCH(switch_vect));
    kemoview_set_VIZ_vector_draw_flags(iflag, TRACER_RENDERING,
                                       kemo_gl->kemoview_data);

    draw_full_gl(kemo_gl);
	return;
};




void set_gtk_fieldline_menu(struct kemoviewer_gl_type *kemo_gl,
                            struct fieldline_gtk_menu *fline_gmenu){
	double current_thick;
	int int_thick, current_digit;
    
    set_gtk4_surf_range_values(kemo_gl, fline_gmenu->iflag_flinemode,
                               fline_gmenu->fline_range_vws);
    set_gtk4_fline_color_sel(kemo_gl, fline_gmenu->iflag_flinemode,
                             fline_gmenu->fline_color_sel_vws);
    reset_colormap_menu_views(kemo_gl, fline_gmenu->fline_color_vws);
    
    kemoview_get_VIZ_color_w_exp(kemo_gl->kemoview_data,
                                 fline_gmenu->iflag_flinemode, ISET_WIDTH,
                                 &current_thick, &current_digit);
	int_thick = (int) current_thick;
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(fline_gmenu->spin_thick), (double) int_thick);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(fline_gmenu->spin_digit), (double) current_digit);
    return;
};

void init_fieldline_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                              struct fieldline_gtk_menu *fline_gmenu,
                              GtkWidget *parent_window){
    GtkWidget *label_tree_color;
    GtkCellRenderer *renderer_color;
    GtkTreeModel *model_color;
    GtkTreeModel *child_model_color;
    int iflag = 0;
    int index = 0;

    GtkAdjustment *adj_min_value, *adj_min_digit;
    GtkAdjustment *adj_max_value, *adj_max_digit;
    
    fline_gmenu->iflag_flinemode = FIELDLINE_RENDERING;
    GtkWidget *dummy_entry = gtk_entry_new();
    g_object_set_data(G_OBJECT(dummy_entry),
                      "fline_view", (gpointer) fline_gmenu);
    g_object_set_data(G_OBJECT(dummy_entry),
                      "kemoview_gl", (gpointer) kemo_gl);
    
    fline_gmenu->fline_color_sel_vws = init_gtk4_fline_color_sel(kemo_gl, 
                                                                 &fline_gmenu->iflag_flinemode,
                                                                 parent_window);
    
    fline_gmenu->fline_switch_bar = gtk_switch_new();
    iflag = kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                            fline_gmenu->iflag_flinemode);
    gtk_switch_set_state(GTK_SWITCH(fline_gmenu->fline_switch_bar), iflag);
    gtk_switch_set_active(GTK_SWITCH(fline_gmenu->fline_switch_bar), FALSE);
    g_signal_connect(G_OBJECT(fline_gmenu->fline_switch_bar), "notify::active",
                     G_CALLBACK(fline_colorbar_switch_CB), (gpointer) dummy_entry);

    if(kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                       fline_gmenu->iflag_flinemode) == 0){
        gtk_switch_set_active(GTK_SWITCH(fline_gmenu->fline_switch_bar), FALSE);
    } else {
        gtk_switch_set_active(GTK_SWITCH(fline_gmenu->fline_switch_bar), TRUE);
    };

    
    fline_gmenu->switch_tube = gtk_switch_new();
    iflag = kemoview_get_line_type_flag(kemo_gl->kemoview_data);
    gtk_switch_set_state(GTK_SWITCH(fline_gmenu->switch_tube), iflag);
    gtk_switch_set_active(GTK_SWITCH(fline_gmenu->switch_tube), FALSE);
    g_signal_connect(G_OBJECT(fline_gmenu->switch_tube), "notify::active",
                     G_CALLBACK(psf_fieldtube_switch_CB), (gpointer) kemo_gl);
    
    GtkAdjustment *adj_thick = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    GtkAdjustment *adj_digit = gtk_adjustment_new(-3, -30, 30, 1, 1, 0.0);
    fline_gmenu->spin_thick = gtk_spin_button_new(GTK_ADJUSTMENT(adj_thick), 0, 0);
    fline_gmenu->spin_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
    g_signal_connect(fline_gmenu->spin_thick, "value-changed",
                     G_CALLBACK(fline_thickness_CB), (gpointer) dummy_entry);
    g_signal_connect(fline_gmenu->spin_digit, "value-changed",
                     G_CALLBACK(fline_digit_CB), (gpointer) dummy_entry);
 
    fline_gmenu->fline_range_vws = init_gtk_range_view(kemo_gl,
                                                       &fline_gmenu->iflag_flinemode,
                                                       parent_window);
    
    fline_gmenu->fline_color_vws = init_gtk_psf_colormap_expander(kemo_gl, 
                                                                  fline_gmenu->iflag_flinemode,
                                                                  parent_window);
    fline_gmenu->expander_fline_color = init_gtk_psf_colormap_expander(kemo_gl, fline_gmenu->flineWin,
                                                                       fline_gmenu->fline_color_vws);
    return;
}

GtkWidget * pack_fieldline_menu_frame(struct fieldline_gtk_menu *fline_gmenu){
    GtkWidget *frame;
    
    GtkWidget *hbox_tube = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_tube), gtk_label_new("Draw tube: "));
	gtk_box_append(GTK_BOX(hbox_tube), fline_gmenu->switch_tube);
	
    GtkWidget *hbox_thickness = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_thickness), gtk_label_new("Thickness: "));
	gtk_box_append(GTK_BOX(hbox_thickness), fline_gmenu->spin_thick);
	gtk_box_append(GTK_BOX(hbox_thickness), gtk_label_new("X 10^"));
	gtk_box_append(GTK_BOX(hbox_thickness), fline_gmenu->spin_digit);
	
    GtkWidget *hbox_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_field), gtk_label_new("Field: "));
    gtk_box_append(GTK_BOX(hbox_field), fline_gmenu->combobox_field);
    
    GtkWidget *hbox_comp = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_comp), gtk_label_new("Component: "));
    gtk_box_append(GTK_BOX(hbox_comp), fline_gmenu->combobox_comp);
    

    GtkWidget *hbox_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_bar), gtk_label_new("Draw color bar: "));
    gtk_box_append(GTK_BOX(hbox_bar), fline_gmenu->fline_switch_bar);
    

    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->closeButton);
    gtk_box_append(GTK_BOX(menu_box), hbox_field);
    gtk_box_append(GTK_BOX(menu_box), hbox_comp);
	gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_color_sel_vws->hbox_fline_color);
    gtk_box_append(GTK_BOX(menu_box), hbox_bar);
    gtk_box_append(GTK_BOX(menu_box), hbox_tube);
	gtk_box_append(GTK_BOX(menu_box), hbox_thickness);

	gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_range_vws->hbox_psf_range);
    
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->expander_fline_color);
    frame = wrap_into_frame_gtk4("Fieldline", menu_box);
    return frame;
}




void init_tracer_menu_hbox(struct kemoviewer_gl_type *kemo_gl,
                           struct fieldline_gtk_menu *fline_gmenu,
                           GtkWidget *parent_window){
    GtkWidget *label_tree_color;
    GtkCellRenderer *renderer_color;
    GtkTreeModel *model_color;
    GtkTreeModel *child_model_color;
    int iflag = 0;
    int index = 0;
    
    fline_gmenu->iflag_flinemode = TRACER_RENDERING;
    GtkWidget *dummy_entry = gtk_entry_new();
    g_object_set_data(G_OBJECT(dummy_entry),
                      "fline_view", (gpointer) fline_gmenu);
    g_object_set_data(G_OBJECT(dummy_entry),
                      "kemoview_gl", (gpointer) kemo_gl);
    
    fline_gmenu->fline_color_sel_vws = init_gtk4_fline_color_sel(kemo_gl, 
                                                                 &fline_gmenu->iflag_flinemode,
                                                                 parent_window);
    
	fline_gmenu->switch_vect = gtk_switch_new();
    iflag = kemoview_get_VIZ_vector_draw_flags(kemo_gl->kemoview_data,
                                               TRACER_RENDERING);
    gtk_switch_set_state(GTK_SWITCH(fline_gmenu->switch_vect), iflag);
	g_signal_connect(G_OBJECT(fline_gmenu->switch_vect), "notify::active",
				G_CALLBACK(psf_vector_switch_CB), (gpointer) kemo_gl);
	
	if(kemoview_get_VIZ_vector_draw_flags(kemo_gl->kemoview_data,
                                          TRACER_RENDERING) == 0){
		gtk_switch_set_active(GTK_SWITCH(fline_gmenu->switch_vect), FALSE);
	} else {
		gtk_switch_set_active(GTK_SWITCH(fline_gmenu->switch_vect), TRUE);
	};
	
    
    fline_gmenu->fline_switch_bar = gtk_switch_new();
    iflag = kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                            fline_gmenu->iflag_flinemode);
    gtk_switch_set_state(GTK_SWITCH(fline_gmenu->fline_switch_bar), iflag);
    gtk_switch_set_active(GTK_SWITCH(fline_gmenu->fline_switch_bar), FALSE);
    g_signal_connect(G_OBJECT(fline_gmenu->fline_switch_bar), "notify::active",
                     G_CALLBACK(fline_colorbar_switch_CB), (gpointer) dummy_entry);

    if(kemoview_get_colorbar_draw_flag(kemo_gl->kemoview_data,
                                       fline_gmenu->iflag_flinemode) == 0){
        gtk_switch_set_active(GTK_SWITCH(fline_gmenu->fline_switch_bar), FALSE);
    } else {
        gtk_switch_set_active(GTK_SWITCH(fline_gmenu->fline_switch_bar), TRUE);
    };
    
    GtkAdjustment *adj_thick = gtk_adjustment_new(1, 0, 9, 1, 1, 0);
    GtkAdjustment *adj_digit = gtk_adjustment_new(-3, -30, 30, 1, 1, 0.0);
    fline_gmenu->spin_thick = gtk_spin_button_new(GTK_ADJUSTMENT(adj_thick), 0, 0);
    fline_gmenu->spin_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_digit), 0, 0);
    g_signal_connect(fline_gmenu->spin_thick, "value-changed",
                     G_CALLBACK(fline_thickness_CB), (gpointer) dummy_entry);
    g_signal_connect(fline_gmenu->spin_digit, "value-changed",
                     G_CALLBACK(fline_digit_CB), (gpointer) dummy_entry);
 
    fline_gmenu->fline_range_vws = init_gtk_range_view(kemo_gl,
                                                       &fline_gmenu->iflag_flinemode,
                                                       parent_window);
    
    
	GtkAdjustment *adj_ref_vect = gtk_adjustment_new(1, 1, 9, 1, 1, 0);
	fline_gmenu->spin_ref_vect = gtk_spin_button_new(GTK_ADJUSTMENT(adj_ref_vect), 0, 0);
	g_signal_connect(fline_gmenu->spin_ref_vect, "value-changed", 
                     G_CALLBACK(set_ref_vector_CB), (gpointer) kemo_gl);
	
	GtkAdjustment *adj_ref_digit = gtk_adjustment_new(0, -10, 10, 1, 1, 0);
	fline_gmenu->spin_ref_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_ref_digit), 0, 0);
	g_signal_connect(fline_gmenu->spin_ref_digit, "value-changed",
                     G_CALLBACK(set_ref_digit_CB), (gpointer) kemo_gl);
	
	GtkAdjustment *adj_vect_inc = gtk_adjustment_new(1, 1, 9, 1, 1, 0);
	fline_gmenu->spin_vect_inc = gtk_spin_button_new(GTK_ADJUSTMENT(adj_vect_inc), 0, 0);
	g_signal_connect(fline_gmenu->spin_vect_inc, "value-changed",
					 G_CALLBACK(set_vect_increment_CB), (gpointer) kemo_gl);
	
	GtkAdjustment *adj_inc_digit = gtk_adjustment_new(0, 0, 10, 1, 1, 0);
	fline_gmenu->spin_inc_digit = gtk_spin_button_new(GTK_ADJUSTMENT(adj_inc_digit), 0, 0);
	g_signal_connect(fline_gmenu->spin_inc_digit, "value-changed",
					 G_CALLBACK(set_increment_digit_CB), (gpointer) kemo_gl);
	
    
    fline_gmenu->fline_color_vws = init_gtk_psf_colormap_expander(kemo_gl, 
                                                                  fline_gmenu->iflag_flinemode,
                                                                  parent_window);
    fline_gmenu->expander_fline_color = init_gtk_psf_colormap_expander(kemo_gl, fline_gmenu->flineWin,
                                                                       fline_gmenu->fline_color_vws);
    return;
}

GtkWidget * pack_tracer_menu_frame(struct fieldline_gtk_menu *fline_gmenu){
    GtkWidget *frame;
	
    GtkWidget *hbox_thickness = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_thickness), gtk_label_new("Thickness: "));
	gtk_box_append(GTK_BOX(hbox_thickness), fline_gmenu->spin_thick);
	gtk_box_append(GTK_BOX(hbox_thickness), gtk_label_new("X 10^"));
	gtk_box_append(GTK_BOX(hbox_thickness), fline_gmenu->spin_digit);
	
    GtkWidget *hbox_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_field), gtk_label_new("Field: "));
    gtk_box_append(GTK_BOX(hbox_field), fline_gmenu->combobox_field);
    
    GtkWidget *hbox_comp = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_comp), gtk_label_new("Component: "));
    gtk_box_append(GTK_BOX(hbox_comp), fline_gmenu->combobox_comp);
    

    GtkWidget *hbox_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_bar), gtk_label_new("Draw color bar: "));
    gtk_box_append(GTK_BOX(hbox_bar), fline_gmenu->fline_switch_bar);
    
	GtkWidget *hbox_vec = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_vec), gtk_label_new("Draw vector: "));
	gtk_box_append(GTK_BOX(hbox_vec), fline_gmenu->switch_vect);
	
	GtkWidget *hbox_22 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_22), gtk_label_new("Increment: "));
	gtk_box_append(GTK_BOX(hbox_22), fline_gmenu->spin_vect_inc);
	gtk_box_append(GTK_BOX(hbox_22), gtk_label_new("X 10^"));
	gtk_box_append(GTK_BOX(hbox_22), fline_gmenu->spin_inc_digit);
	
	GtkWidget *hbox_12 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox_12), gtk_label_new("Reference: "));
	gtk_box_append(GTK_BOX(hbox_12), fline_gmenu->spin_ref_vect);
	gtk_box_append(GTK_BOX(hbox_12), gtk_label_new("X 10^"));
	gtk_box_append(GTK_BOX(hbox_12), fline_gmenu->spin_ref_digit);
    
    
    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->closeButton);
    gtk_box_append(GTK_BOX(menu_box), hbox_field);
    gtk_box_append(GTK_BOX(menu_box), hbox_comp);
	gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_color_sel_vws->hbox_fline_color);
    gtk_box_append(GTK_BOX(menu_box), hbox_bar);
	gtk_box_append(GTK_BOX(menu_box), hbox_thickness);
	gtk_box_append(GTK_BOX(menu_box), hbox_vec);
	gtk_box_append(GTK_BOX(menu_box), hbox_22);
	gtk_box_append(GTK_BOX(menu_box), hbox_12);

	gtk_box_append(GTK_BOX(menu_box), fline_gmenu->fline_range_vws->hbox_psf_range);
    
    gtk_box_append(GTK_BOX(menu_box), fline_gmenu->expander_fline_color);
    frame = wrap_into_frame_gtk4("Fieldline", menu_box);
    return frame;
}

struct fieldline_gtk_menu * alloc_gtk_fline_menu(){
    struct fieldline_gtk_menu *fline_gmenu;
	fline_gmenu = (struct fieldline_gtk_menu *) malloc(sizeof(struct fieldline_gtk_menu));
    if (fline_gmenu == NULL) {
        printf("malloc error for fieldline_gtk_menu\n");
        exit(0);
    }
    return fline_gmenu;
};


void dealloc_gtk_fline_menu(struct fieldline_gtk_menu *fline_gmenu){
    dealloc_gtk_range_view(fline_gmenu->fline_range_vws);
    dealloc_fline_color_views(fline_gmenu->fline_color_sel_vws);
    dealloc_colormap_views(fline_gmenu->fline_color_vws);
    free(fline_gmenu);
    return;
};
