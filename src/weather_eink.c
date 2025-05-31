#include <stdio.h>
#include <unistd.h>

#include "stb_truetype.h"
#include "eInkTools.h"
#include "ftp_tools.h"
#include "sock_util.h"
#include "log.h"


#include "gpioTools.h"





int main(void) {
    init_display();
    usleep(1000*1000);
    hardware_reset();
    

}