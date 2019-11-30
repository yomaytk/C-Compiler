#include <stdio.h>
#include <string.h>



int main(){ int w = 13; int x = 12; int *y; int *z; y = &w; z = &x; y = y+1; return *y;}