import urllib.request
import sys
import time
import os
import json
import threading

#tieba-sipder by berry
#使用GPL v2.0授权，请按协议规定使用代码
#sys.argv: |  meaning:
#    1     |    mode
#    2     |  is_unfirst
#    3     |    tid
#    4     | pid or number of page
#    5     |  number of page

#获取正文线程
class myThread_c (threading.Thread):
	def __init__(self, threadID, begin, end):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.begin = begin
		self.end = end
	def run(self):
		content(self.begin, self.end)


#获取多页正文
def content(begin, end):
	for pn in range(begin,end+1):
		request=urllib.request.Request("https://tieba.baidu.com/p/"+sys.argv[3]+"?pn="+str(pn),headers = header)
		res = urllib.request.urlopen(request)
		ans=""
		ans=res.read()
		tiezi=open(name+"/source/"+name+"_all"+str(pn)+".brysource",mode='wb')
		tiezi.write(ans)
		tiezi.close

#获取多页楼中楼
def lzl(begin, end):
	for pn in range(begin, end+1):
		request = urllib.request.Request("https://tieba.baidu.com/p/comment?tid="+sys.argv[3]+"&pid="+sys.argv[4]+"&pn="+str(pn),headers = header)
		res = urllib.request.urlopen(request)
		ans=""
		ans=res.read()
		tiezi=open(name+"/lzl/"+name+"_lzl_"+str(sys.argv[4])+"_"+str(pn),mode='wb')
		tiezi.write(ans)
		tiezi.close

#通过解析settings.json获取对目录的命名
def getname():
	name = sys.argv[3]
	with open('settings.json', 'r') as f:
		data = json.load(f)
		name = data["name"]
	return name

###############################################################################
# start
print ("python call in [",int(time.time()),"]")
print (sys.argv)

name = getname()

#http请求头
header = {"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36 Edg/80.0.361.111","Connection" : "keep-alive"}

if(sys.argv[1]=="content"): #爬取一页正文
	if(sys.argv[2]=='0'):
		print ("./downloader.py:70")
		request=urllib.request.Request("https://tieba.baidu.com/p/"+sys.argv[3]+"?pn=1",headers = header)
		res = urllib.request.urlopen(request)
		ans=""
		ans=res.read()
		tiezi=open(name+"/source/"+name+"_all1.brysource",mode='wb')
		tiezi.write(ans)
		tiezi.close
	if(sys.argv[2]=='1'): #多线程爬取多页正文（要是遇上几百页的帖子单线程就慢死）
		if(int(sys.argv[4]) <= 10):
			content(2,int(sys.argv[4]))
		if(int(sys.argv[4]) <= 50):
			thread1 = myThread_c(1, 2, int(sys.argv[4])//2)
			thread2 = myThread_c(2, (int(sys.argv[4])//2)+1, int(sys.argv[4]))
			thread1.start()
			thread2.start()
			thread1.join()
			thread2.join()
		if(int(sys.argv[4]) <= 150):
			thread1 = myThread_c(1, 2, int(sys.argv[4])//3)
			thread2 = myThread_c(2, (int(sys.argv[4])//3)+1, 2*(int(sys.argv[4]))//3)
			thread3 = myThread_c(3, (2*(int(sys.argv[4]))//3)+1, int(sys.argv[4]))
			thread1.start()
			thread2.start()
			thread3.start()
			thread1.join()
			thread2.join()
			thread3.join()
		if(int(sys.argv[4]) > 150):
			thread1 = myThread_c(1, 2, int(sys.argv[4])//8)
			thread2 = myThread_c(2, (int(sys.argv[4])//8)+1, 2*(int(sys.argv[4]))//8)
			thread3 = myThread_c(3, (2*(int(sys.argv[4]))//8)+1, 3*(int(sys.argv[4]))//8)
			thread4 = myThread_c(4, (3*(int(sys.argv[4]))//8)+1, 4*(int(sys.argv[4]))//8)
			thread5 = myThread_c(5, (4*(int(sys.argv[4]))//3)+1, 5*(int(sys.argv[4]))//8)
			thread6 = myThread_c(6, (5*(int(sys.argv[4]))//3)+1, 6*(int(sys.argv[4]))//8)
			thread7 = myThread_c(7, (6*(int(sys.argv[4]))//3)+1, 7*(int(sys.argv[4]))//8)
			thread8 = myThread_c(8, (7*(int(sys.argv[4]))//3)+1, int(sys.argv[4]))
			thread1.start()
			thread2.start()
			thread3.start()
			thread4.start()
			thread5.start()
			thread6.start()
			thread7.start()
			thread8.start()
			thread1.join()
			thread2.join()
			thread3.join()
			thread4.join()
			thread5.join()
			thread6.join()
			thread7.join()
			thread8.join()

if(sys.argv[1] == "lzl"):
	if(sys.argv[2]=='0'):
		request=urllib.request.Request("https://tieba.baidu.com/p/comment?tid="+sys.argv[3]+"&pid="+sys.argv[4]+"&pn=1",headers = header)
		res = urllib.request.urlopen(request)
		ans=""
		ans=res.read()
		tiezi=open(name+"/lzl/"+name+"_lzl_"+str(sys.argv[4])+"_1",mode='wb')
		tiezi.write(ans)
		tiezi.close
	if(sys.argv[2]=='1'):
		lzl(2,int(sys.argv[5]))


if(sys.argv[1] == "lzl_list"):
	request=urllib.request.Request("https://tieba.baidu.com/p/totalComment?&tid="+sys.argv[3]+"&pn="+sys.argv[4],headers = header)
	res = urllib.request.urlopen(request)
	ans=""
	ans=res.read()
	tiezi=open(name+"/lzl/"+name+sys.argv[4]+"_list.json",mode='wb')
	tiezi.write(ans)