#ifndef SOCK_UTIL
#define SOCK_UTIL

/* Function Declarations *****************************************************/
#define TCP_BUFF 1024

// Connects to host on service. Host and service use same convention as getaddrinfo
int connect_to_server(char* host, char* service);

// Reads all buffered data on a socket until there is none left ?
char* read_tcp(int sockfd);

#endif