#include <mem.h>

void memcpy(char *src, char *dst, int n) {
    for (int i = 0; i < n; ++i) {
        *(dst + i) = *(src + i);
    }
}
