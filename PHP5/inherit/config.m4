dnl $Id$
dnl config.m4 for extension inherit

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(inherit, for inherit support,
dnl Make sure that the comment is aligned:
dnl [  --with-inherit             Include inherit support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(inherit, whether to enable inherit support,
dnl Make sure that the comment is aligned:
[  --enable-inherit           Enable inherit support])

if test "$PHP_INHERIT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-inherit -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/inherit.h"  # you most likely want to change this
  dnl if test -r $PHP_INHERIT/$SEARCH_FOR; then # path given as parameter
  dnl   INHERIT_DIR=$PHP_INHERIT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for inherit files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       INHERIT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$INHERIT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the inherit distribution])
  dnl fi

  dnl # --with-inherit -> add include path
  dnl PHP_ADD_INCLUDE($INHERIT_DIR/include)

  dnl # --with-inherit -> check for lib and symbol presence
  dnl LIBNAME=inherit # you may want to change this
  dnl LIBSYMBOL=inherit # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $INHERIT_DIR/$PHP_LIBDIR, INHERIT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_INHERITLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong inherit lib version or lib not found])
  dnl ],[
  dnl   -L$INHERIT_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(INHERIT_SHARED_LIBADD)

  dnl PHP_NEW_EXTENSION函数声明了这个扩展的名称、需要的源文件名、此扩展的编译形式。如果我们的扩展使用了多个文件，便可以将这多个文件名罗列在函数的参数里，如：
  dnl PHP_NEW_EXTENSION(sample, sample.c sample2.c sample3.c, $ext_shared)
  dnl 最后的$ext_shared参数用来声明这个扩展不是一个静态模块，而是在php运行时动态加载的。
  PHP_NEW_EXTENSION(inherit, inherit.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
