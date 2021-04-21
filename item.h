//
// Created by Nafany on 07.04.2021.
//

#ifndef LAB3_ITEM_H
#define LAB3_ITEM_H

typedef struct Item Item;
typedef struct KeySpace1 KeySpace1;
typedef struct KeySpace2 KeySpace2;
typedef struct Node1 Node1;

struct Item{
    int offsetInfo;
    int lenInfo;
    int key1;
    int offsetKey2;
    int lenKey2;
};

int createItem(FILE* fd, char* info, int key1, char* key2, int* key2offset, int* key2len); //ВОЗВРАЩАЕТ ОФФСЕТ АЙТЕМА
Item* getItem(FILE* fd, int offset);
char* getInfo(FILE* fd, Item* item);
void printInfo(FILE* fd, int offsetItem);
void freeItem(FILE* fd, int offsetItem);
int copyItem(FILE* fd, int offsetItem);
void printInfoItem(FILE* fd, Item* item);

#endif //LAB3_ITEM_H
