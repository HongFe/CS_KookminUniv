/*
* DRE : SAMPLEBYTE
*/
#include "dre.h"

#define PACKET_FILE "E:\\M\\Kookm\\4-1\\UROP\\pcap file\\10000packet.pcap"
#define NORMAL_FILE "E:\\M\\Kookm\\4-1\\UROP\\work\\DRE_samplebyte\\DRE_samplebyte\\pcaket_storage_normal.txt"
#define SAMPLE_FILE "E:\\M\\Kookm\\4-1\\UROP\\work\\DRE_samplebyte\\DRE_samplebyte\\pcaket_storage_SAMPLEBYTE.txt" 
#define FINGER_FILE "E:\\M\\Kookm\\4-1\\UROP\\work\\DRE_samplebyte\\DRE_samplebyte\\fingerprint.txt" 

int main(int argc, char *argv[])
{
	DRE dre;
	
	if (dre.Store_Packet_Normal(PACKET_FILE, NORMAL_FILE) == 0)
		cout << "clear normal!" << endl << endl;
	if (dre.Store_Packet_SAMPLEBYTE(PACKET_FILE, SAMPLE_FILE) == 0)
		cout << "clear samplebyte!" << endl;
	
	return 0;
}
 