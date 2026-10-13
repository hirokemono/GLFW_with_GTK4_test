#include <gtk/gtk.h>

/* The asynchronous callback function called when the user finishes picking a color

static void on_color_chosen(GObject *source_object, GAsyncResult *res, gpointer user_data){
    GtkColorDialog *dialog = GTK_COLOR_DIALOG(source_object);
    GError *error = NULL;
    GdkRGBA color;

    // Finish the color selection process
    if (gtk_color_dialog_choose_rgba_finish(dialog, res, &error)) {
        // Success: Use the selected color
        g_print("Color selected: RGBA(%g, %g, %g, %g)\n", 
                color.red, color.green, color.blue, color.alpha);
    } else {
        // Error or Cancellation handling
        if (g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_DISMISSED)) {
            g_print("The user cancelled or closed the color dialog.\n");
        } else {
            g_warning("An unexpected error occurred: %s", error->message);
        }
        g_error_free(error);
    }
}
*/

/* Callback function triggered when the user picks a color */
static void
on_color_changed (GObject    *object,
                  GParamSpec *pspec,
                  gpointer    user_data)
{
    GtkWidget *window = GTK_WIDGET(user_data);
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON (object);
    
    GtkColorDialog *retrieved_dialog
            = gtk_color_dialog_button_get_dialog(GTK_COLOR_DIALOG_BUTTON(color_button));
    
/*
    if (retrieved_dialog != NULL) {
        const char *title = gtk_color_dialog_get_title (retrieved_dialog);
        gboolean with_alpha = gtk_color_dialog_get_with_alpha (retrieved_dialog);
        
        g_print ("Retrieved Dialog Title: %s\n", title ? title : "None");
        g_print ("Supports Alpha Channel: %s\n", with_alpha ? "Yes" : "No");
        
        GdkRGBA initial_color = { 0.2, 0.4, 0.6, 1.0 };
        gtk_color_dialog_choose_rgba(retrieved_dialog, window,
                                     &initial_color, g_cancellable_new(), 
                                     on_color_chosen, NULL);
        
    }
*/
    
    /* Get the newly selected color from the button */
    const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);

    /* Print the RGBA values to the console */
    g_print ("Color selected: Red=%.2f, Green=%.2f, Blue=%.2f, Alpha=%.2f\n",
             rgba->red, rgba->green, rgba->blue, rgba->alpha);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    /* Create the main application window */
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "GtkColorDialogButton Example");
    gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);

    /* Use a layout box to center the button */
    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (box, GTK_ALIGN_CENTER);
    gtk_window_set_child (GTK_WINDOW (window), box);

    /* 1. Create a modern Color Dialog configuration object */
    GtkColorDialog *color_dialog = gtk_color_dialog_new ();
    gtk_color_dialog_set_title (color_dialog, "Select a Color");
    gtk_color_dialog_set_with_alpha (color_dialog, TRUE); // Allow transparency adjustments

    /* 2. Create the ColorDialogButton and associate it with the dialog configuration */
    GtkWidget *color_button = gtk_color_dialog_button_new (color_dialog);

    /* Set an initial default color (Optional - defaults to opaque black) */
    GdkRGBA initial_color = { 0.2, 0.4, 0.6, 1.0 }; // A nice blue shade
    gtk_color_dialog_button_set_rgba(GTK_COLOR_DIALOG_BUTTON (color_button), &initial_color);

    /* 3. Connect to the 'notify::rgba' signal to listen for value changes */
    g_signal_connect (color_button, "notify::rgba", G_CALLBACK (on_color_changed), window);
    
    /* Append the button to our box and present the window */
    gtk_box_append (GTK_BOX (box), color_button);
    gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char **argv)
{
    GtkApplication *app = gtk_application_new ("org.gtk.example.colordialog",
                                               G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    
    return status;
}
