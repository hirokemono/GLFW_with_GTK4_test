/*
 *  kemoview_gtk4_menu_button.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 *
 */

#include "kemoview_gtk4_menu_button.h"

static void prefWindowclose_CB (GtkWidget *new_win, gpointer user_data)
{
    GtkWidget *menu_item = GTK_WIDGET(user_data);
    gtk_widget_set_sensitive(menu_item, TRUE);
};

static void pref_menu_CB(GtkWidget *menu_item, gpointer user_data)
{
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "parent_win"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    struct lightparams_view *lightparams_vws
            = (struct lightparams_view *) g_object_get_data(G_OBJECT(user_data), "lights");

    gint size_xy[2];
    size_xy[0] = gtk_widget_get_width (main_window);
    size_xy[1] = gtk_widget_get_height(main_window);
    
    GtkWidget *pref_win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(pref_win), "Preferences");
    gtk_widget_set_size_request(pref_win, 150, -1);
    g_signal_connect(G_OBJECT(pref_win), "destroy",
                     G_CALLBACK(prefWindowclose_CB), G_OBJECT(menu_item));
//    g_signal_connect(G_OBJECT(pref_win), "focus-in-event", G_CALLBACK(gtkFocus_in_CB), NULL);
//    g_signal_connect(G_OBJECT(pref_win), "focus-out-event", G_CALLBACK(gtkFocus_out_CB), NULL);
    
//    GtkWidget *frame_pref = init_preference_scrollbox(kemo_gl, lightparams_vws, pref_win);
//    gtk_window_set_child(GTK_WINDOW(pref_win), frame_pref);
    gtk_widget_set_visible(pref_win, TRUE);
    gtk_widget_set_sensitive(menu_item, FALSE);
}

static void about_callback (GSimpleAction *simple,
                            GVariant      *parameter,
                            gpointer       user_data){
  g_print ("You clicked \"About\"\n");
}

static void new_callback (GSimpleAction *simple,
                          GVariant      *parameter,
                          gpointer       user_data){
  g_print ("You clicked \"New\"\n");
}

static void quit_callback (GSimpleAction *simple,
                           GVariant      *parameter,
                           gpointer       user_data){
    GApplication *application = user_data;
    g_application_quit (application);
}

static void startup (GApplication *app,
                     gpointer      user_data){
  GSimpleAction *new_action = g_simple_action_new ("new", NULL);
  g_signal_connect (new_action, "activate", G_CALLBACK (new_callback), app);
  g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (new_action));

  GSimpleAction *quit_action = g_simple_action_new ("quit", NULL);
  g_signal_connect (quit_action, "activate", G_CALLBACK (quit_callback), app);
  g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (quit_action));
}

GtkWidget *make_gtk_menu_button(struct kemoviewer_gl_type *kemo_gl,
                                GtkWidget *main_window,
//                                struct lightparams_view *lightparams_vws,
                                struct evolution_gtk_menu *evo_gmenu){
    GMenu *menu_model = g_menu_new();
    g_menu_append(menu_model, "About", "win.about");
//    g_menu_append(menu_model, "Preferences...", "win.pref");
//    g_menu_append(menu_model, "Open",           "app.open");
//    g_menu_append(menu_model, "Save Image",     "app.save");
//    g_menu_append(menu_model, "Quit",           "app.quit");
    
    GtkWidget *menu_button = gtk_menu_button_new();
    gtk_widget_set_size_request(menu_button, 60, 35);
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), G_MENU_MODEL(menu_model));
    gtk_menu_button_set_direction(GTK_MENU_BUTTON(menu_button), GTK_ARROW_DOWN);
    
    GSimpleAction *about_action = g_simple_action_new("about", NULL);
    g_signal_connect(about_action, "activate", 
                     G_CALLBACK (about_callback), G_OBJECT(main_window));
    printf("main_window %p %p\n", main_window, about_action);
    g_action_map_add_action(G_ACTION_MAP(main_window), G_ACTION(about_action));
    
//    GSimpleAction *pref_action = g_simple_action_new("pref", NULL);
//    g_signal_connect(pref_action, "preference", G_CALLBACK(preference_CB),
//                     GTK_WINDOW (window));
//    g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (pref_action));

    //    gtk_menu_button_set_active(menu_button, TRUE);
    
//    g_object_set_data(G_OBJECT(menu_button), "kemoview_gl", (gpointer) kemo_gl);
//    g_object_set_data(G_OBJECT(menu_button), "parent_win",  (gpointer) main_window);
//    g_object_set_data(G_OBJECT(menu_button), "lights", (gpointer) lightparams_vws);
//    g_object_set_data(G_OBJECT(menu_button), "tevo_menu", (gpointer) evo_gmenu);
//    
    
    GtkWidget *menuGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID (menuGrid), menu_button, 1, 1, 1, 1);
    
    return menuGrid;
}
