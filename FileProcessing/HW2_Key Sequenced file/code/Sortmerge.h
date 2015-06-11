#ifndef __Sortmerge_H__
#define __Sortmerge_H__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using std::ofstream;
using std::ifstream;
using namespace std;

class Sortmerge
{
public:
	Sortmerge();
	Sortmerge(unsigned int num);

	void makeTrans();
	void sortTrans();
	void Merge();	

private:	
	void prn_ary(int *Ary, int size);
	void swap(string *sx, string *sy, int *x, int *y);
	void bubble_sort(string *str, int *Ary, int First, int ArySize, int dir);
	int CheckKey(const string str);
	char CheckType(const string type);
	void ErrorM(const char type, const int line);
	void PrintArr(int *tmp);

	unsigned int max = 60;
};

#endif