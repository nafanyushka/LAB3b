//
// Created by Nafany on 07.04.2021.
//

#ifndef LAB3_KEYSPACE1_H
#define LAB3_KEYSPACE1_H
#include <stdio.h>
#include <stdlib.h>
#include "item.h"

typedef struct KeySpace1 KeySpace1;
typedef struct Node1 Node1;

struct KeySpace1{
    int key;
    int nodeOffset;
    Node1* node;
};

struct Node1{
    int release;
    int itemOffset;
    int nextOffset;
    Node1* next;
};

/*  TODO:
     * Вставка
     * Удаление
     * Особенное:
     * Поиск в таблице всех версий элемента, заданного ключом, или конкретной (заданной) версии элемента, также
     * заданного своим ключом; результатом поиска должна быть новая таблица, содержащая найденные элементы
     * Удаление из таблицы всех элементов с заданным ключом или элемента определенной версии, также заданного своим
     * ключом
                                                                                                                    */

KeySpace1* initKeySpace1(int maxsize1);
KeySpace1* loadKeySpace1(FILE *fd, int maxsize1, int nsize1);
int addItemKeySpace1(KeySpace1* keySpace1, int offsetItem, int key1, int maxsize1, int* nsize1);
void printKey1(FILE* fd, KeySpace1* keySpace1, int nsize1);
int removeKeySpace1(KeySpace1* keySpace1, int* nsize1, int key, int release);
KeySpace1* findKey1(KeySpace1* keySpace1, int key1, int nsize1);
Item* findRelease(FILE* fd, KeySpace1* keySpace1, int release);
int saveKeySpace1(FILE* fd, KeySpace1* keySpace1, int nsize1, int maxsize1);
int checkKeySpace1(KeySpace1* keySpace1, int key, int maxsize1, int nsize1);
void freeKeySpace1(KeySpace1* keySpace1, int nsize1);
void deleteKey1Offset(KeySpace1* keySpace1, int key1, int itemOffset, int* nsize1);

#endif //LAB3_KEYSPACE1_H
