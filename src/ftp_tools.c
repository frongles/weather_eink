#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


#include "ftp_tools.h"


void get_ftp_code_msg(char* response, char* code, char* msg) {

    strncpy(code, response, 3);
    code[3] = '\0';

    strncpy(msg, response + 4, 128);
    msg[127] = '\0';

    return;
}


void get_pasv_addr_serv(char* response, char* host, char* service) {
    char code[4], msg[128];
    get_ftp_code_msg(response, code, msg);
    if (strcmp(code, "227") != 0) {
        exit(EXIT_FAILURE);
    }
    host[0] = '\0';
    service[0] = '\0';
    char *temp = NULL;
    char* search = strdup(response);
    strtok(search, "(),");
    for (int i = 0; i < 4; i++) {
        temp = strtok(NULL, "(),");
        strcat(host, temp);
        strcat(host, ".");
    }
    host[strlen(host) - 1] = '\0';

    int serv = 0;
    temp = strtok(NULL, "(),");
    serv = atoi(temp) * 256;
    
    temp = strtok(NULL, "(),");
    serv += atoi(temp);
    sprintf(service, "%d", serv);
}


char* read_multiline(int sockfd) {
	char temp[FTP_BUFF];
    char* buffer = calloc(FTP_BUFF, sizeof(char));
	assert(buffer);
    int n = 0,
        read = 0,
        buff_size = FTP_BUFF;

    while((n = recv(sockfd, temp, FTP_BUFF - 1, 0)) > 0) {

        if (read + n >= buff_size) {
            buff_size = buff_size * 2;
            buffer = realloc(buffer, buff_size);
            assert(buffer);
        }
        memcpy(buffer + read, temp, n);
        read = read + n;
        buffer[read] = '\0';
        if (strstr(buffer, "220 ")) break;
    }

	return buffer;
}

int ftp_send(int sockfd, char* msg) {
    char* req = strdup(msg);
    strcat(req, "\r\n");
    send(sockfd, req, strlen(req), 0);
    return 0;
}

int print_ftp_resp(int sockfd) {
    char buffer[128];
    int n = recv(sockfd, buffer, 127, 0);
    buffer[n] = '\0';
    printf("%s\n", buffer);
    return 0;
}