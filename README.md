# Tieba-spider

## __百度贴吧爬虫，用于爬取单个帖子__

__使用了RapidJson库，有关的版权声明与开源协议见[RapidJson](http://github.com/Tencent/rapidjson/blob/master/license.txt)__

### 简介
这是一个功能比较简陋的爬虫，目前只支持单帖子爬取（写个脚本爬多帖子也不是不行233）输出为markdown文档

一开始是为了备份自己喜欢的帖子才做的这个爬虫（我没有找到带楼中楼爬取的，可能是我太菜了QAQ）然后接下来的目标是提高效率，顺便学下面向对象（因为是为了算法竞赛学的编程就只会面向过程），估计会重写很多次吧，会一直 ~~持续~~ 随缘更新

***
### 关于settings.json

"path":保存文件的路径  
"path_to_python":python3的路径  
"is_local":是否使用本地模式，这个是DEBUG的时候节省下载文件的时间用的  
"local_files_page_number":本地模式要分析的页数  
"tid":帖子编号，就是链接/p/后面的数字  
"name":用于保存帖子的文件夹名，应作为path的子文件夹并包含lzl和source文件夹

***
使用MIT协议开源。
