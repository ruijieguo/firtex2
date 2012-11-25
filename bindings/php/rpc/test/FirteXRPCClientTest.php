<?php
include_once dirname(__FILE__).'/../FirteXRPCClient.php';

echo '===test syntaxSearch';
$client = new FirteXRPCClient('localhost', '19801');
$result = $client->syntaxSearch('query=BODY:2;field_select=BODY:snippet');
print_r($result);

echo '===test search';
$client = new FirteXRPCClient('localhost', '19801');
$vals = array('query' => 'BODY:2',
              'fieldSelect' => array(0 => 'BODY:snippet'));
$cmd = new firtex_QueryCommand($vals);
$result = $client->search($cmd);
print_r($result);
?>