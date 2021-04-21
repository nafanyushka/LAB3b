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
               "Введите 2, чтобы добавить элемент в таблицу.\n"
               "Введите 3, чтобы найти элемент(ы) по ключу и версии.\n"
               "Введите 4, чтобы удалить элемент(ы) по ключу.\n"
               "Введите 0, чтобы выйти из приложения.\n\n"
               "\tВаш выбор: ");
        choose = getInt();
        Table* newTable;
        Item* item;
        char* info;
        int key1, release, bug;
        switch(choose){
            case 1:
                printTable1(fd, table);
                break;
            case 2:
                printf("Введите ключ первого пространства: ");
                key1 = getInt();

                do{
                    printf("Введите информацию: ");
                    info = get_String();
                }while(strlen(info) < 1);

                if(addTable(fd, table, key1, info)){
                    printf("Успещное обновление таблицы.\n");
                }
                else{
                    printf("В таблице не оказалось места для элемента.\n");
                }
                free(info);
                break;
            case 3:
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
            case 4:
                printf("Введите ключ для первого пространства: ");
                key1 = getInt();
                printf("Введите версию для удаления, либо 0 для удаления всех элементов с введенным ключом: ");
                release = getInt();

                bug = removeKeySpace1(table->keySpace1, &(table->nsize1), key1, release);
                if(bug == 1){
                    printf("Удачное удаление.\n");
                }
                if(bug == 0){
                    printf("Ключ найден, но версия не найдена.\n");
                }
                if(bug == -1){
                    printf("Ключ не найден.\n");
                }
                break;
            default:
                break;
        }
    }while(choose);

    endOfWork(fd, table);
}