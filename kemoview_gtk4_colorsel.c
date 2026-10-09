/***********************************************************************
 *  kemoview_gtk4_colorsel.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on 12/03/04.
 *  Copyright 2012 Dept. of Earth and Planetary Science, UC Berkeley. All rights reserved.
 ***********************************************************************/

#include "kemoview_gtk4_colorsel.h"

GtkWidget *window_Cmap;

void set_color_to_GTK(float color[4], GdkRGBA *gcolor){
    gcolor->red =   color[0];
    gcolor->green = color[1];
    gcolor->blue =  color[2];
    gcolor->alpha = color[3];
	return;
}

void set_RGB_to_GTK(float color[4], GdkRGBA *gcolor){
    gcolor->red =   color[0];
    gcolor->green = color[1];
    gcolor->blue =  color[2];
    gcolor->alpha = 1.0;
	return;
}

void set_RGB_from_GTK(GdkRGBA *gcolor, float color[4]){
    color[0] = (float) gcolor->red;
    color[1] = (float) gcolor->green;
    color[2] = (float) gcolor->blue;
    color[3] = (float) 1.0;
	return;
}

static void set_color_from_GTK_CB(GObject    *object,
                                  GParamSpec *pspec,
                                  gpointer    user_data){
    GtkColorDialogButton *color_button = GTK_COLOR_DIALOG_BUTTON (object);
	GdkRGBA *rgba = gtk_color_dialog_button_get_rgba(color_button);
	
    g_print("Color selected: Red=%.2f, Green=%.2f, Blue=%.2f, Alpha=%.2f\n",
            rgba->red, rgba->green, rgba->blue, rgba->alpha);
	/*
    color[0] = (float) rgba->red;
    color[1] = (float) rgba->green;
    color[2] = (float) rgba->blue;
    color[3] = (float) 1.0;
	/*printf("New background Color (R,G,B): %.7e %.7e %.7e \n", color[0], color[1], color[2]);*/
	
	return;
}

int kemoview_gtk_colorsel_CB(GtkWindow *parent_win, float color[4]){
    GtkColorDialog *color_dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(color_dialog, "Choose a color");
    gtk_color_dialog_set_with_alpha (color_dialog, TRUE); // Allow transparency adjustments
    gtk_widget_set_visible(color_dialog, TRUE);
    
    GtkWidget *color_button = gtk_color_dialog_button_new(color_dialog);
    
    GdkRGBA initial_color = {0.2, 0.4, 0.6, 1.0}; // A nice blue shade
    gtk_color_dialog_button_set_rgba (GTK_COLOR_DIALOG_BUTTON(color_button), &initial_color);
    
    g_signal_connect (color_button, "notify::rgba", G_CALLBACK(set_color_from_GTK_CB), NULL);
    return 0;
}
