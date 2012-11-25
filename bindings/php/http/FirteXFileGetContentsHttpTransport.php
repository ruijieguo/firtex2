<?php

require_once(dirname(__FILE__) . '/FirteXHttpTransport.php');

/**
 * HTTP Transport implemenation that uses file_get_contents
 */
class FirteXFileGetContentsHttpTransport extends FirteXHttpTransport {
	private $_getContext, $_postContext;
    private $_postOptions;
	
	public function __construct($timeout, $method = self::METHOD_GET) {
        parent::__construct($timeout, $method);
        $getOptions = array('http' => array('method' => 'GET',
                                            'timeout' => $this->getTimeout()));
		$this->_getContext = stream_context_create($getOptions);

		$this->_postContext = stream_context_create();
        $this->_postOptions = array('method' => 'POST',
                                    'timeout' => $this->getTimeout());
	}

	public function doGetRequest($url) {
		$http_response_header = null;
		$responseBody = @file_get_contents($url, false, $this->_getContext);
		return $this->createResponse($responseBody, $http_response_header);
	}

	public function doPostRequest($url, $data) {
        $postData = '';
        $headers = null;
        if (is_array($data)) {
            $postData = $data['post-fields'];
            $headers = isset($data['headers']) ? $data['headers'] : null;
        } else {
            $postData = $data;
        }
        if ($headers != null) {
            $this->_postOptions['header'] = $headers;
        }
        
        $this->_postOptions['content'] = $postData;
        print_r($this->_postOptions);
		stream_context_set_option($this->_postContext, array('http' => $this->_postOptions));

		$http_response_header = null;
        print_r($this->_postContext);
		$responseBody = @file_get_contents($url, false, $this->_postContext);
        print_r("===".$url . ":".$responseBody."===");
		
		// reset content of post context to reclaim memory
		stream_context_set_option($this->_postContext, 'http', 'content', '');
		
		return $this->createResponse($responseBody, $http_response_header);
	}
	
	private function createResponse($rawResponse, $httpHeaders) {
		$status = 0;
		$contentType = false;

		if (is_array($httpHeaders) && count($httpHeaders) > 0) {
			while (isset($httpHeaders[0]) && substr($httpHeaders[0], 0, 4) == 'HTTP') {
				// we can do a intval on status line without the "HTTP/1.X " to get the code
				$status = intval(substr($httpHeaders[0], 9));

				// remove this from the headers so we can check for more
				array_shift($httpHeaders);
			}

			//Look for the Content-Type response header and determine type
			//and encoding from it (if possible - such as 'Content-Type: text/plain; charset=UTF-8')
			foreach ($httpHeaders as $header) {
				// look for the header that starts appropriately
				if (strncasecmp($header, 'Content-Type:', 13) == 0) {
					$contentType = substr($header, 13);
					break;
				}
			}
		}
		
		return new FirteXHttpResponse($status, $contentType, $rawResponse);
	}
}