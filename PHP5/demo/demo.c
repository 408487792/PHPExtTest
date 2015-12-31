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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_demo.h"

/* If you declare any globals in php_demo.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(demo)
*/

/* True global resources - no need for thread safety here */
static int le_demo;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("demo.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_demo_globals, demo_globals)
    STD_PHP_INI_ENTRY("demo.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_demo_globals, demo_globals)
PHP_INI_END()
*/
/* }}} */


/* {{{ php_demo_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_demo_init_globals(zend_demo_globals *demo_globals)
{
	demo_globals->global_value = 0;
	demo_globals->global_string = NULL;
}
*/
/* }}} */


ZEND_FUNCTION(sample_arg_fullnull) {
    zval *val;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z",&val) == FAILURE) {
        RETURN_NULL();
    }
    if (Z_TYPE_P(val) == IS_NULL) {
    	RETURN_NULL();
    }
}


ZEND_FUNCTION(sample_arg_nullok) {
    zval *val;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z!", &val) == FAILURE) {
        RETURN_NULL();
    }
    if (!val) {
    	php_printf("value is: %s \n", val);
    }
}


ZEND_FUNCTION(sample_hello_world) {
    char *name;
    int name_len;
    char *greeting = "Mr./Mrs.";
    int greeting_len = sizeof("Mr./Mrs.") - 1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s",
      &name, &name_len, &greeting, &greeting_len) == FAILURE) {
        RETURN_NULL();
    }
    php_printf("Hello ");
    PHPWRITE(greeting, greeting_len);
    php_printf(" ");
    PHPWRITE(name, name_len);
    php_printf("!\n");
}


ZEND_FUNCTION(sample_onearg) {
    zval *firstarg;
    if (zend_get_parameters(ZEND_NUM_ARGS(), 1, &firstarg)== FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,"Expected at least 1 parameter.");
        RETURN_NULL();
    }
}


PHP_FUNCTION(var_type_cust){
	zval *arg, *ret_val;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		return ;
	}

	switch (Z_TYPE_P(arg)) {
	        case IS_NULL:
	        	php_printf("类型是 IS_NULL");
#if PHP_MAJOR_VERSION == 7
	        	RETVAL_STRING("NULL");
#else
	        	RETVAL_STRING("NULL", 1);
#endif
			break;

	        case IS_BOOL:
	        	php_printf("类型是 IS_BOOL，值是：%d" , Z_LVAL_P(arg));
#if PHP_MAJOR_VERSION == 7
	            RETVAL_STRING("boolean");
#else
	        	RETVAL_STRING("boolean", 1);
#endif
			break;

	        case IS_LONG:
	        	php_printf("类型是 IS_LONG，值是：%ld" , Z_LVAL_P(arg));
#if PHP_MAJOR_VERSION == 7
	            RETVAL_STRING("integer");
#else
	            RETVAL_STRING("integer", 1);
#endif
			break;

	        case IS_DOUBLE:
	        	php_printf("类型是 IS_DOUBLE，值是：%f" , Z_DVAL_P(arg));
#if PHP_MAJOR_VERSION == 7
	            RETVAL_STRING("double");
#else
	            RETVAL_STRING("double", 1);
#endif
			break;

	        case IS_STRING:
	        	//PHPWRITE(Z_STRVAL_P(zstr), Z_STRLEN_P(zstr));
				//这里用了PHPWRITE宏，只要知道它是从Z_STRVAL_P(zstr)地址开始，输出Z_STRLEN_P(zstr)长度的字符就可以了。
				//ZEND_SET_SYMBOL( EG(active_symbol_table) ,  "foo" , fooval);
	        	php_printf("类型是 IS_STRING，值是：%s \n" , Z_STRVAL_P(arg));//get zval
	        	//add_property_long();

#if PHP_MAJOR_VERSION == 7
	        	ZVAL_STRINGL(ret_val, "stringl", sizeof("stringl")-1);//set zval Pointer
	            RETVAL_STRING(Z_STRVAL_P(ret_val));//get zval
#else
	            ZVAL_STRINGL(ret_val, "stringl", sizeof("stringl")-1, 0);
	            RETVAL_STRING(Z_STRVAL_P(ret_val), 1);
#endif
			break;

	        case IS_ARRAY:
#if PHP_MAJOR_VERSION == 7
	            RETVAL_STRING("array");
#else
	            RETVAL_STRING("array", 1);
#endif
			break;

	        case IS_OBJECT:
#if PHP_MAJOR_VERSION == 7
	            RETVAL_STRING("object");
#else
	            RETVAL_STRING("object", 1);
#endif
			break;

	        case IS_RESOURCE:
#if PHP_MAJOR_VERSION == 7
					RETVAL_STRING("resource");
#else
					RETVAL_STRING("resource", 1);
#endif
				break;

	        default:
#if PHP_MAJOR_VERSION == 7
	                    RETVAL_STRING("unknown type");
#else
	                    RETVAL_STRING("unknown type", 1);
#endif
	    }
}


ZEND_FUNCTION(array_range_use)
{
    if (return_value_used) {
        int i;
        //把返回值初始化成一个PHP语言中的数组
        array_init(return_value);
        for(i = 0; i < 10; i++)
        {
            //向retrun_value里不断的添加新元素，值为i
            add_next_index_long(return_value, i);
        }
        return;
    } else {
        //抛出一个E_NOTICE级错误
        php_error_docref(NULL TSRMLS_CC, E_NOTICE,"猫了个咪的，我就知道你没用我的劳动成果！");
        RETURN_NULL();
    }
}


//遍历数组
static void print_var_hash(HashTable *arrht)
{
    for(zend_hash_internal_pointer_reset(arrht);
        zend_hash_has_more_elements(arrht) == SUCCESS;
        zend_hash_move_forward(arrht)) {

#if PHP_MAJOR_VERSION == 7
    	zend_string **key;
    	zend_ulong *ind;
    	zval *tempVal;
    	zend_hash_get_current_key(arrht, &key, &ind);//        key() 引用返回
    	tempVal = zend_hash_get_current_data(arrht);//       current()
    	php_printf("key Is: %s, value Is: %s \n", &key, tempVal);
#else
    	char *key;
		uint keylen;
		ulong idx;
		int type;
		zval **ppzval, tmpcopy;

		type = zend_hash_get_current_key_ex(arrht, &key, &keylen,&idx, 0, NULL);
		if (zend_hash_get_current_data(arrht, (void**)&ppzval) == FAILURE) { continue; }

		//重新copy一个zval，防止破坏原数据
		tmpcopy = **ppzval;
		zval_copy_ctor(&tmpcopy);
		INIT_PZVAL(&tmpcopy);

		convert_to_string(&tmpcopy);

		/* Output */
		php_printf("The value of ");
		if (type == HASH_KEY_IS_STRING) {
			PHPWRITE(key, keylen);
		} else {
			php_printf("%ld", idx);
		}
		php_printf(" is: ");
		PHPWRITE(Z_STRVAL(tmpcopy), Z_STRLEN(tmpcopy));
		php_printf("\n");
		/* Toss out old copy */
		zval_dtor(&tmpcopy);
#endif
    }
}


ZEND_FUNCTION(each_arr){
	zval *arrht;
	HashTable *myht;
	//array_init(arrht);
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z!", &arrht) == FAILURE) {
		php_printf("parameters error");
		RETURN_NULL();
	}
	//arrht = Z_ARRVAL_P(arrht);
	//RETURN_LONG(zend_hash_num_elements(arrht));
	/*if (!arrht){
		php_printf("parameters is null");
		RETURN_NULL();
	}*/
	//SEPARATE_ZVAL(arrht);//手动分离变量 zval_copy_ctor(ret_val);
	//convert_to_array(arrht);
//	php_var_dump(&arrht, 1 TSRMLS_CC);
	myht = Z_ARRVAL_P(arrht);
	print_var_hash(myht);
	/*
	zend_hash_apply();
	zend_hash_internal_pointer_reset();// void reset()
	zend_hash_get_current_key();//        key() 引用返回
	zend_hash_get_current_data();//       current()
	zend_hash_move_forward(); //          next()/each()
	zend_hash_move_backwards(); //        prev()
	zend_hash_internal_pointer_end();//   end()
	zend_hash_get_current_key_type();
	zend_hash_has_more_elements();
	zend_hash_sort();

	zend_hash_init();
	zend_hash_add();
	zend_hash_update();
	zend_hash_index_update();
	zend_hash_next_index_insert();

	zend_hash_find();
	zend_hash_index_find();

	zend_hash_exists();
	zend_hash_index_exists();
	zend_hash_copy();
	zend_hash_merge();

	zend_hash_del();
	zend_hash_index_del();

	zend_hash_clean();
	zend_hash_destroy();

	*/
}



ZEND_FUNCTION(create_arr)
{
    zval *subarray;
    array_init(return_value);

    /* Add some scalars */
    add_assoc_long(return_value, "life", 42);
    add_index_bool(return_value, 123, 1);
    add_next_index_double(return_value, 3.1415926535);

    /* Toss in a static string, dup'd by PHP */
    add_next_index_string(return_value, "Foo", 1);

    /* Now a manually dup'd string */
    add_next_index_string(return_value, estrdup("Bar"), 0);

    /* Create a subarray */
    MAKE_STD_ZVAL(subarray);
    array_init(subarray);

    /* Populate it with some numbers */
    add_next_index_long(subarray, 1);
    add_next_index_long(subarray, 20);
    add_next_index_long(subarray, 300);

    /* Place the subarray in the parent */
    add_index_zval(return_value, 444, subarray);
}


/* {{{ PHP_MINIT_FUNCTION
 */
int time_init_m;
PHP_MINIT_FUNCTION(demo)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	time_init_m = time(NULL);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(demo)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	FILE *fp=fopen("/tmp/time_mshutdown.txt","a+");//请确保文件可写，否则apache会莫名崩溃
	fprintf(fp,"PHP_MSHUTDOWN_FUNCTION %ld\n",time(NULL));
	fclose(fp);

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
int time_init_r;
PHP_RINIT_FUNCTION(demo)
{
#if defined(COMPILE_DL_DEMO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	time_init_r = time(NULL);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(demo)
{
	FILE *fp=fopen("/tmp/time_rshutdown.txt","a+");
	fprintf(fp,"PHP_RSHUTDOWN_FUNCTION %ld\n",time(NULL));//让我们看看是不是每次请求结束都会在这个文件里追加数据
	fclose(fp);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(demo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "demo support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */





PHP_FUNCTION(test_rm_time)
{
    php_printf("time_init_m: %d \n",time_init_m);
    php_printf("time_init_r: %d \n",time_init_r);
    return;
}

/* {{{ demo_functions[]
 *
 * Every user visible function must have an entry in demo_functions[].
 */
const zend_function_entry demo_functions[] = {
	PHP_FE(test_rm_time,	NULL)
	PHP_FE(array_range_use,	NULL)
	PHP_FE(each_arr,		NULL)
	PHP_FE(create_arr,   	NULL)
	PHP_FE(var_type_cust,	NULL)
	PHP_FE_END	/* Must be the last line in demo_functions[] */
};
/* }}} */

/* {{{ demo_module_entry
 */
zend_module_entry demo_module_entry = {
	STANDARD_MODULE_HEADER,
	"demo",
	demo_functions,
	PHP_MINIT(demo),
	PHP_MSHUTDOWN(demo),
	PHP_RINIT(demo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(demo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(demo),
	PHP_DEMO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DEMO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(demo)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
