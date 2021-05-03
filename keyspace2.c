//
// Created by Nafany on 07.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "keyspace2.h"
#include "keyspace1.h"
#include "input.h"

int hash1(char* key){
    int charSum = 0;
    char* charPointer = key;
        while(*charPointer != '\0'){
        charSum += (int)(*charPointer);
        charPointer++;
    }
    return charSum;
}

int hash2(char* key){
    int steps[10] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23};
    int charSum = 0;
    char* charPointer = key;
    while(*charPointer != '\0'){
        charSum += (int)(*charPointer);
        charPointer++;
    }
    return steps[charSum % 10];
}

int getIndex(char* key, int step, int maxsize2){
    return (hash1(key) + step * hash2(key)) % maxsize2;
}

KeySpace2* initKeySpace2(int maxsize2) {
    KeySpace2* keySpace2 = (KeySpace2*) calloc(sizeof(KeySpace2), maxsize2);
    return keySpace2;
}

KeySpace2* loadKeySpace2(FILE* fd, int maxsize2, int maxsize1){
    KeySpace2* keySpace2 = initKeySpace2(maxsize2);
    fread(keySpace2, 1, sizeof(KeySpace2) * maxsize2, fd);
    KeySpace2* keyReader = keySpace2;

    for(int i = 0; i < maxsize2; i++){
        if(keyReader->itemOffset != 0 && keyReader->busy == BUSY){
            keyReader->key2 = (char*)malloc(keyReader->keyLen);
            fseek(fd, keyReader->keyOffset, SEEK_SET);
            fread(keyReader->key2, 1, keyReader->keyLen, fd);
            keyReader++;
        }
        else{
            keyReader->key2 = NULL;
            keyReader++;
        }
    }
    return keySpace2;
}

int addItemKeySpace2(KeySpace2* keySpace2, char* key2, int itemOffset, int index){
    keySpace2[index].key2 = key2;
    keySpace2[index].keyLen = (int)strlen(key2) + 1;
    keySpace2[index].itemOffset = itemOffset;
    keySpace2[index].busy = BUSY;
    return 0;
}

void printKeySpace2(FILE* fd, KeySpace2* keySpace2, int maxsize2){
    KeySpace2* printer = keySpace2;
    for(int i = 0; i < maxsize2; i++){
        if(printer->busy == BUSY) {
            printf("%d. ", (i + 1));
            printInfo(fd, printer->itemOffset);
        }
        else{
            printf("%d.\n", (i + 1));
        }
        printer++;
    }
}

int removeKeySpace2(FILE* fd, KeySpace2* keySpace2, char* key2, int maxsize2, int* key1) {
    KeySpace2 *deleter = NULL;
    for (int step = 0; step < maxsize2; step++) {
        int index = getIndex(key2, step, maxsize2);
        char* key = keySpace2[index].key2;
        if (keySpace2[index].busy == BUSY && strcmp(key, key2) == 0) {
            deleter = &keySpace2[index];
            break;
        }
        if(maxsize2 == (step + 1))
            return 0; //НЕ УДАЛОСЬ НАЙТИ ТАКОЙ ЭЛЕМЕНТ
    }
    Item* item = getItem(fd, deleter->itemOffset);
    *key1 = item->key1;
    free(item);
    int offsetItem = deleter->itemOffset;
    free(deleter->key2);
    deleter->itemOffset = 0;
    deleter->keyOffset = 0;
    deleter->keyLen = 0;
    deleter->busy = DELETED;
    return offsetItem; //УДАЧНОЕ УДАЛЕНИЕ ВОЗВРАЩАЕТ ОФФСЕТ АЙТЕМА ДЛЯ ПОИСКА ЕГО В ПЕРВОМ ПРОСТРАНСТВЕ
}

int findKey2(KeySpace2* keySpace2, char* key2, int maxsize2){
    for(int step = 0; step < maxsize2; step++){
        int index = getIndex(key2, step, maxsize2);
        if(keySpace2[index].busy == BUSY && strcmp(keySpace2[index].key2, key2) == 0){
            return keySpace2[index].itemOffset;
        }
        if(keySpace2[index].busy == FREE)
            break;
    }
    return 0;
}

void saveKeySpace2(FILE* fd, KeySpace2* keySpace2, int maxsize2, int maxsize1){
    fwrite(keySpace2, 1, sizeof(KeySpace2) * maxsize2, fd);
}

int checkKeySpace2(KeySpace2* keySpace2, char* key2, int maxsize2){ //ВОЗВРАЩАЕТ -1 ЕСЛИ НЕ НАШЕЛ ИНДЕКС, ЕГО ЕСЛИ ДА
    for(int step = 0; step < maxsize2; step++){
        int index = getIndex(key2, step, maxsize2);
        if(keySpace2[index].busy == BUSY){
            if(strcmp(keySpace2[index].key2, key2) == 0){
                return -1;
            }
            continue;
        }
        else{
            return index;
        }
    }
    return -1;
}

void freeKeySpace2(KeySpace2* keySpace2, int maxsize2){
    KeySpace2* cleaner = keySpace2;
    for(int i = 0; i < maxsize2; i++){
        if(cleaner->key2 != NULL) {
            free(cleaner->key2);
//            cleaner->key2 = NULL;
        }
        if(maxsize2 == (i + 1))
            break;
        cleaner++;
    }
    free(keySpace2);
}