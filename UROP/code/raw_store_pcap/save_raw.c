#include <sys/time.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <pcap/pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#define MAX_MEMORY 10485760 //10mb
// IP 헤더 구조체
struct ip *iph;

// TCP 헤더 구조체
struct tcphdr *tcph;

char P_BUFFER[MAX_MEMORY];

// 패킷을 받아들일경우 이 함수를 호출한다.  
// packet 가 받아들인 패킷이다.
int Packet_check(const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	static int count = 1;
	struct ether_header *ep;
	unsigned short ether_type;
	int chcnt = 0;
	int length = pkthdr->len;
	char ip_s[16], ip_d[16];

	// 이더넷 헤더를 가져온다. 
	ep = (struct ether_header *)packet;

	// IP 헤더를 가져오기 위해서 
	// 이더넷 헤더 크기만큼 offset 한다.   
	packet += sizeof(struct ether_header);

	// 프로토콜 타입을 알아낸다. 
	ether_type = ntohs(ep->ether_type);

	// 만약 IP 패킷이라면 
	if (ether_type == ETHERTYPE_IP)
	{
		// IP 헤더에서 데이타 정보를 출력한다.  
		iph = (struct ip *)packet;
		printf("IP 패킷\n");
		printf("Version     : %d\n", iph->ip_v);
		printf("Header Len  : %d\n", iph->ip_hl);
		printf("Ident       : %d\n", ntohs(iph->ip_id));
		printf("TTL         : %d\n", iph->ip_ttl);
		printf("Src Address : %s\n", inet_ntoa(iph->ip_src));
		printf("Dst Address : %s\n", inet_ntoa(iph->ip_dst));
		// 만약 TCP 데이타 라면
		// TCP 정보를 출력한다. 
		if (iph->ip_p == IPPROTO_TCP)
		{
			tcph = (struct tcp *)(packet + iph->ip_hl * 4);
			printf("Src Port : %d\n", ntohs(tcph->source));
			printf("Dst Port : %d\n", ntohs(tcph->dest));
		}

		// Packet 데이타를 출력한다. 
		// IP 헤더 부터 출력한다.  
		while (length--)
		{
			printf("%02x", *(packet++));
			if ((++chcnt % 16) == 0)
				printf("\n");
		}
	}
	// IP 패킷이 아니라면 
	else
	{
		printf("NONE IP 패킷\n");
	}
	printf("\n\n");

	return 0;
}

int main(int argc, char **argv)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	int ret;
	struct pcap_pkthdr hdr;
	const u_char *packet;

	struct bpf_program fp;

	int buf_size = 0;

	pcap_t *pcd;  // packet capture descriptor

	pcd = pcap_open_offline(argv[1], errbuf);
	if (pcd == NULL)
	{
		printf("%s\n", errbuf);
		exit(1);
	}
	struct pcap_pkthdr *hhdr = &hdr;
	while (packet = pcap_next(pcd, &hdr)){
		Packet_check(hhdr, packet);
		strcat(P_BUFFER, packet);
		buf_size += hhdr->len;
	}
	/*-----------------Writting RAW DATA-----------------*/
	struct timeval start_point, end_point;
	//file open
	FILE *outFile = fopen("packet_file.txt", "w");
	gettimeofday(&start_point, NULL);

	fwrite(P_BUFFER, 1, buf_size, outFile);

	gettimeofday(&end_point, NULL);

	fclose(outFile);
	/*---------------------------------------------------*/
	double operating_time = (double)(end_point.tv_sec) + (double)(end_point.tv_usec) / 1000000.0 - (double)(start_point.tv_sec) - (double)(start_point.tv_usec) / 1000000.0;
	printf("Writing Time : %f\n", operating_time);
	return 0;
}