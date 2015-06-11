// Student ID : 20093284
// Name : Na Hong Cheol

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

#define PROMPT() {printf("\n>");fflush(stdout);}
#define GETCMD "load"
#define QUITCMD "quit\n"
#define LOCATION "/home/fe/socket/hw2"

int portnum = 0;


int main(int argc, char *argv[])
{
	struct sockaddr_in server, remote;  // server-server, remote-client
	int request_sock, new_sock, fromserver_sock = -1; // request-server, new-new tcp connect socket for server, fromserver-client
	int nfound, maxfd, bytesread;//, addrlen, fd;
	socklen_t clnt_len; 	// length for socket length	
	char buf[BUFSIZ];
	fd_set rmask, mask;

	char *token = NULL;
	char tempc[BUFSIZ];
	struct hostent *hostp;
	FILE *rf;

	char fn[20], *v, *code = "";
	char *temC;
	int er = 0; // server error counter

	char ch[100000] = "";	// data
	char ch1[1];	// copy to write
	int cnt = 0;
	int cnt1 = 0;



	if (argc != 2) {
		(void)fprintf(stderr, "usage: %s portnum \n", argv[0]);
		exit(1);
	}

	portnum = atoi(argv[1]);

	printf("Student ID : 20093284\n");
	printf("Name : Na Hong Cheol\n");

	// Create a Server Socket
	request_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (request_sock < 0){
		perror("socket");
		exit(1);
	}
	memset((void *)&server, 0, sizeof(server));	// Memory allocation to Server addr struct
	server.sin_family = AF_INET;			// Address system : AF_INDET - ipv4 internet protocol
	server.sin_addr.s_addr = INADDR_ANY;		// sin_addr - 32bit ip address
	server.sin_port = htons((u_short)portnum); // sin_port - 16bit port

	// NEED TO bind // Connect socket with port
	if (bind(request_sock, (struct sockaddr *)&server, sizeof(server)) < 0){
		perror("bind");
		exit(1);
	}

	// NEED TO listen // Request for kernel to open
	if (listen(request_sock, SOMAXCONN) < 0){
		perror("listen");
		exit(1);
	}

	// set the mask
	FD_ZERO(&mask);
	FD_SET(fileno(stdin), &mask);
	FD_SET(request_sock, &mask);
	maxfd = request_sock;

	PROMPT();

	for (;;) {
		rmask = mask;
		nfound = select(maxfd + 1, &rmask, (fd_set *)0, (fd_set *)0, NULL);

		if (nfound < 0) {
			if (errno == EINTR) {
				printf("interrupted system call\n");
				continue;
			}
			// something is very wrong! //
			perror("select");
			exit(1);
		}
		// check for stdin
		if (FD_ISSET(fileno(stdin), &rmask)) {
			// Input from the keyboard			
			if (!fgets(buf, sizeof (buf), stdin)) {
				if (ferror(stdin)) {
					perror("stdin");
					exit(1);
				}
				exit(0);
			}

			strcpy(tempc, buf);	// buf copy to temp 

			if (strcmp(tempc, QUITCMD) == 0){	// 'quit' command.				
				if (new_sock)
					close(new_sock);
				if (fromserver_sock)
					close(fromserver_sock);
				close(request_sock);
				exit(1);
			}

			token = strtok(tempc, " ");	// Split Command

			if (strcmp(token, GETCMD) == 0){	// If 'load' commnad. Create New socket for connection to server.
				if ((fromserver_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
					perror("socket");
					exit(1);
				}
				char *hostn;
				hostn = strtok(NULL, " ");
				if ((hostp = gethostbyname(hostn)) == 0) {	// Host address
					fprintf(stderr, "unknown host\n");
					exit(1);
				}

				memset((void *)&remote, 0, sizeof (remote));
				remote.sin_family = AF_INET;
				memcpy((void *)&remote.sin_addr, hostp->h_addr, hostp->h_length);
				remote.sin_port = htons((u_short)atoi(strtok(NULL, " ")));

				// connect to server
				if (connect(fromserver_sock, (struct sockaddr *)&remote, sizeof (remote)) < 0) {
					(void)close(fromserver_sock);
					fprintf(stderr, "connect");
					exit(1);
				}

				if (fromserver_sock > maxfd)
					maxfd = fromserver_sock;
				FD_SET(fromserver_sock, &mask);
				// write header into buf
				sprintf(buf, "GET %s HTTP/1.0\r\nHost: localhost\r\nUser-agent: HW2/1.0\r\nConnection: close\r\n\r\n", strtok(NULL, "\n"));
				// write html file name in buffer
				write(fromserver_sock, buf, strlen(buf));	// sending GET message				
			}
			FD_CLR(fileno(stdin), &rmask);
		}
		// check for request socket
		else if (FD_ISSET(request_sock, &rmask)){
			//connect request from a peer
			// a new connection is available on the connetion socket //
			clnt_len = sizeof(remote);
			new_sock = accept(request_sock,
				(struct sockaddr *)&remote, &clnt_len);
			if (new_sock < 0) {
				perror("accept");
				exit(1);
			}
			printf("connection : Host IP %s, port %d, socket %d\n",
				inet_ntoa(remote.sin_addr), ntohs(remote.sin_port),
				new_sock);

			FD_SET(new_sock, &mask);
			if (new_sock > maxfd)
				maxfd = new_sock;

			FD_CLR(request_sock, &rmask);
		}
		// check for the connection to the server
		else if (FD_ISSET(fromserver_sock, &rmask)){
			// message from a peer
			bytesread = read(fromserver_sock, buf, sizeof buf);
			if (bytesread < 0) {
				perror("read");
				// fall through //
			}
			buf[bytesread] = '\0';
			printf("%s", buf);	// print header+data from server

			FD_CLR(fromserver_sock, &rmask); // disconnect clearly
			FD_CLR(fromserver_sock, &mask); // disconnect clearly
			if (close(fromserver_sock)) perror("close");

			PROMPT();
		}

		// check for the connection from the client
		else if (FD_ISSET(new_sock, &rmask)){

			// message from a client
			bytesread = read(new_sock, buf, sizeof buf);
			if (bytesread < 0) {
				perror("read");
				// fall through //
			}

			buf[bytesread] = '\0';
			printf("%s", buf);
			/////////////////////// send header+data//////////////

			char dir[] = LOCATION;	// direction of root web folder

			cnt = 0;	 // file size for saving
			cnt1 = 0;	 // file size for sending

			temC = strtok(buf, " ");	// "GET"
			temC = strtok(NULL, " ");	// file name
			strcpy(fn, temC);
			strcat(dir, fn);		// file direction

			rf = fopen(dir, "r");
			// if not existing file
			if (rf == NULL){
				code = "404 NOT FOUND";
				er = 1;
			}
			// existing file
			else{
				code = "200 OK";
				er = 0;
				while ((ch[cnt] = fgetc(rf)) != EOF)
					cnt++;	// read file each byte

				fclose(rf);
			}

			v = strtok(NULL, "\r\n"); // version

			temC = strtok(NULL, " "); // HOST:
			temC = strtok(NULL, "\r\n"); // host name

			temC = strtok(NULL, " "); // User-agent:
			temC = strtok(NULL, "\r\n"); // user agent

			temC = strtok(NULL, " "); // Connection:
			temC = strtok(NULL, "\r\n"); // satus

			// write header into buf
			sprintf(buf, "%s %s\r\nConnection: %s\r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n", v, code, temC, cnt);
			write(new_sock, buf, strlen(buf)); // sending header to client
			
			while (cnt1 != cnt){
				ch1[0] = ch[cnt1];
				write(new_sock, ch1, 1);	// sending each byte of data to client			
				cnt1++;
			}
			if (er == 0)
				printf("\nfinish %d %d \n", cnt, cnt1);	// existing file
			else
				printf("\nServer Error : No such file .%s!\n", fn);	//error not existing file

			FD_CLR(new_sock, &rmask); // disconnect clearly
			FD_CLR(new_sock, &mask); // disconnect clearly
			if (close(new_sock)) perror("close");

			PROMPT();
		}
	}

} /* main - hw2.c */

