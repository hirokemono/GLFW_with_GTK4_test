#include <gtk/gtk.h>

/* =========================================================================
 * 1. Define a Custom GObject to hold our float data
 * ========================================================================= */
#define ITEM_TYPE_MODEL (item_model_get_type())
G_DECLARE_FINAL_TYPE(ItemModel, item_model, ITEM, MODEL, GObject)

struct _ItemModel {
    GObject parent_instance;
    double float_value;
};

G_DEFINE_TYPE(ItemModel, item_model, G_TYPE_OBJECT)

static void item_model_init(ItemModel *self) {
    self->float_value = 0.0;
}

static void item_model_class_init(ItemModelClass *klass) {}

ItemModel* item_model_new(double initial_value) {
    ItemModel *self = g_object_new(ITEM_TYPE_MODEL, NULL);
    self->float_value = initial_value;
    return self;
}

/* =========================================================================
 * 2. GtkColumnView Factory Callbacks
 * ========================================================================= */

// Triggered when a spin button is changed by the user
static void on_spin_value_changed(GtkSpinButton *spin, gpointer user_data) {
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    ItemModel *item = ITEM_MODEL(gtk_list_item_get_item(listitem));
    
    if (item) {
        item->float_value = gtk_spin_button_get_value(spin);
        g_print("Model updated! New float value: %.2f\n", item->float_value);
    }
}

// Setup: Allocate and configure the float-capable GtkSpinButton
static void setup_spin_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem, gpointer user_data) {
    // parameters: initial_value, lower, upper, step_increment, page_increment, page_size
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, 0.0, 100.0, 0.25, 1.0, 0.0);
    
    // parameters: adjustment, climb_rate, decimal_digits
    GtkWidget *spin = gtk_spin_button_new(adjustment, 0.25, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_spin_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem, gpointer user_data) {
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    ItemModel *item = ITEM_MODEL(gtk_list_item_get_item(listitem));

    // Block signals temporarily to prevent loop during programmatic change
    g_signal_handlers_block_by_func(spin, on_spin_value_changed, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->float_value);
    g_signal_handlers_unblock_by_func(spin, on_spin_value_changed, listitem);

    // Track user adjustments
    g_signal_connect(spin, "value-changed", G_CALLBACK(on_spin_value_changed), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_spin_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem, gpointer user_data) {
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, on_spin_value_changed, listitem);
}

/* =========================================================================
 * 3. App Activation & Layout
 * ========================================================================= */
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK4 Float SpinButton in ColumnView");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Create a data store and add items with floats
    GListStore *store = g_list_store_new(ITEM_TYPE_MODEL);
    g_list_store_append(store, item_model_new(10.50));
    g_list_store_append(store, item_model_new(24.75));
    g_list_store_append(store, item_model_new(88.13));

    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL(store));
    GtkWidget *column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));

    // Construct the factory for our column
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_spin_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_spin_cb), NULL);
    g_signal_connect(factory, "unbind", G_CALLBACK(unbind_spin_cb), NULL);

    // Create and attach column to view
    GtkColumnViewColumn *column = gtk_column_view_column_new("Float Values", factory);
    gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), column);

    // Layout arrangement inside a scrolled view
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), column_view);
    gtk_window_set_child(GTK_WINDOW(window), scrolled_window);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.example.spincolumn", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
