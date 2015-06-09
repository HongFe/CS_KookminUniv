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
#include <assert.h>

#define TRUE 1
#define FALSE 0
#define PROMPT() {printf("\n>");fflush(stdout);}
#define SERVER_IP_ADDR "localhost"
#define SERVER_PORT 10001
// ~ Message Format
#define REQ "Request"
#define RES "Response"
// ~ Request
#define INIT "@initiate"
#define INVITE "@invite"
#define LEAVE "@leave"
#define LIST "@list"
#define CONNECT "@connect"
#define MSG "@message"
// ~ Response
#define ROK "200"
#define ERROR "400"

#define RES_MSG_OK "Response 200\r\nHost: mint.epril.net\r\n\r\nData"
#define RES_MSG_OK_LIST  "Response 201\r\nHost: mint.epril.net\r\n\r\n%s"
#define RES_MSG_ERROR "Response 400\r\nHost: mint.epril.net\r\n\r\nError Message"
#define REQ_MSG_INIT "Request @initiate\r\nHost: vmint.epril.net\r\n\r\n%s %d"
#define REQ_MSG_CONNECT "Request @connect\r\nHost: mint.epril.net\r\n\r\n%s %d"
#define REQ_MSG_INVITE "Request @invite\r\nHost: mint.epril.net\r\n\r\n%s"
#define REQ_MSG_LEAVE "Request @leave\r\nHost: mint.epril.net\r\n\r\n"
#define REQ_MSG_MSG "Request @message\r\nHost: mint.epril.net\r\n\r\n%s"
#define REQ_MSG_LIST "Request @list\r\nHost: mint.epril.net\r\n\r\n"

typedef struct {
    struct sockaddr_in addr,hAddr;
    int socket;
    char userId[100];
    int isAlive;
    int chatFlag;
} ClientInfo;

ClientInfo user[10];
int numOfClient;

char** str_split(char* a_str,  char* a_delim);
