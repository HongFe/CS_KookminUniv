# -*- coding: cp949 -*-
# ��ǻ�Ͱ��к� 20093284 ��ȫö
# no2_1.py
import glob
import os

# ���丮 ���� �ؽ�Ʈ���� ã��
def serchtext (dirname):
    pathList  = glob.glob("%s/*.txt"%(dirname))  # Ȯ���� .txt �� ���
    
    for text in pathList :
        fr = open(text, 'r')
       
        line = fr.readlines()
        lc = 0   #line count
        cc = 0   #character count
        
        for j in line:
            lc+=1
            ch = j.split(' ')   # �Է¹��� ��Ʈ���� �ܾ�� ����          
            for k in ch:
                cc+=1
        filen = text.replace(dirname+'\\',"")  # ���丮���� �� ����       
        print '<', filen, lc, cc, '>'
        fr.close()

dn = raw_input('Directory : ')

if os.path.lexists(dn):
    serchtext(dn)

else:
    print "ERROR NOT EXIST DIRECTORY"
