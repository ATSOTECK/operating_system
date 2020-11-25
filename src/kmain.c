#include <mem.h>
#include <port.h>

void cls(void);
int add(int x, int y);

void startMsg(void);

void printChar(char c, int col, int row, char attrib);
int getScreenOffset(int col, int row);
int getCursor(void);
void setCursor(int offset);
void printAt(char *msg, int col, int row);
void print(char *msg);
int handleScrolling(int offset);

#define VidMem 0xB8000
#define MaxRows 22
#define MaxCols 80
#define WB 0x0F

#define RegScreenCtrl 0x3D4
#define RegScreenData 0x3D5

void kmain() {
    cls();
    startMsg();
    print("op@os:/> ");
    /*
    for (int i = 0; i < 20; ++i) {
        print("This is a test!\n");
    }*/
    while (1) {};
}

void startMsg(void) {
    print("operating_system\nbeep boop\n\n");
}

void cls(void) {
    for (int r = 0; r < MaxRows; ++r) {
        for (int c = 0; c < MaxCols; ++c) {
            printChar(' ', c, r, WB);
        }
    }
    
    setCursor(0);
}

int add(int x, int y) {
    return x + y;
}

void printChar(char c, int col, int row, char attrib) {
    uchar *vidMem = (uchar *)VidMem;
    
    if (!attrib) {
        attrib = WB;
    }
    
    int offset;
    if (col >= 0 && row >= 0) {
        offset = getScreenOffset(col, row);
    } else {
        offset = getCursor();
    }
    
    if (c == '\n') {
        int rows = offset / (2 * MaxCols);
        offset = getScreenOffset(79, rows);
    } else {
        vidMem[offset] = c;
        vidMem[offset + 1] = attrib;
    }
    
    offset += 2;
    setCursor(handleScrolling(offset));
}

int getScreenOffset(int col, int row) {
    return ((row * MaxCols + col) * 2);
}

int getCursor(void) {
    portByteOut(RegScreenCtrl, 14);
    int offset = portByteIn(RegScreenData) << 8;
    portByteOut(RegScreenCtrl, 15);
    offset += portByteIn(RegScreenData);
    
    return offset * 2;
}

void setCursor(int offset) {
    offset /= 2;
    portByteOut(RegScreenCtrl, 14);
    portByteOut(RegScreenData, (uchar)(offset >> 8));
    portByteOut(RegScreenCtrl, 15);
    portByteOut(RegScreenData, offset);
}

void printAt(char *msg, int col, int row) {
    if (col >= 0 && row >= 0) {
        setCursor(getScreenOffset(col, row));
    }
    for (int i = 0; msg[i] != 0; ++i) {
        printChar(msg[i], col, row, WB);
    }
}

void print(char *msg) {
    printAt(msg, -1, -1);
}

int handleScrolling(int offset) {
    if (offset < MaxRows * MaxCols * 2) {
        return offset;
    }
    
    for (int i = 1; i < MaxRows; ++i) {
        memcpy((char *)getScreenOffset(0, i) + VidMem, (char *)getScreenOffset(0, i - 1) + VidMem, MaxCols * 2);
    }
    
    char *lastLine = (char *)getScreenOffset(0, MaxRows - 1) + VidMem;
    for (int i = 0; i < MaxCols * 2; ++i) {
        lastLine[i] = 0;
    }
    
    return offset - (2 * MaxCols);//getScreenOffset(0, MaxRows - 1);
}
