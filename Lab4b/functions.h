#include <stdio.h>

#define T 2
typedef struct KeyInfo
{
    char *key;
    char *info;
} KeyInfo;

typedef struct Node
{
    int n;
    KeyInfo *keyInfo[2 * T - 1];
    struct Node *childs[2 * T];
    struct Node *parent;
} Node;

typedef struct ResFind
{
    int i;
    Node *node;
} ResFind;

int getInt(int *a);
int LenStr(char *buf);
char *getStr();
char *getStrFromFile(FILE *file);
int difference(int x, int y);
void eraseTree(Node **node);
Node *minElement(Node *node);
Node *maxElement(Node *node);
ResFind *find(Node *node, char *key);
int bypass(Node *node, char *prefix);
int add(Node **root, char *key, char *info);
int load(Node **node, char *fileName);
KeyInfo *delete(Node **node, char *key);
void show(Node *root, int i, FILE *file);
int D_Add(Node **node);
int D_Load(Node **node);
int D_Find(Node **node);
int D_SpecialFind(Node **node);
int D_Bypass(Node **node);
int D_Delete(Node **node);
int D_Show(Node **node);
int D_Timing(Node **);
int dialog(const char *msgs[], int N);
