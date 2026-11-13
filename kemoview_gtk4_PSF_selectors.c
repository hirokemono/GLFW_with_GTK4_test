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

void reset_psf_list_model(struct kemoviewer_gl_type *kemo_gl,
                          GtkStringList * psf_list_model){
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
            printf("%lu: %s\n", strlen(label_tmp), label_tmp);
            gtk_string_list_append(psf_list_model, stripped_filehead->string);
            kemoview_free_kvstring(stripped_filehead);
        };
        printf("kemoview_set_PSF_loaded_params \n");
    };
    kemoview_set_PSF_loaded_params(SET_CURRENT, id_current, kemo_gl->kemoview_data);
    return;
}

int set_selected_psf_list_address(struct kemoviewer_gl_type *kemo_gl,
                                  GtkStringList *psf_list_model){
    struct kv_string *stripped_filehead = kemoview_alloc_kvstring();
    kemoview_get_PSF_file_prefix(kemo_gl, stripped_filehead);
    
    char *text;
    int ipsf;
    int iflag = 0;
    guint num_menu = g_list_model_get_n_items(G_LIST_MODEL(psf_list_model));
    for(ipsf=0; ipsf<num_menu; ipsf++){
        text = (char *) gtk_string_list_get_string(psf_list_model, ipsf);
        iflag = cmp_no_case_c(stripped_filehead->string, text);
        if(iflag > 0) return ipsf;
    }
    kemoview_free_kvstring(stripped_filehead);
    return -1;
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
            printf("%lu: %s\n", strlen(label_tmp), label_tmp);
            
            iflag = cmp_no_case_c(stripped_filehead->string, selected_text);
            kemoview_free_kvstring(stripped_filehead);
            if(iflag > 0) return ipsf;
        };
    };
    printf("No current PSf is found: \n");
    return -1;
}


void reset_psf_field_model(struct kemoviewer_gl_type *kemo_gl, int id_model,
                           GtkStringList * psf_field_model){
    int num_field = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data, id_model,
                                                 NUM_FIELD_FLAG);
    int ifld;
    int n_old = g_list_model_get_n_items(G_LIST_MODEL(psf_field_model));
    for(ifld=0;ifld<n_old;ifld++){
        gtk_string_list_remove(psf_field_model, IZERO);
    };
    
    for(ifld=0;ifld<num_field;ifld++){
        struct kv_string *fieldname = kemoview_alloc_kvstring();
        kemoview_get_VIZ_field_name(kemo_gl->kemoview_data, id_model,
                                    fieldname, ifld);
        gtk_string_list_append(psf_field_model, fieldname->string);
        kemoview_free_kvstring(fieldname);
    };
    return;
}

int set_selected_psf_field_list(struct kemoviewer_gl_type *kemo_gl,
                                int id_model,
                                GtkStringList *psf_field_model){
    struct kv_string *fieldname = kemoview_alloc_kvstring();
    int ifld = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data, id_model,
                                            FIELD_SEL_FLAG);
    kemoview_get_VIZ_field_name(kemo_gl->kemoview_data, id_model,
                                fieldname, ifld);
    
    char *text;
    int iflag = 0;
    int id_selected = -1;
    guint num_menu = g_list_model_get_n_items(G_LIST_MODEL(psf_field_model));
    for(ifld=0; ifld<num_menu; ifld++){
        text = (char *) gtk_string_list_get_string(psf_field_model, ifld);
        iflag = cmp_no_case_c(fieldname->string, text);
        if(iflag > 0) id_selected = ifld;
    }
    kemoview_free_kvstring(fieldname);
    return id_selected;
}

int find_selected_field_id(struct kemoviewer_gl_type *kemo_gl, 
                           int id_model,
                           char *selected_text){
    int i_selected = -1;
    int iflag = 0;
    struct kv_string *fieldname = kemoview_alloc_kvstring();
    int num_field = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data, id_model,
                                                 NUM_FIELD_FLAG);
    int ifld;
    for(ifld=0; ifld<num_field; ifld++){
        kemoview_get_VIZ_field_name(kemo_gl->kemoview_data, id_model,
                                    fieldname, ifld);
        iflag = cmp_no_case_c(fieldname->string, selected_text);
        if(iflag > 0) i_selected = ifld;
    };
    kemoview_free_kvstring(fieldname);
    if(i_selected < 0) printf("No selected field is found: \n");
    return i_selected;
}



void reset_psf_component_model(struct kemoviewer_gl_type *kemo_gl, int id_model,
                               GtkStringList * psf_component_model){
	char comp_name[128];
    
    int ifield = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              id_model, FIELD_SEL_FLAG);
	int npick = (int) kemoview_get_VIZ_num_pick_comps(kemo_gl->kemoview_data,
                                                      id_model, ifield);
    int id_coord = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                                id_model, COORDINATE_FLAG);
    
    int ipick;
    int n_old = g_list_model_get_n_items(G_LIST_MODEL(psf_component_model));
    for(ipick=0;ipick<n_old;ipick++){
        gtk_string_list_remove(psf_component_model, IZERO);
    };
    
    for(ipick=0;ipick<npick;ipick++){
        set_PSF_full_component_name(npick, id_coord, ipick, comp_name);
        gtk_string_list_append(psf_component_model, comp_name);
    };
    return;
}

int set_selected_psf_component_list(struct kemoviewer_gl_type *kemo_gl,
                                    int id_model,
                                    GtkStringList *psf_component_model){
	char comp_name[128];
    struct kv_string *fieldname = kemoview_alloc_kvstring();
    
    int ifield = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              id_model, FIELD_SEL_FLAG);
	int npick = (int) kemoview_get_VIZ_num_pick_comps(kemo_gl->kemoview_data,
                                                      id_model, ifield);
    int id_coord = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                                id_model, COORDINATE_FLAG);
    int ipick = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data, id_model,
                                             COMPONENT_SEL_FLAG);
    set_PSF_full_component_name(npick, id_coord, ipick, comp_name);
    
    char *text;
    int i;
    int iflag = 0;
    int id_selected = -1;
    guint num_menu = g_list_model_get_n_items(G_LIST_MODEL(psf_component_model));
    for(i=0; i<num_menu; i++){
        text = (char *) gtk_string_list_get_string(psf_component_model, i);
        iflag = cmp_no_case_c(comp_name, text);
        if(iflag > 0) id_selected = i;
    }
    kemoview_free_kvstring(fieldname);
    return id_selected;
}

int find_selected_component_id(struct kemoviewer_gl_type *kemo_gl, 
                               int id_model,
                               char *selected_text){
	char comp_name[128];
    
    int ifield = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                              id_model, FIELD_SEL_FLAG);
	int npick = (int) kemoview_get_VIZ_num_pick_comps(kemo_gl->kemoview_data,
                                                      id_model, ifield);
    int id_coord = kemoview_get_VIZ_field_param(kemo_gl->kemoview_data,
                                                id_model, COORDINATE_FLAG);
    
    int i_selected = -1;
    int iflag = 0;
    int ipick;
    for(ipick=0;ipick<npick;ipick++){
        set_PSF_full_component_name(npick, id_coord, ipick, comp_name);
        iflag = cmp_no_case_c(comp_name, selected_text);
        if(iflag > 0) i_selected = ipick;
    };
    if(i_selected < 0) printf("No selected field is found: \n");
    return i_selected;
}

