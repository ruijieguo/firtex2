<?php

require_once __DIR__.'/lib/Thrift/ClassLoader/ThriftClassLoader.php';

use Thrift\ClassLoader\ThriftClassLoader;

$GLOBALS['THRIFT_ROOT'] = dirname(__FILE__) . '/lib';

$loader = new ThriftClassLoader();
$loader->registerNamespace('Thrift', $GLOBALS['THRIFT_ROOT']);
$loader->registerDefinition('shared', $GEN_DIR);
$loader->register();

require_once dirname(__FILE__).'/packages/firtex/ThriftSearchService.php';
require_once dirname(__FILE__).'/packages/firtex/Types.php';
require_once dirname(__FILE__).'/packages/firtex/FirteXQueryResult.php';

use Thrift\Protocol\TBinaryProtocol;
use Thrift\Transport\TSocket;
use Thrift\Transport\TFramedTransport;
use firtex\ThriftSearchServiceClient;

class FirteXRPCClient {
    public function __construct($host, $port) {
        try {
            $socket = new TSocket($host, $port);
            $transport = new TFramedTransport($socket);
            $protocol = new TBinaryProtocol($transport);
            
            $transport->open();
            $this->_searchService = new ThriftSearchServiceClient($protocol);
        } catch(TException $tx) {
            throw FirteXException($tx->getMessage());
        }
    }

    public function syntaxSearch($uri) {
        $result = $this->_searchService->syntaxSearch($uri);
        return new FirteXQueryResult($result);
    }

    public function search($cmd) {
        $result = $this->_searchService->search($cmd);
        return new FirteXQueryResult($result);
    }

    private $_searchService = NULL;
}

?>