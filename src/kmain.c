void cls();
int add(int x, int y);

void kmain() {
    char *videoMem = (char*)0xB8000;
    *videoMem = add(20, 83);
    while (1) {};
}

void cls() {
    /*
    int end = 0xFFFF;
    while (--end != 0) {  
        *(videoMem + end) = 0;
    }
    */
}

int add(int x, int y) {
    return x + y;
}
