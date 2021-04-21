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
    char* key; // *Не более 4 элементов!
    Item* info;
};

KeySpace2* initKeySpace2(int maxsize2);

#endif //LAB3_KEYSPACE2_H
