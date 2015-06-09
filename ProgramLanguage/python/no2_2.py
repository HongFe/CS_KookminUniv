# -*- coding: utf-8 -*-
# 컴퓨터공학부 20093284 나홍철
# no2_2.py
import glob
import os

# 디렉토리내의 파일을 모두 합친다
def serchtext (dirname):
    pathList  = glob.glob("%s/*.txt"%(dirname))  # 확장자 .txt 만 얻기

    path = dirname+"\\all_files.txt"    # 파일저장경로 이름
    fw = open(path,'a')     #  all_files.txt 를 내용 추가식으로 open
    for text in pathList :
        fr = open(text, 'r')
       
        content = fr.read() 
        fw.write(content) # 파일에 내용 입력        

    fw.close()  
    fr.close()

dn = raw_input('Directory : ')

if os.path.lexists(dn):
    serchtext(dn)

else:
    print "ERROR NOT EXIST DIRECTORY"
