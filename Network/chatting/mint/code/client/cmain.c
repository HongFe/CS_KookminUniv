#include <stdio.h>
#include "client.h"

ClientInfo user[10];

int main(int argc, const char * argv[])
{
    struct sockaddr_in loginServer,host,client; // Server 의 Socket addr
    int hostSocket,loginServerSocket; // 서버 연결용 소켓 peer는 로그인서버랑 연결된 소켓 , hostSocket은 내가 호스트가 될경우 연결용소켓
    int clientSocket=-1; // 내가 클라이언트일때 호스트랑 연결하는 소켓이에요
    struct hostent *hostp; // new connect server use

    numOfClient=0;
    char **line;
    char **space;
	char buf[1024];
    char req[1000];
    
	fd_set temps, reads;
	
    int maxfd;
    long byteread;
    
    int i,j,port;
    char ID[10]; //ID 제한은 10글자
    
    socklen_t client_len;
    
    printf("=========================\n");
	printf("\tMint\n20093267 Sung Geun Kim\n20093268 Se Hoon Kim\n20093284 Hong Cheol Na\n");
    printf("=========================\n");
	/**
     *	Make Server
     *	Create(Srever)-> Bind(Server) -> Listen(From Client) ->
     *	Accept(From Client) -> Work(Server)
     */
	// Create(Server)
	if ((hostSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error : [Server Socket] doesn't create\n");
		exit(1);
	} // End Create

	if ((loginServerSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error : [Server Socket] doesn't create\n");
		exit(1);
	}
    PROMPT();
    printf("ID: ");
    scanf("%s",ID);
    strtok(ID, "\n");

    PROMPT();
    printf("Port : ");
    scanf("%d",&port);
    
	// Setting Address Structure
	memset((void *) &host, 0, sizeof(host)); // initiliaze variable tcpServer_addr to 0
	host.sin_family = PF_INET;
	host.sin_addr.s_addr = htonl(INADDR_ANY); // set IP Address
	host.sin_port = htons(port); // set port number
    
	// Bind(Server) Start
	if (bind(hostSocket, (struct sockaddr *) &host, sizeof(host)) < 0) {
		perror("Error : [Server Socket] dosen't bind\n");
		exit(1);
	} // Bind(Server) End
    
	// Wait Listen(From Client)
	if (listen(hostSocket, SOMAXCONN) < 0) {
		perror("Error : [Server Socket] doesn't listen\n");
		exit(1);
	} // Wait Listen(From Client) End
    
	FD_ZERO(&reads); // FD_ZERO - 모든 소켓 기술자를 세크(read)에서 제거합니다.
	FD_SET(hostSocket, &reads); // FT_SET - 지정된 소켓 기술자(tcpServ_sock)를 세트(read)에 추가한다.
    FD_SET(loginServerSocket,&reads);
	FD_SET(fileno(stdin),&reads);
	
    maxfd = 1000;
    
    // ~ [Start] Connect Login Server
    if ((hostp = gethostbyname(SERVER_IP_ADDR)) == 0) {
		perror("Error : [Client Socket] unknown host\n");
		exit(1);
	}

	memset((void *) &loginServer, 0, sizeof(loginServer));
	loginServer.sin_family = PF_INET;
	memcpy((void *) &loginServer.sin_addr, hostp->h_addr, hostp->h_length);
	loginServer.sin_port = htons(SERVER_PORT);

	if (connect(loginServerSocket, (struct sockaddr *) &loginServer, sizeof(loginServer)) < 0) {
		(void) close(loginServerSocket);
		printf("Error : [Client Socket] Connect\n");
		exit(1);
	}
	sprintf(req, REQ_MSG_INIT, ID, port);
	// ~ [ ID & Port] Send To Login Server (Initiate Message)
	if (write(loginServerSocket, req, strlen(req)) < 0) {
		perror("Error : [Client Socket]  write( ) function\n");
		exit(1);
	}// ~ [End] Connect Login Server
    PROMPT();


    while(TRUE)
    {
		temps = reads;
		select(maxfd + 1, &temps, 0, 0, NULL);
		// ~ Event Key Board
		if (FD_ISSET(fileno(stdin), &temps)) {
			memset(buf, 0, strlen(buf));
			fflush(stdin);
			FD_CLR(fileno(stdin), &temps);

			fgets(buf, sizeof(buf), stdin);
			if (buf == NULL || buf[0] == '\n') {
				continue;
			}
            strtok(buf, "\n");
            space = str_split(buf, " ");

            // ~ Request Invite Message Write Any Client To Login Server
            if((space == NULL) || (space[0] == NULL))
            {
                continue;
            }
			if ( strcmp(space[0], INVITE) == 0 ) {
				sprintf(req, REQ_MSG_INVITE, space[1]);
				if (write(loginServerSocket, req, strlen(req)) < 0) {
					perror("Error : [Client Socket]  write( ) function\n");
					exit(1);
				}
			}
			// ~ Request Leave Message Write Any Client To Login Server
            else if (strcmp(space[0], LEAVE) == 0) {
				if (write(loginServerSocket, REQ_MSG_LEAVE,
						strlen(REQ_MSG_LEAVE)) < 0) {
					perror("Error : [Client Socket]  write( ) function\n");
					exit(1);
				}
				if (numOfClient == 0) {
					if (close(clientSocket))
						perror("close");
				}

				if (close(hostSocket))
					perror("close");

				for (i = 0; i < numOfClient; i++) {
					FD_CLR(user[i].socket, &reads);
					if (close(user[i].socket))
						perror("close");
				}
                exit(1);
			}
            // Send list message to server
            else if ( strcmp(space[0], LIST) == 0 ) {
                if (write(loginServerSocket, REQ_MSG_LIST,
                          strlen(REQ_MSG_LIST)) < 0) {
					perror("Error : [Client Socket]  write( ) function\n");
					exit(1);
				}
            }
            
			// ~ Request Text Message Case 1. Host Client Write All Client,
			// Case2. client Write to Host
            else {
				sprintf(req, REQ_MSG_MSG, buf);
				for (i = 0; i < numOfClient; i++) {
					write(user[i].socket, req, sizeof(req));
				}
				if (numOfClient == 0) {
					write(clientSocket, req, sizeof(req));
				}
			}
			PROMPT();
        }
		// ~ Event From Login Sever
        else if (FD_ISSET(loginServerSocket, &temps)) {
			read(loginServerSocket, buf, sizeof(buf) - 1);
			line = str_split(buf, "\r\n");
			space = str_split(line[0], " ");
            
            if (strcmp(space[0], REQ) == 0 && strcmp(space[1], CONNECT) == 0) {
				space = str_split(line[2], " ");

				if ((hostp = gethostbyname(space[0])) == 0) {
					perror("Error : [Peer Socket] unknown host\n");
					exit(1);
				}

				memset((void *) &client, 0, sizeof(client));
				client.sin_family = PF_INET;
				memcpy((void *) &client.sin_addr, hostp->h_addr, hostp->h_length);
				client.sin_port = htons(atoi(space[1]));
                
                if (clientSocket != -1) {
					close(clientSocket);
					FD_CLR(clientSocket, &reads);
					clientSocket = -1;
				}
                
                if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
					perror("Error : [Peer Socket] doesn't create\n");
					exit(1);
				}
                
                if (connect(clientSocket, (struct sockaddr *) &client, sizeof(client)) < 0) {
					(void) close(clientSocket);
					printf("Error : [Peer Socket] connect\n");
					exit(1);
				}
				FD_SET(clientSocket, &reads);
            }else if(strcmp(space[0], RES) == 0 && strcmp(space[1], "201") == 0){
            	// Server로부터 ListClient 받아옴
                line = str_split(line[2], "\t");
                i=0;
                while(line[i] != NULL)
                {
                    printf("%s\n",line[i]);
                    i++;
                }
                PROMPT();
            }
        }
		// ~ Event From Client ( It is host )
		else if (FD_ISSET(hostSocket, &temps)) {
			// Accept (From Host) start
			client_len = sizeof(client);
			user[numOfClient].socket = accept(hostSocket, (struct sockaddr *) &user[numOfClient].addr, &client_len);
			if (user[numOfClient].socket < 0) {
				perror("Error : [Host Socket]  Doesn't accept");
				exit(1);
			}// Accept (From Host) end

			// DEBUG PRINTF
			/*printf("Connection : Host IP %s, Port %d, socket %d\n",
					inet_ntoa(user[numOfClient].addr.sin_addr),
					ntohs(user[numOfClient].addr.sin_port),
					user[numOfClient].socket);*/
			printf("Connection\n");

			FD_SET(user[numOfClient].socket, &reads);
			if (user[numOfClient].socket > maxfd)
				maxfd = user[numOfClient].socket;
			FD_CLR(hostSocket, &temps);
			numOfClient++;

			PROMPT();
        }
		// ~ Event From Host ( It is client )
		else if (numOfClient == 0 && FD_ISSET(clientSocket, &temps)) {
			byteread = read(clientSocket, buf, sizeof(buf) - 1);
			if (byteread < 0) {
				perror("Error : [Server Socket] read\n");
			}
			if (byteread <= 0) {
				FD_CLR(clientSocket, &reads);
				if (close(clientSocket))
					perror("close");
				continue;
			}

			line = str_split(buf, "\r\n");
			space = str_split(line[0], " ");
			if (strcmp(space[0], REQ) == 0 && strcmp(space[1], MSG) == 0) {
				printf("%s\n", line[2]);
				PROMPT();
				FD_CLR(clientSocket, &temps);
			}
		}
        
		// ~ Client Write message Host Read That
        for (i = 0; i < numOfClient; i++) {
			if (FD_ISSET(user[i].socket, &temps)) {
				byteread = read(user[i].socket, buf, sizeof(buf) - 1);
                
				if (byteread < 0) {
					perror("Error : [Server Socket] read\n");
				}

				if (byteread <= 0) {
					FD_CLR(user[i].socket, &reads);
					if (close(user[i].socket))
						perror("close");
					continue;
				}
				
                line = str_split(buf, "\r\n");
				space = str_split(line[0], " ");
				if (strcmp(space[0], REQ) == 0 && strcmp(space[1], MSG) == 0) {
					printf("%s\n", line[2]);
					for (j = 0; j < numOfClient; j++) {
						if (i != j) {
							write(user[j].socket, buf, sizeof(buf));
						}
					}
					PROMPT();
				}
            }
            
        }
        
    }
    
}

