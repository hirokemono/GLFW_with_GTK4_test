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

static void draw_colormap(struct colormap_params *cmap_param, cairo_t *cr, GdkWindow *window)
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
    int i;
    int num_cmap = count_real2_clist(cmap_param->colormap);
	int num_omap = count_real2_clist(cmap_param->opacitymap);
    long ntot = num_cmap + num_omap;
	double d1, v1;
	
	i_point = (long *) calloc(ntot, sizeof(long));
	d_point = (double *) calloc(ntot, sizeof(double));
	for(i=0;i<num_cmap;i++){
		set_from_real2_clist_at_index(i, cmap_param->colormap, &d_point[i], &v1);
        i_point[i] = i;
	}
	
	for(i=0;i<num_omap;i++){
		set_from_real2_clist_at_index(i, cmap_param->opacitymap, &d_point[i+num_cmap], &v1);
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
		
		set_from_real2_clist_at_index(0, cmap_param->colormap, &d_bottom, &v1);
		set_from_real2_clist_at_index(num_cmap-1, cmap_param->colormap, &d_top, &v1);

		set_from_real2_clist_at_index(0, cmap_param->opacitymap, &d1, &v1);
		if(d1 < d_bottom){d_bottom = d1;};
		set_from_real2_clist_at_index(num_omap-1, cmap_param->opacitymap, &d1, &v1);
		if(d1 > d_top){d_top = d1;};
		range = d_top - d_bottom;
		
        struct colormap_array *cmap_array = init_colormap_from_list(cmap_param->colormap);
		struct colormap_array *omap_array = init_colormap_from_list(cmap_param->opacitymap);
        for(i=0;i<ntot-1;i++){
            double rnorm = color_normalize_linear_segment_c(cmap_array->num,
                                                            cmap_array->data,
                                                            cmap_array->value, d_point[i]);
            cal_rgb_from_value_s(cmap_param->id_color_mode, rnorm,
                                 &red1, &green1, &blue1);
            
            o_point = set_opacity_from_value_s(omap_array, d_point[i]) / cmap_param->opacity_max;
			
            height_s = 1.0 - (d_point[i] - d_bottom) / range;
            cairo_pattern_add_color_stop_rgb(pattern1, height_s, red1, green1, blue1);
            cairo_pattern_add_color_stop_rgba(pattern2, height_s, red1, green1, blue1, o_point);
		}
		
        for(i=1;i<10;i++){
            d_current = d_bottom + (double) i * range / 10.0;
            double rnorm = color_normalize_linear_segment_c(cmap_array->num,
                                                            cmap_array->data,
                                                            cmap_array->value, d_current);
            cal_rgb_from_value_s(cmap_param->id_color_mode, rnorm,
                                 &red1, &green1, &blue1);
            
            o_point = set_opacity_from_value_s(omap_array, d_current) / cmap_param->opacity_max;
            
            height_s = 1.0 - (d_current - d_bottom) / range;
            cairo_pattern_add_color_stop_rgb(pattern1, height_s, red1, green1, blue1);
            cairo_pattern_add_color_stop_rgba(pattern2, height_s, red1, green1, blue1, o_point);
        }
		dealloc_colormap_array(omap_array);
        dealloc_colormap_array(cmap_array);

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
			set_from_real2_clist_at_index(i, cmap_param->colormap, &d_current, &v1);
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
			set_from_real2_clist_at_index(i, cmap_param->opacitymap, &d_current, &v1);
			top_s = top + height * (1.0 - (d_current - d_bottom) / range);
             sprintf(c_txt, "%.4e", d_current);
            cairo_move_to(cr, left+width+30, top_s);
            cairo_show_text(cr, c_txt);
        }
    };

//    cairo_destroy(cr);
    free(d_point);
    free(i_point);
    return;
};


gboolean expose_event_CB(cairo_t *cr, struct colormap_view *color_vws)
{ 
	draw_colormap(color_vws->cmap_param, cr,
                  gtk_widget_get_window(color_vws->scrolled_window));
    return FALSE;
}

#endif   /*  KEMOVIEW_CAIRO_COLORMAP_ */
