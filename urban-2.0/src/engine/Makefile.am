bin_PROGRAMS = urban

urban_DEPENDENCIES = ../map/libmap.a \
	../meny/libmeny.a \
	../objects/libobjects.a \
	../intro/libintro.a \
	../font/libfont.a \
	../highscor/libhighscor.a \
	../ctrls/libctrls.a \
	../datfile/libdatfile.a \
	../sound/libsound.a \
	../config/libconfig.a \
	../credits/libcredits.a \
	../outtro/libouttro.a \
	../gfx/libgfx.a
	
LIBS = ../map/libmap.a \
	../meny/libmeny.a \
	../objects/libobjects.a \
	../intro/libintro.a \
	../font/libfont.a \
	../highscor/libhighscor.a \
	../ctrls/libctrls.a \
	../datfile/libdatfile.a \
	../sound/libsound.a \
	../config/libconfig.a \
	../credits/libcredits.a \
	../outtro/libouttro.a \
	../gfx/libgfx.a \
	@GGI_LIBS@ \
	@LIBMIKMOD_LIBS@ \
	@PTHREAD_LIBS@

urban_LDADD = @LIBMIKMOD_LDADD@

CXXFLAGS = @CXXFLAGS@ @LIBMIKMOD_CFLAGS@

urban_SOURCES = engine.cc \
	game.cc \
	icache.cc \
	score.cc

INCLUDES = -I../include
