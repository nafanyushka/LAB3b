//
// Created by Nafany on 07.04.2021.
//

#include "keyspace1.h"
#include "keyspace2.h"
#include "table.h"
#include "item.h"
#include "input.h"

Node1* loadNode1(FILE* fd, int offsetNode){
    Node1* node1 = (Node1*)malloc(sizeof(Node1));
    fseek(fd, offsetNode, SEEK_SET);
    fread(node1, 1, sizeof(Node1), fd);
    Node1* reader = node1;
    while(reader->nextOffset != 0){
        Node1* helper = reader;
        reader->next = (Node1*)malloc(sizeof(Node1));
        reader = reader->next;
        fseek(fd, helper->nextOffset, SEEK_SET);
        fread(reader, 1, sizeof(Node1), fd);
        helper->next = reader;
    }
    reader->next = NULL;
    return node1;
}

Node1* nodeAdd(Node1* node1, int itemOffset){
    Node1* newNode = (Node1*)malloc(sizeof(Node1));
    newNode->nextOffset = 0;
    newNode->next = node1;
    newNode->release = (node1->release) + 1;
    newNode->itemOffset = itemOffset;
    return newNode;
}

void printNode(FILE* fd, Node1* node){
    Node1* printer = node;
    int release = node->release;
    while(printer != NULL){
        printf("\tВерсия %d. ", release);
        printInfo(fd, printer->itemOffset);
        release--;
        printer = printer->next;
    }
}

Node1* copyNode(Node1* node){
    Node1* newNode = (Node1*)malloc(sizeof(Node1));
    Node1* pointer = node;
    Node1* copyPaster = newNode;
    while(pointer != NULL){
        copyPaster->release = pointer->release;
        copyPaster->itemOffset = pointer->itemOffset;
        copyPaster->nextOffset = 0;
        if(pointer->next == NULL)
            copyPaster->next = NULL;
        else
            copyPaster->next = (Node1*)malloc(sizeof(Node1));
        pointer = pointer->next;
        copyPaster = copyPaster->next;
    }
    return newNode;
}

char** freeNode(FILE* fd, int* deletions, Node1* node){
    Node1* cleaner = node;
    char** keys = (char**)malloc(sizeof(char*) * cleaner->release);
    char** writer = keys;
    Item* item;
    *deletions = node->release;
    while(cleaner != NULL){
        Node1* helper = cleaner;
        item = getItem(fd, cleaner->itemOffset);
        *writer = getKey2(fd, item);
        free(item);
        cleaner = cleaner->next;
        free(helper);
        if(cleaner != NULL)
            writer++;
    }
    return keys;
}

int saveNode(FILE* fd, Node1* node, int prevOffset){
    if(node->next == NULL){
        node->nextOffset = 0;
        if(prevOffset == 0){
            fseek(fd, 0, SEEK_END);
            int offset = ftell(fd);
            fwrite(node, 1, sizeof(Node1), fd);
            free(node);
            return offset;
        }
        else{
            fseek(fd, prevOffset, SEEK_SET);
            int offset = ftell(fd);
            fwrite(node, 1, sizeof(Node1), fd);
            free(node);
            return offset;
        }
    }
    else{
        node->nextOffset = saveNode(fd, node->next, node->nextOffset);
        if(prevOffset == 0){
            fseek(fd, 0, SEEK_END);
            int offset = ftell(fd);
            fwrite(node, 1, sizeof(Node1), fd);
            free(node);
            return offset;
        }
        else{
            fseek(fd, prevOffset, SEEK_SET);
            int offset = ftell(fd);
            fwrite(node, 1, sizeof(Node1), fd);
            free(node);
            return offset;
        }
    }
}

KeySpace1* initKeySpace1(int maxsize1){
    KeySpace1* keySpace1 = (KeySpace1*)calloc(sizeof(KeySpace1), maxsize1);
    return keySpace1;
}

KeySpace1* loadKeySpace1(FILE *fd, int maxsize1, int nsize1) {
    KeySpace1* keySpace1 = initKeySpace1(maxsize1);
    fread(keySpace1, 1, sizeof(KeySpace1) * maxsize1, fd);
    int offset = ftell(fd);
    KeySpace1* loader = keySpace1;

    for(int i = 0; i < nsize1; i++){
        loader->node = loadNode1(fd, loader->nodeOffset);
        loader++;
    }

    fseek(fd, offset, SEEK_SET);
    return keySpace1;
}

int checkKeySpace1(KeySpace1* keySpace1, int key, int maxsize1, int nsize1){
    KeySpace1* pointer = keySpace1;
    for(int i = 0; i < nsize1; i++){
        if(pointer->key == key){
            return 1;
        }
        if(i == nsize1 - 1)
            break;
        pointer++;
    }
    if(nsize1 == maxsize1)
        return 0;
    return 1;
}

int addItemKeySpace1(KeySpace1* keySpace1, int offsetItem, int key1, int maxsize1, int* nsize1){
    if(maxsize1 == *nsize1)
        return (-1); //МЕСТО В КЕЙСПЕЙСЕ КОНЧИЛОСЬ

    KeySpace1* checker = keySpace1;
    int i = 0;
    while(key1 != checker->key && i < *nsize1){
        checker++;
        i++;
    }
    if(i == *nsize1){
        (*nsize1)++;
        checker->node = (Node1*)malloc(sizeof(Node1));
        checker->key = key1;
        checker->node->next = NULL;
        checker->node->nextOffset = 0;
        checker->node->itemOffset = offsetItem;
        checker->node->release = 1;
    }
    else{
        checker->node = nodeAdd(checker->node, offsetItem);
    }
    return 0;
}

void printKey1(FILE* fd, KeySpace1* keySpace1, int nsize1){
    KeySpace1* printer = keySpace1;
    for(int i = 0; i < nsize1; i++){
        printf("%d. Ключ %d:\n", (i + 1), printer->key);
        printNode(fd, printer->node);
        printer++;
    }
}

KeySpace1* findKey1(KeySpace1* keySpace1, int key1, int nsize1){
    KeySpace1* pointer = keySpace1;
    int index = 0;
    for(int i = 0; i < nsize1; i++){
        if(pointer->key == key1){
            break;
        }
        index++;
        if(index == nsize1)
            break;
        pointer++;
    }
    if(index == nsize1)
        return NULL; //ВСЕ ПЕРЕБРАЛИ И НЕ НАШЛИ КЛЮЧ
    KeySpace1* newKeySpace1 = (KeySpace1*)malloc(sizeof(KeySpace1));
    newKeySpace1->key = key1;
    newKeySpace1->node = copyNode(pointer->node);

    return newKeySpace1;
}

int push(KeySpace1* keySpace1, KeySpace1* pointer, int nsize1){
    pointer->key = keySpace1[(nsize1) - 1].key;
    keySpace1[(nsize1) - 1].key = 0;
    pointer->node = keySpace1[(nsize1) - 1].node;
    pointer->nodeOffset = keySpace1[(nsize1) - 1].nodeOffset;
    keySpace1[(nsize1) - 1].nodeOffset = 0;
    keySpace1[(nsize1) - 1].node = NULL;
    (nsize1)--;
    return nsize1;
}

char** removeKeySpace1(FILE* fd, KeySpace1* keySpace1, int* nsize1, int key, int* deletions, int release){
    KeySpace1* pointer = keySpace1;
    int index = 0;
    for(index; index < *nsize1; index++){
        if(pointer->key == key)
            break;
        if(index == (*nsize1) - 1){
            *deletions = 0;
            return NULL; //НЕ НАЙДЕН ЭЛЕМЕНТ С КЛЮЧИКОМ
        }
        pointer++;
    }
    if(release > pointer->node->release || release < 0) {
        *deletions = 0;
        return NULL; //НЕТ ТАКОЙ ВЕРСИИ
    }
    if(release == 0){
        char** keys = freeNode(fd, deletions, pointer->node);
        pointer->node = NULL;
        (*nsize1) = push(keySpace1, pointer, *nsize1);
        return keys; //УДАЧНОЕ УДАЛЕНИЕ НОДА
    }
    Node1* node = pointer->node;
    Node1* helper = node;
    while(release != node->release){
        helper = node;
        (node->release)--;
        node = node->next;
    }
    if(helper == node){
        pointer->node = NULL;
        Item* item = getItem(fd, node->itemOffset);
        char** keys = malloc(sizeof(char*));
        *keys = getKey2(fd, item);
        Node1* next = node->next;
        free(item);
        free(node);
        if(next == NULL)
            (*nsize1) = push(keySpace1, pointer, *nsize1);
        else{
            pointer->node = next;
        }
        *deletions = 1;
        return keys; // УДАЛЕНИЕ ОДНОГО ЭЛЕМЕНТИКА ПОСЛЕДНЕГО
    }
    helper->next = node->next;
    helper->nextOffset = node->nextOffset;
    Item* item = getItem(fd, node->itemOffset);
    char** keys = malloc(sizeof(char*));
    *keys = getKey2(fd, item);
    *deletions = 1;
    free(item);
    free(node);
    return keys; //УДАЧНОЕ УДАЛЕНИЕ ВЕРСИИ
}

Item* findRelease(FILE* fd, KeySpace1* keySpace1, int release){
    Node1* node = keySpace1->node;
    if(node->release < release)
        return NULL; //ТАКОГО РЕЛИЗА ТОЧНО НЕ СУЩЕСТВУЕТ
    while(node != NULL && node->release != release){
        node = node->next;
    }
    if(node->release == release)
        return getItem(fd, node->itemOffset);
    return NULL;
}

int saveKeySpace1(FILE* fd, KeySpace1* keySpace1, int nsize1, int maxsize1){
    KeySpace1* pointer = keySpace1;
    for(int i = 0; i < nsize1; i++){
        pointer->nodeOffset = saveNode(fd, pointer->node, pointer->nodeOffset);
        pointer++;
    }
    fseek(fd, sizeof(Table), SEEK_SET);
    fwrite(keySpace1, 1, sizeof(KeySpace1) * maxsize1, fd);
    return 0;
}

void freeKeySpace1(KeySpace1* keySpace1, int nsize1){
    KeySpace1* cleaner = keySpace1;
//    for(int i = 0; i < nsize1; i++){
//        freeNode(cleaner->node);
//        cleaner++;
//    }
    free(keySpace1);
}

void deleteKey1Offset(FILE* fd, KeySpace1* keySpace1, int key1, int itemOffset, int* nsize1){
    KeySpace1* seeker = keySpace1;
    Node1* node = NULL;
    for(int i = 0; i < (*nsize1); i++){
        if(seeker->key == key1){
            node = seeker->node;
            break;
        }
        if((*nsize1) == (i + 1))
            return;
        seeker++;
    }
    while(node != NULL){
        if(node->itemOffset == itemOffset)
            break;
        node = node->next;
    }
    char** key;
    int i;
    key = removeKeySpace1(fd, keySpace1, nsize1, key1, &i, node->release);
    free(key);
}

Item* findOffset1(FILE* fd, KeySpace1* keySpace1, int key, int itemOffset, int nsize){
    KeySpace1* finder = keySpace1;
    for(int i = 0; i < nsize; i++){
        if(finder->key == key)
            break;
        if((i + 1) == nsize)
            return NULL;
        finder++;
    }
    Node1* node = finder->node;
    while(node != NULL){
        if(node->itemOffset == itemOffset){
            return getItem(fd, itemOffset);
        }
        node = node->next;
    }
    return NULL;
}