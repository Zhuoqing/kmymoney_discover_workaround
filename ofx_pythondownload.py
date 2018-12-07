#!/usr/bin/python
import sys, os
import httplib, ssl
from collections import OrderedDict
from shutil import copyfile
from datetime import datetime
import logging

logfileName = os.path.abspath(sys.argv[0]) + '.log';
logging.basicConfig(filename = logfileName, level = logging.WARNING)
#logging.basicConfig(filename = logfileName, level = logging.DEBUG)

logging.info('direct python get starting')

method = sys.argv[1]
url = sys.argv[2];
metaFile = sys.argv[3];
postFile = sys.argv[4];
dstFile = sys.argv[5];

#copyfile(metaFile, os.path.abspath(sys.argv[0]) + '.metaFile.txt')
#copyfile(postFile, os.path.abspath(sys.argv[0]) + '.postFile.txt')

logging.info(str(sys.argv))
try:
	if url == 'https://ofx.discovercard.com':
		logging.info('direct python get for discover')
		in_file = open(postFile, "r")
		content_bytes = in_file.read()
		in_file.close()
	
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

	if url == 'https://ofx.firsttechfed.com':
		logging.info('direct python get for firsttechfed')
		in_file = open(postFile, "r")
		content_bytes = in_file.read()
		in_file.close()
	
		conn = httplib.HTTPSConnection("ofx.firsttechfed.com")#, context = ssl._create_unverified_context())
		conn.set_debuglevel(5)
		conn.connect()
		conn.putrequest(method, 'https://ofx.firsttechfed.com/', skip_host = True, skip_accept_encoding = True)
		conn.putheader('Accept', '*/*')
		conn.putheader('User-Agent', 'InetClntApp/3.0')
		conn.putheader('Date', datetime.utcnow().strftime('%a, %d %b %Y %H:%M:%S GMT'))
		conn.putheader('Content-Type', 'application/x-ofx')
		conn.putheader('Host', 'ofx.firsttechfed.com')
		conn.putheader('Content-Length', str(len(content_bytes)))
		conn.putheader('Connection', 'Keep-Alive')
		conn.endheaders()
		conn.send(content_bytes)
		response = conn.getresponse()
		respBody = response.read()

		out_file = open(dstFile, "w")
		out_file.write(respBody)
		out_file.close()
		#copyfile(dstFile, os.path.abspath(sys.argv[0]) + '.resFile.txt')
		sys.exit(0)

except Exception as e:
    logging.exception("error ofx request")

sys.exit(1)
