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
    int    int4;
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
    PROP_INT4 =  4,
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
    case PROP_INT4:
        g_value_set_int(value, (gint) self->int4);
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
    case PROP_INT4:
        self->int4 = (int) g_value_get_int(value);
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
    item->int4 =  0;
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

    obj_properties[PROP_INT4] = g_param_spec_int("int4",
                                                 "Int4",
                                                 "forth integer",
                                                 -32768,
                                                  32768,
                                                      0,
                                                 G_PARAM_READWRITE);
    
    g_object_class_install_properties(gobject_class, N_PROPERTIES, obj_properties);
}

/*  Callbacks  */
/* =========================================================================
 *   Factory Callbacks for editable label
 * ========================================================================= */

/*  string editable button callbacks */
static void str1_editing_CB(GObject *object, GParamSpec *pspec, gpointer user_data) {
    GtkEditable *editable = GTK_EDITABLE(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("str1_editing_CB %p %p \n", listitem, item);
    
    gboolean is_editing;
    g_object_get(G_OBJECT(editable), "editing", &is_editing, NULL);
    if(!is_editing) {
        const char *new_text = gtk_editable_get_text(GTK_EDITABLE(editable));
        g_free(item->str1);
        item->str1 = g_strdup(new_text);
        g_print("str1 data updated to: %s\n", item->str1);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_str1_label_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    GtkWidget *editable_label = gtk_editable_label_new("");
    
    gtk_list_item_set_child(listitem, editable_label);
}

// 4. Factory bind: Populate the widget with data from the row object
static void bind_str1_label_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *editable_label = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("bind_str1_label_CB %p %p \n", listitem, item);
    
    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(editable_label, str1_editing_CB, listitem);
    gtk_editable_set_text(GTK_EDITABLE(editable_label), item->str1);
    g_signal_handlers_unblock_by_func(editable_label, str1_editing_CB, listitem);
    
    /* Connect the notify::editing signal to catch when editing finishes */
    g_signal_connect(editable_label, "notify::editing", 
                     G_CALLBACK(str1_editing_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_str1_label_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *editable_label = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(editable_label, str1_editing_CB, listitem);
}

/* =========================================================================
 *   Factory Callbacks for GtkEntry
 * ========================================================================= */

/*  string editable button callbacks */
static void str2_activate_CB(GObject *object, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(object);
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("str2_activate_CB %p %p \n", listitem, item);
    
    const char *new_text = gtk_editable_get_text(GTK_EDITABLE(entry));
    g_free(item->str2);
    item->str2 = g_strdup(new_text);
    g_print("str2 data updated to: %s\n", item->str2);
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_str2_label_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    TestColItem *item = gtk_list_item_get_item(listitem);
    GtkWidget *entry = gtk_entry_new();
    
    gtk_list_item_set_child(listitem, entry);
}

// 4. Factory bind: Populate the widget with data from the row object
static void bind_str2_label_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *entry = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("bind_str2_label_CB %p %p \n", listitem, item);
    
    /* Block previous signal handler if recycled, or set text */
    gtk_editable_set_text(GTK_EDITABLE(entry), item->str2 ? item->str2 : "");
    
    /* Disconnect old handler if stored, or connect new change listener */
    g_signal_handlers_block_by_func(entry, str2_activate_CB, listitem);
    gtk_editable_set_text(GTK_EDITABLE(entry), item->str2);
    g_signal_handlers_unblock_by_func(entry, str2_activate_CB, listitem);
    
    /* Connect the activate signal to catch when editing finishes */
    g_signal_connect(entry, "activate", 
                     G_CALLBACK(str2_activate_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_str2_label_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *entry = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(entry, str2_activate_CB, listitem);
}


/* =========================================================================
 *   GtkColumnView Factory Callbacks for real spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void real_spin_value_change_CB(GtkSpinButton *spin, gpointer user_data) {
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("real_spin_value_change_CB %p %p \n", listitem, item);
    
    if(item){
        item->real3 = gtk_spin_button_get_value(spin);
        g_print("Real value updated! New float value: %.2f\n", item->real3);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_real3_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0.0, 0.0, 360.0, 0.25, 1.0, 0.0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 0.25, 2);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_real3_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("bind_real3_spin_CB %p %p \n", listitem, item);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, real_spin_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->real3);
    g_signal_handlers_unblock_by_func(spin, real_spin_value_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", G_CALLBACK(real_spin_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_real3_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, real_spin_value_change_CB, listitem);
}

/* =========================================================================
 *   GtkColumnView Factory Callbacks for integer spin button
 * ========================================================================= */

/*  Float spin button callbacks */
static void int_spin_value_change_CB(GtkSpinButton *spin, gpointer user_data) {
    GtkListItem *listitem = GTK_LIST_ITEM(user_data);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("int_spin_value_change_CB %p %p \n", listitem, item);
    
    if(item){
        item->int4 = gtk_spin_button_get_value_as_int(spin);
        g_print("Real value updated! New integer value: %d\n", item->int4);
    }
}

/* Setup: Allocate and configure the float-capable GtkSpinButton */
static void setup_int4_spin_CB(GtkSignalListItemFactory *factory,
                                GtkListItem *listitem,
                                gpointer user_data){
    /* parameters: initial_value, lower, upper, step_increment, page_increment, page_size */
    GtkAdjustment *adjustment = gtk_adjustment_new(0, -64, 64, 1, 1, 0);
    
    /* parameters: adjustment, climb_rate, decimal_digits */
    GtkWidget *spin = gtk_spin_button_new(adjustment, 1, 0);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin), TRUE);
    
    gtk_list_item_set_child(listitem, spin);
}

// Bind: Populate widget data from model and listen for changes
static void bind_int4_spin_CB(GtkSignalListItemFactory *factory,
                               GtkListItem *listitem,
                               gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    TestColItem *item = gtk_list_item_get_item(listitem);
    printf("bind_int4_spin_CB %p %p \n", listitem, item);

    /* Block signals temporarily to prevent loop during programmatic change */
    g_signal_handlers_block_by_func(spin, int_spin_value_change_CB, listitem);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), item->int4);
    g_signal_handlers_unblock_by_func(spin, int_spin_value_change_CB, listitem);

    /* Track user adjustments */
    g_signal_connect(spin, "value-changed", 
                     G_CALLBACK(int_spin_value_change_CB), listitem);
}

// Unbind: Clean up connection when cell is ready for recycling
static void unbind_int4_spin_CB(GtkSignalListItemFactory *factory,
                                 GtkListItem *listitem,
                                 gpointer user_data){
    GtkWidget *spin = gtk_list_item_get_child(listitem);
    g_signal_handlers_disconnect_by_func(spin, int_spin_value_change_CB, listitem);
}


static void show_button_CB(GtkButton *button, gpointer user_data){
    TestColItem *item;
    GListStore *store = G_LIST_STORE(user_data);
    guint num = g_list_model_get_n_items(G_LIST_MODEL(store));
    printf("Number of items: %d \n", num);
    int i;
    for(i=0;i<num;i++){
        item = TESTCOL_ITEM(g_list_model_get_object(G_LIST_MODEL(store), i));
        printf("Data at %d: %s %s %f %d\n", i, 
               item->str1, item->str2, item->real3, item->int4);
    }
    return;
};






// Create the custom object
static TestColItem * testcol_item_new(const char *str1,
                                      const char *str2,
                                      const float real3,
                                      const int int4){
    TestColItem  *item = g_object_new(TESTCOL_TYPE_ITEM, NULL);
    item->str1 =  g_strdup(str1);
    item->str2 =  g_strdup(str2);
    item->real3 = real3;
    item->int4 =  int4;
    return item;
}   

/* Construct columnview */
GListStore *init_test_list_store(){
    // Create and populate custom list model
    GListStore *store = g_list_store_new(G_TYPE_OBJECT); 
    g_list_store_append(store,testcol_item_new("abc1","ghi1",  8.60,  33));
    g_list_store_append(store,testcol_item_new("def2","aaa2", -10.0, -87));
    g_list_store_append(store,testcol_item_new("aaa3","jkl3", 240.0,  10));
    return store;
};

GtkWidget * columnview_init(GListStore *store){
    GtkWidget *cv = gtk_column_view_new(NULL);

    // Setup sort model then overlay with with columnview
    GtkSorter *sorter = g_object_ref(gtk_column_view_get_sorter (GTK_COLUMN_VIEW (cv)));
    GtkSortListModel *model = gtk_sort_list_model_new(G_LIST_MODEL(store), sorter);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL (model));
    gtk_single_selection_set_autoselect (selection, TRUE);
    gtk_column_view_set_model(GTK_COLUMN_VIEW (cv), GTK_SELECTION_MODEL (selection));

    // Factory setup and bind for column 1 
    GtkListItemFactory *factory1 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory1, "setup", G_CALLBACK(setup_str1_label_CB),    NULL);
    g_signal_connect(factory1, "bind", G_CALLBACK(bind_str1_label_CB),      NULL);
    g_signal_connect(factory1, "unbind", G_CALLBACK(unbind_str1_label_CB),  NULL);
    GtkColumnViewColumn *column1 = gtk_column_view_column_new("Col1", factory1);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM,NULL,"str1")));
    gtk_column_view_column_set_sorter (column1, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column1);

    // Factory setup and bind for column 2
    GtkListItemFactory *factory2 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory2, "setup", G_CALLBACK(setup_str2_label_CB),   NULL);
    g_signal_connect(factory2, "bind", G_CALLBACK(bind_str2_label_CB),     NULL);
    g_signal_connect(factory2, "unbind", G_CALLBACK(unbind_str2_label_CB), NULL);
    GtkColumnViewColumn *column2 = gtk_column_view_column_new("Col2", factory2);
    sorter = GTK_SORTER (gtk_string_sorter_new (gtk_property_expression_new (TESTCOL_TYPE_ITEM, NULL, "str2")));
    gtk_column_view_column_set_sorter (column2, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column2);

    // Factory setup and bind for column 3
    GtkListItemFactory *factory3 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory3, "setup",  G_CALLBACK(setup_real3_spin_CB),  NULL);
    g_signal_connect(factory3, "bind",   G_CALLBACK(bind_real3_spin_CB),   NULL);
    g_signal_connect(factory3, "unbind", G_CALLBACK(unbind_real3_spin_CB), NULL);
    GtkColumnViewColumn *column3 = gtk_column_view_column_new("Real3", factory3);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(TESTCOL_TYPE_ITEM, NULL, "real3")));
    gtk_column_view_column_set_sorter (column3, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column3);

    // Factory setup and bind for column 4
    GtkListItemFactory *factory4 = gtk_signal_list_item_factory_new();
    g_signal_connect(factory4, "setup",  G_CALLBACK(setup_int4_spin_CB),  NULL);
    g_signal_connect(factory4, "bind",   G_CALLBACK(bind_int4_spin_CB),   NULL);
    g_signal_connect(factory4, "unbind", G_CALLBACK(unbind_int4_spin_CB), NULL);
    GtkColumnViewColumn *column4 = gtk_column_view_column_new("Int4", factory4);
    sorter = GTK_SORTER (gtk_numeric_sorter_new(gtk_property_expression_new(TESTCOL_TYPE_ITEM, NULL, "int4")));
    gtk_column_view_column_set_sorter (column4, GTK_SORTER (sorter));
    gtk_column_view_append_column (GTK_COLUMN_VIEW (cv), column4);
    
    return cv;
}

// Activate
static void activate (GtkApplication* app, gpointer user_data)
{
    kemoviewer_data = kemoview_allocate_single_viwewer_struct();
	kemoview_init_phong_light_list(kemoviewer_data);
    light_vws = init_light_views_4_viewer(kemoviewer_data->kemo_buffers->kemo_lights);
    
    
    // Add column view.  Put in scrolled window.  
    GListStore *store = init_test_list_store(); 
    GtkWidget *cv = columnview_init(store);
    gtk_column_view_set_show_column_separators (GTK_COLUMN_VIEW (cv),TRUE);
     
    // Set Windows in child scrolled window
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Test");
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(); 
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window), 240);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), cv);
    
    
    
    GtkWidget *show_button = gtk_button_new_with_label("Show data");
    g_signal_connect(G_OBJECT(show_button), "clicked",
                     G_CALLBACK(show_button_CB), G_OBJECT(store));
    
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(vbox), show_button);
    gtk_box_append(GTK_BOX(vbox), scrolled_window);
    gtk_window_set_child (GTK_WINDOW(window), vbox);
    
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
