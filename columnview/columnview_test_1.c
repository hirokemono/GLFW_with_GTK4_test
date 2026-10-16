// Source - https://stackoverflow.com/a/78185115
// Posted by Holger
// Retrieved 2026-10-14, License - CC BY-SA 4.0


// Use for testing columnview

#include <gtk/gtk.h>

// Setup custom type - each item has two strings which will be displayed in two columns

#define TESTCOL_TYPE_ITEM (testcol_item_get_type())
G_DECLARE_FINAL_TYPE (TestColItem, testcol_item, TESTCOL, ITEM, GObject)

struct _TestColItem
{
    GObject parent_instance;
     char *str1;
     char *str2;
};

struct _TestColItemClass
{
    GObjectClass parent_class;
};

G_DEFINE_TYPE (TestColItem, testcol_item, G_TYPE_OBJECT)

enum {
    PROP_STR1 = 1,
    PROP_STR2 = 2,
    N_PROPERTIES
};

static void
testcol_item_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    TestColItem *self = (TestColItem *)object;

    switch (prop_id) {
    case PROP_STR1:
        g_value_set_string(value, self->str1);
        break;
    case PROP_STR2:
       g_value_set_string(value, self->str2);
       break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void
testcol_item_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    TestColItem *self = (TestColItem *)object;

    switch (prop_id) {
    case PROP_STR1:
        g_free(self->str1);
        self->str1 = g_value_dup_string(value);
        break;
    case PROP_STR2:
        g_free(self->str2);
        self->str2 = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void testcol_item_init(TestColItem *item)
{
    item->str1 = NULL;
    item->str2 = NULL;
}


static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void testcol_item_class_init(TestColItemClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);

    gobject_class->get_property = testcol_item_get_property;
    gobject_class->set_property = testcol_item_set_property;

    obj_properties[PROP_STR1] = g_param_spec_string("str1",
                                                    "Str1",
                                                    "first column",
                                                    NULL,
                                                    G_PARAM_READWRITE);

    obj_properties[PROP_STR2] = g_param_spec_string("str2",
                                                    "Str2",
                                                    "second column",
                                                    NULL,
                                                    G_PARAM_READWRITE);

    g_object_class_install_properties(gobject_class, N_PROPERTIES, obj_properties);
}


// Set label for factory widget
static void setup_cb(GtkSignalListItemFactory *factory,GObject  *listitem)
{
    GtkWidget *label =gtk_label_new(NULL);
    gtk_list_item_set_child(GTK_LIST_ITEM(listitem),label);
}

// Set text from the column1 passed item for label
static void bind_col1_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = item->str1;
    g_print("value is %s\n",string); // Show on std out
    gtk_label_set_text(GTK_LABEL (label), string);
}
// Set text from the column2 passed item for label
static void bind_col2_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = item->str2;
    g_print("value is %s\n",string); // Show on std out
    gtk_label_set_text(GTK_LABEL (label), string);
}

// Create the custom object
static TestColItem * testcol_item_new(const char *str1, const char *str2)
{
    TestColItem  *item = g_object_new(TESTCOL_TYPE_ITEM, NULL);
    item->str1 = g_strdup(str1);
    item->str2 = g_strdup(str2);
    return item;
}   

// Activate
static void activate (GtkApplication* app, gpointer user_data)
{
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT); 
    g_list_store_append(store,testcol_item_new("abc1","ghi1"));
    g_list_store_append(store,testcol_item_new("def2","aaa2"));
    g_list_store_append(store,testcol_item_new("aaa3","jkl3"));

    // Set Windows in child scrolled window
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Test");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    GtkWidget *scrolled_window = gtk_scrolled_window_new (); 
    gtk_window_set_child (GTK_WINDOW(window), scrolled_window);

    // Add column view.  Put in scrolled window.  
    GtkWidget *cv = gtk_column_view_new(NULL); // Null for now want to add sorter
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window),cv);
    gtk_column_view_set_show_column_separators (GTK_COLUMN_VIEW (cv),TRUE);
     
    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref (gtk_column_view_get_sorter (GTK_COLUMN_VIEW (cv)));
    GtkSortListModel *model = gtk_sort_list_model_new (G_LIST_MODEL (store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new (G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model (GTK_COLUMN_VIEW (cv), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_col1_cb),NULL);
    GtkColumnViewColumn *column = gtk_column_view_column_new("Col1", factory);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM,NULL,"str1")));
    gtk_column_view_column_set_sorter (column, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_col2_cb),NULL);
    column = gtk_column_view_column_new("Col2", factory);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM, NULL, "str2")));
    gtk_column_view_column_set_sorter (column, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);

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
