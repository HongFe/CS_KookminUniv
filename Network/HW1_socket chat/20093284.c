/*
Student ID : 20093284
Name : Na Hong Cheol
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display();


int peertcpSocket = -1;	// peer socket

int main(int argc, char **argv) {

	int tcpServ_sock;

	struct sockaddr_in tcpServer_addr;
	struct sockaddr_in tcpClient_addr;
	struct sockaddr_in newTcp_addr;

	// int clnt_len; -> just intager	
	socklen_t clnt_len; 	// length for socket length

	fd_set reads, temps;
	int fd_max;

	char command[1024];

	char *tcpport = NULL;
	char *userid = NULL;

	// NEED TO ADD SOME VARIABLES 
	int bytesread;
	char tempc[1024];
	char *token = NULL;
	struct hostent *hostp;

	if (argc != 3){
		printf("Usage : %s <tcpport> <userid>\n", argv[0]);
		exit(1);
	}
	tcpport = argv[1];
	userid = argv[2];

	display();
	// NEED TO CREATE A SOCKET FOR TCP SERVER	

	tcpServ_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (tcpServ_sock < 0){
		perror("socket");
		exit(1);
	}
	memset((void *)&tcpServer_addr, 0, sizeof(tcpServer_addr));	// Memory allocation to Server addr struct
	tcpServer_addr.sin_family = AF_INET;			// Address system : AF_INDET - ipv4 internet protocol
	tcpServer_addr.sin_addr.s_addr = INADDR_ANY;		// sin_addr - 32bit ip address
	tcpServer_addr.sin_port = htons((u_short)atoi(tcpport)); // sin_port - 16bit port

	// NEED TO bind // Connect socket with port
	if (bind(tcpServ_sock, (struct sockaddr *)&tcpServer_addr, sizeof(tcpServer_addr)) < 0){
		perror("bind");
		exit(1);
	}

	// NEED TO listen // Request for kernel to open
	if (listen(tcpServ_sock, SOMAXCONN) < 0){
		perror("listen");
		exit(1);
	}

	// initialize the select mask variables and set the
	// mask with stdin and the tcp server socket

	FD_ZERO(&reads);
	FD_SET(fileno(stdin), &reads);
	FD_SET(peertcpSocket,&reads);
	FD_SET(tcpServ_sock, &reads);
	
	fd_max = tcpServ_sock;

	printf("%s> \n", userid);

	while (1) {
	
		int nfound;
		temps = reads;
		nfound = select(fd_max + 1, &temps, 0, 0, NULL);
		if (nfound < 0) {
			if (errno == EINTR) {
				printf("interrupted system call\n");
				continue;
			}
			/* something is very wrong! */
			perror("select");
			exit(1);
		}

		if (FD_ISSET(fileno(stdin), &temps)) {
			// Input from the keyboard			
			if (!fgets(command, sizeof (command), stdin)) {
				if (ferror(stdin)) {
					perror("stdin");
					exit(1);
				}
				exit(0);			
			}
			strcpy(tempc,command);	// command copy to temp 
			
			if (strcmp(tempc, "@quit\n") == 0){	// '@quit' command.
				close(tcpServ_sock);
				if(peertcpSocket)
					close(peertcpSocket);				
				exit(1);
			}
			
			token = strtok(tempc, " ");	// Split Command
			
			if (strcmp(token, "@talk") == 0){	// If '@talk' commnad. Create New socket for connection to server.
				if ((peertcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
					perror("socket");
					exit(1);
				}

				if ((hostp = gethostbyname(strtok(NULL, " "))) == 0) {	//
					fprintf(stderr, "unknown host\n");
					exit(1);
				}

				memset((void *)&tcpClient_addr, 0, sizeof (tcpClient_addr));
				tcpClient_addr.sin_family = AF_INET;
				memcpy((void *)&tcpClient_addr.sin_addr, hostp->h_addr, hostp->h_length);
				tcpClient_addr.sin_port = htons((u_short)atoi(strtok(NULL, " ")));

				if (connect(peertcpSocket, (struct sockaddr *)&tcpClient_addr, sizeof (tcpClient_addr)) < 0) {
					(void)close(peertcpSocket);
					fprintf(stderr, "connect");
					exit(1);					
				}
				if (peertcpSocket > fd_max)
					fd_max = peertcpSocket;		
				FD_SET(peertcpSocket, &reads);
				
				printf("%s> \n", userid);
				continue;
			}
			
			// Wtire in buffer to send.			
			char *t = " : ";
			write(peertcpSocket, argv[2], strlen(argv[2]));
			write(peertcpSocket, t, strlen(t));
			write(peertcpSocket, command, strlen(command));

			FD_CLR(fileno(stdin), &temps);

			printf("%s> \n", userid);
		}
		else if (FD_ISSET(tcpServ_sock, &temps))
		{
			//connect request from a peer
			// a new connection is available on the connetion socket //
			clnt_len = sizeof(newTcp_addr);
			peertcpSocket = accept(tcpServ_sock,
				(struct sockaddr *)&newTcp_addr, &clnt_len);
			if (peertcpSocket < 0) {
				perror("accept");
				exit(1);
			}
			printf("connection closed %s, port %d, socket %d\n",
				inet_ntoa(newTcp_addr.sin_addr), ntohs(newTcp_addr.sin_port),
				peertcpSocket);

			FD_SET(peertcpSocket, &reads);
			if (peertcpSocket > fd_max)
				fd_max = peertcpSocket;

			FD_CLR(tcpServ_sock, &temps);
		}

		else if (FD_ISSET(peertcpSocket, &temps))
		{
			// message from a peer
			bytesread = read(peertcpSocket, command, sizeof command);
			if (bytesread < 0) {
				perror("read");
				// fall through //
			}
			if (bytesread <= 0) {
				printf("connection closed %d\n", peertcpSocket);
				FD_CLR(peertcpSocket, &reads);
				if (close(peertcpSocket)) perror("close");
				continue;
			}
			command[bytesread] = '\0';
			printf("%s\n", command);
			FD_CLR(peertcpSocket, &temps);
		}
	}//while End
}//main End

void display() {
	printf("Student ID : 20093284 \n");
	printf("Name : Na Hong Cheol  \n");
}



