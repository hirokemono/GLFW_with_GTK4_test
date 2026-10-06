/*
 *  kemoview_gtk4_viewmode_menu.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_viewmode_menu.h"

const char * const hd_3Dview =  "3D-View";     //  VIEW_3D
const char * const hd_Stereo =  "Stereo-View"; //  VIEW_STEREO
const char * const hd_Mapview = "Map-View";    //  VIEW_MAP
const char * const hd_XYview =  "XY-View";     //  VIEW_XY
const char * const hd_XZview =  "XZ-View";     //  VIEW_XZ
const char * const hd_YZview =  "YZ-View";     //  VIEW_YZ

const char * const view_modes[] = {hd_3Dview,
                                   hd_Stereo,
                                   hd_Mapview,
                                   hd_XYview,
                                   hd_XZview,
                                   hd_YZview,
                                   NULL};

static void init_viewmode_dropdown(struct kemoviewer_gl_type *kemo_gl,
                                   GtkWidget *viewmode_button){
    int iflag_mode = kemoview_get_view_type_flag(kemo_gl->kemoview_data);
    if(iflag_mode == VIEW_YZ){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 5);
    }else if(iflag_mode == VIEW_XZ){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 4);
    }else if(iflag_mode == VIEW_XY){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 3);
    }else if(iflag_mode == VIEW_MAP){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 2);
    }else if(iflag_mode == VIEW_STEREO){
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 1);
    } else {
        gtk_drop_down_set_selected(GTK_DROP_DOWN(viewmode_button), 0);
    };
    return;
}

static int find_selected_viewmode_id(const char *text){
    int index_mode = -100;
    if(kemoview_compare_string((int) strlen(text), text, hd_Stereo) > 0){index_mode = VIEW_STEREO;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_Mapview) > 0){index_mode = VIEW_MAP;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_XYview) > 0){index_mode = VIEW_XY;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_XZview) > 0){index_mode = VIEW_XZ;}
    else if(kemoview_compare_string((int) strlen(text), text, hd_YZview) > 0){index_mode = VIEW_YZ;}
    else {index_mode = VIEW_3D;};
    return index_mode;
}

static void set_GLFW_viewtype(int index_mode, 
                              struct kemoviewer_gl_type *kemo_gl,
                              struct view_widgets *view_menu){
    set_GLFW_viewtype_mode(index_mode);
    kemoview_set_viewtype(index_mode, kemo_gl->kemoview_data);
    draw_full_gl(kemo_gl);
    
    if(kemoview_get_view_type_flag(kemo_gl->kemoview_data) == VIEW_STEREO){
        gtk_widget_set_sensitive(view_menu->Frame_stereo, TRUE);
    }else{
        gtk_widget_set_sensitive(view_menu->Frame_stereo, FALSE);
    };
    return;
}

static void setup_header_CB(GtkSignalListItemFactory *factory,
                            GObject *list_item,
                            gpointer data){
    GtkListHeader *self = GTK_LIST_HEADER (list_item);
    GtkWidget *child = gtk_label_new ("");
    gtk_label_set_xalign(GTK_LABEL (child), 0);
    gtk_label_set_use_markup(GTK_LABEL (child), TRUE);
    gtk_widget_set_margin_top (child, 10);
    gtk_widget_set_margin_bottom(child, 10);
    
    gtk_list_header_set_child(self, child);
}

static void bind_header_CB(GtkSignalListItemFactory *factory,
                           GObject *list_item,
                           gpointer data)
{
    GtkListHeader *self = GTK_LIST_HEADER (list_item);
    GtkWidget *child = gtk_list_header_get_child (self);
    GObject *item = gtk_list_header_get_item (self);
    printf("bind_header_CB\n");
}

static void selected_viewmode_CB(GtkDropDown *dropdown,
                                         GParamSpec *pspec,
                                         gpointer user_data)
{
    struct view_widgets *view_menu
            = (struct view_widgets *) g_object_get_data(G_OBJECT(user_data), "view_menu");
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
/* The dropdown model returns a GtkStringObject when created from strings */
    GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(dropdown));
    
    if(selected_item == NULL) return;
    const char *text = gtk_string_object_get_string (selected_item);
    int index_mode = find_selected_viewmode_id(text);
    set_GLFW_viewtype(index_mode, kemo_gl, view_menu);
}

GtkWidget * make_gtk4_viewmode_menu_box(struct kemoviewer_gl_type *kemo_gl,
                                        struct view_widgets *view_menu){
    GtkWidget *hbox_viewtype;
    /* A dropdown using an expression to obtain strings */
    GtkStringList *viewmode_model = gtk_string_list_new(view_modes);
    GListStore *viewmode_store = g_list_store_new(G_TYPE_LIST_MODEL);
    g_list_store_append(viewmode_store, viewmode_model);
    g_object_unref(viewmode_model);
    GtkFlattenListModel *viewmode_flat = gtk_flatten_list_model_new(G_LIST_MODEL(viewmode_store));
    GtkExpression *viewmode_expression = gtk_property_expression_new (GTK_TYPE_STRING_OBJECT,
                                                             NULL,
                                                             "string");
    
    GtkWidget *entry_buf = gtk_entry_buffer_new("", -1);
    g_object_set_data(G_OBJECT(entry_buf), "view_menu",  (gpointer) view_menu);
    g_object_set_data(G_OBJECT(entry_buf), "kemoview_gl",  (gpointer) kemo_gl);
    
    GtkWidget *viewmode_button = gtk_drop_down_new(G_LIST_MODEL(viewmode_flat), viewmode_expression);
    g_signal_connect(viewmode_button, "notify::selected-item", 
                     G_CALLBACK (selected_viewmode_CB), G_OBJECT(entry_buf));
/*    gtk_drop_down_set_enable_search (GTK_DROP_DOWN (viewmode_button), TRUE); */
    GtkListItemFactory *viewmode_factory = gtk_signal_list_item_factory_new ();
//    g_signal_connect (viewmode_factory, "setup", G_CALLBACK (setup_header_CB), NULL);
//    g_signal_connect (viewmode_factory, "bind", G_CALLBACK (bind_header_CB), NULL);
    gtk_drop_down_set_header_factory (GTK_DROP_DOWN (viewmode_button), viewmode_factory);
//    g_object_unref(viewmode_expression);     Do not release GtkExpression!!
    g_object_unref(viewmode_factory);
    g_object_unref(viewmode_flat);
    
    init_viewmode_dropdown(kemo_gl, viewmode_button);
    
    hbox_viewtype = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(hbox_viewtype), gtk_label_new("View type: "));
    gtk_box_append(GTK_BOX(hbox_viewtype), viewmode_button);
    
    return hbox_viewtype;
}

