#include <stdio.h>
typedef struct Node
{
    int release;
    int infoLen;
    int infoOffset;
    int nodeOffset;
    struct Node *next;
} Node;

typedef struct KeySpace
{
    int keyLen;
    int keyOffset;
    int nodeOffset;
    int ksOffset;
    Node *node;
    struct KeySpace *next;
} KeySpace;

typedef struct Table
{
    int msize;
    int csize;
    KeySpace *ks;
    FILE *file;
} Table;

typedef struct Vector
{
    int size;
    Node **nodes;
} Vector;

int getInt(int *a);
int LenStr(char *buf);
char *getStr();
int hashStr(const char *key);
char *getStrFromFile(FILE *file);
void printNode(FILE *file, Node *node, int offsetLevel);
void eraseTable(Table *table);
Vector *findKey(Table *table, char *key);
Node *findRelease(Vector *vector, int release);
int add(Table *table, char *key, char *info);
int load(Table *table, char *fileName);
int create(Table *table, char *fname, int msize);
Node *delete(Table *table, char *key, int release);
int D_Add(Table *table);
int D_Load(Table *table);
int D_Save(Table *table);
int D_Find(Table *table);
int D_Delete(Table *table);
int D_Show(Table *table);
int dialog(const char *msgs[], int N);
