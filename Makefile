
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
-I$(SRCDIR)/MHD/C_src/GTK       \
-I$(SRCDIR)/MHD/C_src/CONTROLS  \
-I$(SRCDIR)/MHD/C_src/GLSL      \
-I$(SRCDIR)/MHD/C_src/KEMO_GL   \
-I$(SRCDIR)/MHD/C_src/KEMO_GLUT \
-I/Users/matsui/src_kemo/MHD/programs/VIEWER \
-I/opt/homebrew/Cellar/zlib/1.3.1/include

KEMO_LIBS =     -L$(MAKEDIR) -lkemo_c
KEMO_LIB_FILES =  $(MAKEDIR)/libkemo_c.a 


PACKAGES = glfw3 libavutil libavcodec libavformat libswscale libpng

GTK3_CFLAGS = $(shell pkg-config --cflags gtk+-3.0 $(PACKAGES))
GTK3_LIBS =   $(shell pkg-config --libs gtk+-3.0 $(PACKAGES))

GTK4_CFLAGS = $(shell pkg-config --cflags gtk4 $(PACKAGES))
GTK4_LIBS =   $(shell pkg-config --libs gtk4 $(PACKAGES))

FRAMEWORKS = -framework OpenGL  -framework CoreVideo  -framework IOKit -framework Cocoa
FRAMEWORKS+= -L/opt/homebrew/Cellar/zlib/1.3.1/lib -lz 


TARGET = kemo_gtk4_test
SRC = kemo_glfw_gtk4_test.c

OBJS = \
$(MAKEDIR)/view_modifier_glfw.o \
$(MAKEDIR)/render_on_GLFW.o \
$(MAKEDIR)/kemoviewer_gl.o \
$(MAKEDIR)/movie_from_GLFW_by_FFMPEG.o \
$(MAKEDIR)/kemoview_FFMPEG_encoder.o

all: $(TARGET)

$(KEMO_LIB_FILES): 
	cd $(MAKEDIR); make $(KEMO_LIB_FILES)

$(TARGET): $(SRC) $(OBJS) $(KEMO_LIB_FILES)
	$(CC) $(OPTFLAGS) $(GTK4_CFLAGS) $(KEMO_INCLUDE) -o $@  $< \
	$(OBJS) $(KEMO_LIBS)  $(GTK4_LIBS) $(FRAMEWORKS)

clean:
	rm -rf *.o *.mod *~
	for target in $(TARGET); do \
		(rm -fr $${target}*) \
	done; \
