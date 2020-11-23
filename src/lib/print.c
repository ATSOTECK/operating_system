#include <print.h>

#include <bootboot.h>
#include <colors.h>

extern BOOTBOOT bootboot;
extern uint8_t fb;

uint32_t txtColor = c_white;
uint32_t bgColor = c_black;

volatile psf2_t *font = (psf2_t*)&_binary_font_psf_start;

void __initPrint(void) {
    txtColor = c_white;
    bgColor = c_black;
}

void puts(char *s, int x, int y) {
    int cx = 0, cy = 0;
    
    while (*s) {
        if (*s == '\n') {
            cy++;
            cx = 0;
            s++;
        }
        
        putchar(*s, x + cx, y + cy, txtColor, bgColor);
        cx++;
        s++;
    }
}

void prints(char *s, int x, int y) {
    int cx = 0, cy = 0;
    
    while (*s) {
        if (*s == '\n') {
            cy += font->height + 1;
            cx = 0;
            s++;
        }
        
        printchar(*s, x + cx, y + cy, txtColor);
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