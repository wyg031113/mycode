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

def auto_login_hi(url,name,pwd):
	global now,norep
	md=hashlib.md5(pwd).hexdigest()
	#url_hi="http://rs.xidian.edu.cn/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&lssubmit=yes&inajax=1%20HTTP/1.1&username=wyg031113&password=79f5a85378a29725bffc44d737cc4856"
	url_hi="http://rs.xidian.edu.cn/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&lssubmit=yes&inajax=1%20HTTP/1.1"
	cookie=cookielib.CookieJar()
	cj=urllib2.HTTPCookieProcessor(cookie)
	postdata=urllib.urlencode({'username':name,'password':md,'quickforward':"yes",'handlekey':'ls'})
#	postdata=urllib.urlencode({'quickforward':"yes",'handlekey':'ls'})
	request=urllib2.Request(url_hi, postdata)
	opener=urllib2.build_opener(cj)
	f=opener.open(request)
	#print f
	hi_html=opener.open(url)
	bbs=opener.open('http://rs.xidian.edu.cn/forum.php');
	forum= bbs.read()
	#print type(forum)
	#print forum
	pattern = re.compile(ur'<a href="(forum.php\?mod=viewthread&tid=[0-9]+)"')
	#pattern = re.compile(ur'forum.php')
	match = pattern.findall(forum)
	#if match:
	#	print match
	if not match:
		print 'Oh, no, no pages?'
		return None
	for u in match:
		fu = 'http://rs.xidian.edu.cn/'+u
		if fu in norep:
			continue
		print fu
		each = opener.open(fu);
		html_each= each.read()
		#print html_each
		pat_title = re.compile(ur'<title>.*?</title>')
		title_match = pat_title.search(html_each);
		if title_match:
			print title_match.group()
		else:
			print 'no title??'
			norep=norep+[fu]
			continue
		ptitle=title_match.group()
		pat_time = re.compile(ur'span title="([0-9]+)-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+):([0-9]+)">')
		time_match = pat_time.search(html_each);
		if time_match:
			print time_match.group(), time_match.group(1), time_match.group(2),time_match.group(3)
		else:
			norep=norep+[fu]
			continue
		year=time_match.group(1)
		mont=time_match.group(2)
		day=time_match.group(3)
		hour=time_match.group(4)
		mini=time_match.group(5)
		sec=time_match.group(6)
		year=int(year)
		mont=int(mont)
		day=int(day)
		hour=int(hour)
		mini=int(mini)
		sec=int(sec)
		tt=datetime(year,mont,day,hour,mini,sec)
		#formhash=95e9663b
		pat_fhs=re.compile(ur'formhash=([0-9a-fA-F]+)')
		fhs_match=pat_fhs.search(html_each)
		print "formhash::::::::"+fhs_match.group(1)
		fhs=fhs_match.group(1)
		if ptitle.find('西电睿思灌水专区')!=-1:
			print '这个帖子可以灌水'
		print tt,now,(tt-now),(tt-now).days
		if (tt-now).days >= 0:
			print 'have new page!! need to reply!'
			norep=norep+[fu]
			if ptitle.find('西电睿思灌水专区')!=-1:
				print '这个帖子可以灌水'
			else:
				continue
			continue #################################################################
			pat_tid=re.compile('tid=([0-9]+)')
			print "befor match:",fu
			mt=pat_tid.search(fu)
			tid=int(mt.group(1))
			w=time.localtime()
			striso=""
			striso+=str(w.tm_year)+'-'
			striso+=str(w.tm_mon)+'-'
			striso+=str(w.tm_mday)+' '
			striso+=str(w.tm_hour)+":"
			striso+=str(w.tm_min)+':'
			striso+=str(w.tm_sec)
			seconds=ISOString2Time(striso)
			print "seconds=:"+str(int(seconds))
			secx=str(int(seconds))
			#urls='http://rs.xidian.edu.cn/forum.php?mod=post&action=reply&fid=72&tid='+str(tid)+'&extra=&replysubmit=yes&infloat=yes&handlekey=fastpost&inajax=1&message=hahahthisisisisisok&formhash='+fhs+'&usesig=1&posttime='+secx+''
			urls='http://rs.xidian.edu.cn/forum.php?mod=post&action=reply&fid=72&tid='+str(tid)+'&extra=&replysubmit=yes&infloat=yes&handlekey=fastpost&inajax=1'
			print urls
			#fx=opener.open(urls)
			#ck=cookielib.CookieJar()
			#cjx=urllib2.HTTPCookieProcessor(ck)
			msg='我抢沙发最快了。帮楼主顶起来！'
			postdatax=urllib.urlencode({'message':msg,'usesig':'1','formhash':fhs,'subject':'','posttime':secx}) #	postdata=urllib.urlencode({'quickforward':"yes",'handlekey':'ls'})
			
			req=urllib2.Request(urls, postdatax) 
			#openerx=urllib2.build_opener(cjx)
			fx=opener.open(req)
			htmlx=fx.read()
			#print htmlx
			pat_suc=re.compile(r'回复发布成功')
			ma_suc=pat_suc.search(htmlx)
			print ma_suc
			if ma_suc:
				print '成功回复了个帖子'
				time.sleep(15)
			print"\n\n"
		else:
			norep=norep+[fu]
		#time.sleep(1)
	return hi_html
if __name__=='__main__':
	print '西电睿思灌水专区'
	name='wl031113'
	password='03111298'
	url='http://rs.xidian.edu.cn/bt.php?mod=browse&t=all'
	i=0
	while True:
		i=i+1
		h = auto_login_hi(url,name,password)
		time.sleep(8)
		if i % 6==0:
			print "program running.."
	#print h.read()
