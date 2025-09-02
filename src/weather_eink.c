#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#include "stb_truetype.h"
#include "eInkTools.h"
#include "ftp_tools.h"
#include "sock_util.h"
#include "log.h"

// Font paths
// Define your own fonts here.
#define FONTS "/home/frongles/eInkDisplay/fonts/"

#define MONACO  FONTS "Monaco.ttf"
#define QABEXEL FONTS "Qabaxel-2v3el.ttf"
#define UNIFONT FONTS "UnifontExMono.ttf"

void fetch_bom_data();
void get_temp_from_xml(char* xmlfile, int* high, int* low);


int main(int argc, char** argv) {
    init_display();
    clear_display();
    setlocale(LC_ALL, "");

    // get high and low temp from the BOM
    fetch_bom_data();
    int high, low;
    get_temp_from_xml("weather.xml", &high, &low);
    char high_temp_str[128];
    char low_temp_str[128];


    // initialise fonts
    printf("initialising font\n");
    stbtt_fontinfo* fontinfo = init_font(UNIFONT);
    printf("Writing string\n");
    write_string(fontinfo, 24, 100, 0, "Today's weather");
    for(int i = 0; i < 5; i++) {
        display_line_Y(98 - i);
    }

    sprintf(high_temp_str, "H. %dº", high);
    sprintf(low_temp_str, "L. %dº", low);


    write_string(fontinfo, 36, 50, 10, "♪(๑ᴖ◡ᴖ๑)♪");
    write_string(fontinfo, 36, 12, 30, high_temp_str);
    
    printf("String written\n");
    activate_display();

    sleep_display();
    free(fontinfo->data);
    free(fontinfo);
}



void fetch_bom_data() {
    log_msg(LOG_INFO, "Fetching data from BOM");

    int sockfd = connect_to_server("ftp.bom.gov.au", "ftp");
    log_msg(LOG_INFO, "Connected to ftp.bom.gov.au");

    char* buffer = read_multiline(sockfd);
    log_msg(LOG_INFO, "Read from server");
    log_msg(LOG_DEBUG, buffer);


    ftp_send(sockfd, "USER anonymous");

    print_ftp_resp(sockfd);

    ftp_send(sockfd, "PASS");

    print_ftp_resp(sockfd);

    ftp_send(sockfd, "CWD anon/gen/fwo");

    print_ftp_resp(sockfd);

    ftp_send(sockfd, "PASV");

    int n = recv(sockfd, buffer, 127, 0);
    buffer[n] = '\0';
    char host[64], service[64];
    get_pasv_addr_serv(buffer, host, service);
    printf("%s\n", buffer);

    int sock2 = connect_to_server(host, service);

    ftp_send(sockfd, "RETR IDV10450.xml");

    print_ftp_resp(sockfd);

    tcp_to_file(sock2, "weatherdata.xml");
    close(sock2);

    ftp_send(sockfd, "QUIT");

    print_ftp_resp(sockfd);

    free(buffer);
    close(sockfd);

}



void get_temp_from_xml(char* xmlfile, int* high, int* low) {
    FILE *fp = fopen(xmlfile, "r");

    char line[1024];

    char* dest = NULL;

    while (dest == NULL && !feof(fp)) {
        fgets(line, 1024, fp);
        dest = strstr(line, "air_temperature_minimum");
    }

    dest = strstr(line, ">");
    sscanf(dest, ">%d<", low);
    
    dest = NULL;
    while (dest == NULL && !feof(fp)) {
        fgets(line, 1024, fp);
        printf("%s", line);
        dest = strstr(line, "air_temperature_maximum");
    }

    dest = strstr(line, ">");
    sscanf(dest, ">%d<", high);

    fclose(fp);
    
}