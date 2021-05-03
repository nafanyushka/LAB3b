//
// Created by Nafany on 07.04.2021.
//

#ifndef LAB3_TABLE_H
#define LAB3_TABLE_H

#include "keyspace1.h"
#include "keyspace2.h"

typedef struct Table Table;
typedef struct KeySpace1 KeySpace1;
typedef struct KeySpace2 KeySpace2;


struct Table{
    KeySpace1* keySpace1;
    KeySpace2* keySpace2;
    int maxsize1;
    int maxsize2;
    int nsize1;
/*TODO:
 *  Включение нового элемента в таблицу с соблюдением ограничений на уникальность ключей в соответствующих
 * ключевых пространствах (см. предыдущие пункты задания) и уникальности составного ключа (key1, key2);
 *  Поиск в таблице элемента, заданного составным ключом;
 *  Удаление из таблицы элемента, заданного составным ключом;
 *  Поиск в таблице элемента по любому заданному ключу; результатом поиска должна быть копии всех найденных элементов
 * со значениями ключей; *(дополнительно) поиск может быть реализован как итератор одним из возможных способов,
 * например, в этом случае метод возвращает один из возможных элементов, повторный вызов следующий элемент и т.д.
 *  Удаление из таблицы всех элементов, заданного ключом в одном из ключевых пространств;
 *  Вывод содержимого таблицы на экран (или текстовый файл); при этом формат вывода должен соответствовать приведенной
 * выше структуре элемента таблицы;
 *  Особые операции, определяемые в соответствующем пространстве ключей.
 */
};

Table* createTable(FILE* fd, int maxsize1, int maxsize2);
Table* loadTable(FILE* fd);
void printTable1(FILE* fd, Table* table);
Table* getByKey1(FILE* fd, Table* table, int key1);
int uploadTable(FILE* fd, Table** table);
int addTable(FILE* fd, Table* table, int key1, char* key2, char* info);
void endOfWork(FILE* fd, Table* table);
Item* findFullKey(FILE* fd, Table* table, int key1, char* key2);

/* TODO:
 *  Сделать вывод первой штуковины через findByKey_1
                                                    */
#endif //LAB3_TABLE_H