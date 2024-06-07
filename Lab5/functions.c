#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <time.h>
#include "functions.h"

int getInt(int *a)
{
    int n;
    do
    {
        n = scanf("%d", a, sizeof(int));
        if (n < 0)
            return 0;
        if (n == 0)
        {
            printf("%s\n", "Error! Repeat input");
            scanf("%*c", 0);
        }
    } while (n == 0);
    scanf("%*c", 0);
    return 1;
}

int LenStr(char *buf)
{
    int i = 0;
    while (*(buf + i))
    {
        ++i;
    }
    return i;
}

char *getStr()
{
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do
    {
        n = scanf("%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
            {
                return NULL;
            }
        }
        else if (n > 0)
        {
            int chunk_len = LenStr(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else
        {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0)
    {
        res[len] = '\0';
    }

    return res;
}

int getIntFromFile(FILE *file, int *a)
{
    int n;
    n = fscanf(file, "%d\n", a);
    return n < 1 ? 0 : 1;
}

char *getStrFromFile(FILE *file)
{
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do
    {
        n = fscanf(file, "%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
            {
                return NULL;
            }
        }
        else if (n > 0)
        {
            int chunk_len = LenStr(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else
        {
            fscanf(file, "%*c");
        }
    } while (n > 0);

    if (len > 0)
    {
        res[len] = '\0';
    }

    return res;
}

void push(Queue *Q, Vertex *vertex)
{
    Q->vertexes[Q->tail] = vertex;
    Q->tail += 1;
}

Vertex *pop(Queue *Q)
{
    Vertex *vertex = Q->vertexes[Q->head];
    Q->head += 1;
    return vertex;
}

Node *listPop(NodeQueue *list)
{
    Node *node = list->nodes[list->head];
    list->nodes[list->head] = NULL;
    list->head += 1;
    return node;
}

void *listPush(NodeQueue *list, Vertex *vertex, Edge *edge)
{
    Node *node = malloc(sizeof(Node));
    node->vertex = vertex;
    node->edge = edge;
    list->nodes[list->tail] = node;
    list->tail += 1;
}

void eraseGraph(Graph *graph)
{
    Vertex *vertex = graph->vertexes;
    while (vertex)
    {
        deleteVertex(graph, vertex->name);
        vertex = graph->vertexes;
    }
}

Vertex *searchVertex(Graph *G, char *name)
{
    Vertex *vertex = G->vertexes;
    while (vertex)
    {
        if (strcmp(vertex->name, name) == 0)
        {
            return vertex;
        }
        vertex = vertex->next;
    }
    return NULL;
}

Edge *searchEdge(Vertex *vertex, char *name)
{
    Edge *edge = vertex->edges;
    while (edge)
    {
        if (strcmp(edge->vertex->name, name) == 0)
        {
            return edge;
        }
        edge = edge->next;
    }
    return NULL;
}

int addVertex(Graph *graph, char *name, int port)
{
    Vertex *vertex = searchVertex(graph, name);
    if (vertex)
    {
        return 0;
    }

    vertex = malloc(sizeof(Vertex));
    vertex->name = name;
    vertex->port = port;
    vertex->edges = NULL;
    vertex->next = graph->vertexes;
    graph->vertexes = vertex;

    return 1;
}

int addEdge(Graph *graph, char *name1, char *name2, int n, int *ports)
{
    Vertex *vertex1 = searchVertex(graph, name1);
    Vertex *vertex2 = searchVertex(graph, name2);

    if (!vertex1 || !vertex2)
        return 0;
    Edge *edge = malloc(sizeof(Edge));
    edge->vertex = vertex2;
    edge->n = n;
    edge->ports = ports;
    edge->next = vertex1->edges;
    vertex1->edges = edge;

    edge = malloc(sizeof(Edge));
    edge->vertex = vertex1;
    edge->n = n;
    edge->ports = ports;
    edge->next = vertex2->edges;
    vertex2->edges = edge;

    return 1;
}

int deleteEdge(Graph *graph, char *name1, char *name2)
{
    int v1, v2;
    Vertex *vertex1 = searchVertex(graph, name1);
    Vertex *vertex2 = searchVertex(graph, name2);

    if (!vertex1 || !vertex2)
        return -1;

    Edge *prev, *cur;
    prev = NULL;
    cur = vertex1->edges;

    while (cur && strcmp(cur->vertex->name, name2) != 0)
    {
        prev = cur;
        cur = cur->next;
    }
    if (!cur)
        return 0;
    if (!prev)
        vertex1->edges = cur->next;
    else
        prev->next = cur->next;
    free(cur);

    prev = NULL;
    cur = vertex2->edges;

    while (cur && strcmp(cur->vertex->name, name1) != 0)
    {
        prev = cur;
        cur = cur->next;
    }
    if (!cur)
        return 0;
    if (!prev)
        vertex2->edges = cur->next;
    else
        prev->next = cur->next;
    free(cur->ports);
    free(cur);

    return 1;
}

int deleteVertex(Graph *graph, char *name)
{
    Vertex *curVertex = graph->vertexes;
    Vertex *prevVertex = NULL;
    Vertex *vertex;
    while (curVertex && strcmp(curVertex->name, name) != 0)
    {
        prevVertex = curVertex;
        curVertex = curVertex->next;
    }
    if (!curVertex)
        return 0;

    Edge *prevEdge, *curEdge, *baseEdge;
    baseEdge = curVertex->edges;
    while (baseEdge)
    {
        vertex = searchVertex(graph, baseEdge->vertex->name);
        prevEdge = NULL;
        curEdge = vertex->edges;
        while (curEdge && strcmp(curEdge->vertex->name, name) != 0)
        {
            prevEdge = curEdge;
            curEdge = curEdge->next;
        }
        if (!curEdge)
            return 0;
        if (!prevEdge)
            vertex->edges = curEdge->next;
        else
            prevEdge->next = curEdge->next;
        free(curEdge->ports);
        free(curEdge);
        curEdge = baseEdge;
        baseEdge = baseEdge->next;
        free(curEdge);
    }

    if (!prevVertex)
        graph->vertexes = curVertex->next;
    else
        prevVertex->next = curVertex->next;
    free(curVertex->name);
    free(curVertex);

    return 1;
}

int changeVertex(Graph *graph, char *name, int port)
{
    Vertex *vertex = searchVertex(graph, name);
    if (!vertex)
        return 0;
    vertex->port = port;
    return 1;
}
int changeEdge(Graph *graph, char *name1, char *name2, int n, int *ports)
{
    Vertex *vertex1 = searchVertex(graph, name1);
    Vertex *vertex2 = searchVertex(graph, name2);
    if (!vertex1 || !vertex2)
        return 0;
    Edge *edge = searchEdge(vertex1, name2);
    if (!edge)
        return -1;
    free(edge->ports);
    edge->n = n;
    edge->ports = ports;

    edge = searchEdge(vertex2, name1);
    if (!edge)
        return -1;
    edge->n = n;
    edge->ports = ports;
    return 1;
}

void BFS(Graph *graph, Vertex *baseVertex)
{
    Vertex *vertex = graph->vertexes;
    Edge *edge;
    while (vertex)
    {
        vertex->color = WHITE;
        vertex->d = INF;
        vertex->pred = NULL;
        vertex = vertex->next;
    }
    Queue que;
    que.head = 0;
    que.tail = 0;
    baseVertex->color = GREY;
    baseVertex->d = 0;
    push(&que, baseVertex);

    while (que.head != que.tail)
    {
        vertex = pop(&que);
        edge = vertex->edges;
        while (edge)
        {
            if (edge->vertex->color == WHITE)
            {
                int flag1 = 0, flag2 = 0;
                for (int i = 0; i < edge->n; i++)
                {
                    if (edge->ports[i] == vertex->port)
                        flag1 = 1;
                    if (edge->ports[i] == edge->vertex->port)
                        flag2 = 1;
                }

                if (flag1 && flag2)
                {
                    edge->vertex->color = GREY;
                    edge->vertex->d = vertex->d + 1;
                    edge->vertex->pred = vertex;
                    push(&que, edge->vertex);
                }
            }

            edge = edge->next;
        }
        vertex->color = BLACK;
    }
}

void printPath(Graph *graph, Vertex *s, Vertex *v)
{
    if (v == s)
        printf("%s\n", s->name);
    else
    {
        if (v->pred == NULL)
            printf("Trere is no path\n", s->name);
        else
        {
            printPath(graph, s, v->pred);
            printf("%s\n", v->name);
        }
    }
}

void BellmanFord(Graph *graph, Vertex *src)
{
    Vertex *vertex = graph->vertexes;
    Edge *edge;
    int n = 0;
    while (vertex)
    {
        vertex->d = INF;
        vertex->pred = NULL;
        vertex = vertex->next;
        n++;
    }
    src->d = 0;
    for (int i = 0; i < n - 1; i++)
    {
        vertex = graph->vertexes;
        while (vertex)
        {
            edge = vertex->edges;
            while (edge)
            {
                int flag1 = 0, flag2 = 0;
                for (int i = 0; i < edge->n; i++)
                {
                    if (edge->ports[i] == vertex->port)
                        flag1 = 1;
                    if (edge->ports[i] == edge->vertex->port)
                        flag2 = 1;
                }

                if (flag1 && flag2)
                {
                    if (edge->vertex->d - 1 > vertex->d)
                    {
                        edge->vertex->d = vertex->d + 1;
                        edge->vertex->pred = vertex;
                    }
                }

                edge = edge->next;
            }
            vertex = vertex->next;
        }
    }
}

Graph *MSTPrim(Graph *graph, Vertex *vertex)
{
    Vertex *ptr = graph->vertexes;
    while (ptr)
    {
        ptr->color = WHITE;
        ptr = ptr->next;
    }
    vertex->color = BLACK;
    char *name;
    Graph *res = malloc(sizeof(Graph));
    res->vertexes = NULL;
    name = malloc(sizeof(char) * (LenStr(vertex->name) + 1));
    strcpy(name, vertex->name);
    addVertex(res, name, vertex->port);

    NodeQueue list;
    list.head = 0;
    list.tail = 0;

    Edge *edge = vertex->edges;
    Node *node;
    int *ports;
    while (edge)
    {
        int flag1 = 0, flag2 = 0;
        for (int i = 0; i < edge->n; i++)
        {
            if (edge->ports[i] == vertex->port)
                flag1 = 1;
            if (edge->ports[i] == edge->vertex->port)
                flag2 = 1;
        }

        if (flag1 && flag2)
        {
            listPush(&list, vertex, edge);
            edge->vertex->color = BLACK;
        }
        edge = edge->next;
    }

    while (node = listPop(&list))
    {
        vertex = node->vertex;
        edge = node->edge;

        name = malloc(sizeof(char) * (LenStr(edge->vertex->name) + 1));
        strcpy(name, edge->vertex->name);
        addVertex(res, name, edge->vertex->port);
        ports = malloc(sizeof(int) * edge->n);
        memcpy(ports, edge->ports, sizeof(int) * edge->n);
        addEdge(res, vertex->name, name, edge->n, ports);

        vertex = node->edge->vertex;
        edge = vertex->edges;
        while (edge)
        {
            if (edge->vertex->color != BLACK)
            {
                int flag1 = 0, flag2 = 0;
                for (int i = 0; i < edge->n; i++)
                {
                    if (edge->ports[i] == vertex->port)
                        flag1 = 1;
                    if (edge->ports[i] == edge->vertex->port)
                        flag2 = 1;
                }

                if (flag1 && flag2)
                {
                    listPush(&list, vertex, edge);
                    edge->vertex->color = BLACK;
                }
            }
            edge = edge->next;
        }
    }

    return res;
}

int D_AddVertex(Graph *graph)
{
    int rc, port = -1;
    char *name = NULL;

    printf("Enter name of computer: --> ");
    name = getStr();
    if (name == NULL)
        return 0;
    printf("Enter port: --> ");
    char *errmsg = "";
    while (port < 1)
    {
        puts(errmsg);
        errmsg = "Error! Repeat input";
        if (!getInt(&port))
            return 0;
    }

    rc = addVertex(graph, name, port);
    if (rc)
        puts("Ok\n");
    else
    {
        printf("Duplicate name: %s\n", name);
        free(name);
    }
    puts("");

    return 1;
}

int D_AddEdge(Graph *graph)
{
    int rc, n = -1;
    char *name1 = NULL, *name2 = NULL;
    int *ports;

    printf("Enter name of first computer: --> ");
    name1 = getStr();
    if (!name1)
        return 0;
    printf("Enter name of second computer: --> ");
    name2 = getStr();
    if (!name2)
    {
        free(name1);
        return 0;
    }
    printf("Enter number of allowed ports: --> ");
    char *errmsg = "";
    while (n < 1)
    {
        puts(errmsg);
        errmsg = "Error! Repeat input";
        if (!getInt(&n))
            return 0;
    }

    ports = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        errmsg = "";
        printf("Enter allowed port #%d: --> ", i + 1);
        do
        {
            puts(errmsg);
            errmsg = "Error! Repeat input";
            if (!getInt((ports + i)))
            {
                free(name1);
                free(name2);
                free(ports);
                return 0;
            }
        } while (*(ports + i) < 1);
    }
    rc = addEdge(graph, name1, name2, n, ports);
    if (rc)
        puts("Ok\n");
    else
    {
        printf("Computer names are spelled incorrectly\n");
        free(ports);
    }
    free(name1);
    free(name2);
    puts("");

    return 1;
}

int D_DeleteVertex(Graph *graph)
{
    char *name;
    printf("Enter name of computer: -->");
    name = getStr();
    if (!name)
        return 0;
    puts("");
    if (!deleteVertex(graph, name))
    {
        printf("An computer with the same name does not exists\n\n");
    }
    else
    {
        printf("Ok\n");
    }
    free(name);

    return 1;
}

int D_DeleteEdge(Graph *graph)
{
    int rc;
    char *name1 = NULL, *name2 = NULL;
    printf("Enter name of first computer: -->");
    name1 = getStr();
    if (!name1)
        return 0;
    printf("Enter name of second computer: -->");
    name2 = getStr();
    if (!name2)
    {
        free(name1);
        return 0;
    }
    puts("");
    rc = deleteEdge(graph, name1, name2);
    if (rc == 1)
    {
        printf("Ok\n");
    }
    else if (rc == 0)
    {
        printf("Computer names are spelled incorrectly\n\n");
    }
    else
    {
        printf("There is no connection between these computers\n\n");
    }
    free(name1);
    free(name2);

    return 1;
}

int D_ChangeVertex(Graph *graph)
{
    int rc, port = -1;
    char *name = NULL;

    printf("Enter name of computer: --> ");
    name = getStr();
    if (name == NULL)
        return 0;
    printf("Enter port: --> ");
    char *errmsg = "";
    while (port < 1)
    {
        puts(errmsg);
        errmsg = "Error! Repeat input";
        if (!getInt(&port))
            return 0;
    }

    rc = changeVertex(graph, name, port);
    if (rc)
        puts("Ok\n");
    else
    {
        printf("An computer with the same name does not exists\n");
    }
    free(name);
    puts("");

    return 1;
}

int D_ChangeEdge(Graph *graph)
{
    int rc, n = -1;
    char *name1 = NULL, *name2 = NULL;
    int *ports;

    printf("Enter name of first computer: --> ");
    name1 = getStr();
    if (!name1)
        return 0;
    printf("Enter name of second computer: --> ");
    name2 = getStr();
    if (!name2)
    {
        free(name1);
        return 0;
    }
    printf("Enter number of allowed ports: --> ");
    char *errmsg = "";
    while (n < 1)
    {
        puts(errmsg);
        errmsg = "Error! Repeat input";
        if (!getInt(&n))
            return 0;
    }

    ports = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        errmsg = "";
        printf("Enter allowed port #%d: --> ", i + 1);
        do
        {
            puts(errmsg);
            errmsg = "Error! Repeat input";
            if (!getInt((ports + i)))
            {
                free(name1);
                free(name2);
                free(ports);
                return 0;
            }
        } while (*(ports + i) < 1);
    }
    rc = changeEdge(graph, name1, name2, n, ports);
    if (rc == 1)
        puts("Ok\n");
    else if (rc == 0)
    {
        printf("Computer names are spelled incorrectly\n");
        free(ports);
    }
    else
    {
        printf("There is no connection between computers\n");
        free(ports);
    }
    free(name1);
    free(name2);
    puts("");

    return 1;
}

int D_Bypass(Graph *graph)
{
    int port = -1;
    char *name = NULL;

    printf("Enter name of computer: --> ");
    name = getStr();
    if (name == NULL)
        return 0;

    Vertex *s = searchVertex(graph, name);
    if (s)
    {
        BFS(graph, s);
        Vertex *d = graph->vertexes;
        while (d)
        {
            if (d != s && d->d != INF)
            {
                printf("name: %s pathLength: %d\n", d->name, d->d);
                printPath(graph, s, d);
            }
            d = d->next;
        }
        puts("Ok\n");
    }
    else
    {
        printf("An computer with the same name does not exists\n");
    }
    free(name);
    puts("");

    return 1;
}

int D_ShortestPath(Graph *graph)
{
    int port = -1;
    char *name1 = NULL, *name2;

    printf("Enter name of source computer: --> ");
    name1 = getStr();
    if (!name1)
        return 0;
    printf("Enter name of dest computer: --> ");
    name2 = getStr();
    if (!name2)
    {
        free(name1);
        return 0;
    }

    Vertex *src = searchVertex(graph, name1);
    Vertex *dest = searchVertex(graph, name2);
    if (src && dest)
    {
        BellmanFord(graph, src);
        if (dest->d == INF)
        {
            printf("There is no path between the given vertexes\n");
        }
        else
        {
            printf("name: %s pathLength: %d\n", dest->name, dest->d);
            printPath(graph, src, dest);
        }
        puts("Ok\n");
    }
    else
    {
        printf("Computer names are spelled incorrectly\n");
    }
    free(name1);
    free(name2);
    puts("");

    return 1;
}

int D_MST(Graph *graph)
{
    int port = -1;
    char *name = NULL;

    printf("Enter name of computer: --> ");
    name = getStr();
    if (name == NULL)
        return 0;

    Vertex *s = searchVertex(graph, name);
    if (s)
    {
        Graph *res = MSTPrim(graph, s);
        D_Show(res);
        puts("Ok\n");
        eraseGraph(res);
    }
    else
    {
        printf("An computer with the same name does not exists\n");
    }
    free(name);
    puts("");

    return 1;
}

void show(Graph *graph, FILE *file)
{
    Vertex *vertex = graph->vertexes;
    Edge *edge;
    while (vertex)
    {
        vertex->color = WHITE;
        vertex = vertex->next;
    }
    vertex = graph->vertexes;
    while (vertex)
    {
        fprintf(file, "\t%s[label = \"%s : %d\"]\n", vertex->name, vertex->name, vertex->port);
        edge = vertex->edges;
        while (edge)
        {
            if (edge->vertex->color != BLACK)
            {
                fprintf(file, "\t%s -- %s[label=\"%d", vertex->name, edge->vertex->name, edge->ports[0]);
                for (int i = 1; i < edge->n; i++)
                {
                    fprintf(file, ", %d", edge->ports[i]);
                }
                fprintf(file, "\"]\n");
            }
            edge = edge->next;
        }
        vertex->color = BLACK;
        vertex = vertex->next;
    }
}

int D_Show(Graph *graph)
{
    FILE *file;
    file = fopen("g.gv", "w");
    if (file == NULL)
    {
        printf("Ошибка!\n");
        return 0;
    }
    fprintf(file, "graph G{\n");

    show(graph, file);

    fprintf(file, "}");
    fclose(file);
    system("dot -Tpng g.gv -o gv.png");
    system("start mspaint gv.png");

    return 1;
}

int dialog(const char *msgs[], int N)
{
    char *errmsg = "";
    int rc;
    int i, n;

    do
    {
        printf(errmsg);
        errmsg = "You are wrong. Repeate, please!\n";
        for (i = 0; i < N; ++i)
            puts(msgs[i]);
        printf("Make your choice: --> ");
        n = getInt(&rc);
        if (n == 0)
            rc = 0;
    } while (rc < 0 || rc >= N);

    return rc;
}
