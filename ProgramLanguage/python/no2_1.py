# -*- coding: cp949 -*-
# 컴퓨터공학부 20093284 나홍철
# no2_1.py
import glob
import os

# 디렉토리 내부 텍스트파일 찾기
def serchtext (dirname):
    pathList  = glob.glob("%s/*.txt"%(dirname))  # 확장자 .txt 만 얻기
    
    for text in pathList :
        fr = open(text, 'r')
       
        line = fr.readlines()
        lc = 0   #line count
        cc = 0   #character count
        
        for j in line:
            lc+=1
            ch = j.split(' ')   # 입력받은 스트링을 단어로 나누          
            for k in ch:
                cc+=1
        filen = text.replace(dirname+'\\',"")  # 디렉토리명을 뺀 파일       
        print '<', filen, lc, cc, '>'
        fr.close()

dn = raw_input('Directory : ')

if os.path.lexists(dn):
    serchtext(dn)

else:
    print "ERROR NOT EXIST DIRECTORY"
