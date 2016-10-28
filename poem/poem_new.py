# coding: UTF-8
import urllib3
import random
from bs4 import BeautifulSoup
http = urllib3.PoolManager()

base_url = "http://so.gushiwen.org/view_{0}.aspx".format(random.randint(1,72866))
# base_url = "http://so.gushiwen.org/view_20914.aspx"
# base_url = "http://so.gushiwen.org/view_5512.aspx"
try:
    f = http.request('GET', base_url, timeout=4.0)
except urllib3.exceptions.TimeoutError as e:
    print("timeout: " + base_url)
    exit(-1)

soup = BeautifulSoup(f.data, 'lxml')
poem = soup.find('div', 'shileft')
title = poem.find('div', 'son1').find('h1').string

content = poem.find('div', 'son2')
author_info = content.find_all('p')
dynasty = author_info[0].contents[1].string
author_name = author_info[1].contents[1].string

real_content = []
head = author_info[2].nextSibling
while head is not None:
    if head.string is not None:
        real_content.append(head.string.strip())
    head = head.nextSibling

print(title)
print("[" + dynasty + "]•" + author_name)
for item in real_content:
    length = len(item)
    if length <= 32:
        print(item)
    else:
        begin = 0
        end = 32
        while end < length:
            print(item[begin: end])
            begin = end
            end += 32
