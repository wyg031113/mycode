#!/bin/env python
import urllib,urllib2,httplib,cookielib
import hashlib
def auto_login_hi(url,name,pwd):
	md=hashlib.md5(pwd).hexdigest()
	#url_hi="http://rs.xidian.edu.cn/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&lssubmit=yes&inajax=1%20HTTP/1.1&username=wyg031113&password=79f5a85378a29725bffc44d737cc4856"
	url_hi="http://rs.xidian.edu.cn/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&lssubmit=yes&inajax=1%20HTTP/1.1&username=wyg031113&password="+md
	cookie=cookielib.CookieJar()
	cj=urllib2.HTTPCookieProcessor(cookie)
	#postdata=urllib.urlencode({'username':name,'password':pwd,'quickforward':"yes",'handlekey':'ls'})
	postdata=urllib.urlencode({'quickforward':"yes",'handlekey':'ls'})
	request=urllib2.Request(url_hi, postdata)
	opener=urllib2.build_opener(cj)
	f=opener.open(request)
	print f
	hi_html=opener.open(url)
	bbs=opener.open('http://rs.xidian.edu.cn/forum.php');
	print bbs.read()
	return hi_html
if __name__=='__main__':
	name='wyg031113'
	password='03111295'
	url='http://rs.xidian.edu.cn/bt.php?mod=browse&t=all'
	h = auto_login_hi(url,name,password)
	print h.read()
