/***********************************************************************
 *  kemoview_cairo_colormap.c
 *  Kemoview_Cocoa
 *
 *  Created by Hiroaki Matsui on Oct, 20, 2026
 *  Copyright 2026 Dept. of Earth and Planetary Sciences,
 *  Inst. of Science Tokyo. All rights reserved.
 *
 ***********************************************************************/

#include "kemoview_cairo_colormap.h"

gboolean draw_cairo_colormap(GtkDrawingArea *area, cairo_t *cr, 
                             struct kemoviewer_type *kemo_sgl,
                             int iflag_current_model)
{ 
    float top =     50.0;
    float left =   100.0;
    float width =   25.0;
    float height = 200.0;
    float top_s, height_s;
    
    long   *i_point;
	double *d_point;
	double o_point;
    double d_bottom = 0.0;
    double d_top = 0.0;
    double d_current = 0.0;
    double range;
	double dummy_d, dummy_c;
    int i;
    int num_cmap = kemoview_get_viz_colormap_param(kemo_sgl, 
                                                   iflag_current_model,
                                                   ISET_NUM_COLOR);
    int num_omap = kemoview_get_viz_colormap_param(kemo_sgl, 
                                                   iflag_current_model,
                                                   ISET_NUM_OPACITY);
    double opacity_max = kemoview_get_VIZ_opacity_range(kemo_sgl,
                                                   iflag_current_model,
                                                        ISET_OPACITY_MAX);
    opacity_max = 1.0 / opacity_max;
    
    long ntot = num_cmap + num_omap;
    
    i_point = (long *) calloc(ntot, sizeof(long));
    d_point = (double *) calloc(ntot, sizeof(double));
    for(i=0;i<num_cmap;i++){
        kemoview_get_VIZ_color_RGB_value(kemo_sgl,
                                         iflag_current_model,
                                         i, &d_point[i], &dummy_c);
        i_point[i] = i;
    }
    
    for(i=0;i<num_omap;i++){
        kemoview_get_PSF_opacity_items(kemo_sgl,
                                       iflag_current_model,
                                       i, &d_point[i+num_cmap], &dummy_c);
        i_point[i+num_cmap] = i;
    }
    quicksort_double_c(d_point, i_point, 0, (ntot-1));
    
    
//    cr = gdk_cairo_create(window);
    
    /* Set colorbar */
    {        
        cairo_pattern_t *pattern1;
        cairo_pattern_t *pattern2;
        double red1, green1, blue1;
        
        /* Make colormap */
        pattern1 = cairo_pattern_create_linear((left+width), top, (left+width), (top+height));
        pattern2 = cairo_pattern_create_linear((left+2*width), top, (left+2*width), (top+height));
        top_s = top;
		height_s = 1.0;
		
        kemoview_get_VIZ_color_RGB_value(kemo_sgl,
                                         iflag_current_model,
                                         0, &d_bottom, &dummy_c);
        kemoview_get_VIZ_color_RGB_value(kemo_sgl,
                                         iflag_current_model,
                                         (num_cmap-1), &d_top, &dummy_c);
        
        kemoview_get_PSF_opacity_items(kemo_sgl,
                                       iflag_current_model,
                                       0, &dummy_d, &dummy_c);
        if(dummy_d < d_bottom){d_bottom = dummy_d;};
        kemoview_get_PSF_opacity_items(kemo_sgl,
                                       iflag_current_model,
                                       (num_omap-1), &dummy_d, &dummy_c);
        if(dummy_d > d_top){d_top = dummy_d;};
        range = d_top - d_bottom;
        
        for(i=0;i<ntot-1;i++){
            kemoview_get_PSF_rgb_at_value(kemo_sgl, iflag_current_model, d_point[i],
                                          &red1, &green1, &blue1);
            o_point = kemoview_get_PSF_opacity_at_value(kemo_sgl, iflag_current_model, d_point[i]);
            o_point = o_point * opacity_max;
            
            height_s = 1.0 - (d_point[i] - d_bottom) / range;
            cairo_pattern_add_color_stop_rgb(pattern1, height_s, red1, green1, blue1);
            cairo_pattern_add_color_stop_rgba(pattern2, height_s, red1, green1, blue1, o_point);
        }
        
        for(i=1;i<10;i++){
            d_current = d_bottom + (double) i * range / 10.0;
            kemoview_get_PSF_rgb_at_value(kemo_sgl, iflag_current_model, d_current,
                                          &red1, &green1, &blue1);
            o_point = kemoview_get_PSF_opacity_at_value(kemo_sgl, iflag_current_model, d_current);
            o_point = o_point * opacity_max;
            
            height_s = 1.0 - (d_current - d_bottom) / range;
            cairo_pattern_add_color_stop_rgb(pattern1, height_s, red1, green1, blue1);
            cairo_pattern_add_color_stop_rgba(pattern2, height_s, red1, green1, blue1, o_point);
        }
        
        /* Set colorpattern without opacity */
            cairo_set_source(cr, pattern1);
        /* Fill rectangle */
        /* cairo_rectangle(cr, left, top, width, height); */
            cairo_rectangle(cr, left, top, width, height);
            cairo_fill_preserve(cr);
 
        /* Set colorpattern including opacity */
            cairo_set_source(cr, pattern2);
        /* Fill rectangle */
            cairo_rectangle(cr, (left+width), top, width, height);
            cairo_fill(cr);
    }
    /* Make frame */
    {
        cairo_set_line_width(cr, 2.0);
    /* Set rounded corner */
        cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_rectangle(cr, left, top, (2.0*width), height);
        cairo_stroke(cr);
    }
    
    {
        cairo_select_font_face (cr, "Serif", CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 12);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        top_s = top;
        char *c_txt = (char *) calloc(30, sizeof(char));
        cairo_move_to(cr, left-70, top_s-20);
        cairo_show_text(cr, "Color point");
        for(i=0;i<num_cmap;i++){
            kemoview_get_VIZ_color_RGB_value(kemo_sgl,
                                             iflag_current_model,
                                             i, &d_current, &dummy_c);
            top_s = top + height * (1.0 - (d_current - d_bottom) / range);
            sprintf(c_txt, "%.4e", d_current);
            cairo_move_to(cr, left-70, top_s);
            cairo_show_text(cr, c_txt);
        }
        top_s = top;
        height_s = 0.0;
        cairo_move_to(cr, left+width+30, top_s-20);
        cairo_show_text(cr, "Opacity point");
        for(i=0;i<num_omap;i++){
            kemoview_get_PSF_opacity_items(kemo_sgl,
                                           iflag_current_model,
                                           i, &d_current, &dummy_c);
            top_s = top + height * (1.0 - (d_current - d_bottom) / range);
            sprintf(c_txt, "%.4e", d_current);
            cairo_move_to(cr, left+width+30, top_s);
            cairo_show_text(cr, c_txt);
        }
    };

//    cairo_destroy(cr);
    free(d_point);
    free(i_point);
    printf("Draw by Cairo done\n");
    return FALSE;
};

