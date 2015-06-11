/*
* DRE : SAMPLEBYTE
*/
#include <string>
#include <iostream>
#include <pcap.h>

using namespace std;

int packet_info()
{
	string pcap_file = "E:\\M\\Kookm\\4-1\\UROP\\pcap file\\1000packet.pcap";
	/*
	* Step 3 - Create an char array to hold the error. 256bytes
	*/
	char errbuff[PCAP_ERRBUF_SIZE];

	/*
	* Step 4 - Open the file and store result in pointer to pcap_t
	*/
	// Use pcap_open_offline
	pcap_t * pcap = pcap_open_offline(pcap_file.c_str(), errbuff);

	/*
	* Step 5 - Create a header and a data object
	*/
	// Create a header object:
	struct pcap_pkthdr *header;

	// Create a character array using a u_char
	// u_char is defined here:
	// C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\WinSock2.h
	// typedef unsigned char   u_char;
	const u_char *data;

	/*
	* Step 6 - Loop through packets and print them to screen
	*/
	u_int packetCount = 0;

	while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
	{
		// Show the packet number
		printf("Packet # %i\n", ++packetCount);

		// Show the size in bytes of the packet
		printf("Packet size: %d bytes\n", header->len);

		// Show a warning if the length captured is different
		if (header->len != header->caplen)
			printf("Warning! Capture size different than packet size: %ld bytes\n", header->len);

		// Show Epoch Time
		printf("Epoch Time: %d:%d seconds\n", header->ts.tv_sec, header->ts.tv_usec);

		// loop through the packet and print it as hexidecimal representations of octets
		// We also have a function that does this similarly below: PrintData()
		for (u_int i = 0; (i < header->caplen); i++)
		{
			// Start printing on the next after every 16 octets
			if ((i % 16) == 0) printf("\n");

			// Print each octet as hex (x), make sure there is always two characters (.2).
			printf("%.2x ", data[i]);
		}

		// Add two lines between packets
		printf("\n\n");
	}

	return 0;
}
