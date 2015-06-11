/*
* DRE : SAMPLEBYTE
	packet의 처음 byte건너띈다.
	pcaket의 마지막 byte는 검사하지않고 마지막 chunk의 끝
*/
#include "dre.h"

/* Jenkins Hash */
uint32_t jenkins_one_at_a_time_hash(char *key, size_t len)
{
	uint32_t hash, i;
	for (hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}


int DRE::Store_Packet_SAMPLEBYTE(string pcap_file, string out_file)
{
	int p = 32;
	int w = 32;
	packetCount = 0;
	BOOLEAN LOOKUPTABLE[MAX_TABLE] = { FALSE, };	// dlimeter table
	u_int chunk_begin = 0, chunk_end = 0;	// chunk begin & end point
	int data_size=0;
	/*Lookuptable*/
	//Making_LOOKUP(pcap_file, "E:\\M\\Kookm\\4-1\\UROP\\work\\DRE_samplebyte\\DRE_samplebyte\\fingerprint.txt", LOOKUPTABLE);
	
	pcap = pcap_open_offline(pcap_file.c_str(), errbuff);	// open pcap file
	packet_out.open(out_file);	// save file

	hash_map<string, int> hashmap1; // hash map
	/*
	u_int fingerprint=0;
	u_int finger_table[10000];
	*/

	/* Marking LOOKUPTABLE */
	
	int ranNum = 0;
	packet_out << "marking number ";
	for (int i = 0; i < 50; i++)
	{
		ranNum = rand() % MAX_TABLE;
		LOOKUPTABLE[ranNum] = TRUE;
		packet_out << ranNum << " ";
	}
	packet_out << endl;
	
	/* --------------------------- */

	start_time = time(0);
	packet_out << "start time : " << start_time << endl;


	/* Packet Checking */
	while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
	{
		packet_out << "Packet # " << ++packetCount << endl;
		packet_out << "Packet size: " << header->len << endl;
		if (header->len != header->caplen)
			packet_out << "Warning! Capture size different than packet size: " << header->len << "bytes" << endl;

		chunk_begin = 0;
		chunk_end = 1;

		string data_string(reinterpret_cast<const char *>(data), header->caplen); // u_char to strting

		data_size = header->caplen;
		/* Checking 1Byte of the packet */
		while (chunk_end < data_size)
		{
			if (LOOKUPTABLE[(u_int)data[chunk_end]] == TRUE)
			{	
				/*
				string chunk = data_string.substr(chunk_begin, chunk_begin - 1);
				hash_map<string, int>::iterator FindIter = hashmap1.find(chunk);
				
				// not found -> insert hash
				if (FindIter == hashmap1.end())
					hashmap1.insert(hash_map<string, int>::value_type(chunk, 1)); // insert hash with fingerprint
				*/
				packet_out << "[b " << (u_int)data[chunk_begin]
					<< "] " << "[e " << (u_int)data[chunk_end - 1] << "] ";
				chunk_begin = chunk_end;
				chunk_end = chunk_end + p / 2;
			}
			else
				chunk_end++;

			//last chunk
			if (chunk_end >= data_size)
			{
				packet_out << "[b " << (u_int)data[chunk_begin]
					<< "][e " << (u_int)data[data_size-1] << "] ";
			}
		}
		packet_out << endl;
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

	
	/* hash print */
	/*
	ofstream hash_out;
	hash_out.open("E:\\M\\Kookm\\4-1\\UROP\\work\\DRE_samplebyte\\DRE_samplebyte\\pcaket_storage_SAMPLEBYTE_hash_table.txt");	// save file
	hash_map<string, int>::iterator Iter1;
	int i=0;
	for (Iter1 = hashmap1.begin(); Iter1 != hashmap1.end(); ++Iter1)
	{
		hash_out << "[" << i++ <<"]" << Iter1->first << endl;
	}
	hash_out.close();
	*/
	return 0;
}


/* LOOKUPTABLE 120~170 selected */
int DRE::Making_LOOKUP(string pcap_file, string out_file, BOOLEAN *LOOKUPTABLE)
{
	int finger_data[256] = { 0, };
	int begin = 0;
	ofstream ofout;
	ofout.open(out_file);

	pcap = pcap_open_offline(pcap_file.c_str(), errbuff);	// open pcap file
	/* Packet Checking */
	while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
	{
		begin = 0;
		/* Checking 1Byte of the packet */
		while (begin < header->caplen - 1)
			finger_data[(u_int)data[begin++]]++;
	}

	pcap_close(pcap);
	
	int num = 0;
	int tmp = 0;
	int index = 0;
	int rank_index[256] = { 0, };
	int rank_data[256] = {0 , };
	for (int i = 0; i < 256; i++)
	{
		index = i;
		finger_data[i]= num;
		for (int j = 0; j < 256; j++)
		{
			if (rank_data[j] > num)
			{
				tmp = rank_index[j];
				rank_index[j] = index;
				index = tmp;

				tmp = rank_data[j];
				rank_data[j] = num;
				num = tmp;
			}
			
		}
	}
	cout << "RANK!! ";
	for (int j = 100; j < 150; j++)
	{
		LOOKUPTABLE[rank_index[j]] = TRUE;
		ofout << rank_index[j] << ":" << rank_data[j] << endl;
	}	
	
	ofout.close();

	return 0;
}