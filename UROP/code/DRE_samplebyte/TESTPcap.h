#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif


