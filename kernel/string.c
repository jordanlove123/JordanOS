#include "../include/string.h"

int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a - *b;
}

int strncmp(const char* a, const char* b, int n) {
    while (*a && *a == *b && n > 0) {
        a++;
        b++;
        n--;
    }
    return *a - *b;
}

size_t strlen(const char* a) {
    int count = 0;
    while (*a != '\0') {
        count++;
        a++;
    }
    return count;
}

void strcpy(char* buf, const char* a) {
    while (*a) {
        *buf = *a;
        a++;
        buf++;
    }
    *buf = '\0';
}

void strncpy(char* buf, const char* a, int n) {
    while (*a && n > 0) {
        *buf = *a;
        a++;
        buf++;
        n--;
    }
    *buf = '\0';
}
