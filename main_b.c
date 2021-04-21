#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>

#include "dialogue.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);


/*

    Table* table;
    FILE* fd = openFile(&table);

                                */

    Table* table;
    FILE* fd = openFile(&table);

    mainDialogue(fd, table);

/*

    char* fileName = get_String();
    FILE* fd = fopen(fileName, "r+b");

    if(fd == NULL)
        fd = fopen(fileName, "wb");

    int* offsets = (int*)calloc(sizeof(int), 10);
    int index = 0;
    int key1;
    do{
        printf("KEY1: ");
        key1 = getInt();
        char* info;
        char* key2;
        do {
            printf("KEY2: ");
            key2 = get_String();
        }while(strlen(key2) < 1);
        do {
            printf("INFO: ");
            info = get_String();
        }while(strlen(info) < 1);
        offsets[index] = createItem(fd, info, key1, key2);
        index++;
    }while(key1 != 0);

    fclose(fd);
    fd = fopen(fileName, "r+b");

    for(int i = 0; i < index; i++)
        printInfo(fd, offsets[i]);

    system("pause");

    for(int i = 0; i < index; i++){
        freeItem(fd, offsets[i]);
    }
    for(int i = 0; i < index; i++)
        printInfo(fd, offsets[i]);
    system("pause");
                                                        */
    return 0;
}