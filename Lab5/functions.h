#include <stdio.h>

#define WHITE -1
#define GREY 0
#define BLACK 1
#define INF INT_MAX

typedef struct Edge
{
    struct Vertex *vertex;
    int n;
    int *ports;
    struct Edge *next;
} Edge;

typedef struct Vertex
{
    char *name;
    int port;
    Edge *edges;
    struct Vertex *next;
    int color;
    struct Vertex *pred;
    int d;
} Vertex;

typedef struct Graph
{
    Vertex *vertexes;
} Graph;

typedef struct Queue
{
    Vertex *vertexes[10000];
    int head;
    int tail;
} Queue;

typedef struct Node
{
    Vertex *vertex;
    Edge *edge;
} Node;

typedef struct NodeQueue
{
    Node *nodes[10000];
    int head;
    int tail;
} NodeQueue;

int getInt(int *a);
int LenStr(char *buf);
char *getStr();
char *getStrFromFile(FILE *file);
void push(Queue *Q, Vertex *vertex);
Vertex *pop(Queue *Q);
void *listPush(NodeQueue *list, Vertex *vertex, Edge *edge);
Node *listPop(NodeQueue *list);
void eraseGraph(Graph *graph);
Vertex *searchVertex(Graph *G, char *name);
Edge *searchEdge(Vertex *vertex, char *name);
int addVertex(Graph *graph, char *name, int port);
int addEdge(Graph *graph, char *name1, char *name2, int n, int *ports);
int deleteEdge(Graph *graph, char *name1, char *name2);
int deleteVertex(Graph *graph, char *name);
int changeVertex(Graph *graph, char *name, int port);
int changeEdge(Graph *graph, char *name1, char *name2, int n, int *ports);
void BFS(Graph *graph, Vertex *baseVertex);
void printPath(Graph *graph, Vertex *s, Vertex *v);
void BellmanFord(Graph *graph, Vertex *src);
Graph *MSTPrim(Graph *graph, Vertex *vertex);
int D_AddVertex(Graph *graph);
int D_AddEdge(Graph *graph);
int D_DeleteVertex(Graph *graph);
int D_DeleteEdge(Graph *graph);
int D_ChangeVertex(Graph *graph);
int D_ChangeEdge(Graph *graph);
int D_Bypass(Graph *graph);
int D_ShortestPath(Graph *graph);
int D_MST(Graph *graph);
int D_Show(Graph *graph);
int dialog(const char *msgs[], int N);
