# coding: UTF-8
import urllib3
import random
import sys
from bs4 import BeautifulSoup, Tag, NavigableString
http = urllib3.PoolManager()

while True:
    base_url = "http://so.gushiwen.org/view_{0}.aspx".format(
        random.randint(1, 72866))
    # base_url = "http://so.gushiwen.org/view_20914.aspx"
    # base_url = "http://so.gushiwen.org/view_5512.aspx"
    try:
        f = http.request('GET', base_url, timeout=4.0)
    except:
        print("timeout: " + base_url)
        sys.exit(-1)

    soup = BeautifulSoup(f.data, 'lxml')
    poem = soup.find('div', 'main3')
    if poem is not None:
        break
total_content = poem.find('div', 'left').find('div',
                                              'sons').find('div', 'cont')
title = total_content.find('h1').string

author_info = total_content.find('p', 'source').find_all('a')
dynasty = author_info[0].contents[0]
author_name = author_info[1].contents[0]

content = total_content.find('div', 'contson').contents
print('\33[96m' + title)
print("[" + dynasty + "]•" + author_name)
for item in content:
    if isinstance(item, NavigableString):
        print(item.replace("\n", ""))
print('\033[0m')
