#!/usr/bin/python
# -*- coding: utf-8 -*-

import httplib2, urllib
params = urllib.urlencode({'ip':'192.243.115.223','datatype':'string'})
url = 'http://api.ip138.com/query/?'+params
headers = {"token":"33bf8d7573a233c33baad9307a2d6e47"}
http = httplib2.Http()
response, content = http.request(url,'GET',headers=headers)
print content
