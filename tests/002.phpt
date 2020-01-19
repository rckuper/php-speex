--TEST--
speex_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('speex')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = speex_test1();

var_dump($ret);
?>
--EXPECT--
The extension speex is loaded and working!
NULL
