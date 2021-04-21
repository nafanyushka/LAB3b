//
// Created by Nafany on 07.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyspace2.h"
#include "keyspace1.h"
#include "input.h"

KeySpace2* initKeySpace2(int maxsize2) {
    KeySpace2* keySpace2 = (KeySpace2*) calloc(sizeof(KeySpace2), maxsize2);
    return keySpace2;
}