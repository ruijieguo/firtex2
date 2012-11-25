<?php

/**
 * Represents the required pieces of an HTTP response provided by HTTP transport
 * implementations
 */
class FirteXHttpResponse {
	/**
	 * Map status code to status messages
	 */
	static private $STATUS_TO_MESSAGE = array(
		0 => "Communication Error",
		
		// Informational 1XX
		100 => "Continue",
		101 => "Switching Protocols",
		
		// Successful 2XX
		200 => "OK",
		201 => "Created",
		202 => "Accepted",
		203 => "Non-Authoritative Information",
		204 => "No Content",
		205 => "Reset Content",
		206 => "Partial Content",
		
		// Redirection 3XX
		300 => "Multiple Choices",
		301 => "Moved Permanently",
		302 => "Found",
		303 => "See Other",
		304 => "Not Modified",
		305 => "Use Proxy",
		307 => "Temporary Redirect",
		
		// Client Error 4XX
		400 => "Bad Request",
		401 => "Unauthorized",
		402 => "Payment Required",
		403 => "Forbidden",
		404 => "Not Found",
		405 => "Method Not Allowed",
		406 => "Not Acceptable",
		407 => "Proxy Authentication Required",
		408 => "Request Timeout",
		409 => "Conflict",
		410 => "Gone",
		411 => "Length Required",
		412 => "Precondition Failed",
		413 => "Request Entity Too Large",
		414 => "Request-URI Too Long",
		415 => "Unsupported Media Type",
		416 => "Request Range Not Satisfiable",
		417 => "Expectation Failed",
		
		// Server Error 5XX
		500 => "Internal Server Error",
		501 => "Not Implemented",
		502 => "Bad Gateway",
		503 => "Service Unavailable",
		504 => "Gateway Timeout",
		505 => "HTTP Version Not Supported"
	);
	
	/**
	 * The response's HTTP status code
	 *
	 * @var integer
	 */
	private $_statusCode;
	
	/**
	 * The response's HTTP status message
	 *
	 * @var string
	 */
	private $_statusMessage;
	
	/**
	 * The response's mime type
	 *
	 * @var string
	 */
	private $_mimeType;
	
	/**
	 * The response's character encoding
	 *
	 * @var string
	 */
	private $_encoding;
	
	/**
	 * The response's data
	 *
	 * @var string
	 */
	private $_responseBody;

	/**
	 * Get the HTTP status message based on status code
	 *
	 * @return string
	 */
	public static function getStatusMessageByCode($statusCode) {
		$statusCode = (int) $statusCode;
		
		if (isset(self::$STATUS_TO_MESSAGE[$statusCode])) {
			return self::$STATUS_TO_MESSAGE[$statusCode];
		}
		
		return "Unknown Status";
	}
	
	/**
	 * Construct a HTTP response
	 * 
	 * @param integer $statusCode The HTTP status code
	 * @param string $contentType The VALUE of the Content-Type HTTP header
	 * @param string $responseBody The body of the HTTP response
	 */
	public function __construct($statusCode, $contentType, $responseBody) {
		$this->_statusCode = (int) $statusCode;
		$this->_statusMessage = self::getStatusMessageByCode($this->_statusCode);
		$this->_responseBody = (string) $responseBody;
		
		$this->_mimeType = "text/plain";
		$this->_encoding = "UTF-8";
		
		if ($contentType) {
			// now break apart the header to see if there's character encoding
			$contentTypeParts = explode(';', $contentType, 2);

			if (isset($contentTypeParts[0])) {
				$this->_mimeType = trim($contentTypeParts[0]);
			}

			if (isset($contentTypeParts[1])) {
				// we have a second part, split it further
				$contentTypeParts = explode('=', $contentTypeParts[1]);

				if (isset($contentTypeParts[1])) {
					$this->_encoding = trim($contentTypeParts[1]);
				}
			}
		}
	}
	
	/**
	 * Get the status code of the response
	 *
	 * @return integer
	 */
	public function getStatusCode() {
		return $this->_statusCode;
	}
	
	/**
	 * Get the status message of the response
	 *
	 * @return string
	 */
	public function getStatusMessage() {
		return $this->_statusMessage;
	}
	
	/**
	 * Get the mimetype of the response body
	 *
	 * @return string
	 */
	public function getMimeType() {
		return $this->_mimeType;
	}
	
	/**
	 * Get the charset encoding of the response body.
	 *
	 * @return string
	 */
	public function getEncoding() {
		return $this->_encoding;
	}
	
	/**
	 * Get the raw response body
	 *
	 * @return string
	 */
	public function getBody() {
		return $this->_responseBody;
	}

	/**
	 * Get the length of raw response body
	 *
	 * @return int value
	 */
	public function getBodyLength() {
		return strlen($this->_responseBody);
	}
}
