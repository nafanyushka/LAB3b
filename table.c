//
// Created by Nafany on 07.04.2021.
//


#include <string.h>
#include "table.h"
#include "input.h"

Table* createTable(FILE* fd, int maxsize1, int maxsize2){
    Table* table = (Table*)malloc(sizeof(Table));
    table->maxsize1 = maxsize1;
    table->maxsize2 = maxsize2;
    table->nsize1 = 0;

    table->keySpace1 = initKeySpace1(table->maxsize1);
    table->keySpace2 = initKeySpace2(table->maxsize2);

    fseek(fd, 0, SEEK_SET);
    fwrite(table, 1, sizeof(Table), fd);
    fwrite(table->keySpace1, 1, sizeof(KeySpace1) * maxsize1, fd);
    fwrite(table->keySpace2, 1, sizeof(KeySpace2) * maxsize2, fd);

    return table;
}

Table* loadTable(FILE* fd){
    Table* table = (Table*)malloc(sizeof(Table));
    fseek(fd, 0, SEEK_SET);
    fread(table, 1, sizeof(Table), fd);

    table->keySpace1 = loadKeySpace1(fd, table->maxsize1, table->nsize1);
    printf("Загрузалась короче табла и кейспейс, вот первый ключ: %d, nsize1 = %d, maxsize1 = %d, maxsize2 = %d,"
           " nodeOffset %d\n",
           table->keySpace1->key, table->nsize1, table->maxsize1, table->maxsize2, table->keySpace1->nodeOffset     );
//    table->keySpace2 = initKeySpace2(table->maxsize2);
//    fread(table->keySpace2, 1, sizeof(KeySpace2) * table->maxsize2, fd);

    return table;
}

int addTable(FILE* fd, Table* table, int key1, char* info){
    if(checkKeySpace1(table->keySpace1, key1, table->maxsize1, table->nsize1)){
        fseek(fd, 0, SEEK_END);
        int offset = createItem(fd, info, key1, "");
        addItemKeySpace1(table->keySpace1, offset, key1, table->maxsize1, &table->nsize1);
        free(info);
        return 1;
    }
    else{
        free(info);
        return 0;
    }
}

void printTable1(FILE* fd, Table* table){
    printf("\n\t/////////// ТАБЛИЦА ///////////\n");
    printKey1(fd, table->keySpace1, table->nsize1);
    printf("\t///////////////////////////////\n");
}

Table* getByKey1(FILE* fd, Table* table, int key1){
    Table* newTable = (Table*)malloc(sizeof(Table));
    newTable->keySpace1 = findKey1(table->keySpace1, key1, table->nsize1);
    if(newTable->keySpace1 == NULL){
        free(newTable);
        return NULL;
    }
    newTable->nsize1 = 1;
    newTable->maxsize1 = 1;
    return newTable;
}

int uploadTable(FILE* fd, Table** table){
    fseek(fd, 0, SEEK_SET);
    fwrite(*table, 1, sizeof(Table), fd);
    saveKeySpace1(fd, (*table)->keySpace1, (*table)->nsize1, (*table)->maxsize1);
//    free((*table)->keySpace1);
    return 0;
}

void endOfWork(FILE* fd, Table* table){
    uploadTable(fd, &table);
    freeKeySpace1(table->keySpace1, table->nsize1);
    free(table);
}