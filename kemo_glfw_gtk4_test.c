
/* kemo_gkt4_test.c */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gio/gio.h>


// #include "calypso_GTK.h"
#include "view_modifier_glfw.h"

#define NPIX_X  960
#define NPIX_Y  800

struct kemoviewer_type *single_kemoview;
struct kemoviewer_gl_type *kemoview_gl;

GLFWwindow *glfw_win;
int iflag_glfw_focus = 0;
int iflag_glfw_end = 0;
int iflag_gtk_focus = 0;

int iflag_msg_fade;
float message_opacity;
double msg_timer_start;
double delta_t;

int iflag_fast_prev;
int iflag_fast_draw;
double fast_draw_start;


GtkWidget *gtk_win;

struct main_buttons *mbot;

static void mainloop_4_glfw(){
    int icou = 0;
    int jcou = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(glfw_win)){
		glfwWindowShouldClose(glfw_win);
		
		if(glfwGetWindowAttrib(glfw_win, GLFW_FOCUSED) != 0){
            glfwPollEvents();
            icou++;
        }
		
        if(iflag_msg_fade == 1){
	        delta_t = glfwGetTime() - msg_timer_start;
    	    if(delta_t < 4.5){
	    	    message_opacity = log10(10.0 - 2.0*delta_t);
                kemoview_set_message_opacity(message_opacity,
                                             single_kemoview);
                draw_full_gl(kemoview_gl);
            }else{
                iflag_msg_fade = 0;
            };
        };
        
        if(iflag_fast_draw == 1){
            if(iflag_fast_prev == 0){
                fast_draw_start = glfwGetTime();
                iflag_fast_prev = 1;
            };

            delta_t = glfwGetTime() - fast_draw_start;
            if(delta_t > 1.5){
                draw_full_gl(kemoview_gl);
                iflag_fast_prev = 0;
            	iflag_fast_draw = 0;
            };
        };

        /* Collect GTK events */
        if(iflag_glfw_end == 1) return;
//        set_viewmatrix_value(single_kemoview, mbot->view_menu, gtk_win);

		if(glfwGetWindowAttrib(glfw_win, GLFW_FOCUSED) == 0){
            while (g_main_context_pending(NULL)) g_main_context_iteration(NULL, TRUE);
            jcou++;
        };
//        if(icou%1000==0 || jcou%1000==0) printf("GLFW count: %d, GTK count: %d\n", icou, jcou);
	};
	return;
}

/* Callback functions for GTK */

static void gtkWindowclose_CB(GtkButton *button, gpointer user_data){
    printf("Destroy\n");
	gtk_window_destroy(GTK_WINDOW(gtk_win));
	glfwSetWindowShouldClose(glfw_win, GLFW_TRUE);
}

/* Callbacks for GLFW */ 

void glfwWindowFocus_CB(GLFWwindow *window, int focused) {
	if(focused){
/*		printf("GLFW window focused\n"); */
//		iflag_glfw_focus = 1;
	} else {
/*		printf("GLFW window lost focuse\n"); */
//		iflag_glfw_focus = 0;
	}
}

void glfwWindowclose_CB(GLFWwindow *window) {
	gtk_window_destroy(GTK_WINDOW(gtk_win));
	glfwSetWindowShouldClose(window, GLFW_TRUE);
	iflag_glfw_end = 1;
	return;
}

void dropFileToGlfw_CB(GLFWwindow *window, int num, const char **paths) {
	struct kv_string *filename;
	for (int i = 0; i < num; i++) {
		filename = kemoview_init_kvstring_by_string(paths[i]);
//        open_kemoviewer_file_glfw(kemoview_gl, filename, mbot, gtk_win);
        printf("Drugged file: %s\n", filename->string);
	}
}

void windowSizeCB(GLFWwindow *window, int width, int height) {
    int nx_buf, ny_buf;
	glfwGetFramebufferSize(glfw_win, &nx_buf, &ny_buf);
	
    message_opacity = 1.0;
	kemoview_update_projection_by_viewer_size(nx_buf, ny_buf,
                                              width, height,
                                              single_kemoview);
	kemoview_set_message_opacity(message_opacity, single_kemoview);
	glViewport(IZERO, IZERO, (GLint) nx_buf, (GLint) ny_buf);
    iflag_msg_fade = 1;
    msg_timer_start = glfwGetTime();
	
//    update_windowsize_menu(kemoview_gl, mbot->view_menu, gtk_win);
    printf("retinemode %d\n", kemoview_get_retinamode(single_kemoview));
}

void frameBufferSizeCB(GLFWwindow *window, int nx_buf, int ny_buf){
	int npix_x, npix_y;
	glfwGetWindowSize(window, &npix_x, &npix_y);
/*	printf("frameBufferSizeCB %d %d\n", nx_buf, ny_buf); */
	
    message_opacity = 1.0;
	kemoview_update_projection_by_viewer_size(nx_buf, ny_buf,
                                              npix_x, npix_y,
                                              single_kemoview);
    kemoview_set_message_opacity(message_opacity, single_kemoview);
	glViewport(IZERO, IZERO, (GLint) nx_buf, (GLint) ny_buf);
    iflag_msg_fade = 1;
    msg_timer_start = glfwGetTime();
	
//	update_windowsize_menu(kemoview_gl, mbot->view_menu, gtk_win);
}


static void file_opened (GObject      *source,
                         GAsyncResult *result,
                         void         *data){
    GFile *file;
    GError *error = NULL;
    
    file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG (source), result, &error);
    
    if (!file)
    {
        g_print ("%s\n", error->message);
        g_error_free (error);
        return;
    }
    
    printf("Open file: %s \n", g_file_get_path(file));
    
    g_object_unref(file);
}

static void kemoview_gtk_read_file_select(GtkButton *button, gpointer entry_data){
	int response;
	GtkEntry *entry = GTK_ENTRY(entry_data);
    GtkWidget *parent = GTK_WIDGET(g_object_get_data(G_OBJECT(entry_data), "parent"));
	GtkFileChooser *chooser;
	
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

	/* generate file selection widget*/
	GtkFileDialog *dialog = gtk_file_dialog_new();
    GtkFileFilter *filter = gtk_file_filter_new();
    g_object_unref (filter);
    
    gtk_file_dialog_open (dialog, parent, NULL, file_opened, entry);
	return;
}

static void open_file_CB(GtkButton *button, gpointer user_data){
    struct kv_string *filename;
    GtkWidget *main_window = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "window"));
    struct kemoviewer_gl_type *kemo_gl
            = (struct kemoviewer_gl_type *) g_object_get_data(G_OBJECT(user_data), "kemoview_gl");
    
    kemoview_gtk_read_file_select(button, user_data);
//    if(iflag_set == IZERO) return;
//    GtkEntry *entry = GTK_ENTRY(user_data);
//    struct main_buttons *mbot = (struct main_buttons *) g_object_get_data(G_OBJECT(user_data), "buttons");
//    filename = kemoview_init_kvstring_by_string(gtk_entry_get_text(entry));
//    
//    open_kemoviewer_file_glfw(kemo_gl, filename, mbot, main_window);
    return;
};

static GtkWidget * make_gtk_open_file_box(struct kemoviewer_gl_type *kemo_gl,
                                          GtkWidget *main_window,
                                          struct main_buttons *mbot){
    GtkWidget *hbox_open;
    
    GtkWidget *entry_file = gtk_entry_new();
    g_object_set_data(G_OBJECT(entry_file), "buttons", (gpointer)  mbot);
    g_object_set_data(G_OBJECT(entry_file), "kemoview_gl", (gpointer) kemo_gl);
    g_object_set_data(G_OBJECT(entry_file), "window", (gpointer) main_window);

//    GtkWidget *menuGrid = make_gtk_menu_button(kemo_gl, main_window,
//                                               mbot->lightparams_vws,
//                                               mbot->evo_gmenu);
    
    GtkWidget *open_Button = gtk_button_new_with_label("Open...");
    g_signal_connect(G_OBJECT(open_Button), "clicked",
                     G_CALLBACK(open_file_CB), (gpointer)entry_file);
    
    hbox_open = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
//    gtk_box_append(GTK_BOX(hbox_open), menuGrid, TRUE, TRUE, 0);
    gtk_box_append(GTK_BOX(hbox_open), gtk_label_new("File: "));
    gtk_box_append(GTK_BOX(hbox_open), entry_file);
    gtk_box_append(GTK_BOX(hbox_open), open_Button);
    return hbox_open;
}


static GtkWidget * make_gtk_save_file_box(GtkWidget *quitButton,
                                          struct kemoviewer_gl_type *kemo_gl){
    GtkWidget *savebox;
//    GtkWidget *entry_save_file = gtk_entry_new();
//    g_object_set_data(G_OBJECT(entry_save_file), "kemoview_gl", (gpointer) kemo_gl);
    GtkWidget *imageSave_Button = gtk_button_new_with_label("Save Image...");
//    g_signal_connect(G_OBJECT(imageSave_Button), "clicked",
//                     G_CALLBACK(image_save_CB), (gpointer) entry_save_file);
    
//    GtkClipboard *clipboard;
//    clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);                                                            
//    gtk_clipboard_clear(clipboard);                                                                                  
//    gtk_clipboard_set_text(clipboard, "", 0);                                                                        

//    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);                                                          
//    gtk_clipboard_clear(clipboard);                                                                                
//    gtk_clipboard_set_text(clipboard, "", 0);
//    g_object_set_data(G_OBJECT(clipboard), "kemoview_gl", (gpointer) kemo_gl);
    
    GtkWidget *copyButton = gtk_button_new_with_label("Copy");
//    g_signal_connect(G_OBJECT(copyButton), "clicked",
//                     G_CALLBACK(gtkCopyToClipboard_CB), (gpointer) clipboard);
    
    savebox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(savebox), imageSave_Button);
    gtk_box_append(GTK_BOX(savebox), copyButton);
    gtk_box_append(GTK_BOX(savebox), quitButton);
    return savebox;
};

static GtkWidget * make_gtk_main_menu_box(struct main_buttons *mbot,
                                          GtkWidget *quitButton,
                                          GtkWidget *main_window,
                                          struct kemoviewer_gl_type *kemo_gl){
    GtkWidget *vbox_menu;
    
    GtkWidget *hbox_open = make_gtk_open_file_box(kemo_gl, main_window, mbot);
    GtkWidget *savebox = make_gtk_save_file_box(quitButton, kemo_gl);
    
//    GtkWidget *hbox_viewtype = make_gtk_viewmode_menu_box(kemo_gl, mbot->view_menu);
//    GtkWidget *hbox_axis = make_axis_menu_box(kemo_gl, main_window);
//    GtkWidget *expander_rot = init_rotation_menu_expander(kemo_gl, mbot->rot_gmenu,
//                                                          main_window);
//    mbot->itemTEvo = init_evolution_menu_expander(kemo_gl, mbot->evo_gmenu, main_window);

//    mbot->expander_view = init_viewmatrix_menu_expander(kemo_gl, mbot->view_menu,
//                                                        main_window);
//    mbot->expander_quilt = init_quilt_menu_expander(kemo_gl, mbot->quilt_gmenu,
//                                                    mbot->view_menu, main_window);
    
    vbox_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(vbox_menu), hbox_open);
    gtk_box_append(GTK_BOX(vbox_menu), savebox);
    //  gtk_box_append(GTK_BOX(vbox_menu), hbox_viewtype);
    //  gtk_box_append(GTK_BOX(vbox_menu), hbox_axis);
    //  gtk_box_append(GTK_BOX(vbox_menu), expander_rot);
    //  gtk_box_append(GTK_BOX(vbox_menu), mbot->itemTEvo);
    //  gtk_box_append(GTK_BOX(vbox_menu), mbot->expander_quilt);
    //    gtk_box_append(GTK_BOX(vbox_menu), mbot->expander_view);
    return vbox_menu;
}


/* Main GTK window */
static void kemoview_activate(GApplication *app, gpointer user_data)
{
    gtk_win = gtk_window_new();
    g_signal_connect(G_OBJECT(gtk_win), "destroy",
                     G_CALLBACK(gtkWindowclose_CB), NULL);
    gtk_window_set_application(GTK_WINDOW(gtk_win), GTK_APPLICATION(app));
    
    iflag_fast_prev = 0;
    GtkWidget *quitButton = gtk_button_new_with_label("Quit");
    g_signal_connect(G_OBJECT(quitButton), "clicked",
                     G_CALLBACK(gtkWindowclose_CB), NULL);
    
    GtkWidget *vbox_main = make_gtk_main_menu_box(mbot, quitButton, gtk_win,
                                                  kemoview_gl);
    
    gtk_window_set_child(GTK_WINDOW(gtk_win), vbox_main);
    gtk_widget_set_visible(gtk_win, TRUE);
    
    mainloop_4_glfw();
}

/* Main routine for C */

int draw_mesh_kemo(int argc, char *argv[]) {
	GtkApplication *app;
    
	int iflag_retinamode = 1;
	/*! glfw Initialization*/
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MacOS
	if(iflag_retinamode == 1){
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
	} else{
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
	};
#endif
	
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
	    
/* Create a windowed mode window and its OpenGL context */
    int nx_buf, ny_buf;
	glfw_win = open_kemoviwer_glfw_window(NPIX_X, NPIX_Y);
    glfwGetFramebufferSize(glfw_win, &nx_buf, &ny_buf);
    fprintf(
            stdout,
            "INFO: OpenGL Version: %s\n",
            glGetString(GL_VERSION)
            );
    

/* Initialize arrays for viewer */
        
    single_kemoview = kemoview_allocate_single_viwewer_struct();
    kemoview_gl = kemoview_allocate_gl_pointers(single_kemoview);
    kemoview_init_lighting(single_kemoview);
    
    kemoview_set_windowsize(nx_buf, ny_buf, NPIX_X, NPIX_Y,
                            single_kemoview);
	
/* set callback for GLfw*/
	kemoviewer_reset_to_init_angle(single_kemoview);
	glfw_callbacks_init(single_kemoview, kemoview_gl);
	
	/* Set Cllback for drug and Drop into window */
	glfwSetDropCallback(glfw_win, dropFileToGlfw_CB);
	
	/* Set callback for window size changing */
	glfwSetWindowSizeCallback(glfw_win, windowSizeCB);
	/* Set callback for window framebuffer size changing
	glfwSetFramebufferSizeCallback(glfw_win, frameBufferSizeCB);
	*/
	/* set callback for window focus */
	glfwSetWindowFocusCallback(glfw_win, glfwWindowFocus_CB);
	/* set callback for window focus */
	glfwSetWindowCloseCallback(glfw_win, glfwWindowclose_CB);
	
	/* ! set the perspective and lighting */
    kemoview_init_background_color(single_kemoview);
    kemoview_gl_background_color(single_kemoview);
	kemoview_init_phong_light_list(single_kemoview);
	
//	iflag_gtk_focus = 1;
	glClear(GL_COLOR_BUFFER_BIT);
    draw_full_gl(kemoview_gl);
    glfwPollEvents();
	glfwPostEmptyEvent();
	
    /*! GTK Initialization*/
    /* gtk_set_locale(); */
    app = gtk_application_new("com.example.KemoViewTest", 
                              G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(G_OBJECT(app), "activate", G_CALLBACK(kemoview_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
	glfwTerminate();
    
    g_object_unref(app);
    return 0;
};


int main(int argc, char *argv[]){
	int ierr = draw_mesh_kemo(argc, argv);
	return ierr;
};


