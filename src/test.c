#include "memurija.h"
#include <string.h>
#include <stdio.h>

int main() {
    char* word = (char*)malloc(sizeof(char) * 10);
    strcpy(word, "hihihaha\0");
    printf("'%s'\n", word);
    memurija_report();
    free(word);
    memurija_report();
    return 0;
}
