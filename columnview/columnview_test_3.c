// Source - https://stackoverflow.com/a/78185115
// Posted by Holger
// Retrieved 2026-10-14, License - CC BY-SA 4.0


// Use for testing columnview

#include <gtk/gtk.h>

#include "kemoviewer.h"
#include "m_kemoviewer_data.h"
#include "kemoview_gtk4_light_menu.h"


struct kemoviewer_type *kemoviewer_data;
struct lightparams_view *light_vws;

// Setup custom type - each item has two strings which will be displayed in two columns

#define TESTCOL_TYPE_ITEM (testcol_item_get_type())
G_DECLARE_FINAL_TYPE (TestColItem, testcol_item, TESTCOL, ITEM, GObject)

struct _TestColItem
{
    GObject parent_instance;
    char  *str1;
    char  *str2;
    float real3;
};

struct _TestColItemClass
{
    GObjectClass parent_class;
};

G_DEFINE_TYPE (TestColItem, testcol_item, G_TYPE_OBJECT)

enum {
    PROP_STR1 =  1,
    PROP_STR2 =  2,
    PROP_REAL3 = 3,
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
    case PROP_REAL3:
        g_value_set_float(value, (gfloat) self->real3);
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
    case PROP_REAL3:
        self->real3 = (float) g_value_get_float(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void testcol_item_init(TestColItem *item)
{
    item->str1 = NULL;
    item->str2 = NULL;
    item->real3 = 0.;
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

    obj_properties[PROP_REAL3] = g_param_spec_float("real3",
                                                    "Real3",
                                                    "third real",
                                                    -1.0e+30,
                                                     1.0e+30,
                                                     0.0,
                                                    G_PARAM_READWRITE);

    g_object_class_install_properties(gobject_class, N_PROPERTIES, obj_properties);
}


// Set label for factory widget
static void setup_cb(GtkSignalListItemFactory *factory,GObject  *listitem)
{
    GtkWidget *label = gtk_label_new(NULL);
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

// Set value from the column2 passed item for label
static void bind_real3_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    
    char string[8];
    snprintf(string, sizeof(string), "%f", item->real3);
    g_print("value is %f\n", item->real3); // Show on std out
    gtk_label_set_text(GTK_LABEL (label), string);
}

// Create the custom object
static TestColItem * testcol_item_new(const char *str1,
                                      const char *str2,
                                      const float real3){
    TestColItem  *item = g_object_new(TESTCOL_TYPE_ITEM, NULL);
    item->str1 =  g_strdup(str1);
    item->str2 =  g_strdup(str2);
    item->real3 = real3;
    return item;
}   

// Activate
static void activate (GtkApplication* app, gpointer user_data)
{
    kemoviewer_data = kemoview_allocate_single_viwewer_struct();
	kemoview_init_phong_light_list(kemoviewer_data);
    light_vws = init_light_views_4_viewer(kemoviewer_data->kemo_buffers->kemo_lights);
    
    
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT); 
    g_list_store_append(store,testcol_item_new("abc1","ghi1",  8.60));
    g_list_store_append(store,testcol_item_new("def2","aaa2", -10.0));
    g_list_store_append(store,testcol_item_new("aaa3","jkl3", 365.0));

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
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW (cv)));
    GtkSortListModel *model = gtk_sort_list_model_new (G_LIST_MODEL (store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW (cv), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory1, "bind", G_CALLBACK(bind_col1_cb),NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Col1", factory1);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM,NULL,"str1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory2, "bind", G_CALLBACK(bind_col2_cb),NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Col2", factory2);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM, NULL, "str2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory3, "bind", G_CALLBACK(bind_real3_cb),NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Real3", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(TESTCOL_TYPE_ITEM, NULL, "real3")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column3);

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
