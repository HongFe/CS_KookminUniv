# -*- coding: utf-8 -*-
# 컴퓨터공학부 20093284 나홍철
# no6.py
import sys
import re

def changeText (filen) :
    date=['[0-9]+년',"<DATE>"]
    date2=['[0-9]+일',"</DATE>"]
    time=['[0-9]+시',"<TIME>"]
    time2=['[0-9]+분',"</TIME>"]
    qua=['[0-9]+병|[0-9]+개',"<QUANTITY>","</QUANTITY>"]

    fr = open(filen,'r')
    fw = open("ouput6.txt",'w')     # 출력파일 'output6.txt'
    l=fr.read()
    a = l.split(' ')    # 단어로 나누기

    for i in a:
        if bool(re.search(date[0],i)):      # 년 일때
           fw.write(re.sub(date[0],date[1]+i,i))
           
        elif bool(re.search(date2[0],i)):   # 월 일떄
           fw.write(re.sub("일","일"+date2[1],i))
              
        elif bool(re.search(time[0],i)):    # 시 일때
           fw.write(re.sub(time[0],time[1]+i,i))
            
        elif bool(re.search(time2[0],i)):   #분 일떄
           fw.write(re.sub("분","분"+time2[1],i))
  
        elif bool(re.search(qua[0],i)):     # 병|개 일때
             c = re.findall(qua[0],i)    
             fw.write(re.sub(qua[0],qua[1]+c[0]+qua[2],i))
        else:   # 이외 일때
            fw.write(i)
        fw.write(" ")
        
    fr.close()            
    fw.close()

args = raw_input("TEXT : ")
if args.find(".txt"):
    changeText(args)
else:
    print "WRONG TEXT FILE"

