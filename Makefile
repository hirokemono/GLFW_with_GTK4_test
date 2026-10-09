
SHELL = /bin/sh

SRCDIR = /Users/matsui/src_kemo
MAKEDIR = $(SRCDIR)/work

CC=mpicc

OPTFLAGS = -O3 -Wall -g
CPPFLAGS = -D__APPLE__ -DPNG_OUTPUT -DZLIB_IO -DGLFW3 -DFFMPEG
CPPFLAGS+= -DGL_SILENCE_DEPRECATION
CPPFLAGS+= -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

OPTFLAGS+= $(CPPFLAGS)

KEMO_INCLUDE= -I. -I$(GTK3_DIR) \
-I$(MAKEDIR)                    \
-I$(SRCDIR)/MHD/C_src/CORE_C    \
-I$(SRCDIR)/MHD/C_src/CONTROLS  \
-I$(SRCDIR)/MHD/C_src/GLSL      \
-I$(SRCDIR)/MHD/C_src/KEMO_GL   \
-I/opt/homebrew/Cellar/zlib/1.3.1/include

KEMO_LIBS =     -L$(MAKEDIR) -lkemo_c
KEMO_LIB_FILES =  $(MAKEDIR)/libkemo_c.a 


PACKAGES = glfw3 libavutil libavcodec libavformat libswscale libpng

GTK4_CFLAGS = $(shell pkg-config --cflags gtk4 $(PACKAGES))
GTK4_LIBS =   $(shell pkg-config --libs gtk4 $(PACKAGES))

FRAMEWORKS = -framework OpenGL  -framework CoreVideo  -framework IOKit -framework Cocoa
FRAMEWORKS+= -L/opt/homebrew/Cellar/zlib/1.3.1/lib -lz 


TARGET = kemo_gtk4_test
SRC = \
kemoview_FFMPEG_encoder.c \
render_on_GLFW.c \
movie_from_GLFW_by_FFMPEG.c \
view_modifier_glfw.c \
\
calypso_GTK4.c \
tree_view_real3_GTK.c \
tree_views_4_fixed_lists_GTK.c        \
kemoview_gtk4_routines.c              \
kemoview_gtk4_image_format_selector.c \
kemoview_gtk4_rotation_menu.c         \
kemoview_gtk4_rotation_expander.c     \
kemoview_gtk4_viewmatrix_menu.c       \
kemoview_gtk4_viewmode_menu.c         \
kemoview_gtk4_fileselector.c          \
kemoview_gtk4_evolution_menu.c        \
kemoview_gtk4_PSF_menu.c              \
kemoview_gtk4_PSF_window.c            \
kemoview_gtk4_light_menu.c            \
kemoview_gtk4_preference_menu.c       \
kemoview_gtk4_menu_button.c           \
kemoview_gtk4_main_menu.c             \
kemo_glfw_gtk4_test.c

OBJS = \
$(MAKEDIR)/kemoviewer_gl.o

all: $(TARGET)

$(KEMO_LIB_FILES): 
	cd $(MAKEDIR); make $(KEMO_LIB_FILES)

$(TARGET): $(SRC) $(OBJS) $(KEMO_LIB_FILES)
	$(CC) $(OPTFLAGS) $(GTK4_CFLAGS) $(KEMO_INCLUDE) -o $@ $(SRC) \
	$(OBJS) $(KEMO_LIBS)  $(GTK4_LIBS) $(FRAMEWORKS)

menu_test: menutest/menu_test.c
	$(CC) $(OPTFLAGS) $(GTK4_CFLAGS) $(KEMO_INCLUDE) -o $@ $< \
	$(OBJS) $(KEMO_LIBS)  $(GTK4_LIBS) $(FRAMEWORKS)

%.o: %.c
	$(CC) $(OPTFLAGS) $(GTK4_CFLAGS) $(KEMO_INCLUDE) -c $<

clean:
	rm -rf *.o *.mod *~
	rm -rf menu_test*
	for target in $(TARGET); do \
		(rm -fr $${target}*) \
	done; \
