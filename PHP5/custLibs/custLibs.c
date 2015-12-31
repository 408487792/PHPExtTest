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
#include "snprintf.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_rand.h"
#include "ext/pcre/php_pcre.h"
#include "ext/json/php_json.h"
#include "Zend/zend_alloc.h"
#include "SAPI.h"
#include "php_custLibs.h"

/* If you declare any globals in php_custLibs.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(custLibs)
*/

/* True global resources - no need for thread safety here */
//static int le_custLibs;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("custLibs.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_custLibs_globals, custLibs_globals)
    STD_PHP_INI_ENTRY("custLibs.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_custLibs_globals, custLibs_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */


zend_class_entry *custLibs_ce;
ZEND_METHOD(custLibs, __construct){
	php_printf("new __construct \n");
}
ZEND_METHOD(custLibs, public_method){//classname, name
	php_printf("public \n");
}


ZEND_METHOD(custLibs, output){
	sapi_add_header("Content-type: application/json", strlen("Content-type: application/json"), 1);
	zval *arr;
	long code;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l/z", &code, &arr) == FAILURE) {
		php_printf("{\"status\": 200, \"result\": {}}");
		return;
	}

	if (zend_hash_num_elements(Z_ARRVAL_P(arr)) == 0) {
		php_printf("{\"status\": 200, \"result\": {}}");
		return;
	}

	if(Z_TYPE_P(arr) == IS_ARRAY){
		array_init(return_value);
		add_assoc_long(return_value, "status", code);
		Z_ADDREF_P(arr);
		add_assoc_zval(return_value, "result", arr);

		zend_try {
			smart_str buf = {0};
			php_json_encode(&buf, return_value, 1 TSRMLS_CC);
			PHPWRITE(buf.c, buf.len);
			smart_str_free(&buf);
		} zend_catch {
			RETURN_FALSE;
		} zend_end_try();
	}else {
		convert_to_string(arr);
		php_printf("{\"status\": %d, \"result\": \"%s\"}", code, Z_STRVAL_P(arr));
	}
	RETURN_TRUE;

	/**
	 * 		zval  *name, *pzval;
			zend_string	*key = NULL;
			ulong	idx	 = 0;
			HashTable 	*ht;

			array_init(ret);

			ht = Z_ARRVAL(subparts);
			ZEND_HASH_FOREACH_KEY_VAL(ht, idx, key, pzval) {

				if (key) {
					Z_TRY_ADDREF_P(pzval);
					zend_hash_update(Z_ARRVAL_P(ret), key, pzval);
				} else {
					if (map && (name = zend_hash_index_find(Z_ARRVAL_P(map), idx)) != NULL && Z_TYPE_P(name) == IS_STRING) {
						Z_TRY_ADDREF_P(pzval);
						zend_hash_update(Z_ARRVAL_P(ret), Z_STR_P(name), pzval);
					}
				}
			} ZEND_HASH_FOREACH_END();

			zval_ptr_dtor(&subparts);
			return;
	 * */
}

/**
 * var_dump(custLibs::checkAgent("ssdsdsdsx", "a"));
 * var_dump(custLibs::checkAgent($_SERVER['HTTP_USER_AGENT']));
 */
ZEND_METHOD(custLibs, checkAgent){
	char * agent_str, *t;
	size_t agent_len = 0, t_len = 0;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &agent_str, &agent_len, &t, &t_len) == FAILURE) {
		php_error(E_WARNING, "%s() parameters error", get_active_function_name(TSRMLS_C));
		RETURN_FALSE;
	}

	if (agent_len == 0) {
		RETURN_FALSE;
	}

	php_strtolower(agent_str, agent_len);
	if (t_len > 0) {
		if (php_memnstr(agent_str, t, t_len, agent_str + agent_len)){
			RETURN_TRUE;
		}else{
			RETURN_FALSE;
		}
	}

	array_init(return_value);
	int in_ipad = php_memnstr(agent_str, ZEND_STRL("ipad"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_ipad", in_ipad);

	int in_windows = php_memnstr(agent_str, ZEND_STRL("windows"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_pc", in_windows);

	int in_iphone = php_memnstr(agent_str, ZEND_STRL("iphone"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_iphone", in_iphone);

	int in_android = php_memnstr(agent_str, ZEND_STRL("android"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_android", in_android);

	int in_weixin = php_memnstr(agent_str, ZEND_STRL("micromessenger"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_weixin", in_weixin);

	int in_chrome = php_memnstr(agent_str, ZEND_STRL("chrome"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_chrome", in_chrome);

	int in_firefox = php_memnstr(agent_str, ZEND_STRL("firefox"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_firefox", in_firefox);

	int in_webkit = php_memnstr(agent_str, ZEND_STRL("applewebkit"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_webkit", in_webkit);

	int in_linux = php_memnstr(agent_str, ZEND_STRL("linux"), agent_str + agent_len) ? 1 : 0;
	add_assoc_bool(return_value, "in_linux", in_linux);
}


/**
 * var_dump(custLibs::regularCheck("408487792@qq.com", '/^4\w+@\w+\.\w+m$/i'));
 * var_dump(custLibs::regularCheck("408487792@qq.com"));
 */
ZEND_METHOD(custLibs, regularCheck){
	char *regex = "/^([0-9a-zA-Z]+[_.0-9a-zA-Z-]*)@[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)*$/i";
	int regex_len = strlen(regex);
	char *subject;
	int subject_len;
	pcre_cache_entry *pce;
	zval *subpats = NULL;
	long flags = 0;
	long start_offset = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &subject, &subject_len, &regex, &regex_len) == FAILURE) {
		RETURN_FALSE;
	}

	/* Compile regex or get it from cache. */
	if ((pce = pcre_get_compiled_regex_cache(regex, regex_len TSRMLS_CC)) == NULL) {
		RETURN_FALSE;
	}

	array_init(return_value);
	php_pcre_match_impl(pce, subject, subject_len, return_value, subpats, 0, 0, flags, start_offset TSRMLS_CC);

	if(Z_LVAL_P(return_value) == 1){
		RETURN_TRUE
	}
	RETURN_FALSE;
}


/**
 * var_dump(custLibs::randCode("pre", 3));
 * var_dump(custLibs::randCode("pre", 5));
 */
ZEND_METHOD(custLibs, randCode){
	const char * lower = "abcdefghijklmnopqrstuvwxyz";
	char * uper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char * num = "0123456789";
	char * current;
	long size = 10, ignore = 1, incNum = 1, i, maxSize, number, pre_str_len = 0;
	char * pre_str = "";

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|slll", &pre_str, &pre_str_len, &size, &ignore, &incNum) == FAILURE) {
		RETURN_FALSE;
	}

	if (size < 1 || size > 100) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Expected at size error.");
		RETURN_FALSE;
	}

	if (size <= pre_str_len) {
		RETURN_STRINGL(pre_str, pre_str_len, 1);
	}

	if (ignore == 0){
		if (incNum == 1){
			spprintf(&current, 0, "%s%s%s", lower, uper, num);
		}else {
			spprintf(&current, 0, "%s%s", lower, uper);
		}
	}else{
		if (incNum == 1){
			spprintf(&current, 0, "%s%s", lower, num);
		}else {
			spprintf(&current, 0, "%s", lower);
		}
	}

	char * result = emalloc(size);//char * result[size] 两种字符数组申明 不能混用
	maxSize = strlen(current)-1;
	strcpy(result, pre_str);
	for (i = pre_str_len; i < size; i++) {
		number = php_rand(TSRMLS_C);
		RAND_RANGE(number, 0, maxSize, PHP_MT_RAND_MAX);
		strncpy(result+i, current+number, 1);//a+i为指针的地址！a[i]相当于*（a+i），是一个字符！！
		//memcpy(result+i, current+number, 1);
		//php_printf("rand number is %d / %d    i is => %d  result+i => %s   current+number => %s \n", maxSize, number, i, result, current+number);
	}
	RETVAL_STRINGL(result, size, 1);
	efree(result);
}


/**
 * var_dump(custLibs::hexHash(3));
 */
ZEND_METHOD(custLibs, hexHash){
	long num;
	char buf[3];
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
		RETURN_FALSE;
	}

	if (num < 0) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Expected at num error.");
		RETURN_FALSE;
	}
	num %= 256;
	sprintf(buf, "%02x", num);
	RETURN_STRINGL(buf, 2, 1);
}


/**
 * var_dump(custLibs::testLog(["sdads"=>[1,1]]));
 */
ZEND_METHOD(custLibs, testLog){
	zval *var, *content;
	char * path = "test.log", * mode = "a+", date_str[23];
	long path_len = 8, mode_len = 3, writeLen;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|ss", &var, &path, &path_len, &mode, &mode_len) == FAILURE) {
		RETURN_FALSE;
	}

	FILE *fp=fopen(path, mode);
	if (fp == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "open file failure");
		RETURN_FALSE;
	}

	time_t ts = time(NULL);//%F 等同于 "%Y-%m-%d"
	strftime(date_str, sizeof(date_str), "%F %H:%M:%S %w", localtime(&ts));
	fputs(date_str, fp);
	fprintf(fp," %d", ts);
	fputs(PHP_EOL, fp);
	/*date_str = php_format_date("Y-m-d H:i:s w", strlen("Y-m-d H:i:s w"), time(NULL), 1 TSRMLS_CC);
	fputs(date_str, fp);
	fprintf(fp," %d",time(NULL));
	fputs(PHP_EOL, fp);*/

	php_output_start_default(TSRMLS_C);
	zend_print_zval_r(var, 0 TSRMLS_CC);
	php_output_get_contents(return_value TSRMLS_CC);
	php_output_discard(TSRMLS_C);

	writeLen = fputs(Z_STRVAL_P(return_value), fp);
	if (writeLen == EOF) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "write data failure");
		RETURN_FALSE;
	}
	fputs(PHP_EOL, fp);
	fclose(fp);
}


/**
 *  public static function getCurrentDate($format = 'Y-m-d H:i:s', $time = 0){
		if ($time == 0) $time = empty($_SERVER['REQUEST_TIME']) ? time() : $_SERVER['REQUEST_TIME'];
		return date($format, $time);
	}
	var_dump(custLibs::getFDate("w"));
 */
ZEND_METHOD(custLibs, getFDate){
	char * format = "Y-m-d H:i:s w", * string;
	long ts = time(NULL), format_len = strlen(format);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sl", &format, &format_len, &ts) == FAILURE) {
		RETURN_FALSE;
	}

	string = php_format_date(format, format_len, ts, 1 TSRMLS_CC);
 	RETURN_STRINGL(string, strlen(string), 0);
}

/**
 * public static function checkToken($data = []) {
    if ($data) {
        switch (strtolower(API_V)) {
            case 'v1':
                $data['salt'] = 'V1';
            break;
            case 'v2':
                $data['salt'] = 'V2';
            break;
        }
        if (isset($data['_v'])) unset($data['_v']);
        if (isset($data['_method'])) unset($data['_method']);
        if (empty($data['token'])) {
            $token = 'z';
        }else {
            $token = $data['token'];
            unset($data['token']);
        }
        ksort($data);
        $str = '';
        foreach ($data as $key => $value)
            $str .= $key.'='.$value;
        if ($token == md5($str)) return true;
        else {
            if (__ENV__ != 'ONLINE') self::output(900, ['originData'=>$str, 'originToken'=>$token, 'md5Token'=>md5($str),'msg' => 'token错误']);
            return false;
        }
    }else return false;
} */
ZEND_METHOD(custLibs, checkToken){
	zval **apiVer, **dataToken;
	char *token;
	HashTable * hdata;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "H", &hdata) == FAILURE) {
		RETURN_FALSE;
	}
	if (zend_hash_num_elements(hdata) < 1) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "array note null");
		RETURN_FALSE;
	}

	if (zend_hash_find(hdata, "_v", 2, (void **) &apiVer) == FAILURE){
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "note find _v param");
		RETURN_FALSE;
	}
	array_init(return_value);
	//myht = Z_ARRVAL_P(input);
	Z_ARRVAL_P(return_value) = hdata;
	return;
	switch (Z_LVAL_PP(apiVer)) {
		case 1:
			add_assoc_stringl(hdata, "salt", "V1", 2, 0);
		break;
		case 2:
			add_assoc_stringl(hdata, "salt", "V2", 2, 0);
		break;
		default:
			add_assoc_stringl(hdata, "salt", "V0", 2, 0);
		break;
	}
	if (zend_hash_del(hdata, "_v", 2) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "deltete _v failure");
		RETURN_FALSE;
	}
	zend_hash_del(hdata, "_method", 7);
	if (zend_hash_find(hdata, "token", 5, (void **) &dataToken) == FAILURE){
		token = "z";
	}else{
		*token = Z_STRVAL_PP(dataToken);
		if (zend_hash_del(hdata, "token", 5) == FAILURE) {
			RETURN_FALSE;
		}
	}

	RETURN_ZVAL(hdata, 1, 0);
}


	/** 密码复杂度检测扩展 **/
ZEND_METHOD(custLibs, checkPwdStrong){

}


ZEND_METHOD(custLibs, checkData){

	/**
	 * public static function checkData($data = [], $isforce = true) {
        if (is_array($data)){
            foreach ($data as $value) {
                self::checkData($value, $isforce);
            }
        }else {
            if ($isforce) {
                if (empty($data) && strlen($data) == 0) return false;
            }else{
                if (empty($data)) return false;
            }
        }
        return true;
    }
	 */
}


/**
 * public static function convertSqlIN($data, $key) {
        if (empty($data)) return '';
        $data = is_array($data) ? $data : explode(',', $data);
        $num = count($data);
        if ($num == 1) $item["$key"] = $data[0];
        else if ($num > 1) $item["$key IN"] = $data;
        return $item;
    }
 */
ZEND_METHOD(custLibs, convertSqlIN){
	zval * data, * result_z, ** ppzval, tmpcopy;
	char * key, * result_key_s, * datastr, * result = NULL;
	long key_len, cnt;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z!s", &data, &key, &key_len) == FAILURE) {
		/**
		 * 1.在方法定义时加上TSRMLS_D（如果方法没有参数用这个）或者TSRMLS_DC（有1个以上的参数）
		 * 2.在方法调用时用TSRMLS_C（如果方法没有参数用这个）或者TSRMLS_CC（有1个以上的参数）
		 * D Define
		 * C Call
		 */
		RETURN_FALSE;
	}
	if (!data) {
		RETURN_EMPTY_STRING();
	}

	ALLOC_INIT_ZVAL(result_z);
	array_init(result_z);
	if (Z_TYPE_P(data) != IS_ARRAY) {
		if (Z_TYPE_P(data) != IS_STRING) {
			convert_to_string(data);
		}
		php_printf("pre data => %s \n", Z_STRVAL_P(data));
		datastr = Z_STRVAL_P(data);
		result = strtok(datastr, ",");
		if (result == NULL) {
			add_next_index_string(result_z, Z_STRVAL_P(data), 1);
		}else {
			add_next_index_string(result_z, result, 1);
			while(result != NULL) {
				result = strtok(NULL, ",");
				if (result != NULL) {
					php_printf("result => %s data => %s, datastr => %s \n", result, Z_STRVAL_P(data), datastr);
					RETURN_FALSE;
					add_next_index_string(result_z, result, 1);
				}else {
					php_printf("result => %s  \n", result);
					RETURN_FALSE;
				}
			}
		}
	}else {
		ZVAL_ZVAL(result_z, data, 1, 0);
	}

	array_init(return_value);
	cnt = zend_hash_num_elements(Z_ARRVAL_P(result_z));
	if (cnt > 1){
		result_key_s = emalloc(strlen(key)+4);
		strcpy(result_key_s, key);
		strcat(result_key_s, " IN");

		add_assoc_zval(return_value, result_key_s, result_z);
		efree(result_key_s);
	}else {
		if (zend_hash_get_current_data(Z_ARRVAL_P(result_z), (void**)&ppzval) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "get value failure");
			RETURN_FALSE;
		}
		tmpcopy = **ppzval;
		zval_copy_ctor(&tmpcopy);
		INIT_PZVAL(&tmpcopy);
		convert_to_string(&tmpcopy);
		add_assoc_stringl(return_value, key, Z_STRVAL(tmpcopy), Z_STRLEN(tmpcopy), 0);
	}
}


ZEND_METHOD(custLibs, timeTest){
	time_t calendar_time = time(NULL);
	php_printf("calendar_time :%ld \n", calendar_time);

	char * calendar_str = ctime(&calendar_time);
	php_printf("calendar_str  :%s \n", calendar_str);

	struct tm * tm_local = localtime(&calendar_time);
	php_printf("localtime :year=%d mon=%d mday=%d hour=%d min=%d sec=%d   wday=%d yday=%d isdst=%d \n",
		   tm_local->tm_year + 1900, tm_local->tm_mon + 1, tm_local->tm_mday, tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec,
		   tm_local->tm_wday, tm_local->tm_yday, tm_local->tm_isdst);

	char * asc_time = asctime(tm_local);
	php_printf("asc_time  :%s", asc_time);
	char * c_time = ctime(&calendar_time);
	php_printf("c_time    :%s", c_time);

	time_t mk_time = mktime(tm_local);
	php_printf("\nmk_time   :%ld \n\n", mk_time);

	char str_f_t [100];
	strftime(str_f_t, sizeof(str_f_t), "%G-%m-%d %H:%M:%S", tm_local);
	php_printf("local  :format=%s \n", str_f_t);
	struct tm * gm_time = gmtime(&calendar_time);
	strftime(str_f_t, sizeof(str_f_t), "%G-%m-%d %H:%M:%S", gm_time);
	php_printf("gmtime :format=%s \n\n", str_f_t);

	struct tm my_tm = {2,2,2,16,2,2015,0,0,0};
	time_t my_time_t = mktime(&my_tm);
	php_printf("my yday:%d \n", my_tm.tm_yday);
}



/**
 * public static function createOrderUniqId($ShopId = '') {
       return $ShopId ? date('YmdHisw').sprintf('%07d',$ShopId).mt_rand(10000, 99999) : date('YmdHis').mt_rand(100000, 999999).mt_rand(10000, 99999);
   }
   var_dump(custLibs::getOrderUniqId(100254));
 */
ZEND_METHOD(custLibs, getOrderUniqId){
	long shop_id = 0, number;
	char date_str[16], *rand_str, shop_id_str[8], *result;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &shop_id) == FAILURE) {
		RETURN_FALSE;
	}

	time_t ts = time(NULL);
	strftime(date_str, sizeof(date_str), "%Y%m%d%H%M%S%w", localtime(&ts));
	if (shop_id > 0){
		sprintf(shop_id_str, "%07d" , shop_id);
	}
	number = php_rand(TSRMLS_C);
	RAND_RANGE(number, 10000, 99999, PHP_MT_RAND_MAX);
	//sprintf(rand_str, "%d" , number);
	spprintf(&rand_str, 0, "%d", number);

	result = emalloc(strlen(date_str)+strlen(shop_id_str)+strlen(rand_str)+1);
	strcpy(result, date_str);
	if (shop_id > 0){
		strcpy(result+strlen(date_str), shop_id_str);
		strcpy(result+strlen(date_str)+strlen(shop_id_str), rand_str);
	}else{
		//result = emalloc(strlen(date_str)+strlen(rand_str)+1);
		//strcpy(result, date_str);
		strcpy(result+strlen(date_str), rand_str);
	}
	RETVAL_STRINGL(result, strlen(result), 0);
	//efree(result);
}

/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_custLibs_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_custLibs_init_globals(zend_custLibs_globals *custLibs_globals)
{
	custLibs_globals->global_value = 0;
	custLibs_globals->global_string = NULL;
}
*/
/* }}} */


/* {{{ custLibs_functions[]
 *
 * Every user visible function must have an entry in custLibs_functions[].
 */
const zend_function_entry custLibs_methods[] = {
	ZEND_ME(custLibs,    __construct,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	ZEND_ME(custLibs,    public_method,  NULL,   ZEND_ACC_PUBLIC)
	ZEND_ME(custLibs,    output,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    checkAgent,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    regularCheck,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    randCode,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    hexHash,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    testLog,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    getFDate,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    checkToken,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    checkData,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    convertSqlIN,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    timeTest,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(custLibs,    getOrderUniqId,    NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
/**
 * ZEND_ACC_STATIC
 * ZEND_ACC_PROTECTED
 * ZEND_ACC_PRIVATE
 * ZEND_ACC_ABSTRACT
 * ZEND_ACC_FINAL
 *
 * ZEND_ACC_FINAL_CLASS
 * ZEND_ACC_DTOR
 * ZEND_ACC_CLONE
 */
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(custLibs)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;//class 容器
	INIT_CLASS_ENTRY(ce,"custLibs",custLibs_methods);// class容器 class name  class mothod
	custLibs_ce = zend_register_internal_class(&ce TSRMLS_CC);
	//定义属性
	zend_declare_class_constant_stringl(custLibs_ce, "author", strlen("author"), "fangbo", strlen("fangbo") TSRMLS_CC);
	zend_declare_class_constant_double(custLibs_ce, "version", strlen("version"), 0.1 TSRMLS_DC);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(custLibs)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(custLibs)
{
#if defined(COMPILE_DL_CUSTLIBS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(custLibs)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(custLibs)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "custLibs support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* {{{ custLibs_module_entry
 */
zend_module_entry custLibs_module_entry = {
	STANDARD_MODULE_HEADER,
	"custLibs",
	custLibs_methods,
	PHP_MINIT(custLibs),
	PHP_MSHUTDOWN(custLibs),
	PHP_RINIT(custLibs),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(custLibs),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(custLibs),
	PHP_CUSTLIBS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CUSTLIBS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(custLibs)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
