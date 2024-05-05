#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
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

void eraseTable(Table *table)
{
    for (int i = 0; i < table->msize; i++)
    {
        KeySpace *keySpace = table->ks + i;
        KeySpace *next;
        Node *node, *nextNode;
        if (keySpace->keyLen != 0)
        {
            node = keySpace->node;
            do
            {
                nextNode = node->next;
                free(node);
                node = nextNode;
            } while (nextNode != NULL);
            keySpace = keySpace->next;
            next = keySpace;
            while (next != NULL)
            {
                next = keySpace->next;
                node = keySpace->node;
                do
                {
                    nextNode = node->next;
                    free(node);
                    node = nextNode;
                } while (nextNode != NULL);
                free(keySpace);
                keySpace = next;
            }
        }
    }
    free(table->ks);
}
int hashStr(const char *key)
{
    int hash = INT_MAX;
    for (int i = 0; i < strlen(key); ++i)
        hash = 37 * hash + key[i];

    return abs(hash);
}

void printNode(FILE *file, Node *node, int offsetLevel)
{
    int len = node->infoLen;
    fseek(file, node->infoOffset, SEEK_SET);
    char *info = malloc(sizeof(char) * (len + 1));
    fread(info, sizeof(char), len, file);
    info[len] = '\0';
    for (int i = 0; i < offsetLevel; i++)
    {
        printf("    ");
    }
    printf("release : %d\n", node->release);
    for (int i = 0; i < offsetLevel; i++)
    {
        printf("    ");
    }
    printf("info : %s\n", info);
    free(info);
    puts("");
}

Vector *findKey(Table *table, char *key)
{
    int n = hashStr(key) % table->msize;
    int offset = 0;
    char *str;
    KeySpace *keySpace = table->ks + n;
    KeySpace *prev = keySpace;
    if (keySpace->keyLen == 0)
        return NULL;
    while (keySpace != NULL)
    {
        str = malloc(sizeof(char) * (keySpace->keyLen + 1));
        fseek(table->file, keySpace->keyOffset, SEEK_SET);
        fread(str, sizeof(char), keySpace->keyLen, table->file);
        str[keySpace->keyLen] = '\0';
        if (strcmp(str, key) == 0)
        {
            free(str);
            break;
        }
        free(str);
        offset = prev->ksOffset;
        prev = keySpace;
        keySpace = keySpace->next;
    }
    if (keySpace != NULL)
    {
        int nodeOffset = keySpace->nodeOffset;
        int count = 0;
        Node *curNode = keySpace->node;
        while (curNode != NULL)
        {
            curNode = curNode->next;
            count++;
        }
        Vector *vector = malloc(sizeof(Vector));
        vector->size = count;
        vector->nodes = malloc(sizeof(Node *) * count);
        curNode = keySpace->node;
        count = 0;
        while (curNode != NULL)
        {
            vector->nodes[count] = curNode;
            curNode = curNode->next;
            count++;
        }
        return vector;
    }

    return NULL;
}

Node *findRelease(Vector *vector, int release)
{
    for (int i = 0; i < vector->size; i++)
    {
        if (vector->nodes[i]->release == release)
        {
            return vector->nodes[i];
        }
    }
    return NULL;
}

int add(Table *table, char *key, char *info)
{
    int offset = 0;
    char *str;
    Node *node = malloc(sizeof(Node));
    node->nodeOffset = 0;
    node->next = NULL;
    node->infoLen = strlen(info) * sizeof(char);
    fseek(table->file, 0, SEEK_END);
    node->infoOffset = ftell(table->file);
    fwrite(info, sizeof(char), node->infoLen, table->file);
    int n = hashStr(key) % table->msize;
    if ((table->ks + n)->keyLen == 0)
    {
        node->release = 1;
        (table->ks + n)->node = node;
        (table->ks + n)->next = NULL;
        (table->ks + n)->ksOffset = 0;
        (table->ks + n)->nodeOffset = ftell(table->file);
        fwrite(node, sizeof(int), 4, table->file);
        (table->ks + n)->keyLen = strlen(key) * sizeof(char);
        (table->ks + n)->keyOffset = ftell(table->file);
        fwrite(key, sizeof(char), (table->ks + n)->keyLen, table->file);
        fseek(table->file, 8 + 16 * n, SEEK_SET);
        table->csize++;
        return 1;
    }
    KeySpace *keySpace = table->ks + n;
    KeySpace *prev = keySpace;
    while (keySpace != NULL)
    {
        str = malloc(sizeof(char) * (keySpace->keyLen + 1));
        fseek(table->file, keySpace->keyOffset, SEEK_SET);
        fread(str, sizeof(char), keySpace->keyLen, table->file);
        str[keySpace->keyLen] = '\0';
        if (strcmp(str, key) == 0)
        {
            free(str);
            break;
        }
        free(str);
        offset = prev->ksOffset;
        prev = keySpace;
        keySpace = keySpace->next;
    }
    if (keySpace != NULL)
    {
        node->release = keySpace->node->release + 1;
        node->next = keySpace->node;
        node->nodeOffset = keySpace->nodeOffset;
        keySpace->node = node;
        fseek(table->file, 0, SEEK_END);
        keySpace->nodeOffset = ftell(table->file);
        fwrite(node, sizeof(int), 4, table->file);
        if (offset == 0)
        {
            fseek(table->file, 2 * sizeof(int) + 4 * sizeof(int) * n + 2 * sizeof(int), SEEK_SET);
        }
        else
        {
            fseek(table->file, prev->ksOffset + 2 * sizeof(int), SEEK_SET);
        }
        fwrite(&keySpace->nodeOffset, sizeof(int), 1, table->file);
    }
    else
    {
        node->release = 1;
        keySpace = malloc(sizeof(KeySpace));
        keySpace->node = node;
        keySpace->next = NULL;
        keySpace->ksOffset = 0;
        fseek(table->file, 0, SEEK_END);
        keySpace->nodeOffset = ftell(table->file);
        fwrite(node, sizeof(int), 4, table->file);
        keySpace->keyLen = strlen(key) * sizeof(char);
        keySpace->keyOffset = ftell(table->file);
        fwrite(key, sizeof(char), keySpace->keyLen, table->file);
        prev->next = keySpace;
        prev->ksOffset = ftell(table->file);
        fwrite(keySpace, sizeof(int), 4, table->file);
        if (offset == 0)
            offset = 2 * sizeof(int) + 4 * sizeof(int) * n;
        fseek(table->file, offset + 3 * sizeof(int), SEEK_SET);
        fwrite(&prev->ksOffset, sizeof(int), 1, table->file);
    }
    return 1;
}

int load(Table *table, char *fname)
{
    table->file = fopen(fname, "r+b");
    if (table->file == NULL)
        return 0;
    fread(&table->msize, sizeof(int), 1, table->file);
    table->ks = (KeySpace *)calloc(table->msize, sizeof(KeySpace));
    fread(&table->csize, sizeof(int), 1, table->file);
    for (int i = 0; i < table->msize; i++)
    {
        KeySpace *keySpace = table->ks + i;
        Node *node = NULL;

        fseek(table->file, 8 + 16 * i, SEEK_SET);
        fread(keySpace, sizeof(int), 4, table->file);
        if (keySpace->keyLen != 0)
        {
            keySpace->node = malloc(sizeof(Node));
            fseek(table->file, keySpace->nodeOffset, SEEK_SET);
            fread(keySpace->node, sizeof(int), 4, table->file);
            node = keySpace->node;
        }
        else
        {
            continue;
        }
        while (node->nodeOffset != 0)
        {
            node->next = malloc(sizeof(Node));
            fseek(table->file, node->nodeOffset, SEEK_SET);
            fread(node->next, sizeof(int), 4, table->file);
            node = node->next;
        }
        node->next = NULL;

        while (keySpace->ksOffset != 0)
        {
            fseek(table->file, keySpace->ksOffset, SEEK_SET);
            keySpace->next = malloc(sizeof(KeySpace));
            fread(keySpace->next, sizeof(int), 4, table->file);
            keySpace = keySpace->next;
            keySpace->node = malloc(sizeof(Node));
            fseek(table->file, keySpace->nodeOffset, SEEK_SET);
            fread(keySpace->node, sizeof(int), 4, table->file);
            node = keySpace->node;
            while (node->nodeOffset != 0)
            {
                node->next = malloc(sizeof(Node));
                fseek(table->file, node->nodeOffset, SEEK_SET);
                fread(node->next, sizeof(int), 4, table->file);
                node = node->next;
            }
            node->next = NULL;
        }
        keySpace->next = NULL;
    }
    return 1;
}

int create(Table *table, char *fname, int msize)
{
    KeySpace keySpace = {0, 0, 0, 0, NULL, NULL};
    table->msize = msize;
    table->csize = 0;
    table->file = fopen(fname, "w+b");
    if (table->file == NULL)
    {
        table->ks = NULL;
        return 0;
    }
    table->ks = (KeySpace *)calloc(table->msize, sizeof(KeySpace));
    for (int i = 0; i < table->msize; i++)
    {
        *(table->ks + i) = keySpace;
    }
    fwrite(&table->msize, sizeof(int), 1, table->file);
    fwrite(&table->csize, sizeof(int), 1, table->file);
    fwrite(table->ks, 4 * sizeof(int), table->msize, table->file);
    return 1;
}

Node *delete(Table *table, char *key, int release)
{
    int n = hashStr(key) % table->msize;
    int offset = 0;
    char *str;
    KeySpace *keySpace = table->ks + n;
    KeySpace *prev = keySpace;
    if (keySpace->keyLen == 0)
        return NULL;
    while (keySpace != NULL)
    {
        str = malloc(sizeof(char) * (keySpace->keyLen + 1));
        fseek(table->file, keySpace->keyOffset, SEEK_SET);
        fread(str, sizeof(char), keySpace->keyLen, table->file);
        str[keySpace->keyLen] = '\0';
        if (strcmp(str, key) == 0)
        {
            free(str);
            break;
        }
        free(str);
        offset = prev->ksOffset;
        prev = keySpace;
        keySpace = keySpace->next;
    }
    if (keySpace != NULL)
    {
        int nodeOffset = keySpace->nodeOffset;
        Node *curNode = keySpace->node;
        Node *prevNode = curNode;
        if (curNode->release == release)
        {
            if (curNode->next == NULL)
            {
                prev->next = keySpace->next;
                prev->ksOffset = keySpace->ksOffset;
                if (offset == 0)
                {
                    offset = 2 * sizeof(int) + 4 * sizeof(int) * n;
                    if (keySpace->next != NULL)
                    {
                        *(table->ks + n) = *keySpace->next;
                        free(keySpace->next);
                    }
                    else
                    {
                        keySpace->keyLen = 0;
                        keySpace->keyOffset = 0;
                        keySpace->ksOffset = 0;
                        keySpace->nodeOffset = 0;
                        keySpace->node = NULL;
                        keySpace->next = NULL;
                        table->csize--;
                    }
                }
                else
                {
                    free(keySpace);
                }
                fseek(table->file, offset + 3 * sizeof(int), SEEK_SET);
                fwrite(&prev->ksOffset, sizeof(int), 1, table->file);
            }
            else
            {
                keySpace->node = curNode->next;
                keySpace->nodeOffset = curNode->nodeOffset;
                if (offset == 0)
                {
                    offset = 2 * sizeof(int) + 4 * sizeof(int) * n;
                }
                else
                {
                    offset = prev->ksOffset;
                }
                fseek(table->file, offset + 2 * sizeof(int), SEEK_SET);
                fwrite(&keySpace->nodeOffset, sizeof(int), 1, table->file);
            }
            curNode->next = NULL;
            curNode->nodeOffset = 0;
            return curNode;
        }
        curNode = curNode->next;
        while (curNode != NULL)
        {
            if (curNode->release == release)
            {
                prevNode->next = curNode->next;
                prevNode->nodeOffset = curNode->nodeOffset;
                curNode->next = NULL;
                curNode->nodeOffset = 0;
                fseek(table->file, nodeOffset + 3 * sizeof(int), SEEK_SET);
                fwrite(&prevNode->nodeOffset, sizeof(int), 1, table->file);
                return curNode;
            }
            nodeOffset = prevNode->nodeOffset;
            prevNode = curNode;
            curNode = curNode->next;
        }
    }

    return NULL;
}

int D_Add(Table *table)
{
    int rc;
    char *key, *info = NULL;
    printf("Enter key: --> ");
    key = getStr();
    if (key == NULL)
        return 0;
    printf("Enter info: --> ");
    info = getStr();
    if (info == NULL)
        return 0;
    rc = add(table, key, info);
    free(key);
    free(info);
    puts("");
    return 1;
}

int D_Load(Table *table)
{
    int msize;
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = getStr();
    if (fname == NULL)
        return 0;
    if (load(table, fname) == 0)
    {
        printf("Enter possible vector size: -->");
        if (getInt(&msize) == 0)
            return 0;
        create(table, fname, msize);
    }
    free(fname);
    puts("");
    return 1;
}

int D_Save(Table *table)
{
    fseek(table->file, sizeof(int), SEEK_SET);
    fwrite(&table->csize, sizeof(int), 1, table->file);
    for (int i = 0; i < table->msize; i++)
    {
        fwrite((table->ks + i), sizeof(int), 4, table->file);
    }
    fclose(table->file);
    table->file = NULL;
    return 1;
}

int D_Find(Table *table)
{
    int release;
    char *key;
    Vector *vector;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    vector = findKey(table, key);
    free(key);
    if (vector == NULL)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printf("Enter release(Enter 0 for print all releases): -->");
        if (getInt(&release) < 1)
        {
            free(vector->nodes);
            free(vector);
            return 0;
        }
        if (release == 0)
        {
            for (int i = 0; i < vector->size; i++)
            {
                printNode(table->file, vector->nodes[i], 0);
            }
        }
        else
        {
            Node *node = findRelease(vector, release);
            if (node == NULL)
            {
                printf("An element with the same release does not exists\n\n");
            }
            else
            {
                printNode(table->file, node, 0);
            }
        }
        free(vector->nodes);
        free(vector);
    }
    return 1;
}

int D_Delete(Table *table)
{
    char *key;
    Node *node;
    int release;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    printf("Enter release: -->");
    if (getInt(&release) < 1)
        return 0;
    puts("");
    node = delete (table, key, release);
    if (node == NULL)
    {
        printf("An element with the same key and release does not exists\n\n");
    }
    else
    {
        printf("Deleted element :\n");
        printNode(table->file, node, 0);
        free(node);
    }
    free(key);
    return 1;
}

int D_Show(Table *table)
{
    KeySpace *keySpace;
    Node *node;
    int len;
    char *key;
    printf("msize = %d\ncsize = %d\n", table->msize, table->csize);
    for (int i = 0; i < table->msize; i++)
    {
        printf("i = %d\n", i);
        keySpace = table->ks + i;
        if (keySpace->keyLen == 0)
        {
            printf("No elements\n\n");
            continue;
        }
        node = keySpace->node;
        len = keySpace->keyLen;
        key = malloc(sizeof(char) * (len + 1));
        fseek(table->file, keySpace->keyOffset, SEEK_SET);
        fread(key, sizeof(char), len, table->file);
        key[len] = '\0';
        printf("key : %s\n", key);
        free(key);
        if (node != NULL)
        {
            printNode(table->file, node, 1);
            while (node->next != NULL)
            {
                node = node->next;
                printNode(table->file, node, 1);
            }
        }
        while (keySpace->next != NULL)
        {
            keySpace = keySpace->next;
            node = keySpace->node;
            len = keySpace->keyLen;
            key = malloc(sizeof(char) * (len + 1));
            fseek(table->file, keySpace->keyOffset, SEEK_SET);
            fread(key, sizeof(char), len, table->file);
            key[len] = '\0';
            printf("key : %s\n", key);
            free(key);
            if (node != NULL)
            {
                printNode(table->file, node, 1);
                while (node->next != NULL)
                {
                    node = node->next;
                    printNode(table->file, node, 1);
                }
            }
        }
    }
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