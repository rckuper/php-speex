/* speex extension for PHP */

#ifndef PHP_SPEEX_H
# define PHP_SPEEX_H

extern zend_module_entry speex_module_entry;
# define phpext_speex_ptr &speex_module_entry

# define PHP_SPEEX_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_SPEEX)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_SPEEX_H */

