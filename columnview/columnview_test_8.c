#include <gtk/gtk.h>

// Define a simple custom object for row data
#define MY_TYPE_ITEM (my_item_get_type())
G_DECLARE_FINAL_TYPE(MyItem, my_item, MY, ITEM, GObject)

struct _MyItem {
    GObject parent_instance;
    char *text;
};

G_DEFINE_TYPE(MyItem, my_item, G_TYPE_OBJECT)

static void my_item_finalize(GObject *object) {
    MyItem *self = MY_ITEM(object);
    g_free(self->text);
    G_OBJECT_CLASS(my_item_parent_class)->finalize(object);
}

static void my_item_class_init(MyItemClass *klass) {
    G_OBJECT_CLASS(klass)->finalize = my_item_finalize;
}

static void my_item_init(MyItem *self) {}

static MyItem *my_item_new(const char *text) {
    MyItem *self = g_object_new(MY_TYPE_ITEM, NULL);
    self->text = g_strdup(text);
    return self;
}

// Callback when GtkEntry text changes
static void on_entry_changed(GObject *object, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(object);
    MyItem *item = MY_ITEM(user_data);
    const char *new_text = gtk_editable_get_text(GTK_EDITABLE(entry));
    g_free(item->text);
    item->text = g_strdup(new_text);
    printf("edited: %s\n", item->text);
}

// Factory setup: create the GtkEntry widget
static void setup_cb(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    GtkWidget *entry = gtk_entry_new();
    gtk_list_item_set_child(list_item, entry);
}

// Factory bind: link data model to GtkEntry
static void bind_cb(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    GtkWidget *entry = gtk_list_item_get_child(list_item);
    MyItem *item = MY_ITEM(gtk_list_item_get_item(list_item));

    // Block previous signal handler if recycled, or set text
    gtk_editable_set_text(GTK_EDITABLE(entry), item->text ? item->text : "");

    // Disconnect old handler if stored, or connect new change listener
    g_signal_handlers_block_by_func(entry, on_entry_changed, list_item);
    gtk_editable_set_text(GTK_EDITABLE(entry), item->text);
    g_signal_handlers_unblock_by_func(entry, on_entry_changed, list_item);
    
    /* Connect the activate signal to catch when editing finishes */
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_changed), list_item);
}

static void unbind_cb(GtkSignalListItemFactory *factory,
                                 GtkListItem *list_item,
                                 gpointer user_data){
    GtkWidget *entry = gtk_list_item_get_child(list_item);
    g_signal_handlers_disconnect_by_func(entry, on_entry_changed, list_item);
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ColumnView Editable Entry Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GListStore *store = g_list_store_new(MY_TYPE_ITEM);
    g_list_store_append(store, my_item_new("Editable row 1"));
    g_list_store_append(store, my_item_new("Editable row 2"));

    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL(store));
    GtkWidget *column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_cb), NULL);
    g_signal_connect(factory, "unbind", G_CALLBACK(unbind_cb), NULL);

    GtkColumnViewColumn *column = gtk_column_view_column_new("Editable Text", factory);
    gtk_column_view_append_column(column_view, column);

    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), column_view);
    gtk_window_set_child(GTK_WINDOW(window), scrolled);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.example.columnview_entry", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
