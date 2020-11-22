#ifndef __LIB_PRINT_H__
#define __LIB_PRINT_H__

#include "types.h"

/**************************
 * Display text on screen *
 **************************/
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;
extern volatile unsigned char _binary_font_psf_start;

void puts(char *s, int x, int y);
void prints(char *s, int x, int y);
void putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg);
void printchar(unsigned short int c, int cx, int cy, uint32_t color);

#endif //__LIB_PRINT_H__
