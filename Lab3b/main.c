#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show};

int main()
{
    Table table = {0, 0, NULL, NULL};
    int rc;

    if (D_Load(&table) == 0)
        return 1;

    while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](&table))
            break;
    printf("That's all. Bye!\n");
    D_Save(&table);
    eraseTable(&table);

    return 0;
}