#include <gtk/gtk.h>

/* Callback function triggered when the user picks a color */
static void
on_color_changed (GObject    *object,
                  GParamSpec *pspec,
                  gpointer    user_data)
{
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON (object);
    
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
    gtk_color_dialog_button_set_rgba (GTK_COLOR_DIALOG_BUTTON (color_button), &initial_color);

    /* 3. Connect to the 'notify::rgba' signal to listen for value changes */
    g_signal_connect (color_button, "notify::rgba", G_CALLBACK (on_color_changed), NULL);

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
