<?php

require_once(dirname(__FILE__) . '/FirteXHttpTransport.php');

/**
 * A Curl based HTTP transport. 
 * All requests shares a single curl session.
 */
class FirteXCurlHttpTransport extends FirteXHttpTransport
{
	private $_curl;

	public function __construct($timeout, $method = self::METHOD_GET) {
        parent::__construct($timeout, $method);
		$this->_curl = curl_init();

		curl_setopt_array($this->_curl, array(
			// return the response body from curl_exec
			CURLOPT_RETURNTRANSFER => true,
			// get the output as binary data
			CURLOPT_BINARYTRANSFER => true,
			// No headers in the output
			CURLOPT_HEADER => false
		));
	}

	function __destruct() {
		curl_close($this->_curl);
	}

	protected function doGetRequest($url) {
		curl_setopt_array($this->_curl, array(
			// return the body
			CURLOPT_NOBODY => false,
			// GET request
			CURLOPT_HTTPGET => true,
			// set URL
			CURLOPT_URL => $url,
			// set timeout
			CURLOPT_TIMEOUT => $this->getTimeout()
		));

		$responseBody = curl_exec($this->_curl);
		$statusCode = curl_getinfo($this->_curl, CURLINFO_HTTP_CODE);
		$contentType = curl_getinfo($this->_curl, CURLINFO_CONTENT_TYPE);

		return new FirteXHttpResponse($statusCode, $contentType, $responseBody);
	}

	protected function doPostRequest($url, $data)
	{
        $postData = '';
        $headers = null;
        if (is_array($data)) {
            $postData = $data['post-fields'];
            $headers = isset($data['headers']) ? $data['headers'] : null;
        } else {
            $postData = $data;
        }

		curl_setopt_array($this->_curl, array(
			// return the body
			CURLOPT_NOBODY => false,
			// POST request
			CURLOPT_POST => true,
			// set URL
			CURLOPT_URL => $url,
			// set post data
			CURLOPT_POSTFIELDS => $postData,
			// set content type
			CURLOPT_HTTPHEADER => $headers,
			// set timeout
			CURLOPT_TIMEOUT => $this->getTimeout()
		));

		$responseBody = curl_exec($this->_curl);
		$statusCode = curl_getinfo($this->_curl, CURLINFO_HTTP_CODE);
		$contentType = curl_getinfo($this->_curl, CURLINFO_CONTENT_TYPE);
        return new FirteXHttpResponse($statusCode, $contentType, $responseBody);
    }
}