/*
* DRE : SAMPLEBYTE
*/
#include <string>
#include <iostream>
#include <fstream>
#include <pcap.h>
#include <hash_map>
#include <stdint.h>

using namespace std;
using namespace stdext;     //hashmap ���־� ��Ʃ��� �� �����Ϸ�

#define MAX_TABLE 256

int dre_samplebyte(string pcap_file, string out_file);
