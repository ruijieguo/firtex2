from thrift import Thrift
from firtex import ThriftSearchService
from firtex.ttypes import QueryCommand
from firtex.ttypes import *
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

class FirteXRPCClient:
    def __init__(self, host, port):
        sock = TSocket.TSocket(host, port)
        transport = TTransport.TFramedTransport(sock)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)

        self.client = ThriftSearchService.Client(protocol)
        transport.open()
    
    def syntaxSearch(self, uri):
        result = self.client.syntaxSearch(uri)
        return result
        
    def search(self, cmd):
        result = self.client.search(cmd)
        return result
