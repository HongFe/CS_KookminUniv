#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY 1610600000 //1.5GB

char Packet_Buffer[MAX_MEMORY];

int main(int argc, char *argv[]){

	FILE *r_pcap,*w_pcap;
	struct timeval start_point, end_point;
	double operating_time;
	int buf_size=0;
	char ch;

	if(argc < 3){
		printf("./pg [ReadFile] [WriteFile]\n");
		return 1;
	}
	
	r_pcap = fopen(argv[1], "r");
	w_pcap = fopen(argv[2], "w");

	 int nRead = 0;
        while( nRead = fread( &Packet_Buffer[buf_size], sizeof(char), 1, r_pcap ) ) 
		buf_size++;
	
	

	/*-----------------Writting RAW DATA-----------------*/
	//file open
	gettimeofday(&start_point, NULL);

	fwrite (Packet_Buffer , 1 , buf_size-1 , w_pcap );

	gettimeofday(&end_point, NULL);	
	
	printf("%d\n",buf_size);

   	fclose (r_pcap);
	fclose (w_pcap);
	/*---------------------------------------------------*/
	operating_time = (double)(end_point.tv_sec) + (double)(end_point.tv_usec) / 1000000.0 - (double)(start_point.tv_sec) - (double)(start_point.tv_usec) / 1000000.0;
	
	printf("File size : %d\n ",buf_size+1);
	printf("Operation Time(s) : %f\n ",operating_time);
	return 0; //done


}
