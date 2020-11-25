#include <port.h>

uint8_t portByteIn(ushort port) {
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void portByteOut(ushort port, uint8_t data) {
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}

ushort portWordIn(ushort port) {
    ushort result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void portWordOut(ushort port, ushort data) {
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}
