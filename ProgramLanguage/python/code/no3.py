# -*- coding: cp949 -*-
# 컴퓨터공학부 20093284 나홍철
# no3.py
import sys
import os
import glob

# 파일내의 스트링 찾기
def serchLine (string,name):    
    fw = open(name,'r')     #  all_files.txt 를 내용 추가식으로 open
    line = fw.readlines()
    for text in line :
        if text.find(string)!=-1:   # 라인에 원하는 스트링이 있을때
            print "스트링이 있는 파일: ",name
            print text
    fw.close()

# 해당 디렉토리의 텍스트 찾기
def serchText (string,name):
    pathList  = glob.glob("%s/*.txt"%(name))  # 확장자 .txt 만 얻기
    
    for fname in pathList :
        serchLine (string,fname)

args = sys.argv[1:]

if args[0]=="grep":     # grep 명령
    if os.path.lexists(args[2]):    # 위치에 존재하면
        if args[2].count(".txt"):     # 디렉토리명이 아닌 txt파일인경우
            serchLine(args[1],args[2])
        else :
            serchText(args[1],args[2])  
    else:
        print "WRONG TYPE"
else:
    print "ERROR COMMAND"
