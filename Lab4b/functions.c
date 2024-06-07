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

int difference(int x, int y)
{
    int res = x - y;
    if (res < 0)
        return -res;
    else
        return res;
}

void eraseTree(Node **node)
{
    if (*node)
    {
        Node *x = *node;
        for(int i = 0; i < x->n + 1; i ++)
        {
            eraseTree(&(x->childs[i]));
        }
        for(int i = 0; i < x->n; i ++)
        {
            free(x->keyInfo[i]->key);
            free(x->keyInfo[i]->info);
            free(x->keyInfo[i]);
        }
        free(x);
    }
    *node = NULL;
}


Node *minElement(Node *node)
{
    if (node)
        while (node->childs[0])
            node = node->childs[0];
    return node;
}

Node *maxElement(Node *node)
{
    if (node)
        while (node->childs[node->n])
            node = node->childs[node->n];
    return node;
}

ResFind *find(Node *node, char *key)
{
    while (node)
    {
        int i;
        int n = 1;
        for (i = 0; i < node->n && n > 0; i++)
        {
            n = strcmp(key, node->keyInfo[i]->key);
            if (n == 0)
            {
                ResFind *resFind = malloc(sizeof(ResFind));
                resFind->i = i;
                resFind->node = node;
                return resFind; // В РОДИТЕЛЬСКОЙ ФУНКЦИИ ОСВОБОДИТЬ КЛЮЧ И ИНФУ
            }
        }
        if (n < 0)
            i--;
        node = node->childs[i];
    }
    return NULL;
}

int bypass(Node *node, char *prefix)
{
    if (node)
    {
        for (int i = 0; i < node->n + 1; i++)
        {
            bypass(node->childs[i], prefix);
        }
        for (int i = 0; i < node->n; i++)
        {
            if (strncmp(prefix, node->keyInfo[i]->key, strlen(prefix)) == 0)
                printf("Found element : %s\n", node->keyInfo[i]->key);
        }
    }
    return 1;
}

int split(Node *node, int i)
{
    Node *next = node->childs[i];
    int n = 2 * T - 1;
    node->childs[node->n + 1] = node->childs[node->n];
    for (int j = node->n; j > i; j--)
    {
        node->keyInfo[j] = node->keyInfo[j - 1];
        node->childs[j] = node->childs[j - 1];
    }
    Node *newNode = malloc(sizeof(Node));
    newNode->n = 0;
    newNode->parent = node;
    for (int j = 0; j < 2 * T - 1; j++)
    {
        newNode->childs[j] = NULL;
        newNode->keyInfo[j] = NULL;
    }
    newNode->childs[2 * T - 1] = NULL;
    for (int j = n / 2 + 1; j < 2 * T - 1; j++)
    {
        newNode->keyInfo[j - n / 2 - 1] = next->keyInfo[j];
        newNode->childs[j - n / 2 - 1] = next->childs[j];
        if (newNode->childs[j - n / 2 - 1])
        {
            newNode->childs[j - n / 2 - 1]->parent = newNode;
        }
        newNode->n++;
        next->keyInfo[j] = NULL;
        next->childs[j] = NULL;
        next->n--;
    }
    newNode->childs[2 * T - n / 2 - 2] = next->childs[2 * T - 1];
    if (newNode->childs[2 * T - n / 2 - 2])
    {
        newNode->childs[2 * T - n / 2 - 2]->parent = newNode;
    }
    next->childs[2 * T - 1] = NULL;

    node->childs[i + 1] = newNode;
    newNode->parent = node;
    node->keyInfo[i] = next->keyInfo[n / 2];
    next->keyInfo[n / 2] = NULL;
    next->n--;
    node->n++;

    return 1;
}

int add(Node **root, char *key, char *info)
{
    if (*root == NULL)
    {
        *root = malloc(sizeof(Node));
        (*root)->n = 0;
        (*root)->parent = NULL;
        for (int i = 0; i < 2 * T - 1; i++)
        {
            (*root)->keyInfo[i] = NULL;
            (*root)->childs[i] = NULL;
        }
        (*root)->childs[2 * T - 1] = NULL;
    }

    Node *node = *root;

    if (node->n == 2 * T - 1)
    {
        Node *newNode = malloc(sizeof(Node));
        newNode->n = 0;
        newNode->parent = NULL;
        for (int i = 0; i < 2 * T - 1; i++)
        {
            newNode->keyInfo[i] = NULL;
            newNode->childs[i] = NULL;
        }
        newNode->childs[2 * T - 1] = NULL;

        newNode->childs[0] = node;
        node->parent = newNode;
        *root = newNode;
        split(newNode, 0);
        node = newNode;
    }

    while (node->childs[0] != NULL)
    {
        int n = 1;
        int i;
        for (i = 0; i < node->n && n > 0; i++)
        {
            n = strcmp(key, node->keyInfo[i]->key);
            if (n == 0)
                return 0; // В РОДИТЕЛЬСКОЙ ФУНКЦИИ ОСВОБОДИТЬ КЛЮЧ И ИНФУ
        }
        if (n < 0)
            i--;
        Node *next = node->childs[i];
        if (next->n == 2 * T - 1)
        {
            split(node, i);
            n = strcmp(key, node->keyInfo[i]->key);
            if (n == 0)
                return 0; // В РОДИТЕЛЬСКОЙ ФУНКЦИИ ОСВОБОДИТЬ КЛЮЧ И ИНФУ
            if (strcmp(key, node->keyInfo[i]->key) > 0)
                next = node->childs[i + 1];
        }
        node = next;
    }

    int n = 1;
    int i;
    for (i = 0; i < node->n && n > 0; i++)
    {
        n = strcmp(key, node->keyInfo[i]->key);
        if (n == 0)
            return 0; // В РОДИТЕЛЬСКОЙ ФУНКЦИИ ОСВОБОДИТЬ КЛЮЧ И ИНФУ
    }
    if (n < 0)
        i--;

    for (int j = node->n; j > i; j--)
    {
        node->keyInfo[j] = node->keyInfo[j - 1];
    }

    KeyInfo *newKeyInfo = malloc(sizeof(KeyInfo));
    newKeyInfo->key = key;
    newKeyInfo->info = info;
    node->keyInfo[i] = newKeyInfo;
    node->n++;

    return 1;
}

int load(Node **node, char *fileName)
{
    char *key, *info;
    int n;
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        while (!feof(file))
        {
            key = getStrFromFile(file);
            info = getStrFromFile(file);
            if (!key || !info || !add(node, key, info))
            {
                eraseTree(node);
                free(key);
                free(info);
                fclose(file);
                return 0;
            }
        }
        fclose(file);
        return 1;
    }
    else
        return -1;
}

KeyInfo *delete(Node **node, char *key)
{
    Node *x = (*node);
    Node *y;
    Node *z;
    KeyInfo *result = NULL;
    char *inKey = key;
    if (!x)
        return NULL;
    if (x->parent == NULL && x->n == 1 && x->childs[0])
    {
        y = x->childs[0];
        z = x->childs[1];
        if (y->n == T - 1 && z->n == T - 1)
        {
            y->keyInfo[T - 1] = x->keyInfo[0];
            y->n++;
            for (int i = 0; i < T - 1; i++)
            {
                y->keyInfo[T + i] = z->keyInfo[i];
                y->childs[T + i] = z->childs[i];
                if (y->childs[T + i])
                    y->childs[T + i]->parent = y;
                y->n++;
            }
            y->childs[2 * T - 1] = z->childs[T - 1];
            if (y->childs[2 * T - 1])
                y->childs[2 * T - 1]->parent = y;
            y->parent = NULL;
            (*node) = y;
            free(x);
            free(z);
            x = y;
        }
    }

    while (x->childs[0] != NULL)
    {
        int i, n = 1;
        for (i = 0; i < x->n && n > 0; i++)
        {
            n = strcmp(inKey, x->keyInfo[i]->key);
        }

        if (n <= 0)
            i--;

        if (n != 0)
        {
            y = x->childs[i];
            if (y->n == T - 1)
            {
                if (i > 0 && x->childs[i - 1]->n > T - 1)
                {
                    // правый поворот
                    z = x->childs[i - 1];
                    y->childs[y->n + 1] = y->childs[y->n];
                    for (int j = y->n; j > 0; j--)
                    {
                        y->keyInfo[j] = y->keyInfo[j - 1];
                        y->childs[j] = y->childs[j - 1];
                    }
                    if (n < 0)
                    {
                        y->keyInfo[0] = x->keyInfo[i];
                        x->keyInfo[i] = z->keyInfo[z->n - 1];
                    }
                    else
                    {
                        y->keyInfo[0] = x->keyInfo[i - 1];
                        x->keyInfo[i - 1] = z->keyInfo[z->n - 1];
                    }
                    y->childs[0] = z->childs[z->n];
                    if (y->childs[0])
                        y->childs[0]->parent = y;
                    z->keyInfo[z->n - 1] = NULL;
                    z->childs[z->n] = NULL;
                    y->n++;
                    z->n--;
                }
                else if (i < x->n && x->childs[i + 1]->n > T - 1)
                {
                    // левый поворот
                    z = x->childs[i + 1];

                    y->keyInfo[y->n] = x->keyInfo[i];
                    x->keyInfo[i] = z->keyInfo[0];
                    y->childs[y->n + 1] = z->childs[0];
                    if (y->childs[y->n + 1])
                        y->childs[y->n + 1]->parent = y;
                    for (int j = 0; j < z->n - 1; j++)
                    {
                        z->keyInfo[j] = z->keyInfo[j + 1];
                        z->childs[j] = z->childs[j + 1];
                    }
                    z->childs[z->n - 1] = z->childs[z->n];
                    z->keyInfo[z->n - 1] = NULL;
                    z->childs[z->n] = NULL;
                    y->n++;
                    z->n--;
                }
                else
                {
                    if (i > 0)
                    {
                        z = y;
                        y = x->childs[i - 1];
                        i--;
                    }
                    else
                        z = x->childs[i + 1];
                    y->keyInfo[y->n] = x->keyInfo[i];
                    y->n++;
                    for (int j = i; j < x->n - 1; j++)
                    {
                        x->keyInfo[j] = x->keyInfo[j + 1];
                        x->childs[j + 1] = x->childs[j + 2];
                    }
                    x->keyInfo[x->n - 1] = NULL;
                    x->childs[x->n] = NULL;
                    x->n--;

                    for (int j = 0; j < z->n; j++)
                    {
                        y->keyInfo[T + j] = z->keyInfo[j];
                        y->childs[T + j] = z->childs[j];
                        if (y->childs[T + j] != NULL)
                            y->childs[T + j]->parent = y;
                        y->n++;
                    }
                    y->childs[2 * T - 1] = z->childs[T - 1];
                    if (y->childs[2 * T - 1] != NULL)
                        y->childs[2 * T - 1]->parent = y;
                    x->childs[i] = y;

                    free(z);
                }
            }
            x = y;
        }
        else
        {
            result = x->keyInfo[i];
            if (x->childs[i]->n > T - 1)
            {
                y = x->childs[i];
                z = maxElement(y);
                x->keyInfo[i] = z->keyInfo[z->n - 1];
                inKey = z->keyInfo[z->n - 1]->key;
                x = y;
            }
            else if (x->childs[i + 1]->n > T - 1)
            {
                y = x->childs[i + 1];
                z = minElement(y);
                x->keyInfo[i] = z->keyInfo[0];
                inKey = z->keyInfo[0]->key;
                x = y;
            }
            else
            {
                y = x->childs[i];
                z = x->childs[i + 1];
                y->keyInfo[T - 1] = x->keyInfo[i];
                y->n++;
                for (int j = i; j < x->n - 1; j++)
                {
                    x->keyInfo[j] = x->keyInfo[j + 1];
                    x->childs[j + 1] = x->childs[j + 2];
                }
                x->keyInfo[x->n - 1] = NULL;
                x->childs[x->n] = NULL;
                x->n--;

                for (int j = 0; j < T - 1; j++)
                {
                    y->keyInfo[T + j] = z->keyInfo[j];
                    y->childs[T + j] = z->childs[j];
                    if (y->childs[T + j] != NULL)
                        y->childs[T + j]->parent = y;
                    y->n++;
                }
                y->childs[2 * T - 1] = z->childs[T - 1];
                if (y->childs[2 * T - 1] != NULL)
                    y->childs[2 * T - 1]->parent = y;

                free(z);

                x = y;
            }
        }
    }

    int i, n = 1;
    for (i = 0; i < x->n && n > 0; i++)
    {
        n = strcmp(inKey, x->keyInfo[i]->key);
        if (n == 0 && !result)
            result = x->keyInfo[i];
    }
    if (n == 0)
    {
        i--;
        for (int j = i; j < x->n - 1; j++)
        {
            x->keyInfo[j] = x->keyInfo[j + 1];
        }
        x->keyInfo[x->n - 1] = NULL;
        x->n--;
    }
    return result;
}

int D_Add(Node **node)
{
    int rc, n;
    char *key, *info = NULL;

    printf("Enter key: --> ");
    key = getStr();
    if (key == NULL)
        return 0;
    printf("Enter info: --> ");
    info = getStr();
    if (info == NULL)
        return 0;

    rc = add(node, key, info);
    if (rc)
        puts("Ok\n");
    else
    {
        printf("Duplicate key: %s\n", key);
        free(key);
        free(info);
    }
    puts("");

    return 1;
}

int D_Load(Node **node)
{
    int msize;
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = getStr();
    if (fname == NULL)
        return 0;
    if (load(node, fname) == 0)
    {
        free(fname);
        return 0;
    }
    free(fname);
    puts("");
    return 1;
}

int D_Find(Node **node)
{
    char *key;
    ResFind *resFind;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    resFind = find(*node, key);
    if (resFind == NULL)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printf("key = %s was found\n\n", resFind->node->keyInfo[resFind->i]->key);
        free(resFind);
    }
    free(key);

    return 1;
}

int D_SpecialFind(Node **node)
{
    Node *res;
    res = minElement(*node);
    if (res == NULL)
    {
        printf("Tree is empty\n\n");
    }
    else
    {
        printf("Minimal key = %s\n\n", res->keyInfo[0]->key);
    }
    return 1;
}

int D_Bypass(Node **node)
{
    char *prefix;
    printf("Enter the prefix : -->");
    prefix = getStr();
    if (prefix == NULL)
        return 0;
    bypass(*node, prefix);
    free(prefix);
    puts("");
    return 1;
}

int D_Delete(Node **node)
{
    char *key;
    KeyInfo *del;
    printf("Enter key: -->");
    key = getStr();
    if (!key)
        return 0;
    puts("");
    del = delete (node, key);
    if (!del)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printf("Ok\n");
        free(del->info);
        free(del->key);
        free(del);
    }
    free(key);
    return 1;
}


void show(Node *root, int i, FILE *file)
{
    if (root)
    {
        fprintf(file, "\tnode%p[label = \"", root->keyInfo);
        for (int j = 0; j < 2 * T - 1; j++)
        {
            if (root->keyInfo[j])
            {
                fprintf(file, "<f%d>|%s|", j, root->keyInfo[j]->key);
            }
            else
            {
                fprintf(file, "<f%d>|null|", j);
            }
        }
        fprintf(file, "<f%d>\"]\n", 2 * T - 1);

        if (root->parent)
        {
            fprintf(file, "\t\"node%p\":f%d -- \"node%p\"\n", root->parent->keyInfo, i, root->keyInfo);
        }

        for (int j = 0; j < 2 * T; j++)
        {
            show(root->childs[j], j, file);
        }
    }
}

int D_Show(Node **root)
{
    FILE *file;
    file = fopen("g.gv", "w");
    if (file == NULL)
    {
        printf("Ошибка!\n");
        return 0;
    }
    fprintf(file, "graph G{\n\tnode [shape = record,height=.1]\n");

    show(*root, 0, file);

    fprintf(file, "}");
    fclose(file);
    system("dot -Tpng g.gv -o gv.png");
    system("start mspaint gv.png");
    return 1;
}

int D_Timing(Node **root)
{
    Node *node = NULL;
    char *info = "info";
    char key[10000][16], *k;
    int n = 10, length, cnt = 1000000, i, j, m;
    clock_t first, last;
    srand(time(NULL));
    while (n-- > 0)
    {
        for (i = 0; i < 10000; ++i)
        {
            length = rand() % 15;
            for (j = 0; j < length; j++)
            {
                key[i][j] = rand() % 26 + 97;
            }
            key[i][length] = '\0';
        }

        for (i = 0; i < cnt;)
        {
            length = rand() % 15;
            k = malloc(sizeof(char) * (length + 1));
            for (j = 0; j < length; j++)
            {
                k[j] = rand() % 26 + 97;
            }
            k[length] = '\0';
            if (add(&node, k, info))
                ++i;
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i)
            if (find(node, key[i]))
                ++m;
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n) * cnt, last - first);
    }
    eraseTree(&node);
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
