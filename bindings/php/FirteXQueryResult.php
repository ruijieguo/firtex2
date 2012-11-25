<?php

require_once(dirname(__FILE__) . '/FirteXException.php');

class FirteXQueryResult implements IteratorAggregate {
    /**
     * Save the raw result
     */
    protected $_rawResult;

    /**
     * Holds hits of each cluster
     *
     * @var array
     */
    protected $_results;

	/**
	 * Constructor. Takes the raw HTTP response body and the exploded HTTP headers
	 *
	 * @param FirteXHttpTransportResponse HTTP response
	 */
	public function __construct($result) {
		$this->_rawResult = $result;
        if ($result != null) {
            $this->doParseData();
        }
	}

    /**
     * Set error message
     */
    public function setError($error) {
        $this->_results[] = array('error'=>(string)$error);
    }

	/**
	 * Get the raw response as it was given to this object
	 *
	 * @return string
	 */
	public function getRawResult() {
		return $this->_rawResult;
	}

    /**
     * Return the time cost of query execution
     */
    public function getCost($cluster = null) {
        if ($cluster != null) {
            if (in_array($cluster, $this->_results)) {
                return $this->_results[$cluster]['cost'];
            }
        } else if (count($this->_results) > 0) {
            foreach ($this->_results AS $result) {
                return $result['cost'];
            }
        }
        return '0';
    }

    /**
     * Return the total number of hits
     */
    public function getTotalHits($cluster = null) {
        if ($cluster != null) {
            if (in_array($cluster, $this->_results)) {
                return $this->_results[$cluster]['total_hits'];
            }
        } else if (count($this->_results) > 0) {
            foreach ($this->_results AS $result) {
                return $result['total_hits'];
            }
        }
        return '0';
    }

    /**
     * Return the number of hits
     */
    public function getNumberHits($cluster = null) {
        if ($cluster != null) {
            if (in_array($cluster, $this->_results)) {
                return $this->_results[$cluster]['number_hits'];
            }
        } else if (count($this->_results) > 0) {
            foreach ($this->_results AS $result) {
                return $result['number_hits'];
            }
        }
        return '0';
    }

    /**
     * IteratorAggregate implementation function. Allows usage:
     *
     * <code>
     * foreach ($document as $key => $value)
     * {
     *      ...
     * }
     * </code>
     */
    public function getIterator() {
        $arrayObject = new ArrayObject($this->_results);

        return $arrayObject->getIterator();
    }

	/**
	 * Parse the raw response into the parsed_data array for access
	 *
	 * @throws FirteXParserException If the data could not be parsed
	 */
	protected function doParseData() {
        try {
            $data = simplexml_load_string($this->_rawResult,
                                          'SimpleXMLElement', LIBXML_NOCDATA);
        } catch(Exception $e) {
            $data = null;
        }

		// check that we receive a valid xml response - we should never receive a null
		if ($data === null) {
			throw new FirteXParserException('The result string does not appear to be valid XML');
		}

        if (isset($data->hits)) {
            foreach($data->hits AS $hits) {
                $result = array();
                $result['number_hits'] = (string)$hits->attributes()->number_hits;
                $result['total_hits'] = (string)$hits->attributes()->total_hits;
                $result['cost'] = (string)$hits->attributes()->cost;

                $hits_out = array();
                foreach ($hits AS $hit) {
                    $hit_out = array();
                    if (isset($hit->shardid)) {
                        $hit_out['shardid'] = (string)$hit->shardid;
                    }
                    if (isset($hit->docid)) {
                        $hit_out['docid'] = (string)$hit->docid;
                    }
                    if (isset($hit->score)) {
                        $hit_out['score'] = (string)$hit->score;
                    }

                    if (isset($hit->fields)) {
                        $fields = array();
                        foreach($hit->fields AS $name=>$field) {
                            foreach($field AS $k=>$v) {
                                $fields[$k] = (string)$v;
                            }
                        }
                        $hit_out['fields'] = $fields;
                    }
                    $hits_out[] = $hit_out;
                }
                $result['hits'] = $hits_out;
                
                if (isset($hits->attributes()->cluster)) {
                    $this->_results[(string)$hits->attributes()->cluster] = $result;
                } else {
                    $this->_results[] = $result;
                }
            }//end foreach
        } else if (isset($data->error)) {
            foreach($data->error AS $error) {
                $result = array('error'=>(string)$error);
                if (isset($error->attributes()->cluster)) {
                    $this->_results[(string)$error->attributes()->cluster] = $result;
                } else {
                    $this->_results[] = $result;
                }
            }
        }
    }
}