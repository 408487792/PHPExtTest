dnl $Id$
dnl config.m4 for extension custLibs

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(custLibs, for custLibs support,
dnl Make sure that the comment is aligned:
dnl [  --with-custLibs             Include custLibs support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(custLibs, whether to enable custLibs support,
dnl Make sure that the comment is aligned:
[  --enable-custLibs           Enable custLibs support])

if test -z "$PHP_DEBUG" ; then
  AC_ARG_ENABLE(debug,
    [--enable-debug compile with debugging system],
    [PHP_DEBUG=$enableval],[PHP_DEBUG=no]
  )
fi

if test "$PHP_CUSTLIBS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-custLibs -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/custLibs.h"  # you most likely want to change this
  dnl if test -r $PHP_CUSTLIBS/$SEARCH_FOR; then # path given as parameter
  dnl   CUSTLIBS_DIR=$PHP_CUSTLIBS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for custLibs files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CUSTLIBS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CUSTLIBS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the custLibs distribution])
  dnl fi

  dnl # --with-custLibs -> add include path
  dnl PHP_ADD_INCLUDE($CUSTLIBS_DIR/include)

  dnl # --with-custLibs -> check for lib and symbol presence
  dnl LIBNAME=custLibs # you may want to change this
  dnl LIBSYMBOL=custLibs # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CUSTLIBS_DIR/$PHP_LIBDIR, CUSTLIBS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CUSTLIBSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong custLibs lib version or lib not found])
  dnl ],[
  dnl   -L$CUSTLIBS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CUSTLIBS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(custLibs, custLibs.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
