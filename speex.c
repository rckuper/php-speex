/* speex extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_speex.h"
#include <speex/speex.h>

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

#define PHP_SPEEX_RESOURCE_NAME "speex"
static int speex_resource_handle;

typedef struct _speex_decoder_t {
    void *state;
    SpeexBits bits;
    size_t otput_length;
    size_t cbits_length;
    size_t frame_size;
    float* otput;
    short* cbits;
} speex_decoder_t;

void static destruct_speex(zend_resource* rsrc)
{
    speex_decoder_t *ctx = (speex_decoder_t *)rsrc->ptr;
    
    php_printf("destruct_speex(%p)\n", ctx);
    if (ctx != NULL) {
        efree(ctx);
    }
}

/* {{{ void speex_test1()
 */
PHP_FUNCTION(speex_decoder_create)
{
	ZEND_PARSE_PARAMETERS_NONE();

    int tmp = 5;
    int ratio = 16000;
    int enh = 1;
    size_t otput_length = 160;
    size_t cbits_length = 300;

	ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(otput_length)
		Z_PARAM_LONG(cbits_length)
	ZEND_PARSE_PARAMETERS_END();

    php_printf("otput_length=%d,cbits_length=%d\n", otput_length, cbits_length);

    if (otput_length <= 0 || cbits_length <= 0) {
        php_printf("otput params invalidate: otput_length=%d, cbits_length=%d", otput_length, cbits_length);
        RETURN_FALSE;
    }

    speex_decoder_t* decoder = (speex_decoder_t*)emalloc(sizeof(speex_decoder_t) + 
            sizeof(float) * otput_length + cbits_length * 2);
    if (!decoder) {
        RETURN_FALSE;
    }

    decoder->otput = (float*)(&decoder[1]);
    decoder->cbits = (short*)(&decoder->otput[otput_length]);
    decoder->otput_length = otput_length;
    decoder->cbits_length = cbits_length;

    decoder->state = speex_decoder_init(&speex_wb_mode);
    tmp = 5;
    speex_decoder_ctl(decoder->state, SPEEX_SET_QUALITY, &tmp);
    tmp = 2;
    speex_decoder_ctl(decoder->state, SPEEX_SET_COMPLEXITY, &tmp);
    speex_decoder_ctl(decoder->state, SPEEX_SET_SAMPLING_RATE, &ratio);
    speex_decoder_ctl(decoder->state, SPEEX_SET_ENH, &enh);
    speex_decoder_ctl(decoder->state, SPEEX_GET_FRAME_SIZE, &tmp);
    decoder->frame_size = tmp;

    speex_bits_init(&decoder->bits);
    
    RETURN_RES(zend_register_resource(decoder, speex_resource_handle));
}
/* }}} */

/* {{{ string speex_decode( [ string $var ] )
 */
PHP_FUNCTION(speex_decode)
{   
    zval *z_decoder;
    speex_decoder_t* decoder = NULL;
    char *var = NULL;
    short* otput = NULL;
    size_t i, n, var_len = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(z_decoder)
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

    if (z_decoder == NULL || var == NULL) {
        php_printf("otput params invalidate, decoder=%p, var=%p\r\n", z_decoder, var);
        RETURN_FALSE;
    }

    decoder = (speex_decoder_t*)zend_fetch_resource_ex(z_decoder, PHP_SPEEX_RESOURCE_NAME, speex_resource_handle);
    if (decoder == NULL) {
        php_printf("decoder is null\n");
        RETURN_FALSE;
    }

    SpeexBits bits;
    speex_bits_init(&bits);
    speex_bits_read_from(&bits, var, var_len);
//    speex_decode(decoder->state, &bits, decoder->otput);
//    otput = decoder->cbits;
//    for (i = 0; i < decoder->frame_size; ++ i) {
//        otput[i] = (short)decoder->otput[i];
//    }

    speex_decode_int(decoder->state, &bits, decoder->cbits);

    RETURN_STRINGL((char*)decoder->cbits, decoder->frame_size * 2);
}

PHP_FUNCTION(speex_decoder_destroy)
{
    zval *z_decoder;
    speex_decoder_t* decoder = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(z_decoder)
	ZEND_PARSE_PARAMETERS_END();

    if (z_decoder == NULL) {
        php_printf("otput params invalidate, decoder=%p", z_decoder);
        RETURN_FALSE;
    }

    decoder = (speex_decoder_t*)zend_fetch_resource_ex(z_decoder, PHP_SPEEX_RESOURCE_NAME, speex_resource_handle);
    if (decoder == NULL) {
        php_printf("decoder is null\n");
        RETURN_FALSE;
    }

    speex_decoder_destroy(decoder->state);
    speex_bits_destroy(&decoder->bits);

    zend_list_close(Z_RES_P(z_decoder));
}
/* }}}*/


PHP_MINIT_FUNCTION(speex)
{
    speex_resource_handle = zend_register_list_destructors_ex(destruct_speex, NULL, PHP_SPEEX_RESOURCE_NAME, module_number);
    if (speex_resource_handle == FAILURE) {
        php_printf("speex_resource_handle failed\r\n");
    }

    return SUCCESS;
}
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(speex)
{
#if defined(ZTS) && defined(COMPILE_DL_SPEEX)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(speex)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "speex support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_speex_decoder_create, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_speex_decode, 2)
	ZEND_ARG_INFO(0, decoder)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_speex_decoder_destroy, 0)
	ZEND_ARG_INFO(0, decoder)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ speex_functions[]
 */
static const zend_function_entry speex_functions[] = {
	PHP_FE(speex_decoder_create,	arginfo_speex_decoder_create)
    PHP_FE(speex_decode,	     	arginfo_speex_decode)
	PHP_FE(speex_decoder_destroy,		arginfo_speex_decoder_destroy)
	PHP_FE_END
};
/* }}} */

/* {{{ speex_module_entry
 */
zend_module_entry speex_module_entry = {
	STANDARD_MODULE_HEADER,
	"speex",					/* Extension name */
	speex_functions,			/* zend_function_entry */
	PHP_MINIT(speex),			/* PHP_MINIT - Module initialization */
	NULL,						/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(speex),			/* PHP_RINIT - Request initialization */
	NULL,						/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(speex),			/* PHP_MINFO - Module info */
	PHP_SPEEX_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SPEEX
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(speex)
#endif

