#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BIT_MASK 33554431 //  3bytes = '1' 

char P_BUFFER[BIT_MASK];

int main(){

	FILE *pcap,*wpcap;
	struct timeval start_point, end_point;
	int buf_size;

	pcap = fopen("./020747.pcap", "r");
	wpcap = fopen("text.txt", "w");

	while (1){

		
	}

	gettimeofday(&start_point, NULL);

	fwrite(P_BUFFER, 1, buf_size, wpcap);

	gettimeofday(&end_point, NULL);



}