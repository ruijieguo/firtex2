<?php
include_once dirname(__FILE__).'/../FirteXHTTPClient.php';

$client = new FirteXHTTPClient('http://localhost', '19801');
$client->setFields('BODY:snippet');
$result = $client->search('BODY:2');
print_r($result);
?>