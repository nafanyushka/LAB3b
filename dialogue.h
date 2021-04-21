//
// Created by Nafany on 14.04.2021.
//

#ifndef LAB3_DIALOGUE_H
#define LAB3_DIALOGUE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "keyspace1.h"
#include "keyspace2.h"
#include "item.h"
#include "input.h"
#include "table.h"

FILE* openFile(Table** table);
void mainDialogue(FILE* fd, Table* table);

#endif //LAB3_DIALOGUE_H
