#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void printElement(KeySpace *keySpace)
{
    printf("Key : %s\n", keySpace->key);
    printf("Parent key : %s\n", keySpace->par);
    printf("Info : %s\n\n", keySpace->info);
}

Table *initTable()
{
    Table *table = malloc(sizeof(Table));
    table->csize = 0;
    table->msize = MAX_SIZE;
    table->ks = NULL;
}

void eraseTable(Table *table)
{
    KeySpace *keySpace = table->ks;
    KeySpace *delete;
    while (keySpace != NULL)
    {
        delete = keySpace;
        keySpace = delete->next;
        free(delete->key);
        free(delete->par);
        free(delete->info);
        free(delete);
    }
}

KeySpace *find(Table *table, char *key)
{
    KeySpace *keySpace = table->ks;
    while (keySpace != NULL)
    {
        if (strcmp(keySpace->key, key) == 0)
        {
            return keySpace;
        }
        keySpace = keySpace->next;
    }
    return NULL;
}

Table *deepFind(Table *table, char *par)
{
    Table *result = initTable();
    KeySpace *keySpace = table->ks;
    char *zeroString = "0\0";
    while (keySpace != NULL)
    {
        if (strcmp(keySpace->par, par) == 0)
        {
            add(result, keySpace->key, zeroString, keySpace->info);
            result->ks->par = keySpace->par;
        }
        keySpace = keySpace->next;
    }
    if (result->ks == NULL)
    {
        free(result);
        return NULL;
    }
    return result;
}

int add(Table *table, char *key, char *par, char *info)
{
    char str[2] = "0\0";
    if (find(table, key) != NULL)
    {
        return -1;
    }
    else if (strcmp(par, str) != 0 && find(table, par) == NULL)
    {
        return 0;
    }
    else
    {
        KeySpace *keySpace = malloc(sizeof(KeySpace));
        keySpace->key = key;
        keySpace->par = par;
        keySpace->info = info;
        keySpace->next = table->ks;
        table->ks = keySpace;
        table->csize += 1;
        return 1;
    }
}

int load(Table *table, char *fileName)
{
    char *key, *par, *info;
    int n;
    Table *result = initTable();
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        while (!feof(file))
        {
            key = getStrFromFile(file);
            if (!key)
            {
                free(key);
                break;
            }
            par = getStrFromFile(file);
            info = getStrFromFile(file);
            if (!par || !info)
            {
                eraseTable(result);
                free(result);
                free(key);
                free(par);
                free(info);
                fclose(file);
                return 0;
            }
            n = add(result, key, par, info);
            if (n != 1)
            {
                eraseTable(result);
                free(result);
                free(key);
                free(par);
                free(info);
                fclose(file);
                return 0;
            }
        }
        fclose(file);
    }
    else
    {
        free(result);
        return -1;
    }
    eraseTable(table);
    *table = *result;
    free(result);
    return 1;
}

int addFromFile(Table *table, char *fileName)
{
}

KeySpace *delete(Table *table, char *key)
{
    KeySpace *keySpace = table->ks;
    if (strcmp(keySpace->key, key) == 0)
    {
        table->ks = keySpace->next;
        table->csize--;
        return keySpace;
    }
    while (keySpace->next != NULL)
    {
        if (strcmp(keySpace->next->key, key) == 0)
        {
            KeySpace *delete = keySpace->next;
            keySpace->next = delete->next;
            table->csize--;
            return delete;
        }
        keySpace = keySpace->next;
    }

    return NULL;
}

Table *deepDelete(Table *table, char *key)
{
    KeySpace *keySpace = delete (table, key);
    if (keySpace == NULL)
    {
        return NULL;
    }
    Table *result = initTable();
    char *par = keySpace->par;
    keySpace->par = "0\0";
    add(result, keySpace->key, keySpace->par, keySpace->info);
    result->ks->par = par;
    free(keySpace);
    keySpace = table->ks;
    if (strcmp(keySpace->par, key) == 0)
    {
        table->ks = keySpace->next;
        table->csize--;
        add(result, keySpace->key, keySpace->par, keySpace->info);
        free(keySpace);
    }
    while (keySpace->next != NULL)
    {
        if (strcmp(keySpace->next->par, key) == 0)
        {
            KeySpace *delete = keySpace->next;
            keySpace->next = delete->next;
            table->csize--;
            add(result, delete->key, delete->par, delete->info);
            free(delete);
        }
        else
        {
            keySpace = keySpace->next;
        }
    }
    return result;
}

int D_Add(Table *table)
{
    int rc;
    char *key, *par, *info = NULL;
    printf("Enter key: --> ");
    key = getStr();
    if (key == NULL)
        return 0;
    printf("Enter parent key: --> ");
    par = getStr();
    if (par == NULL)
        return 0;
    printf("Enter info: --> ");
    info = getStr();
    if (info == NULL)
        return 0;
    rc = add(table, key, par, info);
    if (rc == -1)
    {
        printf("\nAn element with the same key already exists\n");
    }
    if (rc == 0)
    {
        printf("\nThe table does not have an element with this parent key\n");
    }
    puts("");
    return 1;
}

int D_Load(Table *table)
{
    int rc;
    char *fileName;
    printf("Enter name of file: --> ");
    fileName = getStr();
    if (fileName == NULL)
        return 0;
    rc = load(table, fileName);
    if (rc == -1)
    {
        printf("\nCould not open file with this name\n");
    }
    if (rc == 0)
    {
        printf("\nThe table is filled in incorrectly\n");
    }
    puts("");
    free(fileName);
    return 1;
}

int D_Find(Table *table)
{
    char *key;
    KeySpace *keySpace;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    puts("");
    keySpace = find(table, key);
    if (keySpace == NULL)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printElement(keySpace);
    }
    free(key);
    return 1;
}

int D_DeepFind(Table *table)
{
    char *key;
    Table *result;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    result = deepFind(table, key);
    if (result == NULL)
    {
        printf("\nAn element with the same parent key does not exists\n\n");
    }
    else
    {
        D_Show(result);
        KeySpace *keySpace = result->ks;
        KeySpace *delete;
        while (keySpace != NULL)
        {
            delete = keySpace;
            keySpace = delete->next;
            free(delete);
        }
        free(result);
    }
    free(key);
    return 1;
}

int D_Delete(Table *table)
{
    char *key;
    KeySpace *keySpace;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    puts("");
    keySpace = delete (table, key);
    if (keySpace == NULL)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printf("Deleted element :\n\n");
        printElement(keySpace);
        free(keySpace->key);
        free(keySpace->par);
        free(keySpace->info);
        free(keySpace);
    }

    free(key);
    return 1;
}

int D_DeepDelete(Table *table)
{
    char *key;
    Table *result;
    printf("Enter key: -->");
    key = getStr();
    if (key == NULL)
        return 0;
    puts("");
    result = deepDelete(table, key);
    if (result == NULL)
    {
        printf("An element with the same key does not exists\n\n");
    }
    else
    {
        printf("Deleted elements :\n");
        D_Show(result);
        eraseTable(result);
        free(result);
    }
    free(key);
    return 1;
}

int D_Show(Table *table)
{
    puts("");
    int i = table->csize;
    KeySpace *keySpace = table->ks;
    while (keySpace != NULL)
    {
        printf("Element #%d\n", i);
        printElement(keySpace);
        keySpace = keySpace->next;
        i--;
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