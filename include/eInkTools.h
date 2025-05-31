/**Tools for interfacing with the 2.13inch e-Paper display from waveshare
 */


#ifndef EINKTOOLS
#define EINKTOOLS

#include <stdint.h>


// Font paths
#define FONTS "/home/frongles/eInkDisplay/fonts/"

#define MONACO  FONTS "Monaco.ttf"
#define QABEXEL FONTS "Qabaxel-2v3el.ttf"
#define UNIFONT FONTS "UnifontExMono.ttf"

#define HEIGHT 250 // in pixels
#define WIDTH 122 // in pixels
#define WHITE 1
#define BLACK 0


// Initialise the display
// This must be run first
int init_display();

// Send a single command to the display
int write_command(uint8_t command);

// Send a single byte of data to the display
// The data read is small endian
int write_data(uint8_t data);

// Refreshes the display, writing any data in RAM to the pixels
int activate_display();

// Clears the display
int clear_display();

// Creates a pattern on the screen - debugging
int pattern_display();

// Put the display to sleep - low power mode
// The display should be left in sleep mode when not in use
int sleep_display();

stbtt_fontinfo* init_font(char* font, int fontsize);

// Writes a character to the display ram with the specified font, fontsize, and x y coords.
// char* font => file path to the .ttf font file to be used.
// Currently no support for .ttc font file collections.
int write_char(stbtt_fontinfo* fontInfo, int fontsize, int x, int y, int *width, int *height, int character);

int write_string(stbtt_fontinfo* fontInfo, int fontsize, int x, int y, char*string);

// Writes a pixel to the display ram at the coords
int write_pixel(int colour, int x, int y);

// Clear the screen and put to sleep for storage/unplugging the device
int cleanup();

// Displays a 32x32 pixel grid on the display
int display_grid(int pixels_per_square);

int display_line_X(int y);

int display_line_Y(int x);

int display_cross(int x, int y);

#endif