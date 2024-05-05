#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
const char *msgs[] = {"0. Quit", "1. Add", "2. Load", "3. Find", "4. DeepFind", "5. Delete", "6. DeepDelete", "7. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Table *) = {NULL, D_Add, D_Load, D_Find, D_DeepFind, D_Delete, D_DeepDelete, D_Show};

int main()
{
    Table *table = initTable();
    int rc;
    while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](table))
            break;
    printf("That's all. Bye!\n");
    eraseTable(table);
    free(table);

    return 0;
}