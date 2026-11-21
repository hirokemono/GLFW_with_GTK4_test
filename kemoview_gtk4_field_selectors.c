/***********************************************************************
 *  kemoview_gtk4_field_selectors.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_gtk4_field_selectors.h"

int kemoview_psf_select_CB(struct kemoviewer_gl_type *kemo_gl,
                           int id_model, GtkDropDown *dropdown){
    int id_psf = -1;
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    if(selected_item == NULL){return id_psf;};
    char *selected_text = (char *) gtk_string_object_get_string(selected_item);
    printf("selected PSF: %s \n", selected_text);
    
    id_psf = find_selected_psf_id(kemo_gl, selected_text);
    if(id_psf < 0){return id_psf;};
    printf("New current PSF ID: %d \n", id_psf);
    
    kemoview_set_PSF_loaded_params(SET_CURRENT, id_psf,
                                   kemo_gl->kemoview_data);
    kemoview_set_VIZ_field_param(IZERO, id_model,
                                 FIELD_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    kemoview_set_VIZ_field_param(IZERO, id_model,
                                 COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
	return id_psf;
};

int kemoview_field_select_CB(struct kemoviewer_gl_type *kemo_gl,
                             int id_model, GtkDropDown *dropdown){
    int id_field = -1;
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    if(selected_item == NULL){return id_field;};
    char *selected_text = (char *) gtk_string_object_get_string(selected_item);
    printf("selected field: %s \n", selected_text);
    
    id_field = find_selected_field_id(kemo_gl, id_model, selected_text);
    if(id_field < 0){return id_field;};
    kemoview_set_VIZ_field_param(id_field, id_model, 
                                 FIELD_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    kemoview_set_VIZ_field_param(IZERO, id_model,
                                 COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
	return id_field;
};

int kemoview_component_select(struct kemoviewer_gl_type *kemo_gl,
                              int id_model, GtkDropDown *dropdown){
    int id_pick = -1;
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    printf("selected component: %p \n", selected_item);
    if(selected_item == NULL){return id_pick;};
    
    char *selected_text = (char *) gtk_string_object_get_string(selected_item);
    id_pick = find_selected_component_id(kemo_gl, id_model, selected_text);
    if(id_pick < 0){return id_pick;};
    
    kemoview_set_VIZ_field_param(id_pick, id_model, 
                                 COMPONENT_SEL_FLAG,
                                 kemo_gl->kemoview_data);
    return id_pick;
};


