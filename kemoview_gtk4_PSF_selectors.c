/************************************************************************
 *  kemoview_gtk4_PSF_selectors.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_PSF_selectors.h"

/*

void kemoview_field_select_CB(GtkComboBox *combobox_field, int id_model, 
                              struct kemoviewer_gl_type *kemo_gl)
{
    int index_mode = gtk_selected_combobox_index(combobox_field);
    int num_fld = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                               id_model, NUM_FIELD_FLAG);
    if(index_mode >= num_fld || index_mode < 0){
        index_mode = 0;
    }
    
    kemoview_set_VIZ_field_param(index_mode,
                                 id_model, FIELD_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    kemoview_set_VIZ_field_param(IZERO,
                                 id_model, COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
	return;
};

void kemoview_component_select_CB(GtkComboBox *combobox_comp, int id_model, 
                                  struct kemoviewer_gl_type *kemo_gl)
{
    int index_mode = gtk_selected_combobox_index(combobox_comp);
    int if_psf = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              id_model,
                                              FIELD_SEL_FLAG);
    int ncomp = (int) kemoview_get_VIZ_num_component(kemo_gl->kemoview_data,
                                                     id_model, if_psf);
    if(index_mode >= ncomp || index_mode < 0){
        index_mode = 0;
    }
	
    kemoview_set_VIZ_field_param(index_mode,
                                 id_model, COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    return;
};

GtkWidget * draw_viz_field_gtk_box(struct kemoviewer_gl_type *kemo_gl, int id_model){
	int num_field = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data, id_model,
                                                 NUM_FIELD_FLAG);
	int if_fline =  kemoview_get_VIZ_field_param(kemo_gl->kemoview_data, id_model,
                                                 FIELD_SEL_FLAG);
	int ifld;
	
	GtkWidget *label_tree_field = create_fixed_label_w_index_tree();
	GtkTreeModel *model_field =       gtk_tree_view_get_model(GTK_TREE_VIEW(label_tree_field));  
	GtkTreeModel *child_model_field = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(model_field));
	int index = 0;
	for(ifld=0;ifld<num_field;ifld++){
        struct kv_string *colorname = kemoview_alloc_kvstring();
        kemoview_get_VIZ_field_name(kemo_gl->kemoview_data, id_model,
                                    colorname, ifld);
		index = append_ci_item_to_tree(index, colorname->string, ifld, child_model_field);
        kemoview_free_kvstring(colorname);
    };
	
    GtkCellRenderer *renderer_field = gtk_cell_renderer_text_new();
	GtkWidget *combobox_field = gtk_combo_box_new();
    gtk_combo_box_set_model(GTK_COMBO_BOX(combobox_field), child_model_field);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_field), if_fline);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_field), renderer_field, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_field), renderer_field,
                                   "text", COLUMN_FIELD_NAME, NULL);
    return combobox_field;
}


GtkWidget * draw_viz_component_gtk_box(struct kemoviewer_gl_type *kemo_gl, int id_model){
	char comp_name[128];
    
    int if_fline = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                                id_model, FIELD_SEL_FLAG);
	int ic_fline = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                                id_model, COMPONENT_SEL_FLAG);
	int ncomp = (int) kemoview_get_VIZ_num_component(kemo_gl->kemoview_data,
                                                     id_model, if_fline);
	
    GtkWidget *label_tree_comp = create_fixed_label_w_index_tree();
    GtkTreeModel *model_comp =       gtk_tree_view_get_model(GTK_TREE_VIEW(label_tree_comp));
    GtkTreeModel *child_model_comp = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(model_comp));
    int id_coord = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                                id_model, COORDINATE_FLAG);
    int icomp;
    int index = 0;
    for(icomp=0;icomp<ncomp;icomp++){
        set_PSF_component_name(ncomp, id_coord, icomp, comp_name);
        index = append_ci_item_to_tree(index, comp_name, icomp, child_model_comp);
    };
    
    GtkWidget *combobox_comp = gtk_combo_box_new_with_model(child_model_comp);
    GtkCellRenderer *renderer_comp = gtk_cell_renderer_text_new();
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_comp), ic_fline);
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox_comp), renderer_comp, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox_comp), renderer_comp,
                                   "text", COLUMN_FIELD_NAME, NULL);
	return combobox_comp;
}

*/

const char * const null_text_list[] = {NULL};

GtkStringList * init_psf_list_model(struct kemoviewer_gl_type *kemo_gl){
    GtkStringList * psf_list_model = gtk_string_list_new(null_text_list);
	struct kv_string *stripped_filehead;
	char label_tmp[512];
	int id_current = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data, SET_CURRENT);
    int nmax_loaded = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data, MAX_LOADED);
    
    int ipsf;
    for(ipsf=0; ipsf<nmax_loaded; ipsf++){
        if(kemoview_get_PSF_loaded_flag(kemo_gl->kemoview_data, ipsf) > 0) {
            kemoview_set_PSF_loaded_params(SET_CURRENT, ipsf, kemo_gl->kemoview_data);
            stripped_filehead = kemoview_alloc_kvstring();
            kemoview_get_PSF_file_prefix(kemo_gl, stripped_filehead);
            sprintf(label_tmp, "%d: %s", ipsf, stripped_filehead->string);
            printf("%d: %s\n", strlen(label_tmp), label_tmp);
            gtk_string_list_append(psf_list_model, stripped_filehead->string);
            kemoview_free_kvstring(stripped_filehead);
        };
        printf("kemoview_set_PSF_loaded_params \n");
    };
    kemoview_set_PSF_loaded_params(SET_CURRENT, id_current, kemo_gl->kemoview_data);
    return psf_list_model;
}

int set_selected_psf_list_address(struct kemoviewer_gl_type *kemo_gl,
                                  GtkStringList *psf_list_model){
    struct kv_string *stripped_filehead = kemoview_alloc_kvstring();
    kemoview_get_PSF_file_prefix(kemo_gl, stripped_filehead);
    
    char *text;
    int ipsf;
    int iflag = 0;
    int i_selected = -1;
    guint num_menu = g_list_model_get_n_items(G_LIST_MODEL(psf_list_model));
    for(ipsf=0; ipsf<num_menu; ipsf++){
        text = gtk_string_list_get_string(psf_list_model, ipsf);
        iflag = cmp_no_case_c(stripped_filehead->string, text);
        if(iflag > 0) return i_selected = iflag;
    }
    kemoview_free_kvstring(stripped_filehead);
    return i_selected;
}

int find_selected_psf_id(struct kemoviewer_gl_type *kemo_gl,
                         char *selected_text){
    int iflag = 0;
    
    int nmax_loaded = kemoview_get_PSF_loaded_params(kemo_gl->kemoview_data, MAX_LOADED);
	struct kv_string *stripped_filehead;
	char label_tmp[512];
    int ipsf;
    for(ipsf=0; ipsf<nmax_loaded; ipsf++){
        if(kemoview_get_PSF_loaded_flag(kemo_gl->kemoview_data, ipsf) > 0) {
            kemoview_set_PSF_loaded_params(SET_CURRENT, ipsf, kemo_gl->kemoview_data);
            stripped_filehead = kemoview_alloc_kvstring();
            kemoview_get_PSF_file_prefix(kemo_gl, stripped_filehead);
            sprintf(label_tmp, "%d: %s", ipsf, stripped_filehead->string);
            printf("%d: %s\n", strlen(label_tmp), label_tmp);
            
            iflag = cmp_no_case_c(stripped_filehead->string, selected_text);
            kemoview_free_kvstring(stripped_filehead);
            if(iflag > 0) return ipsf;
        };
    };
    printf("No current PSf is found: \n");
    return -1;
}
