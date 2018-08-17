#! /usr/bin/env python
# coding=utf-8

'''
爬取一本小说将其存入到本地文档中
'''

import urllib2
import re
from bs4 import BeautifulSoup


#通过参数中的网址构造一个请求并激活它，然后将网址里边所有的内容返回
def OpenPage(url):
    myheader = {} #构造一个请求报头
    request = urllib2.Request(url,headers = myheader) #构造一个request请求
    f = urllib2.urlopen(request) #激活请求
    data = f.read()  #读取网页里边的内容
    return data.decode("GBK",errors="ignore").encode("utf-8")#解码与编码

#将网页里的内容转换成一个类，根据类提供的方法将内容中的url提取出来放到一个列表中
def ParseMainpage(page):
    soup = BeautifulSoup(page,"html.parser") #将网页内容转成一个对象
    ListCharts = soup.find_all(href = re.compile("read")) #compile函数用于构建一个正则匹配对象
    UrlList = [] #用于存放每一个章节的url
    for item in ListCharts:
        url = "http://www.shengxu6.com"+item["href"]
        UrlList.append(url)
    return UrlList

#通过一个章节的url将这个章节的标题和内容获取到并返回
def ParseDetailPage(page):
    soup = BeautifulSoup(page,"html.parser")#解析成一个html网页类
    title = soup.find_all(class_ = "panel-heading")[0].get_text()#获取章节的标题
    content = soup.find_all(class_ = "panel-body")[0].get_text()#获取章节的内容
    return title,content

#将标题和内容拷贝到本地文件中
def WriteDatatoFile(data):
    f = open("output.txt","a+")#以追加写的方式打开输出文件
    f.write(data)
    f.close()

#主函数
if __name__ == "__main__":
    url = "http://www.shengxu6.com/book/2967.html"
    mainpage = OpenPage(url) #获取网页的内容
    urlList = ParseMainpage(mainpage) # 解析网页的内容，获取各个章节的url
    for item in urlList:
        print "Clone: "+item
        page = OpenPage(item) # 通过各个章节的url获取其内容
        title,content = ParseDetailPage(page) # 获取各个章节的标题和内容
        print "Clone title is"+title
        data = "\n\n\n"+title+"\n\n\n"+content
        WriteDatatoFile(data.encode("utf-8")) # 将各个章节写入到文件中去
