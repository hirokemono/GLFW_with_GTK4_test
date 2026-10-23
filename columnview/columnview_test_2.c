// Source - https://stackoverflow.com/a/77759424
// Posted by Holger
// Retrieved 2026-10-14, License - CC BY-SA 4.0

 /*
  * The sole purpose of this program is to test some approaches to using 
  * columnview with GTK4 and C. It may contain errors and does not 
  * claim to be considered "best practice".
  */

#include<gtk/gtk.h>

#define CAPITAL_TYPE_ITEM (capital_item_get_type())
G_DECLARE_FINAL_TYPE (CapitalItem, capital_item, CAPITAL, ITEM, GObject)

struct _CapitalItem
{
    GObject parent_instance;
    const char *name;
    const char *country;
    const char *founded;
    const char *population;
    const char *area;
};

struct _CapitalItemClass
{
    GObjectClass parent_class;
};

G_DEFINE_TYPE (CapitalItem, capital_item, G_TYPE_OBJECT)

static void capital_item_init(CapitalItem *item)
{
    printf("capital_item_init \n");
}

static void capital_item_class_init(CapitalItemClass *class)
{
    printf("capital_item_class_init \n");
}

static CapitalItem * capital_item_new(const char *name,
                const char *country,
                const char *founded,
                const char *population,
                const char *area )
{
    CapitalItem  *item = g_object_new(CAPITAL_TYPE_ITEM, NULL);
    item->name = g_strdup(name);
    item->country = g_strdup(country);
    item->founded = g_strdup(founded);
    item->population = g_strdup(population);
    item->area = g_strdup(area);
    return item;
}

// a funktion that creates a GListModel with capital_item  objects
GListModel * create_capital_model(void)
{
    GListStore *store = g_list_store_new(G_TYPE_OBJECT);
    printf("g_list_store_append 1\n");
    g_list_store_append(store, capital_item_new("Washington,D.C.","USA","1790","712,816","177"));
    printf("g_list_store_append 2\n");
    g_list_store_append(store, capital_item_new("London","Britain","43","8,799,800","1,572.03"));
    printf("g_list_store_append 3\n");
    g_list_store_append(store, capital_item_new("Paris","France","3rd c. BC","2,161,000","105"));
    printf("g_list_store_append 4\n");
    g_list_store_append(store, capital_item_new("Berlin","Germany","13th century","3,850,809","891.3"));
    printf("g_list_store_append 5\n");
    g_list_store_append(store, capital_item_new("Rome","Italy","753 BC","2,860,009","1,285"));
    return G_LIST_MODEL(store);
}

static const char* capital_item_get_name(CapitalItem *item)
{
    return item->name;
}

static const char* capital_item_get_country(CapitalItem *item)
{
    return item->country;
}

static const char* capital_item_get_founded(CapitalItem *item)
{
    return item->founded;
}

static const char* capital_item_get_population(CapitalItem *item)
{
    return item->population;
}

static const char* capital_item_get_area(CapitalItem *item)
{
    return item->area;
}

static void setup_cb(GtkSignalListItemFactory *factory,GObject  *listitem)
{
    GtkWidget *label =gtk_label_new(NULL);
    gtk_list_item_set_child(GTK_LIST_ITEM(listitem),label);
}

static void bind_name_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = capital_item_get_name(CAPITAL_ITEM(item));
    gtk_label_set_text(GTK_LABEL (label), string);
}

static void bind_country_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = capital_item_get_country(CAPITAL_ITEM(item));
    gtk_label_set_text(GTK_LABEL (label), string);
}

static void bind_founded_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = capital_item_get_founded(CAPITAL_ITEM(item));
    gtk_label_set_text(GTK_LABEL (label), string);
}

static void bind_population_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = capital_item_get_population(CAPITAL_ITEM(item));
    gtk_label_set_text(GTK_LABEL (label), string);
}

static void bind_area_cb(GtkSignalListItemFactory *factory, GtkListItem *listitem)
{
    GtkWidget *label = gtk_list_item_get_child(listitem);
    GObject *item = gtk_list_item_get_item(GTK_LIST_ITEM(listitem));
    const char *string = capital_item_get_area(CAPITAL_ITEM(item));
    gtk_label_set_text(GTK_LABEL (label), string);
}

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkListItemFactory *factory;
    GListModel *model;
    GtkSingleSelection *selection;
    GtkColumnViewColumn *column;
    GtkWidget *cv;

    model = create_capital_model();
    selection = gtk_single_selection_new(G_LIST_MODEL(model));
    gtk_single_selection_set_autoselect(selection,TRUE);

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW(window), "ColumnView Example");
    gtk_window_set_default_size (GTK_WINDOW(window), 470, 200);
    scrolled_window = gtk_scrolled_window_new ();
    gtk_window_set_child (GTK_WINDOW(window), scrolled_window);

    cv = gtk_column_view_new(GTK_SELECTION_MODEL (selection));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window),cv);
    gtk_column_view_set_show_column_separators (GTK_COLUMN_VIEW (cv),TRUE);
    // one factory for each column
    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_name_cb),NULL);
    column = gtk_column_view_column_new("capital city", factory);
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);
    g_object_unref (column);

    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_country_cb),NULL);
    column = gtk_column_view_column_new("country", factory);
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);
    g_object_unref (column);

    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_founded_cb),NULL);
    column = gtk_column_view_column_new("founded", factory);
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);
    g_object_unref (column);

    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_population_cb),NULL);
    column = gtk_column_view_column_new("population", factory);
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);
    g_object_unref (column);

    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb),NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_area_cb),NULL);
    column = gtk_column_view_column_new("area (km²)", factory);
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column);
    g_object_unref (column);

    gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv)
{
    GtkApplication *app; int status;

    app = gtk_application_new("org.gtk.columnview",G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate",G_CALLBACK(activate),NULL);
    status = g_application_run (G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
