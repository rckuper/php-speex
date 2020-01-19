dnl config.m4 for extension speex

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(speex, for speex support,
dnl Make sure that the comment is aligned:
dnl [  --with-speex             Include speex support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(speex, whether to enable speex support,
dnl Make sure that the comment is aligned:
[  --enable-speex          Enable speex support], no)

if test "$PHP_SPEEX" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, SPEEX_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-speex -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/speex.h"  # you most likely want to change this
  dnl if test -r $PHP_SPEEX/$SEARCH_FOR; then # path given as parameter
  dnl   SPEEX_DIR=$PHP_SPEEX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for speex files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SPEEX_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SPEEX_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the speex distribution])
  dnl fi

  dnl # --with-speex -> add include path
  dnl PHP_ADD_INCLUDE($SPEEX_DIR/include)

  dnl # --with-speex -> check for lib and symbol presence
  dnl LIBNAME=SPEEX # you may want to change this
  dnl LIBSYMBOL=SPEEX # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SPEEX_DIR/$PHP_LIBDIR, SPEEX_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SPEEXLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong speex lib version or lib not found])
  dnl ],[
  dnl   -L$SPEEX_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SPEEX_SHARED_LIBADD)

  PHP_ADD_LIBRARY_WITH_PATH(speex, /usr/local/speex/lib, SPEEX_SHARED_LIBADD)
  PHP_SUBST(SPEEX_SHARED_LIBADD)
  dnl # In case of no dependencies
  AC_DEFINE(HAVE_SPEEX, 1, [ Have speex support ])

  PHP_NEW_EXTENSION(speex, speex.c, $ext_shared)
  PHP_ADD_INCLUDE([/usr/local/speex/include])
fi
