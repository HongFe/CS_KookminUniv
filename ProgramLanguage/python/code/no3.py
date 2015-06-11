# -*- coding: cp949 -*-
# ��ǻ�Ͱ��к� 20093284 ��ȫö
# no3.py
import sys
import os
import glob

# ���ϳ��� ��Ʈ�� ã��
def serchLine (string,name):    
    fw = open(name,'r')     #  all_files.txt �� ���� �߰������� open
    line = fw.readlines()
    for text in line :
        if text.find(string)!=-1:   # ���ο� ���ϴ� ��Ʈ���� ������
            print "��Ʈ���� �ִ� ����: ",name
            print text
    fw.close()

# �ش� ���丮�� �ؽ�Ʈ ã��
def serchText (string,name):
    pathList  = glob.glob("%s/*.txt"%(name))  # Ȯ���� .txt �� ���
    
    for fname in pathList :
        serchLine (string,fname)

args = sys.argv[1:]

if args[0]=="grep":     # grep ���
    if os.path.lexists(args[2]):    # ��ġ�� �����ϸ�
        if args[2].count(".txt"):     # ���丮���� �ƴ� txt�����ΰ��
            serchLine(args[1],args[2])
        else :
            serchText(args[1],args[2])  
    else:
        print "WRONG TYPE"
else:
    print "ERROR COMMAND"
