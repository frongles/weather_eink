#ifndef FTP_TOOLS
#define FTP_TOOLS

#define FTP_BUFF 1024

/* Things to do

Connect to ftp server
- read response (and log?)
Log in
- user
- password
List directories
- open connection and read response
Move to directory
Retrieve
- open connection and read response
Close


Functions you want
- connect to server with address name on port 21
- write arbitrary command
- Recieve response
- connect to server with arbitrary ip and port

*/
// Gets code and msg from ftp message
void get_ftp_code_msg(char* response, char* code, char* msg);

void get_pasv_addr_serv(char* msg, char* host, char* service);

char* read_multiline(int sockfd);

int ftp_send(int sockfd, char* msg);

int print_ftp_resp(int sockfd);
#endif