<?php
require_once(dirname(__FILE__) . '/../FirteXException.php');
require_once(dirname(__FILE__) . '/FirteXHttpResponse.php');
require_once(dirname(__FILE__) . '/FirteXHttpTransport.php');
require_once(dirname(__FILE__) . '/../FirteXQueryResult.php');


class FirteXHTTPClient {
    
    /**
     * string
     * Server host name or IP address
     */
	protected $_host;
    
    /**
     * int
     * Server port
     */
    protected $_port;
    
    /**
     * string
     * Request URI, constructed from $_host and $_port
     */
    protected $_requestURI;

    /**
     * int
     * Timeout for http request
     */
    protected $_timeout;

	/**
	 * HTTP Transport implementation
	 */
	protected $_httpTransport = false;

    /**
     * Is square brackets are escaped
     */
    protected $_isSquareBracketsEscaped = true;

    /**
     * The starting offset for result documents
     */
    protected $_offset = 0;

    /*
	 * The maximum number of result documents to return
     */
    protected $_limit = 10;

    /**
     * The document fields to return
     */
    protected $_fieldsSelect = NULL;

    /**
     * Whats encode of the request query
     */
    protected $_encode = NULL;

    /**
     * The cluster who receive the request query
     */
    protected $_cluster = NULL;

    /**
     * The trace level for the request query
     */
    protected $_traceLevel = NULL;

    // Default timeout for http request (15 seconds)
    const DEFAULT_TIMEOUT = 15;

	/**
	 * Constructor.
	 *
	 * @param string $host
	 * @param string $port
	 * @param FirteXHttpTransport $httpTransport
	 */
	public function __construct($host = 'localhost', $port = 8180, $httpTransport = false)
	{
        $this->_timeout = self::DEFAULT_TIMEOUT;
		$this->setHost($host);
		$this->setPort($port);
        $this->_requestURI = $host . ':' . strval($port) . '/';

		if ($httpTransport)
		{
			$this->setHttpTransport($httpTransport);
		}
        // Square brackets are escaped in 5.1.3
		$this->_isSquareBracketsEscaped = version_compare(phpversion(), '5.1.3', '>=');
	}

	/**
	 * Returns the set host
	 *
	 * @return string
	 */
	public function getHost()
	{
		return $this->_host;
	}

	/**
	 * Set the server host 
	 *
	 * @param string $host
	 *
	 * @throws FirteXInvalidArgumentException if the host parameter is empty
	 */
	public function setHost($host)
	{
		if (empty($host))
		{
			throw new FirteXInvalidArgumentException('Host parameter is empty');
		}
		else
		{
			$this->_host = $host;
		}
	}

	/**
	 * Get the port
	 *
	 * @return integer
	 */
	public function getPort()
	{
		return $this->_port;
	}

	/**
	 * Set the port. 
	 *
	 * @param integer $port
	 *
	 * @throws FirteXInvalidArgumentException if the port parameter is empty
	 */
	public function setPort($port)
	{
		$port = (int) $port;

		if ($port <= 0)
		{
			throw new FirteXInvalidArgumentException('Port is not a valid port number');
		}
		else
		{
			$this->_port = $port;
		}
	}

	/**
	 * Get the current configured HTTP Transport
	 *
	 * @return FirteXHttpTransport
	 */
	public function getHttpTransport()
	{
		if ($this->_httpTransport === false)
		{
			require_once(dirname(__FILE__) . '/FirteXFileGetContentsHttpTransport.php');

			$this->_httpTransport = new FirteXFileGetContentsHttpTransport($this->_timeout, 
                                                                           FirteXHttpTransport::METHOD_GET);
		}

		return $this->_httpTransport;
	}

	/**
	 * Set the HTTP Transport implemenation that will be used for all HTTP requests
	 *
	 * @param FirteXHttpTransport_Interface
	 */
	public function setHttpTransport(FirteXHttpTransport $httpTransport)
	{
		$this->_httpTransport = $httpTransport;
	}

	/**
	 * Get the current default timeout
	 *
	 * @return float in seconds
	 *
	 * @deprecated Use the getTimeout method on the HTTP transport implementation
	 */
	public function getTimeout()
	{
		return $this->_timeout;
	}

	/**
	 * Set the default timeout for all calls that aren't passed a specific timeout
	 *
	 * @param float $timeout Timeout value in seconds
	 */
	public function setTimeout($timeout)
	{
        $this->_timeout = $timeout;
		$this->getHttpTransport()->setTimeout($timeout);
	}

    /**
     * Set http method
     */
    public function setHttpMethod($method = FirteXHttpTransport::METHOD_GET) 
    {
        $this->getHttpTransport()->setMethod($method);
    }

    /**
     * Get http method
     */
    public function getHttpMethod() 
    {
        return $this->getHttpTransport()->getMethod();
    }

    /**
	 * @param int $offset The starting offset for result documents
	 * @param int $limit The maximum number of result documents to return
     */
    public function setLimit($offset, $limit) 
    {
        $this->_offset = $offset;
        $this->_limit = $limit;
    }

    /**
     * Set whats fields to return
     * @param string $fields which field to return, format: field name or field_name:snippet
     *        list $fields which fields to return
     */
    public function setFields($fields)
    {
        $this->_fieldsSelect = $fields;
    }

    /**
     * Set the encode of the request query
     * @param $encode the encode name
     */
    public function setEncode($encode)
    {
        $this->_encode = $encode;
    }
    
    /**
     * Set the cluster who receive the request query
     * 
     * @param string $cluster cluster name, format: cluster name or cluster_name{!query_for_this_cluster}
     *        list $cluster multi cluster names
     */
    public function setCluster($cluster)
    {
        $this->_cluster = $cluster;
    }

    /**
     * Set trace level for the request query
     * @param string $traceLevel trace level, 
     *        can be: info, debug and trace
     */
    public function setTraceLevel($traceLevel)
    {
        $this->_traceLevel = $traceLevel;
    }
    
    protected function listToString($list)
    {
        $str = '';
        foreach($list AS $i => $item) {
            $str .= $item . ',';
        }
        return rtrim($str);
    }

	/**
	 * Simple search API
	 *
	 * @param string $query The raw query string (see FirteX documentation)
	 * @param int $offset The starting offset for result documents
	 * @param int $limit The maximum number of result documents to return
     *
	 * @return FirteXQueryResult
	 *
	 * @throws FirteXHttpTransportException if an error occurs during the service call
	 * @throws FirteXInvalidArgumentException if an invalid HTTP method is used
	 */
	public function search($query, $sortBy = NULL, $filter = NULL, $offset = NULL, $limit = NULL) {
		$parameter['query'] = $query;
        if (!is_null($offset))
            $parameter['parameter'] = 'start:'. $offset;
        else 
            $parameter['parameter'] = 'start:'. $this->_offset;

        if (!is_null($limit))
            $parameter['parameter'] .= ',top:'. $limit;
        else
            $parameter['parameter'] .= ',top:'. $this->_limit;

        if (!is_null($sortBy)) {
            if (!is_array($sortBy))
                $parameter['sort_by'] = $sortBy;
            else $parameter['sort_by'] = $this->listToString($sortBy);
        }
        if (!is_null($filter)) {
            $parameter['filter'] = $filter;
        }
        
        if (!is_null($this->_fieldsSelect)) {
            if (!is_array($this->_fieldsSelect))
                $parameter['field_select'] = $this->_fieldsSelect;
            else $parameter['field_select'] = $this->listToString($this->_fieldsSelect);
        }

        if (!is_null($this->_encode)) {
            $parameter['urlencode'] = $this->_encode;
        }

        if (!is_null($this->_cluster)) {
            if (!is_array($this->_cluster))
                $parameter['cluster'] = $this->_cluster;
            else $parameter['cluster'] = listToString($this->_cluster);
        }

        if (!is_null($this->_traceLevel)) {
            $parameter['parameter'] .= ',trace:' . $this->_traceLevel;
        }
        var_dump($this->buildQueryString('search?', $parameter));
        return $this->sendAndReceive($this->buildQueryString('search?', $parameter));
	}

    /**
     * Send echo command to firtex server 
     *
     * @return bool true if the server is accessible
     */
    function ping() {
        $httpTransport = $this->getHttpTransport();
		$httpResponse = $httpTransport->sendAndReceive($this->_requestURI, 'echo?ping');
        return ($httpResponse->getStatusCode() == 200) ? true : false;
    }

	protected function buildQueryString($prefix, $params) {
        if ($this->getHttpTransport()->getMethod() == FirteXHttpTransport::METHOD_GET) {
            if ($this->_isSquareBracketsEscaped) {
                $queryString = http_build_query($params, null, ';');
                return $prefix . preg_replace('/%5B(?:[0-9]|[1-9][0-9]+)%5D=/', '=', $queryString);
            } else {
                $queryString = http_build_query($params, null, ';');
                return $prefix . preg_replace('/\\[(?:[0-9]|[1-9][0-9]+)\\]=/', '=', $queryString);
            }
        } else {
            return $prefix . $this->array_implode('=', ';', $params);
		}
	}

	/**
	 * Send request to server and receive response
	 *
	 * @param string $queryStr
	 * @return FirteXQueryResult
	 *
	 * @throws FirteXHttpTransportException if a non 200 response status is returned
	 */
	protected function sendAndReceive($queryStr)
	{
		$httpTransport = $this->getHttpTransport();
		$httpResponse = $httpTransport->sendAndReceive($this->_requestURI, $queryStr);
        if ($httpResponse->getStatusCode() == 200) {
            return new FirteXQueryResult($httpResponse->getBody());
        } else {
            $result = new FirteXQueryResult(null);
            $result->setError($httpResponse->getStatusMessage());
            return $result;
        }
	}

    /**
     * Implode an array with the key and value pair giving
     * a glue, a separator between pairs and the array
     * to implode.
     * @param string $glue The glue between key and value
     * @param string $separator Separator between pairs
     * @param array $array The array to implode
     * @return string The imploded array
     */
    private function array_implode( $glue, $separator, $array ) {
        if ( ! is_array( $array ) ) return $array;
        $string = array();
        foreach ( $array as $key => $val ) {
            if ( is_array( $val ) )
                $val = implode( ',', $val );
            $string[] = "{$key}{$glue}{$val}";
            
        }
        return implode( $separator, $string );
    }
}