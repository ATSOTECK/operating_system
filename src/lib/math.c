#include <math.h>

void swapi(int *n1, int *n2) {
    int tmp = *n1;
    *n1 = *n2;
    *n2 = tmp;
}

int absi(int n) {
    if (n < 0) {
        return -n;
    }
    
    return n;
}

float absf(float n) {
    if (n < 0) {
        return -n;
    }
    
    return n;
}