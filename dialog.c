//
// Created by Nafany on 13.04.2021.
//

#include "dialogue.h"

FILE* openFile(Table** table){
    char* fileName;
    do {
        printf("Введите название файла: ");
        fileName = get_String();
    }while(strlen(fileName) < 1);

    FILE* fd = fopen(fileName, "r+b");
    if(fd == NULL) {
        printf("Не удалось найти файла с таким названием, он будет создан.\n");
        fd = fopen(fileName, "wb");

        int maxsize1;
        do{
            printf("Введите размер первого пространства: ");
            maxsize1 = getInt();
        }while(maxsize1 < 1);

        int maxsize2;

        do{
            printf("Введите размер второго пространства: ");
            maxsize2 = getInt();
        }while(maxsize2 < 1);

        *table = createTable(fd, maxsize1, maxsize2);
    }
    else{
        printf("Ваша таблица будет загружена в оперативную память.\n");
        *table = loadTable(fd);
    }
    fclose(fd);

    return fopen(fileName, "r+b");
}

void mainDialogue(FILE* fd, Table* table){
    int choose = 0;

    do {
        printf("\nВведите 1, чтобы посмотреть таблицу.\n"
               "Введите 2, чтобы вывести второе пространство.\n"
               "Введите 3, чтобы добавить элемент в таблицу.\n"
               "Введите 4, чтобы найти элемент(ы) по первому ключу и версии.\n"
               "Введите 5, чтобы найти элемент по второму ключу.\n"
               "Введите 6, чтобы найти элемент по составному ключу.\n"
               "Введите 7, чтобы удалить элемент(ы) по первому ключу.\n"
               "Введите 8, чтобы удалить элемент по второму ключу.\n"
               "Введите 9, чтобы удалить элемент по составному ключу.\n"
               "Введите 0, чтобы выйти из приложения.\n\n"
               "\tВаш выбор: ");
        choose = getInt();
        Table* newTable;
        Item* item;
        char** keys;
        char* info, * key2;
        int key1, release, bug;
        switch(choose){
            case 1:
                printTable1(fd, table);
                break;
            case 2:
                printKeySpace2(fd, table->keySpace2, table->maxsize2);
                break;
            case 3:
                printf("Введите ключ первого пространства: ");
                key1 = getInt();

                do{
                    printf("Введите ключ второго пространства: ");
                    key2 = get_String();
                }while(strlen(key2) < 1);

                do{
                    printf("Введите информацию: ");
                    info = get_String();
                }while(strlen(info) < 1);

                if(addTable(fd, table, key1, key2, info)){
                    printf("Успещное обновление таблицы.\n");
                }
                else{
                    printf("В таблице не оказалось места для элемента.\n");
                }
                free(info);
                break;
            case 4:
                printf("Введите ключ первого пространства: ");
                key1 = getInt();
                newTable = getByKey1(fd, table, key1);
                if(newTable == NULL){
                    printf("Не удалось найти такой ключ.\n");
                    break;
                }

                printf("Введите желаемую версию, либо 0, чтобы получить все что удалось найти: ");
                release = getInt();

                if(release == 0){
                    printTable1(fd, newTable);
                    free(newTable->keySpace1);
                    free(newTable);
                    break;
                }
                if(release < 0){
                    printf("Ну такой версии точно не бывает.\n");
                    free(newTable->keySpace1);
                    free(newTable);
                    break;
                }
                item = findRelease(fd, newTable->keySpace1, release);
                if(item == NULL){
                    printf("Такой версии найти не удалось.\n");
                    free(newTable->keySpace1);
                    free(newTable);
                }else {
                    printInfoItem(fd, item);
                    free(newTable->keySpace1);
                    free(newTable);
                }
                break;
            case 5:
                do{
                    printf("Введите ключ второго пространства: ");
                    key2 = get_String();
                }while(strlen(key2) < 1);
                bug = findKey2(table->keySpace2, key2, table->maxsize2);
                if(bug == 0)
                    printf("Не найден такой объект.\n");
                else
                    printInfo(fd, bug);
                free(key2);
                break;
            case 6:
                printf("Введите ключ первого пространства: ");
                key1 = getInt();
                do{
                    printf("Введите ключ второго пространства: ");
                    key2 = get_String();
                }while(strlen(key2) < 1);
                item = findFullKey(fd, table, key1, key2);
                if(item == NULL)
                    printf("Не удалось найти такого объекта.\n");
                else{
                    printInfoItem(fd, item);
                    free(item);
                }
                break;
            case 7:
                printf("Введите ключ для первого пространства: ");
                key1 = getInt();
                printf("Введите версию для удаления, либо 0 для удаления всех элементов с введенным ключом: ");
                release = getInt();

                keys = removeKeySpace1(fd, table->keySpace1, &(table->nsize1), key1, &bug, release);
                if(bug == 0){
                    printf("Не найден предмет с таким ключом.\n");
                }
                if(bug == 1){
                    removeKeySpace2(fd, table->keySpace2, *keys, table->maxsize2, &key1);
                    free(*keys);
                    free(keys);
                }

                if(bug > 1){
                    char** key = keys;
                    for(int i = 0; i < bug; i++){
                        removeKeySpace2(fd, table->keySpace2, *key, table->maxsize2, &key1);
                        free(*key);
                        if(i + 1 != bug) {
                            key++;
                        }
                    }
//                    free(&keys);
                }
                break;
            case 8:
                do{
                printf("Введите ключ второго пространства (4 знака): ");
                key2 = get_String();
                }while(strlen(key2) < 1 || strlen(key2) > 4);
                bug = removeKeySpace2(fd, table->keySpace2, key2, table->maxsize2, &key1);
                if(bug == 0){
                    printf("Не удалось найти ничего с таким ключом.\n");
                }
                else{
                    deleteKey1Offset(fd, table->keySpace1, key1, bug, &(table->nsize1));
                    printf("Успешное удаление.\n");
                }
                free(key2);
                break;
            case 9:
                printf("Введите ключ первого пространства: ");
                key1 = getInt();
                do{
                    printf("ВВедите ключ второго пространства: ");
                    key2 = get_String();
                }while(strlen(key2) < 1 || strlen(key2) > 4);
                bug = findKey2(table->keySpace2, key2, table->maxsize2);
                if(bug != 0){
                    item = getItem(fd, bug);
                    if(key1 == item->key1) {
                        bug = removeKeySpace2(fd, table->keySpace2, key2, table->maxsize2, &key1);
                        deleteKey1Offset(fd, table->keySpace1, key1, bug, &(table->nsize1));
                        printf("Успешное удаление.\n");
                    }
                    else
                        printf("Не найдено такого составного ключа.\n");
                    free(item);
                }
                else{
                    printf("Не найдено предмета.\n");
                }
                free(key2);
                break;
            default:
                break;
        }
    }while(choose);

    endOfWork(fd, table);
}