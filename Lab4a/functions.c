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
        eraseTree(&((*node)->left));
        eraseTree(&((*node)->right));
        free((*node)->info);
        free(*node);
    }
    *node = NULL;
}

void printTree(Node *node, int level)
{
    int i = level;
    if (node)
    {
        printTree(node->right, level + 1);
        while (i-- > 0)
            printf(" ");
        printf("%d %s\n", node->key, node->info);
        printTree(node->left, level + 1);
    }
}

Node *minElement(Node *node)
{
    if (node)
        while (node->left)
            node = node->left;
    return node;
}

Node *maxElement(Node *node)
{
    if (node)
        while (node->right)
            node = node->right;
    return node;
}

Node *prevElement(Node *node)
{
    if (node->left)
    {
        return maxElement(node->left);
    }
    else
    {
        Node *parent = node->parent;
        while (parent && node->key < parent->key)
        {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

Node *nextElement(Node *node)
{
    if (node->right)
    {
        return minElement(node->right);
    }
    else
    {
        Node *parent = node->parent;
        while (parent && node->key > parent->key)
        {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

Node *find(Node *node, int key)
{
    while (node)
    {
        if (node->key == key)
            return node;

        node = key < node->key ? node->left : node->right;
    }
    return NULL;
}

Node **specialFind(Node *node, int key)
{
    if (!node)
        return NULL;
    Node **result = malloc(sizeof(Node *) * 2);
    Node *min = minElement(node);
    Node *max = maxElement(node);

    if (min == max)
    {
        result[0] = max;
        result[1] = NULL;
    }
    else
    {
        int dif1 = difference(key, min->key);
        int dif2 = difference(key, max->key);
        if (dif1 == dif2)
        {
            result[0] = min;
            result[1] = max;
        }
        else
        {
            result[0] = dif1 > dif2 ? min : max;
            result[1] = NULL;
        }
    }
    return result;
}

int bypass(Node *node, int tens)
{
    if (node)
    {
        if ((node->key / 10) % 10 == tens)
            printf("Found element : %d\n", node->key);
        bypass(node->left, tens);
        bypass(node->right, tens);
    }
    return 1;
}

int add(Node **root, int key, char *info)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->key = key;
    newNode->info = info;
    newNode->left = NULL;
    newNode->right = NULL;

    if ((*root) == NULL)
    {
        newNode->parent = NULL;
        *root = newNode;
    }
    else
    {
        Node *curNode = *root;
        Node *parNode;
        while (curNode != NULL)
        {
            if (curNode->key == newNode->key)
            {
                free(newNode);
                return 0;
            }
            parNode = curNode;
            if (newNode->key < curNode->key)
                curNode = curNode->left;
            else
                curNode = curNode->right;
        }
        newNode->parent = parNode;
        if (newNode->key < parNode->key)
            parNode->left = newNode;
        else
            parNode->right = newNode;
    }

    return 1;
}

int load(Node **node, char *fileName)
{
    char *info;
    int n, key;
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        while (!feof(file))
        {
            if (fscanf(file, "%d\n", &key))
                info = getStrFromFile(file);
            if (!info || !add(node, key, info))
            {
                // eraseTree(node);
                free(info);
                fclose(file);
                return 0;
            }
        }
        free(info);
        fclose(file);
        return 1;
    }
    else
        return -1;
}

Node *delete(Node **node, int key)
{
    Node *x = find(*node, key), *y, *p, *par;
    char *info;
    if (!x)
        return NULL;
    y = !x->left || !x->right ? x : nextElement(x);
    p = y->left ? y->left : y->right;
    par = y->parent;
    if (p)
        p->parent = par;
    if (!par)
        *node = p;
    else
    {
        if (par->left == y)
            par->left = p;
        else
            par->right = p;
    }
    if (y != x)
    {
        x->key = y->key;
        info = x->info;
        x->info = y->info;
    }
    y->right = NULL;
    y->left = NULL;
    y->parent = NULL;
    return y;
}

int D_Add(Node **node)
{
    int key, rc, n;
    char *info = NULL;

    printf("Enter key: --> ");
    if (getInt(&key) == 0)
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
        printf("Duplicate key: %d\n", key);
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
    int key;
    Node *res;
    printf("Enter key: -->");
    if (getInt(&key) == 0)
        return 0;
    res = find(*node, key);
    if (res == NULL)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printf("key = %d was found\n\n", res->key);
    }
    return 1;
}

int D_SpecialFind(Node **node)
{
    int key;
    Node **res;
    printf("Enter key: -->");
    if (getInt(&key) == 0)
        return 0;
    res = specialFind(*node, key);
    if (res == NULL)
    {
        printf("Tree is empty\n\n");
    }
    else
    {
        for (int i = 0; i < 2; i++)
            if (res[i])
                printf("Maximally different key = %d\n\n", (*(res + i))->key);
    }
    return 1;
}

int D_Bypass(Node **node)
{
    int tens;
    do
    {
        printf("Enter number of decimal places(0-9) : -->");
        if (getInt(&tens) == 0)
            return 0;
    } while (tens > 9 || tens < 0);
    bypass(*node, tens);
    puts("");
    return 1;
}

int D_Delete(Node **node)
{
    int key;
    Node *del;
    int release;
    printf("Enter key: -->");
    if (!getInt(&key))
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
        free(del);
    }
    return 1;
}

// int D_Show(Node **node)
// {
//     printf("\n=== Your tree: ===\n\n");
//     printTree(*node, 0);
//     return 1;
// }

void show(Node *root, FILE *file)
{
    if (root)
    {
        if (root->left)
            fprintf(file, "\t%d -- %d\n", root->key, root->left->key);
        if (root->right)
            fprintf(file, "\t%d -- %d\n", root->key, root->right->key);
        show(root->left, file);
        show(root->right, file);
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
    fprintf(file, "graph G{\n");

    show(*root, file);

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
    int n = 10, key[10000], k, cnt = 1000000, i, m;
    clock_t first, last;
    srand(time(NULL));
    while (n-- > 0)
    {
        for (i = 0; i < 10000; ++i)
            key[i] = rand() * rand();
        for (i = 0; i < cnt;)
        {
            k = rand() * rand();
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
