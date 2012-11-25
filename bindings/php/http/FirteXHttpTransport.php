<?php

require_once(dirname(__FILE__) . '/FirteXHttpResponse.php');

/**
 * Interface that all Transport (HTTP Requester) implementations must implement. 
 */
abstract class FirteXHttpTransport
{
	/**
	 * HTTP Methods
	 */
    const METHOD_GET = 'GET';
    const METHOD_POST = 'POST';

	/**
	 * Default timeout value for requests
	 *
	 * @var float
	 */
	private $_timeout = false;

    /**
     * HTTP method GET/POST
     */
    private $_method = null;

    /**
     * Constructor
     * @param int $timeout timeout for http requests
     * @param string $method http request method
     */
    public function __construct($timeout, $method = self::METHOD_GET) {
        $this->_timeout = $timeout;
        $this->_method = $method;
    }

	/**
	 * Return the current timeout for all HTTP requests
	 *
	 * @return int
	 */
	public function getTimeout() {
		if ($this->_timeout === false) {
			$this->_timeout = (int) ini_get('default_socket_timeout');

			// double check we didn't get 0 for a timeout
			if ($this->_timeout <= 0) {
				$this->_timeout = 60;
			}
		}
		
		return $this->_timeout;
    }
	
	/**
	 * Set the current timeout for all HTTP requests
	 *
	 * @param int $timeout
	 */
	public function setTimeout($timeout) {
		$timeout = (float) $timeout;
		
		if ($timeout >= 0) {
			$this->_timeout = $timeout;
		}
    }

    /**
     * Return method type (GET/POST)
     */
    public function getMethod() {
        return $this->_method;
    }
		
	/**
	 * Send HTTP request and receive response
	 *
	 * @param string $url 
     * @param string $data data to send, ignored if HTTP method is GET
	 * @return FirteXHttpResponse HTTP response
	 */
	public function sendAndReceive($url, $data) {
        if ($this->_method == self::METHOD_GET) {
            return $this->doGetRequest($url . $data);
        } else {
            return $this->doPostRequest($url, $data);
        }
    }

    protected abstract function doGetRequest($url);
    protected abstract function doPostRequest($url, $data);
}