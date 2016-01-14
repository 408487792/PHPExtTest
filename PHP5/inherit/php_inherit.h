/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_INHERIT_H
#define PHP_INHERIT_H

extern zend_module_entry inherit_module_entry;
#define phpext_inherit_ptr &inherit_module_entry

#define PHP_INHERIT_VERSION "0.1.0" /* Replace with version number for your extension */


/*
 * use namespace
 */
#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)

#define INHERIT_INIT_CLASS_ENTRY(ce, name, name_ns, methods) INIT_CLASS_ENTRY(ce, name_ns, methods);
#else

#define INHERIT_INIT_CLASS_ENTRY(ce, name, name_ns, methods) INIT_CLASS_ENTRY(ce, name, methods)
#endif


#ifdef PHP_WIN32
#	define PHP_INHERIT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_INHERIT_API __attribute__ ((visibility("default")))
#else
#	define PHP_INHERIT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
	定义所有的全局变量
*/
ZEND_BEGIN_MODULE_GLOBALS(inherit)
	long global_val;
	char *global_str;
	zend_bool bool;
	zend_bool use_namespace;
ZEND_END_MODULE_GLOBALS(inherit)


/* Always refer to the globals in your function as INHERIT_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
   INHERIT_G(v) ZEND_TSRMG(inherit_globals_id, zend_inherit_globals *, v)
*/

#ifdef ZTS
#define INHERIT_G(v) TSRMG(inherit_globals_id, zend_inherit_globals *, v)
#ifdef COMPILE_DL_INHERIT
ZEND_TSRMLS_CACHE_EXTERN();
#endif
#else
#define INHERIT_G(v) (inherit_globals.v)
#endif


#endif	/* PHP_INHERIT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
