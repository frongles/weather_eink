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





int main(void) {
    init_display();
    clear_display();
    setlocale(LC_ALL, "");

    printf("initialising font\n");
    stbtt_fontinfo* fontinfo = init_font(UNIFONT, 32);
    printf("Writing string\n");
    write_string(fontinfo, 30, 64, 16, "sup nerd");
    
    printf("String written\n");
    activate_display();

    sleep_display();
    free(fontinfo->data);
    free(fontinfo);
}