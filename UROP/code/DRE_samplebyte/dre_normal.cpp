/*
* nomal
*/
#include "dre.h"


DRE::DRE()
{
	packetCount = 0;
	start_time = NULL;
	finish_time = NULL;
}


DRE::~DRE()
{

}

int DRE::Store_Packet_Normal(string pcap_file, string out_file)
{
	pcap = pcap_open_offline(pcap_file.c_str(), errbuff);
	packet_out.open(out_file);

	start_time = time(0);
	packet_out << "start time : " << start_time << endl;

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
		while (chunk_begin < header->caplen )
		{
			packet_out << (int)data[chunk_begin] << " ";
			chunk_begin++;
		}
		
		/* RAW data save*/
//		packet_out << data_string;
		packet_out << endl << endl;
		
	}
	finish_time = time(0);
	packet_out << "end time : " << finish_time << endl;
	packet_out << "time : " << finish_time - start_time << endl;
	
	/* print time */
	cout << "start time : " << start_time << endl;
	cout << "end time : " << finish_time << endl;
	cout << "time : " << finish_time - start_time << endl;

	packet_out.close();
	pcap_close(pcap);

	return 0;
}


