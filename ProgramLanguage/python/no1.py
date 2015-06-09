# -*- coding: utf-8 -*-
# 컴퓨터공학부 20093284 나홍철
# no1.py
fr = open("input.txt", 'r')
fw = open("output.txt", 'w')

line = fr.read()
for i in line:
    fw.write(i)
    fw.write('\n')
    
fw.close()
fr.close()
