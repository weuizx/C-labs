#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int getInt(int *a)
{
    int n;
    do
    {
        n = scanf_s("%d", a, sizeof(int));
        if (n < 0)
            return 0;
        if (n == 0)
        {
            printf("%s\n", "Error! Repeat input");
            scanf_s("%*c", 0);
        }
    } while (n == 0);
    return 1;
}

int input(Matrix *rm)
{
    const char *pr = "";
    int m;
    int i, j;
    int *p;
    do
    {
        printf("%s\n", pr);
        printf("Enter number of lines: --> ");
        pr = "You are wrong; repeat, please!";
        if (getInt(&m) == 0)
            return 0;
    } while (m < 1);
    rm->lines = m;
    rm->matrix = (Line *)calloc(m, sizeof(Line));
    for (i = 0; i < rm->lines; ++i)
    {
        pr = "";
        do
        {
            printf("%s\n", pr);
            printf("Enter number of items in line %d: --> ", i + 1);
            pr = "You are wrong; repeat, please!";
            if (getInt(&m) == 0)
            {
                rm->lines = i;
                eraseMatrix(rm);
                return 0;
            }
        } while (m < 1);
        rm->matrix[i].n = m;
        p = (int *)malloc(sizeof(int) * m);
        rm->matrix[i].a = p;
        printf("Enter items for matrix line #%d:\n", i + 1);
        for (j = 0; j < m; ++j, ++p)
            if (getInt(p) == 0)
            {
                rm->lines = i + 1;
                eraseMatrix(rm);
                return 0;
            }
    }
    return 1;
}

void outputMatrix(const char *msg, Matrix a)
{
    int i, j;
    int *p;
    printf("%s:\n", msg);
    for (i = 0; i < a.lines; ++i)
    {
        p = a.matrix[i].a;
        for (j = 0; j < a.matrix[i].n; ++j, ++p)
            printf("%11d ", *p);
        printf("\n");
    }
}

void eraseMatrix(Matrix *a)
{
    int i;
    for (i = 0; i < a->lines; ++i)
        free(a->matrix[i].a);
    free(a->matrix);
    a->lines = 0;
    a->matrix = NULL;
}

Line returnVector(Matrix matrix)
{
    int *p;
    int s;
    Line result;
    result.n = matrix.lines;
    result.a = (int *)malloc(sizeof(int) * result.n);
    for (int i = 0; i < matrix.lines; i++, p++)
    {
        s = 0;
        for (int j = 0; j < matrix.matrix[i].n; j++)
        {
            if (!isHasZero(matrix.matrix[i].a[j]))
                s += matrix.matrix[i].a[j];
        }
        result.a[i] = s;
    }
    return result;
}

int isHasZero(int a)
{
    if (a < 0)
        a *= (-1);
    if (a == 0)
        return 1;

    while (a >= 10)
    {
        if (a % 10 == 0)
            return 1;
        a /= 10;
    }
    return 0;
}

void outputVector(const char *msg, Line line)
{
    int j;
    int *p;
    printf("%s:\n", msg);
    p = line.a;
    for (j = 0; j < line.n; ++j, ++p)
        printf("%11d ", *p);
    printf("\n");
}