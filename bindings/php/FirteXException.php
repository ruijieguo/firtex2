<?php

class FirteXException extends Exception {
	const CLASS_NAME = 'FirteXException';
}

class FirteXHttpTransportException extends FirteXException {
	private $_response;

	public function __construct(FirteXHttpTransportResponse $response) {
		parent::__construct("'{$response->getStatusCode()}' Status: {$response->getStatusMessage()}",
                            $response->getStatusCode());

		$this->_response = $response;
	}

	public function getResponse() {
		return $this->_response;
	}
}

class FirteXInvalidArgumentException extends FirteXException {
}

class FirteXNoServiceAvailableException extends FirteXException {
}

class FirteXParserException extends FirteXException {
}