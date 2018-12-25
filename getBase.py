#!/usr/bin/env python3
# coding : utf-8

import urllib.request
import re
from  bs4 import BeautifulSoup
import sys


TAG_RE = re.compile(r'<[^>]+>')

## 從網路讀入
page = urllib.request.urlopen('http://invoice.etax.nat.gov.tw/')
html = page.read().decode()


## 從檔案讀入
#with open("index.html","r") as fn:
#    html_doc = fn.readlines()
#    html="\n".join(html_doc)

soup = BeautifulSoup(html, "html.parser")
#print(soup.prettify())

# get Numbers
numRE = re.compile(r'、')
monthList=[]
numList=[]
#for tag in soup.select('.t18Red'):
for tag in soup.find_all('span', class_="t18Red"):
    numList.append(numRE.split(*tag.contents))
# 因為 class 是 Python 關鍵字，所以要用 class_ 加個底線

for month in soup.find_all('h2'):
    if "年" in month.getText() and "月" in month.getText():
        monthList.append( month.getText() )

for n,item in enumerate(monthList):
    print("[{:1d}] {:s}".format(n,item))
sel = input("選擇月份: ")
try:
    sel = int(sel)
    if not 0<= sel < 2: raise 
except:
    print("看不懂你輸入的. 我直接假設你要的是 [0]")
    sel = 0

## 輸出到檔案
prizeNotion = "SFHA"

with open("{:s}.txt".format(monthList[sel]), "w") as f:
    print("### {} ###".format(monthList[sel]), file=f)
    for i, p in enumerate(prizeNotion):
        print("{} {}".format(p, len(numList[4*sel+i])), file=f)
        for lotnum in numList[4*sel+i]:
            print("{:s}".format(lotnum), file=f)

print("Saved {}.txt".format(monthList[sel]))
