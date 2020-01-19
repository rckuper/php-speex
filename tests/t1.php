<?php

$name = 'test.speex';
$output = 'out.pcm';

$decoder = speex_decoder_create();

$fp = fopen($name, 'rb');

$res = '';
while (!feof($fp)) {
    $buf = fread($fp, 4);
    if (!$buf) break;
    $len = unpack('V', $buf)[1];
    echo "length=".$len."\n";
    $buf = fread($fp, $len);
    if (strlen($buf) != $len) {
        echo "length not match\n";
    }

    $r = speex_decode($decoder, $buf);

    echo "[DECODE] result-len=".strlen($r)."\n";

    if ($r) $res.=$r;
}

fclose($fp);
speex_decoder_destroy($decoder);

$fp = fopen($output, 'wb');
fwrite($fp, $res);
fclose($fp);
