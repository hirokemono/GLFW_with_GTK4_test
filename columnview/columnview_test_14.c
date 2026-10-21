// Source - https://stackoverflow.com/a/78185115
// Posted by Holger
// Retrieved 2026-10-14, License - CC BY-SA 4.0


// Use for testing columnview

#include "columnview_test.h"

/*
#include "kemoviewer.h"
#include "m_kemoviewer_data.h"
#include "kemoview_gtk4_light_menu.h"

struct kemoviewer_type *kemoviewer_data;
struct lightparams_view *light_vws;
*/


// Activate
static void activate(GtkApplication* app, gpointer user_data)
{
    /*
    kemoviewer_data = kemoview_allocate_single_viwewer_struct();
	kemoview_init_phong_light_list(kemoviewer_data);
    light_vws = init_light_views_4_viewer(kemoviewer_data->kemo_buffers->kemo_lights);
    */
    
    // Set Windows in child scrolled window
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Test");
    gtk_window_set_default_size (GTK_WINDOW (window), 420, 300);
    
    GtkWidget *grid = init_list_store_test_grid();
    gtk_window_set_child (GTK_WINDOW(window), grid);
    
    // Show window
    gtk_window_present (GTK_WINDOW (window));
}

// Init application
int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
