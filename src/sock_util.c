#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "sock_util.h"
#include "log.h"


int connect_to_server(char* host, char* service) {

	log_msg(LOG_INFO, "Connecting to %s on %s", host, service);
	int sockfd, s;
	struct addrinfo hints, *servinfo, *rp;

	// Create address
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;
	

	// Get addrinfo of server. From man page:
	// The getaddrinfo() function combines the functionality provided by the
	// gethostbyname(3) and getservbyname(3) functions into a single interface
	s = getaddrinfo(host, service, &hints, &servinfo);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	// Connect to first valid result
	// Why are there multiple results? see man page (search 'several reasons')
	// How to search? enter /, then text to search for, press n/N to navigate
	for (rp = servinfo; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1)
			continue;
		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break; // success

		close(sockfd);
	}
	if (sockfd < 0) exit(EXIT_FAILURE);
	if (rp == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(servinfo);
	return sockfd;
}


char* read_tcp(int sockfd) {
	char temp[TCP_BUFF];
    char* buffer = calloc(TCP_BUFF, sizeof(char));
	assert(buffer);
    int n = 0,
        read = 0,
        buff_size = TCP_BUFF;

    while((n = recv(sockfd, temp, TCP_BUFF, 0)) > 0) {

        if (read >= buff_size) {
            buff_size = buff_size * 2;
            buffer = realloc(buffer, buff_size);
            assert(buffer);
        }
        memcpy(buffer + read, temp, n);
        read = read + n;
    } 
	buffer[read] = '\0';
	return buffer;
}