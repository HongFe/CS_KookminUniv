# -*- coding: cp949 -*-
# ��ǻ�Ͱ��к� 20093284 ��ȫö
# no7.py
import sys
import re

# word-count ���
def countF (fname) :
    rf = open(fname,"r")    
    content = rf.read()    
    word = content.split()
    print ''
    dic = {}    # �ܾ��� ī���� ��ųʸ� ����
   
    for i in word:
        if dic.has_key(i):            
            dic[i]+=1
        else:
            dic[i]=1
    for j in dic.keys():
        print j," : ", dic[j]
    print '\n[Finished]\n'
    rf.close()
    
# word-replace ���
def replaceF(fname):
    fr = open(fname,"r")        
    content=fr.read()
    
    while 1:        
        com=raw_input("word-replace > ")
        if not com.find("edit"):        # edit ��ɾ��
            inW=raw_input("Input Word : ")
            inR=raw_input("Input Replace Word : ")
            a=inW.capitalize()  # ù���� �빮��
            b=inW.upper()   # �ҹ���
            c=inW.lower()   # �빮��
            content=content.replace(a,inR)
            content=content.replace(b,inR)
            content=content.replace(c,inR)

        elif not com.find("print"):     # print ��ɾ��            
            fn=raw_input("Input Newfile Name : ")            
            wf=open(fn,"w")
            wf.write(content)
            print '\n',content           
            print '\n[Finished]\n'
            break
        else:
            print "Wrong Command"
        
# �ʱ� ��ɾ�
def inCom (fname):
    while 1:
        comm = raw_input("Input Command : ")
        if not comm.find("count"):  # count ��ɾ��     
            countF(fname)       

        elif not comm.find("replace"):  # replace ��ɾ��
            replaceF(fname)

        elif not comm.find("quit"): # ����
            break
        
        else:
            print "WRONG COMMAND"

args = sys.argv[1:]
inCom(args[0])
