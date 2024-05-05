#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
const char *msgs[] = {"0. Quit", "1. Add", "2. Load", "3. Find", "4. SpecialFind", "5. Bypass", "6. Delete", "7. Show", "8. Timing"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Node **) = {NULL, D_Add, D_Load, D_Find, D_SpecialFind, D_Bypass, D_Delete, D_Show, D_Timing};

int main()
{
    Node *root = NULL;
    int rc;

    while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](&root))
            break;
    printf("That's all. Bye!\n");
    eraseTree(&root);

    return 0;
}