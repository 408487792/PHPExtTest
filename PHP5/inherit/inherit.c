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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_inherit.h"
#include "Zend/zend_interfaces.h"


/* If you declare any globals in php_inherit.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(inherit)

/* True global resources - no need for thread safety here */
static int le_inherit;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini */
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("inherit.intval",      "42", PHP_INI_PERDIR, OnUpdateLong, global_val, zend_inherit_globals, inherit_globals)
	STD_PHP_INI_ENTRY("inherit.hello", "hello php", PHP_INI_SYSTEM, OnUpdateString, global_str, zend_inherit_globals, inherit_globals)
	PHP_INI_ENTRY("inherit.fltval", "0.1", PHP_INI_USER, NULL)
	PHP_INI_ENTRY("inherit.boolval", "0", PHP_INI_ALL, NULL)
PHP_INI_END()


/*
 *
<?php
	interface i_myinterface
	{
		public function hello();
	}

	class parent_class implements i_myinterface
	{
		public function hello()
		{
			echo "Good Morning!\n";
		}
	}

	final class myclass extends parent_class
	{
		public function call_hello()
		{
			$this->hello();
		}
	}

	class baby
	{
		public $age;
		public static $area;

		public function __construct($age, $area)
		{
			$this->age = $age;
			self::$area = $area;

			var_dump($this->age, self::$area);
		}
	}

	function test_call(){
		$obj = new myclass();
		$obj->hello();
		echo "call test_call";
	}

 * */

//三个zend_class_entry
zend_class_entry *i_myinterface_ce, *parent_class_ce, *myclass_ce, *baby_ce;

//parent_class的hello方法   i_myinterface_ce接口方法没有方法体
ZEND_METHOD(parent_class, hello){
	php_printf("hello parent_class \n");
	php_printf("sizeof(area) == %d \n", sizeof("area"));
	php_printf("strlen(area) == %d \n", strlen("area"));
	php_printf("PHP_MAJOR_VERSION: %d \n", PHP_MAJOR_VERSION);
}

//myclass的call_hello方法
ZEND_METHOD(myclass, call_hello){
	zval *this_zval;
	this_zval = getThis();
	//这个方法在zend_interfaces.h 中 需要 #include "zend_interfaces.h"
	zend_call_method_with_0_params(&this_zval, myclass_ce, NULL, "hello", NULL);
}

ZEND_METHOD(myclass, __construct){
	php_printf("myclass __construct \n");
}

ZEND_METHOD(baby, __construct){
	zval *age, *area;
	zend_class_entry *self;
	self = Z_OBJCE_P(getThis());

#if PHP_MAJOR_VERSION == 7
	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_ZVAL(age)
		Z_PARAM_ZVAL_EX(area, 1, 0)
		//Z_PARAM_OPTIONAL() == '|'
	ZEND_PARSE_PARAMETERS_END();
#else
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &age, &area) == FAILURE){
		php_error(E_WARNING, "%s() parameters error",
			get_active_function_name(TSRMLS_C));
		RETURN_FALSE;
	}
#endif

	zend_update_property(self, getThis(), ZEND_STRL("age"), age TSRMLS_CC);
	zend_update_static_property(self, ZEND_STRL("area"), area TSRMLS_CC);
	age = NULL;
	area = NULL;
#if PHP_MAJOR_VERSION == 7
	age = zend_read_property(self, getThis(), ZEND_STRL("age"), 0, NULL);// php7 需要一个参数 zval *rv 默认传NULL
#else
	age = zend_read_property(self, getThis(), ZEND_STRL("age"), 0);
#endif

	if (ZVAL_IS_NULL(age)) {
		php_error(E_WARNING, "age param Is NULL");
		RETURN_FALSE;
	}
	php_var_dump(&age, 1 TSRMLS_CC);
	area = zend_read_static_property(self, ZEND_STRL("area"), 0);
	if (ZVAL_IS_NULL(area)) {
		zend_error(E_ERROR, "age param Is NULL: %s::$area", self->name);
		RETURN_FALSE;
	}
	php_var_dump(&area, 1 TSRMLS_CC);

	const char *strval = INI_STR("inherit.hello");
	long lval = INI_ORIG_INT("inherit.intval");//获取初始值
	double dval = INI_FLT("inherit.fltval");//获取ini变量的值
	zend_bool bval = INI_BOOL("inherit.boolval");
	php_printf("php_ini_set: inherit.hello: %s, inherit.intval: %d, inherit.fltval: %f, inherit.boolval: %d \n", strval, lval, dval, bval);

	/*  silent参数：
		0: 如果属性不存在，则抛出一个notice错误。
		1: 如果属性不存在，不报错。

		sizeof strlen
		char str[20] = "0123456789";
		strlen(str) == 10
		sizeof(str) == 20

		char *strs = "0123456789";
		sizeof(strs) == 4 pointer
		sizeof(*strs) == 1 字符串首字符
	 */
}


ZEND_FUNCTION(test_call){
	zval * obj;
	MAKE_STD_ZVAL(obj);
	object_init_ex(obj, myclass_ce);
	//如果确认此类没有构造函数就不用调用了。
	zend_call_method_with_0_params(&obj, myclass_ce, NULL, "__construct", NULL);
	zend_call_method_with_0_params(&obj, myclass_ce, NULL, "hello", NULL);
	php_printf("call test_call \n");
	zval_ptr_dtor(&obj);
	return ;
}

//各自的zend_function_entry
const zend_function_entry i_myinterface_method[] = {
	ZEND_ABSTRACT_ME(i_myinterface, hello, NULL)
	ZEND_FE_END
};

const zend_function_entry parent_class_method[] = {
	ZEND_ME(parent_class, hello, NULL, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

const zend_function_entry myclass_method[] = {
	ZEND_ME(myclass, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	ZEND_ME(myclass, call_hello, NULL, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

const zend_function_entry baby_method[] = {
	ZEND_ME(baby, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	ZEND_FE_END
};

PHP_MINIT_FUNCTION(inherit)
{
	REGISTER_INI_ENTRIES();//注册ini变量
	//定义常量
	/* 等价于define('Author', 'YunDuan'); */
	REGISTER_STRINGL_CONSTANT("Author", "YunDuan", sizeof("YunDuan") - 1, CONST_PERSISTENT | CONST_CS);
	REGISTER_STRINGL_CONSTANT("Version", PHP_INHERIT_VERSION, sizeof(PHP_INHERIT_VERSION) - 1, CONST_PERSISTENT | CONST_CS);

	zend_class_entry ce, p_ce, i_ce,b_ce;
	INIT_CLASS_ENTRY(i_ce, "i_myinterface", i_myinterface_method);
	i_myinterface_ce = zend_register_internal_class(&i_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(p_ce, "parent_class", parent_class_method);
	parent_class_ce = zend_register_internal_class(&p_ce TSRMLS_CC);
	zend_class_implements(parent_class_ce TSRMLS_CC, 1, i_myinterface_ce);

	//定义子类，使用zend_register_internal_class_ex函数
	INIT_CLASS_ENTRY(ce, "myclass", myclass_method);
#if PHP_MAJOR_VERSION == 7
	myclass_ce = zend_register_internal_class_ex(&ce, parent_class_ce);
#else
	myclass_ce = zend_register_internal_class_ex(&ce,parent_class_ce,"parent_class" TSRMLS_CC);
	//注意：ZEND_ACC_FINAL是用来修饰方法的，而ZEND_ACC_FINAL_CLASS是用来修饰类的,但php7中已删除
	myclass_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;
#endif

	INIT_CLASS_ENTRY(b_ce, "baby", baby_method);
	baby_ce = zend_register_internal_class(&b_ce TSRMLS_CC);
	//定义类常量 静态属性
	zend_declare_class_constant_stringl(baby_ce, ZEND_STRL("author"), ZEND_STRL("yunduan") TSRMLS_CC);
	zend_declare_class_constant_stringl(baby_ce, ZEND_STRL("version"), ZEND_STRL(PHP_INHERIT_VERSION) TSRMLS_DC);
	zend_declare_property_null(baby_ce, ZEND_STRL("area"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(baby_ce, ZEND_STRL("age"), ZEND_ACC_PUBLIC TSRMLS_CC);

#ifdef ZEND_ENGINE_3
	long TEST = 1;
#endif

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(inherit)
{
	UNREGISTER_INI_ENTRIES();//注销ini变量
	return SUCCESS;
}

PHP_RINIT_FUNCTION(inherit)
{
#if defined(COMPILE_DL_INHERIT) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(inherit)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(inherit)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "inherit support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();//ini param set
}

const zend_function_entry inherit_functions[] = {
	PHP_FE(test_call,	NULL)
	PHP_FE_END	/* Must be the last line in inherit_functions[] */
};
zend_module_entry inherit_module_entry = {
	STANDARD_MODULE_HEADER,
	"inherit",
	inherit_functions,
	PHP_MINIT(inherit),
	PHP_MSHUTDOWN(inherit),
	PHP_RINIT(inherit),
	PHP_RSHUTDOWN(inherit),
	PHP_MINFO(inherit),
	PHP_INHERIT_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_INHERIT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(inherit)
#endif


/**
 *
 *
echo '<pre>';
test_call();

echo "<br>";
echo "author:  ",baby::author,"<br>";
echo "version: ",baby::version,"<br>";

$obj = new baby("10", "www");
echo "author:  ",Author,"<br>";
echo "version: ",Version,"<br>";

output:
myclass __construct
hello parent_class
sizeof(area) == 5
strlen(area) == 4
PHP_MAJOR_VERSION: 5
call test_call

author:  yunduan
version: 0.1.0
string(2) "10"
string(2) "wh"
php_ini_set: inherit.hello: hello php, inherit.intval: 42, inherit.fltval: 0.100000, inherit.boolval: 0
author:  YunDuan
version: 0.1.0



inherit phpinfo :
inherit support	enabled
Directive	Local Value	Master Value
inherit.boolval	0	0
inherit.fltval	0.1	0.1
inherit.hello	hello php	hello php
inherit.intval	42	42
 *
 *
 *
 */
