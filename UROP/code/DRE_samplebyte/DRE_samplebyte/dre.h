/*
* nomal
*/
#include <string>
#include <iostream>
#include <fstream>
#include <pcap.h>

#include <hash_map>
#include <stdint.h>

using namespace std;
using namespace stdext;     //hashmap 비주얼 스튜디오 런 컴파일러

#define MAX_TABLE 256
using namespace std;

class DRE
{
public:
	ofstream packet_out;
	time_t start_time, finish_time;

	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t * pcap;
	struct pcap_pkthdr *header;	// header of packet

	const u_char *data;		// data of packet
	u_int packetCount;
	
	DRE();	// constructor
	~DRE();	// destructor

	int Store_Packet_Normal(string pcap_file, string out_file);

	int Store_Packet_SAMPLEBYTE(string pcap_file, string out_file);

	int DRE::Making_LOOKUP(string pcap_file, string out_file, BOOLEAN *LOOKUPTABLE);
	
};
