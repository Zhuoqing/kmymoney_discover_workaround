#!/usr/bin/python
import sys, os
#import requests
import httplib, ssl
from collections import OrderedDict
import logging

logfileName = os.path.abspath(sys.argv[0]) + '.log';
logging.basicConfig(filename = logfileName, level = logging.WARNING)
#logging.basicConfig(filename = logfileName, level = logging.DEBUG)

method = sys.argv[1]
url = sys.argv[2];
metaFile = sys.argv[3];
postFile = sys.argv[4];
dstFile = sys.argv[5];

logging.info(str(sys.argv))
try:
	if url == 'https://ofx.discovercard.com':
		logging.info('direct python get for discover')
		in_file = open(postFile, "r")
		content_bytes = in_file.read()
		in_file.close()
	
		#od = OrderedDict()
		#od['Content-Type'] = 'application/x-ofx'
		#od['Host'] = 'ofx.discovercard.com'
		#od['Content-Length'] = str(len(content_bytes))
		#od['Connection'] = 'Keep-Alive'
		#s = requests.Session()
		#s.headers = od
		# remove default headers for some reason 'Accept-Encoding': None doesnt work
		#req = requests.Request(method, url, headers = {'Accept': None, 'User-Agent': None}, data = content_bytes)
		#prepped = s.prepare_request(req)
		#logging.info('prepared headers %s', resp.request.headers)
		#del prepped.headers['Accept-Encoding']
		#resp = s.send(prepped, verify = False)
		#logging.info('request headers %s', resp.request.headers)

		conn = httplib.HTTPSConnection("ofx.discovercard.com")#, context = ssl._create_unverified_context())
		#conn.set_debuglevel(1)
		conn.connect()
		conn.putrequest(method, '/', skip_host = True, skip_accept_encoding = True)
		conn.putheader('Content-Type', 'application/x-ofx')
		conn.putheader('Host', 'ofx.discovercard.com')
		conn.putheader('Content-Length', str(len(content_bytes)))
		conn.putheader('Connection', 'Keep-Alive')
		conn.endheaders()
		conn.send(content_bytes)
		response = conn.getresponse()
		respBody = response.read()

		out_file = open(dstFile, "w")
		out_file.write(respBody)
		out_file.close()
		sys.exit(0)

except Exception as e:
    logging.exception("error ofx request")

sys.exit(1)
