# php-speex
speex php7 extention

not support encode now

#decoder

$decoder = speex_decoder_create();

$r = speex_decode($decoder, $buf);

speex_decoder_destroy($decoder);
