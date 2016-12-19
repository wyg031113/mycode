#!/bin/env python
#coding:utf-8
import urllib,urllib2,httplib,cookielib
import hashlib
import re
import time
from datetime import datetime,timedelta
now=datetime.now()
#now=datetime(2016,7,8,18,0,0)
print now
norep=[]
def ISOString2Time( s ):
	d=datetime.strptime(s,"%Y-%m-%d %H:%M:%S")
	return time.mktime(d.timetuple())

def auto_login_hi(url):
	global now,norep
	
	cookie=cookielib.CookieJar()
	cj=urllib2.HTTPCookieProcessor(cookie)
#	postdata=urllib.urlencode({'username':name,'password':md,'quickforward':"yes",'handlekey':'ls'})
#	postdata=urllib.urlencode({'quickforward':"yes",'handlekey':'ls'})
#	request=urllib2.Request(url_hi, postdata)
	opener=urllib2.build_opener(cj)
#	f=opener.open(request)
	#print f
	hi_html=opener.open(url)
	print hi_html
	bbs=opener.open(url);
	forum= bbs.read()

	#print type(forum)
	print forum
	"""
(Request-Line)	POST /qiang/index.aspx/SubScore HTTP/1.1
Host	fen.woniuziben.com
User-Agent	Mozilla/5.0 (Windows NT 10.0; WOW64; rv:49.0) Gecko/20100101 Firefox/49.0
Accept	application/json, text/javascript, */*; q=0.01
Accept-Language	zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3
Accept-Encoding	gzip, deflate
Content-Type	application/json; charset=utf-8
X-Requested-With	XMLHttpRequest
Referer	http://fen.woniuziben.com/qiang/index.aspx?u=oOE7BuA4BiDfl__3U9eza3a_dUd8
Content-Length	146
Cookie	pgv_pvi=1745826816; pgv_si=s1234400256
Connection	keep-alive
	"""

	url_hi="http://fen.woniuziben.com/qiang/index.aspx/SubScore"                       
	postdata=urllib.urlencode( {'openid':'oOE7BuA4BiDfl__3U9eza3a_dUd8','log':'1920:1080_Mozilla/5.0 (Windows NT 10.0; WOW64; rv:49.0) Gecko/20100101 Firefox/49.0&61.150.43.68'})
	request=urllib2.Request(url_hi, postdata)
	f=opener.open(request)
	print f
	return hi_html
if __name__=='__main__':
	url='http://fen.woniuziben.com/qiang/index.aspx?u=oOE7BuA4BiDfl__3U9eza3a_dUd8'
	i=0
	while True:
		i=i+1
		h = auto_login_hi(url)
		time.sleep(8)
		if i % 6==0:
			print "program running.."
	#print h.read()
