# coding: UTF-8
import urllib.request
import re
import random

base_url = "http://so.gushiwen.org/view_{0}.aspx".format(random.randint(1,70001))
# base_url = "http://so.gushiwen.org/view_20914.aspx"
# base_url = "http://so.gushiwen.org/view_5512.aspx"
f = urllib.request.urlopen(base_url)

line = ""
currentLine = 0
while True:
    line = f.readline().decode("UTF-8")
    currentLine += 1
    if re.search('shileft', line):
        break
    if currentLine > 3000:
        print(base_url)
        exit(-1)

f.readline()
poem_name = re.search('<h1>([^<]*)</h1>', f.readline().decode("UTF-8")).group(1)
print('\33[5;36;40m'+poem_name)

while True:
    line = f.readline().decode("UTF-8")
    if re.search('朝代', line):
        break
generation = re.search("([\u4e00-\u9fa5]*)</p>", line).group(1)
poet_name = re.search("([\u4e00-\u9fa5]*)(</a>)?</p>", f.readline().decode("UTF-8")).group(1)
print("["+generation+"]•" + poet_name)

f.readline()
brElement = re.compile("<br ?/>")
pElement = re.compile("</?p>")
while True:
    line = f.readline().decode("UTF-8").strip()
    line = re.sub(brElement, "", line)
    if re.search("[\u4e00-\u9fa5]", line):
        line = re.sub(pElement, "", line)
        print(line.replace("。", "。\n").replace(".", "。\n"), end='')
    elif len(line) == 0:
        continue
    elif re.search('</div>', line):
        break
    else:
        print(line)
        print("hello")
        break
print('\033[0m')
