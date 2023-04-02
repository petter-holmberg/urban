dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/engine/engine.cc)
AM_INIT_AUTOMAKE(urban, 2.0.1)
AM_CONFIG_HEADER(amconfig.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS(SFML/Graphics.hpp SFML/System.hpp SFML/Window.hpp,GGI_LIBS="-lsfml-graphics -lsfml-system -lsfml-window",AC_ERROR("Urban needs SFML header files"))
AC_SUBST(GGI_LIBS)
AC_LANG_POP([C++])

AC_CHECK_HEADERS(pthread.h)
AC_CHECK_LIB(pthread,pthread_mutex_init,PTHREAD_LIBS="-lpthread",AC_ERROR("Urban needs pthreads"))
AC_SUBST(PTHREAD_LIBS)

AM_PATH_LIBMIKMOD(3.1.7, have_mikmod=yes, have_mikmod=no)
AM_CONDITIONAL(HAVE_MIKMOD, test "x$have_mikmod" = xyes)

datdir=$datadir/urban
AC_SUBST(datdir)


AC_ARG_ENABLE( cheat-codes,
[ --enable-cheat-codes  Enables cheat-codes
[default=disable]],CXXFLAGS="$CXXFLAGS -DALLOW_Fx_CHEATING",)

AC_ARG_WITH( global-highscore,
[ --with-global-highscore=filename  Use a global higscore file for all users
[default=no]],global_highscore=$withval,global_highscore="no")

if test "$global_highscore" != "no"; then
  CXXFLAGS="$CXXFLAGS -DHSFILENAME=\\\"$global_highscore\\\""
fi

CXXFLAGS="$CXXFLAGS -DDATPATH=\\\"$datdir\\\" -D_REENTRANT"

CXXFLAGS="-std=c++20 -Wall -Wextra -Weffc++ -Werror $CXXFLAGS"

AC_OUTPUT(
Makefile
src/Makefile
src/data/Makefile
src/editor/Makefile
src/include/Makefile
src/credits/Makefile
src/outtro/Makefile
src/intro/Makefile
src/datfile/Makefile
src/highscor/Makefile
src/font/Makefile
src/ctrls/Makefile
src/objects/Makefile
src/engine/Makefile
src/map/Makefile
src/meny/Makefile
src/gfx/Makefile
src/sound/Makefile
src/config/Makefile
)





