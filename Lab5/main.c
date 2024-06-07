#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
const char *msgs[] = {"0. Quit", "1. Add computer", "2. Add connection", "3. Delete computer", "4. Delete connection", "5. Change computer", "6. Change connection", "7. Bypass", "8. Shortest Path", "9. MST", "10. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int (*fptr[])(Graph *) = {NULL, D_AddVertex, D_AddEdge, D_DeleteVertex, D_DeleteEdge, D_ChangeVertex, D_ChangeEdge, D_Bypass, D_ShortestPath, D_MST, D_Show};

int main()
{
    Graph graph;
    graph.vertexes = NULL;
    int rc;
    while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](&graph))
            break;
    printf("That's all. Bye!\n");
    eraseGraph(&graph);

    return 0;
}