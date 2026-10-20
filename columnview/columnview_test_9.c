#include <gtk/gtk.h>

// 1. Define a custom GObject to hold our row data
#define TYPE_ROW_DATA (row_data_get_type())
G_DECLARE_FINAL_TYPE(RowData, row_data, ROW, DATA, GObject)

struct _RowData {
    GObject parent_instance;
    char *text;
};

G_DEFINE_TYPE(RowData, row_data, G_TYPE_OBJECT)

static void row_data_finalize(GObject *object) {
    RowData *self = ROW_DATA(object);
    g_free(self->text);
    G_OBJECT_CLASS(row_data_parent_class)->finalize(object);
}

static void row_data_init(RowData *self) { self->text = NULL; }
static void row_data_class_init(RowDataClass *klass) {
    G_OBJECT_CLASS(klass)->finalize = row_data_finalize;
}

RowData* row_data_new(const char *text) {
    RowData *self = g_object_new(TYPE_ROW_DATA, NULL);
    self->text = g_strdup(text);
    return self;
}

// 2. Callback when the user finishes editing the text
static void on_cell_edited(GtkEditable *editable, gpointer user_data) {
    GtkListItem *list_item = GTK_LIST_ITEM(user_data);
    RowData *data = ROW_DATA(gtk_list_item_get_item(list_item));
    
    const char *new_text = gtk_editable_get_text(editable);
    
    // Update your backend/model data here
    g_free(data->text);
    data->text = g_strdup(new_text);
    g_print("Data updated to: %s\n", data->text);
}

static void on_cell_editing(GObject *object, GParamSpec *pspec, gpointer user_data) {
    GtkListItem *list_item = GTK_LIST_ITEM(user_data);
    RowData *data = ROW_DATA(gtk_list_item_get_item(list_item));
    GtkEditable *editable = GTK_EDITABLE(object);
    gboolean is_editing;
    g_object_get(G_OBJECT(editable), "editing", &is_editing, NULL);
    
    if (!is_editing) {
        // The user has finished editing and saved/committed the change
        const char *new_text = gtk_editable_get_text(editable);
        g_print("Finished editing! New text: %s\n", new_text);
        
        g_free(data->text);
        data->text = g_strdup(new_text);
        g_print("Data updated to: %s\n", data->text);
    }
}

// 3. Factory Setup: Instantiates the editable widget
static void factory_setup_cb(GtkSignalListItemFactory *factory, 
                             GtkListItem *list_item, 
                             gpointer user_data) {
    // GtkEditableLabel looks like a normal label until clicked/double-clicked to edit
    GtkWidget *editable_label = gtk_editable_label_new("");
    gtk_list_item_set_child(list_item, editable_label);
//    printf("setup list_item %p \n", list_item);

}

// 4. Factory Bind: Populates the widget with initial data from the model
static void factory_bind_cb(GtkSignalListItemFactory *factory,
                            GtkListItem *list_item,
                            gpointer user_data) {
    RowData *data = ROW_DATA(gtk_list_item_get_item(list_item));
    GtkWidget *editable_label = gtk_list_item_get_child(list_item);
//    printf("binded list_item %p \n", list_item);
//    printf("binded data %p \n", data);
//    printf("binded editable_label %p \n", editable_label);
    
    // Block signals momentarily so setting the initial text doesn't trigger the edited callback
    g_signal_handlers_block_by_func(editable_label,
                                    G_CALLBACK(on_cell_edited),
                                    G_OBJECT(list_item));
    g_signal_handlers_block_by_func(editable_label,
                                    G_CALLBACK(on_cell_editing),
                                    G_OBJECT(list_item));
    gtk_editable_set_text(GTK_EDITABLE(editable_label), data->text);
    g_signal_handlers_unblock_by_func(editable_label,
                                      G_CALLBACK(on_cell_edited),
                                      G_OBJECT(list_item));
    g_signal_handlers_unblock_by_func(editable_label,
                                      G_CALLBACK(on_cell_editing),
                                      G_OBJECT(list_item));
    
    // Connect the notify::text signal or "changed"/"activate" signals depending on requirements
    printf("g_signal_connect %p \n", list_item);
    g_signal_connect(editable_label, "changed", 
                     G_CALLBACK(on_cell_edited), G_OBJECT(list_item));
    g_signal_connect(editable_label, "notify::editing", 
                     G_CALLBACK(on_cell_editing), G_OBJECT(list_item));
}

// 5. Clean up when widget is unbound
static void factory_unbind_cb(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    // Optional cleanup logic
}


static void show_button_CB(GtkButton *button, gpointer user_data){
    GListStore *store = G_LIST_STORE(user_data);
    guint num = g_list_model_get_n_items(G_LIST_MODEL(store));
    printf("Number of items: %d \n", num);
    int i;
    GObject *item;
    RowData *data;
    for(i=0;i<num;i++){
        item = g_list_model_get_object(G_LIST_MODEL(store), i);
        data = ROW_DATA(item);
        printf("Data type at %d: %s %s\n", i, G_OBJECT_TYPE_NAME(item), data->text);
    }
    return;
};


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Editable Column View Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    // Create a ListStore model and add dummy items
    GListStore *store = g_list_store_new(TYPE_ROW_DATA);
    g_list_store_append(store, row_data_new("Item 1 (Click me to edit)"));
    g_list_store_append(store, row_data_new("Item 2"));
    g_list_store_append(store, row_data_new("Item 3"));

    // Wrap in a selection model (required by ColumnView)
    GtkNoSelection *selection_model = gtk_no_selection_new(G_LIST_MODEL(store));
    GtkWidget *column_view = gtk_column_view_new(NULL);
    gtk_column_view_set_model(GTK_COLUMN_VIEW(column_view), GTK_SELECTION_MODEL(selection_model));

    // Setup the ListItem Factory for the editable column
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(factory_setup_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(factory_bind_cb), NULL);
    g_signal_connect(factory, "unbind", G_CALLBACK(factory_unbind_cb), NULL);

    // Append the column to the ColumnView
    GtkColumnViewColumn *column = gtk_column_view_column_new("Editable Items", factory);
    gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), column);

    // Put ColumnView inside a scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window), 240);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), column_view);
    
    
    
    GtkWidget *show_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_button), "clicked",
                     G_CALLBACK(show_button_CB), G_OBJECT(store));
    
    
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(vbox), show_button);
    gtk_box_append(GTK_BOX(vbox), scrolled_window);
    
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.example.editablecolumn", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
