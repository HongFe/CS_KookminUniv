# -*- coding: cp949 -*-
# 컴퓨터공학부 20093284 나홍철
# no7.py
import sys
import re

# word-count 명령
def countF (fname) :
    rf = open(fname,"r")    
    content = rf.read()    
    word = content.split()
    print ''
    dic = {}    # 단어의 카운팅 딕셔너리 변수
   
    for i in word:
        if dic.has_key(i):            
            dic[i]+=1
        else:
            dic[i]=1
    for j in dic.keys():
        print j," : ", dic[j]
    print '\n[Finished]\n'
    rf.close()
    
# word-replace 명령
def replaceF(fname):
    fr = open(fname,"r")        
    content=fr.read()
    
    while 1:        
        com=raw_input("word-replace > ")
        if not com.find("edit"):        # edit 명령어시
            inW=raw_input("Input Word : ")
            inR=raw_input("Input Replace Word : ")
            a=inW.capitalize()  # 첫문자 대문자
            b=inW.upper()   # 소문자
            c=inW.lower()   # 대문자
            content=content.replace(a,inR)
            content=content.replace(b,inR)
            content=content.replace(c,inR)

        elif not com.find("print"):     # print 명령어시            
            fn=raw_input("Input Newfile Name : ")            
            wf=open(fn,"w")
            wf.write(content)
            print '\n',content           
            print '\n[Finished]\n'
            break
        else:
            print "Wrong Command"
        
# 초기 명령어
def inCom (fname):
    while 1:
        comm = raw_input("Input Command : ")
        if not comm.find("count"):  # count 명령어시     
            countF(fname)       

        elif not comm.find("replace"):  # replace 명령어시
            replaceF(fname)

        elif not comm.find("quit"): # 종료
            break
        
        else:
            print "WRONG COMMAND"

args = sys.argv[1:]
inCom(args[0])
