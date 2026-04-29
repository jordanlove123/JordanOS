#ifndef STRING_H
#define STRING_H
#include <stddef.h>

int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, int n);
size_t strlen(const char* a);
void strcpy(char* buf, const char* a);
void strncpy(char* buf, const char* a, int n);

#endif
