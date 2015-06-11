#include <string>
#include <iostream>
#include <fstream>
#include <pcap.h>

#include <stdint.h>

#define MAX_TABLE 256

#define PACKET_FILE "./10000packet.pcap"
#define NORMAL_FILE "./pcaket_storage_normal.txt"
#define MAX_MEMORY 209715200 // 200mb

char BUFFER[MAX_MEMORY];

int Store_Packet_Normal(string pcap_file, string out_file);

int main(int argc, char *argv[])
{

	if (Store_Packet_Normal(PACKET_FILE, NORMAL_FILE) == 0)
		cout << "clear normal!" << endl << endl;

	return 0;
}


int Store_Packet_Normal(char* pcap_file, char* out_file)
{
	FILE packet_out;
	time_t start_time, finish_time;

	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t * pcap;
	struct pcap_pkthdr *header;	// header of packet

	const u_char *data;		// data of packet
	u_int packetCount;
	
	struct timeval start_time, end_time;
	double operating_time;


	pcap = pcap_open_offline(pcap_file.c_str(), errbuff);
	packet_out.open(out_file);
	
	int chunk_begin = 0;
	packetCount = 0;

	while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
	{
		string data_string(reinterpret_cast<const char *>(data), header->caplen);  // u_char to strting
		packet_out << "Packet # " << ++packetCount << endl;
		packet_out << "Packet size: " << header->len << endl;
		if (header->len != header->caplen)
			packet_out << "Warning! Capture size different than packet size: " << header->len << "bytes" << endl;

		/* save byte*/
		chunk_begin = 0;
		while (chunk_begin < header->caplen)
		{
			packet_out << (int)data[chunk_begin] << " ";
			chunk_begin++;
		}

		/*-----------------Writting RAW DATA-----------------*/
		gettimeofday(&start_time, NULL);
		/* RAW data save*/
		//		packet_out << data_string;

		gettimeofday(&end_time, NULL);
		/*---------------------------------------------------*/
		operating_time = (double)(end_point.tv_sec) + (double)(end_point.tv_usec) / 1000000.0 - (double)(start_point.tv_sec) - (double)(start_point.tv_usec) / 1000000.0;
		
	}
	packet_out.close();
	pcap_close(pcap);

	return 0;
}
























