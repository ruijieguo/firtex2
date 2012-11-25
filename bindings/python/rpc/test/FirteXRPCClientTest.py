import sys
sys.path.append('../')
sys.path.append('../../')
from firtex import ThriftSearchService
from firtex.ttypes import *
from rpc import FirteXRPCClient

print 'test syntaxSearch'
client = FirteXRPCClient.FirteXRPCClient('localhost', 19801)
result = client.syntaxSearch('query=BODY:2;field_select=BODY:snippet')
print result

print 'test search'
fields = ['BODY:snippet']
cmd = QueryCommand('BODY:2', None, None, 0, 10, fields)
result = client.search(cmd)
print result


