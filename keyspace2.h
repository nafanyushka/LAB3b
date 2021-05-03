//
// Created by Nafany on 07.04.2021.
//

#ifndef LAB3_KEYSPACE2_H
#define LAB3_KEYSPACE2_H

#include "item.h"

#define BUSY -1
#define FREE 0
#define DELETED 1
#define MAXSTRING 4

typedef struct KeySpace2 KeySpace2;

struct KeySpace2{
    int busy;
    int keyOffset; // *Не более 4 элементов!
    int keyLen;
    int itemOffset;
    char* key2;
};

KeySpace2* initKeySpace2(int maxsize2);
KeySpace2* loadKeySpace2(FILE* fd, int maxsize2, int maxsize1);
int addItemKeySpace2(KeySpace2* keySpace2, char* key2, int itemOffset, int index);
void printKeySpace2(FILE* fd, KeySpace2* keySpace2, int maxsize2);
int removeKeySpace2(FILE* fd, KeySpace2* keySpace2, char* key2, int maxsize2, int* key1);
int findKey2(KeySpace2* keySpace2, char* key2, int maxsize2);
void saveKeySpace2(FILE* fd, KeySpace2* keySpace2, int maxsize2, int maxsize1);
int checkKeySpace2(KeySpace2* keySpace2, char* key2, int maxsize2);
void freeKeySpace2(KeySpace2* keySpace2, int maxsize2);




#endif //LAB3_KEYSPACE2_H
