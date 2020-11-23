#include <colors.h>
#include <print.h>
#include <math.h>
#include <types.h>

#include <bootboot.h>

#define itoc(__i) __i + '0'

void drawRect(int _x, int _y, int w, int h, int color);
void setPixel(int x, int y, int color);
void clear();
void clearWith(int color);

void line(int x1, int y1, int x2, int y2, uint32_t color);

/* imported virtual addresses, see linker script */
extern BOOTBOOT bootboot;               // see bootboot.h
extern unsigned char environment[4096]; // configuration, UTF-8 text key=value pairs
extern uint8_t fb;                      // linear framebuffer mapped

int clearColor = c_black;

/*
 * Entry point. Must be first function in the file.
 */

void _start() {
    __initPrint();
    
    
    /*** NOTE: this code runs on all cores in parallel ***/
    //int w=bootboot.fb_width, h=bootboot.fb_height;
    //clear();

    drawRect(0, 0, 20, 20, 0x002244FF);

    // cross-hair to see screen dimension detected correctly
    //line(0, h/2, w, h/2, c_black);
    //line(w/2, 0, w/2 + 1, h, c_black);
    
    //line(0, 0, w, h, c_black);
    //line(0, h, w, 0, c_black);
    
    // red, green, blue boxes in order
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
        putchar(itoc(i), 20, i, c_red, c_blue);
    }
    
    //setPixel(w/2, h/2, c_red);
    
    //clearWith(c_red);
    
    puts("The quick brown fox jumped over the lazy dog\nhello", 22, 10);
    prints("The quick brown fox jumped over the lazy dog\nhello", 200, 200);

    // hang for now
    while(true) {}
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

/*
void line(int x1, int y1, int x2, int y2, uint32_t color) {
    bool steep = false;
    
    if (absi(x1 - x2) < absi(y1 - y2)) {
        swapi(&x1, &x2);
        swapi(&y1, &y2);
        steep = true;
    }
    
    if (x1 > x2) {
        swapi(&x1, &x2);
        swapi(&y1, &y2);
    }
    
    if (x1 == x2) {
        for (int y = y1; y <= y2; y++) {
            setPixel(x1, y, color);
        }
        
        return;
    }
    
    int dx = x2 - x1; 
    int dy = y2 - y1; 
    float derror = absf(dy/(float)dx); 
    float error = 0; 
    int y = y1; 
    for (int x = x1; x <= x2; x++) { 
        if (steep) { 
            setPixel(y, x, color); 
        } else { 
            setPixel(x, y, color); 
        }
        
        error += derror; 
        if (error > 0.5f) { 
            y += (y2 > y1 ? 1 : -1); 
            error -= 1.; 
        } 
    }
}
*/