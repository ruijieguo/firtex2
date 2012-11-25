<?php
include_once dirname(__FILE__).'/../FirteXQueryResult.php';

$GLOBALS['THRIFT_ROOT'] = dirname(__FILE__);
include_once $GLOBALS['THRIFT_ROOT'].'/Thrift.php';
require_once $GLOBALS['THRIFT_ROOT'].'/protocol/TBinaryProtocol.php';
require_once $GLOBALS['THRIFT_ROOT'].'/transport/TSocket.php';
require_once $GLOBALS['THRIFT_ROOT'].'/transport/TFramedTransport.php';
require_once $GLOBALS['THRIFT_ROOT'].'/packages/SearchService/ThriftSearchService.php';

class FirteXRPCClient {
    public function __construct($host, $port) {
        try {
            $socket = new TSocket($host, $port);
            $transport = new TFramedTransport($socket);
            $protocol = new TBinaryProtocol($transport);
            
            $transport->open();
            $this->_searchService = new firtex_ThriftSearchServiceClient($protocol);
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