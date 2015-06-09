#include "server.h"

void tokenize(char* buf, int idx) {
	char** line;
	char** space;
    
	int count = 0;
    
	line = str_split(buf, "\r\n");
	// TODO 에러처리
	space = str_split(line[0], " ");
	// TODO 에러처리
    
	while (TRUE) {
		if (line[count] == NULL)
			break;
		count++;
	}
	count--;
	if (strcmp(space[0], REQ) == 0) {
		request(space[1], line[count], idx);
	} else if (strcmp(space[0], RES) == 0) {
		response(space[1], line[count], idx);
	} else {
		//TODO 에러처리
	}
    
}

void request(char* command, char *body, int idx) {
	int i, flag = FALSE, tempSocket = 0;
	char request[1000];
    char charTemp[1000];
	char **temp;
	strtok(body, "\n"); // Remove New Line
	if (strcmp(INIT, command) == 0) {
		temp = str_split(body, " ");
		strcpy(user[idx].userId, temp[0]);
		user[idx].addr.sin_port = htons(atoi(temp[1]));
        user[idx].isAlive = TRUE;
        
		if (write(user[idx].socket, RES_MSG_OK, strlen(RES_MSG_OK)) < 0) {
			perror("Error :  [Server Socket] write( ) function \n");
			exit(1);
		}
	} else if (strcmp(INVITE, command) == 0) {
		// 요청받은 UserID 를 찾는다
		for (i = 0; i < numOfClient; i++) {
			if (strcmp(body, user[i].userId) == 0) {
				// 대상 User가 채팅 중이라면 에러
				if (user[i].chatFlag) {
					if (write(user[idx].socket, RES_MSG_ERROR, strlen(RES_MSG_ERROR)) < 0) {
						perror("Error :  [Server Socket] write( ) function \n");
						exit(1);
					}
				} else {
					if (user[idx].chatFlag == FALSE) {
						user[idx].hAddr.sin_addr = user[idx].addr.sin_addr;
						user[idx].hAddr.sin_port = user[idx].addr.sin_port;
						user[idx].hAddr.sin_family = user[idx].addr.sin_family;
						user[idx].chatFlag = TRUE;
					}
					sprintf(request, REQ_MSG_CONNECT,
							inet_ntoa(user[idx].hAddr.sin_addr),
							htons(user[idx].hAddr.sin_port));
					if (write(user[i].socket, request, strlen(request)) < 0) {
						// TODO 클라이언트가 메세지 받아서 호스트랑 연결취해야함
						perror("Error :  [Server Socket] Invite write( ) function \n");
						exit(1);
					}
					user[i].hAddr.sin_addr = user[idx].hAddr.sin_addr;
					user[i].hAddr.sin_port = user[idx].hAddr.sin_port;
					user[i].hAddr.sin_family = user[idx].hAddr.sin_family;
					user[i].chatFlag = TRUE; // 응답을 받은셈치고 트루로 바꿈
				}
			}
		}
	} else if (strcmp(LEAVE, command) == 0) {
        // 내가 호스트 클라이언트면
		if (strcmp(inet_ntoa(user[idx].addr.sin_addr), inet_ntoa(user[idx].hAddr.sin_addr)) == 0) {
			user[idx].chatFlag = FALSE;
			for (i = 0; i < numOfClient; i++) {
				if (user[i].chatFlag == TRUE && strcmp(inet_ntoa(user[idx].addr.sin_addr), inet_ntoa(user[i].hAddr.sin_addr)) == 0) {
					if (flag == FALSE) {
						user[i].hAddr.sin_addr = user[i].addr.sin_addr;
						user[i].hAddr.sin_port = user[i].addr.sin_port;
						user[i].hAddr.sin_family = user[i].addr.sin_family;
						tempSocket = i;
						flag = TRUE;
					} else {
						sprintf(request, REQ_MSG_CONNECT,
								inet_ntoa(user[tempSocket].hAddr.sin_addr),
								htons(user[tempSocket].hAddr.sin_port));
						if (write(user[i].socket, request, strlen(request)) < 0) {
							// TODO 클라이언트가 메세지 받아서 호스트랑 연결취해야함
							perror("Error :  [Server Socket] Leave write( ) function \n");
							exit(1);
						}
						user[i].hAddr.sin_addr = user[tempSocket].addr.sin_addr;
						user[i].hAddr.sin_port = user[tempSocket].addr.sin_port;
						user[i].hAddr.sin_family =
                        user[tempSocket].addr.sin_family;
					}
				}
                
			}
		}
        user[idx].isAlive = FALSE;
	} else if (strcmp(LIST, command) == 0) {
		memset((void *) &charTemp, 0, sizeof(charTemp));
		for (i = 0; i < numOfClient; i++) {
			if (user[i].isAlive == TRUE)
				sprintf(charTemp, "%s %s %s\t", charTemp, user[i].userId, user[i].chatFlag ? "chating" : "not chating");
		}
		// DEBUG PRINTF
		/*
			printf("%s\n", charTemp);
		 */
        sprintf(request, RES_MSG_OK_LIST, charTemp);
        if (write(user[idx].socket, request, sizeof(request)) < 0) {
            perror("Error :  [Server Socket] write( ) function \n");
            exit(1);
        }
	} else {
		//error
	}
}
void response(char* command, char *body, int idx) {
	if (strcmp( ROK, command) == 0) {
        
	} else if (strcmp(ERROR, command) == 0) {
        
	} else {
		//error
	}
}
