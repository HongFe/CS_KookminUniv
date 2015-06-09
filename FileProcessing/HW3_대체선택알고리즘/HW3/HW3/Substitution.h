#ifndef __Substitution_H__
#define __Substitution_H__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#define TRUE 1
#define FALSE 0

using std::ofstream;
using std::ifstream;
using namespace std;

class Substitution
{
public:
	Substitution();
	Substitution(unsigned int num);
	int replacementSelection();
private:
	unsigned int max=4;
};

#endif