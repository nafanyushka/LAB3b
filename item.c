//
// Created by Nafany on 07.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "item.h"
#include "keyspace1.h"
#include "keyspace2.h"
#include "input.h"

Item* getItem(FILE* fd, int offset){
    fseek(fd, offset, SEEK_SET);
    Item* item = (Item*)malloc(sizeof(Item));
    fread(item, 1, sizeof(Item), fd);
    return item;
}

char* getKey2(FILE* fd, Item* item){
    char* key2 = (char*)malloc(item->lenKey2);
    fseek(fd, item->offsetKey2, SEEK_SET);
    fread(key2, 1, item->lenKey2, fd);
    return key2;
}

int createItem(FILE* fd, char* info, int key1, char* key2, int* key2offset, int* key2len){
    Item* item = (Item*)malloc(sizeof(Item));
    item->key1 = key1;
    fseek(fd, 0, SEEK_END);

    item->offsetKey2 = ftell(fd);
    (*key2offset) = item->offsetKey2;
    item->lenKey2 = (int)strlen(key2) + 1;
    (*key2len) = item->lenKey2;
    fwrite(key2, 1, item->lenKey2, fd);

    item->offsetInfo = ftell(fd);
    item->lenInfo = (int)strlen(info) + 1;
    fwrite(info, 1, item->lenInfo, fd);

    int offset = ftell(fd);
    fwrite(item, 1, sizeof(Item), fd);
    free(item);

    return offset;

}

char* getInfo(FILE* fd, Item* item){
    char* info = (char*)malloc(item->lenInfo);
    fseek(fd, item->offsetInfo, SEEK_SET);
    fread(info, 1, item->lenInfo, fd);
    return info;
}

void printInfo(FILE* fd, int offsetItem){

    Item* item = getItem(fd, offsetItem);
    char* info = getInfo(fd, item);
    char* key2 = getKey2(fd, item);

    printf("Информация: \"%s\".\tПервый ключ: %d. Второй ключ: %s.\n", info, item->key1, key2);

    free(info);
    free(key2);
    free(item);
}

void freeItem(FILE* fd, int offsetItem){
    Item* item = (Item*)calloc(sizeof(Item), 1);
    fseek(fd, offsetItem, SEEK_SET);
    fwrite(item, 1, sizeof(Item), fd);
    free(item);
}

//int copyItem(FILE* fd, int offsetItem){
//    Item* item = getItem(fd, offsetItem);
//    char* info = getInfo(fd, item);
//    char* key2 = getKey2(fd, item);
//
//    int offset = createItem(fd, info, item->key1, key2);
//
//    free(item);
//    free(info);
//    free(key2);
//
//    return offset;
//}

void printInfoItem(FILE* fd, Item* item){

    char* info = getInfo(fd, item);
    char* key2 = getKey2(fd, item);

    printf("Информация: \"%s\".\tПервый ключ: %d. Второй ключ: %s.\n", info, item->key1, key2);

    free(info);
    free(key2);
    free(item);
}