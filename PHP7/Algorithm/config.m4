dnl $Id$
dnl config.m4 for extension Algorithm

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(Algorithm, for Algorithm support,
dnl Make sure that the comment is aligned:
dnl [  --with-Algorithm             Include Algorithm support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(Algorithm, whether to enable Algorithm support,
dnl Make sure that the comment is aligned:
dnl [  --enable-Algorithm           Enable Algorithm support])

if test "$PHP_ALGORITHM" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-Algorithm -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/Algorithm.h"  # you most likely want to change this
  dnl if test -r $PHP_ALGORITHM/$SEARCH_FOR; then # path given as parameter
  dnl   ALGORITHM_DIR=$PHP_ALGORITHM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for Algorithm files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ALGORITHM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ALGORITHM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the Algorithm distribution])
  dnl fi

  dnl # --with-Algorithm -> add include path
  dnl PHP_ADD_INCLUDE($ALGORITHM_DIR/include)

  dnl # --with-Algorithm -> check for lib and symbol presence
  dnl LIBNAME=Algorithm # you may want to change this
  dnl LIBSYMBOL=Algorithm # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ALGORITHM_DIR/$PHP_LIBDIR, ALGORITHM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ALGORITHMLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong Algorithm lib version or lib not found])
  dnl ],[
  dnl   -L$ALGORITHM_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ALGORITHM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(Algorithm, Algorithm.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
