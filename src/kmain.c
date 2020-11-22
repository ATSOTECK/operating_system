#include "common.h"

#include <bootboot.h>

#define c_red   0x00FF0000
#define c_green 0x0000FF00
#define c_blue  0x000000FF
#define c_white 0x00FFFFFF
#define c_black 0x00000000

#define itoc(__i) __i + '0'

void puts(char *s, int x, int y);
void prints(char *s, int x, int y);
void putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg);
void printchar(unsigned short int c, int cx, int cy, uint32_t color);
void drawRect(int _x, int _y, int w, int h, int color);
void setPixel(int x, int y, int color);
void clear();
void clearWith(int color);

/* imported virtual addresses, see linker script */
extern BOOTBOOT bootboot;               // see bootboot.h
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped

int clearColor = c_white;

/*
 * Entry point. Must be first function in the file.
 */

void _start() {
    /*** NOTE: this code runs on all cores in parallel ***/
    int x, y, s=bootboot.fb_scanline, w=bootboot.fb_width, h=bootboot.fb_height;
    clear();

    drawRect(0, 0, 20, 20, 0x002244FF);

    // cross-hair to see screen dimension detected correctly
    for(y=0;y<h;y++) { *((uint32_t*)(&fb + s*y + (w*2)))=0x00FFFFFF; }
    for(x=0;x<w;x++) { *((uint32_t*)(&fb + s*(h/2)+x*4))=0x00FFFFFF; }

    // red, green, blue boxes in order
    //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
    //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
    //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }
    drawRect( 20, 20, 20, 20, c_red);
    drawRect( 50, 20, 20, 20, c_green);
    drawRect( 80, 20, 20, 20, c_blue);
    drawRect(110, 20, 20, 20, 0x00FF00FF);
    //drawRect(140, 20, 20, 20, s, 0x00BADBEE);

    // say hello
    puts("Hello World from a simple kernel!", 0, 0);
    prints("Hello World from a simple kernel!", 0, 20);
    for (int i = 3; i < 40; ++i) {
        puts("YA YEET", 0, i);
        prints("YA YEET", 64, i * 16);
        putchar(itoc(i), 20, i, c_red, c_black);
    }
    
    setPixel(w/2, h/2, c_black);
    
    //clearWith(c_red);

    // hang for now
    while(true) {
        /*
        clear(c_red);
        drawRect(0, 0, 100, 100, c_green);
        clear(c_black);
        */
    }
}

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

void puts(char *s, int x, int y) {
    int cx = 0, cy = 0;
    
    while (*s) {
        putchar(*s, x + cx, y + cy, c_white, c_black);
        cx++;
        s++;
    }
}

void prints(char *s, int x, int y) {
    int cx = 0, cy = 0;
    psf2_t *font = (psf2_t*)&_binary_font_psf_start;
    
    while (*s) {
        printchar(*s, x + cx, y + cy, c_black);
        cx += font->width + 1;
        s++;
    }
}

void putchar(
    /* note that this is int, not char as it's a unicode character */
    unsigned short int c,
    /* cursor position on screen, in characters not in pixels */
    int cx, int cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg)
{
    /* cast the address to PSF header struct */
    psf2_t *font = (psf2_t*)&_binary_font_psf_start;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font->width+7)/8;
    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    unsigned char *glyph =
     (unsigned char*)&_binary_font_psf_start +
     font->headersize +
     (c>0&&c<font->numglyph?c:0)*font->bytesperglyph;
    /* calculate the upper left corner on screen where we want to display.
       we only do this once, and adjust the offset later. This is faster. */
    int offs =
        (cy * font->height * bootboot.fb_scanline) +
        (cx * (font->width+1) * 4);
    /* finally display pixels according to the bitmap */
    int x,y, line,mask;
    for(y=0;y<font->height;y++){
        /* save the starting position of the line */
        line=offs;
        mask=1<<(font->width-1);
        /* display a row */
        for(x=0;x<=font->width;x++){
            *((uint32_t*)(&fb + line)) = ((int)*glyph) & (mask) ? fg : bg;
            /* adjust to the next pixel */
            mask >>= 1;
            line += 4;
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += bootboot.fb_scanline;
    }
}

void printchar(unsigned short int c, int cx, int cy, uint32_t color) {
    /* cast the address to PSF header struct */
    psf2_t *font = (psf2_t*)&_binary_font_psf_start;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font->width+7)/8;
    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    unsigned char *glyph =
     (unsigned char*)&_binary_font_psf_start +
     font->headersize +
     (c>0&&c<font->numglyph?c:0)*font->bytesperglyph;
    /* calculate the upper left corner on screen where we want to display.
       we only do this once, and adjust the offset later. This is faster. */
    int offs = (cx * 4 + cy * bootboot.fb_scanline);
    /* finally display pixels according to the bitmap */
    int x,y, line,mask;
    for(y=0;y<font->height;y++){
        /* save the starting position of the line */
        line=offs;
        mask=1<<(font->width-1);
        /* display a row */
        for(x=0;x<=font->width;x++){
            *((uint32_t*)(&fb + line)) = ((int)*glyph) & (mask) ? color : *(uint32_t*)(&fb + line);
            /* adjust to the next pixel */
            mask >>= 1;
            line += 4;
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += bootboot.fb_scanline;
    }
}

void drawRect(int _x, int _y, int w, int h, int color) {
    for (int x = _x; x < _x + w; ++x) {
        for (int y = _y; y < _y + h; ++y) {
            setPixel(x, y, color);
        }
    }
}

void setPixel(int x, int y, int color) {
    if (x >= bootboot.fb_width || x < 0 || y >= bootboot.fb_height || y < 0) {
        return;
    }
    
    *((uint32_t*)(&fb + x * 4 + y * bootboot.fb_scanline)) = color;
}

void clear() {
    for(int y = 0; y < bootboot.fb_height; y++) {
        for(int x = 0; x < bootboot.fb_width; x++) {
            setPixel(x, y, clearColor);
        }
    }
}

void clearWith(int color) {
    for(int y = 0; y < bootboot.fb_height; y++) {
        for(int x = 0; x < bootboot.fb_width; x++) {
            setPixel(x, y, color);
        }
    }
}
