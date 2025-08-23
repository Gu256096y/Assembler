#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "systemErrors.h"

#define BASE 32
#define SIGNES "!@#$%^&*<>abcdefghijklmnopqrstuv"
#define TWO_SIGNES 2
#define MAX_IN_FIVE_BITS 31
#define MOVE 5
#define FIRST_CODE_CELL 100
#define DOT "."
#define OB ".ob"
#define ENT_F ".ent"
#define END_S '\0'
#define EXT_F ".ext"

void obj_maker(short code[], short data[], int code_size, int data_size, char *file_name);
char* base32_maker(short num);

