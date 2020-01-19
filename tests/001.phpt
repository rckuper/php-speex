--TEST--
Check if speex is loaded
--SKIPIF--
<?php
if (!extension_loaded('speex')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "speex" is available';
?>
--EXPECT--
The extension "speex" is available
