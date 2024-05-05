#include <stdio.h>
typedef struct Node
{
    int key;
    char *info;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

int getInt(int *a);
int LenStr(char *buf);
char *getStr();
char *getStrFromFile(FILE *file);
int difference(int x, int y);
void printTree(Node *node, int level);
void eraseTree(Node **node);
Node *minElement(Node *node);
Node *maxElement(Node *node);
Node *prevElement(Node *node);
Node *nextElement(Node *node);
Node *find(Node *node, int key);
Node **specialFind(Node *node, int key);
int bypass(Node *node, int tens);
int add(Node **root, int key, char *info);
int load(Node **node, char *fileName);
Node *delete(Node **node, int key);
void show(Node *root,FILE *file);
int D_Add(Node **node);
int D_Load(Node **node);
int D_Find(Node **node);
int D_SpecialFind(Node **node);
int D_Bypass(Node **node);
int D_Delete(Node **node);
int D_Show(Node **node);
int D_Timing(Node **);
int dialog(const char *msgs[], int N);
