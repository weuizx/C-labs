#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
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

field *initField()
{

    int r, c;
    field *field = malloc(sizeof(field));
    printf("Enter the initial number of filled cells\n");
    getInt(&(field->size));
    field->field = malloc(field->size * sizeof(int));

    for (int i = 0; i < field->size; i++)
    {
        printf("Enter the row of filled cell #%d\n", i + 1);
        do
        {
            getInt(&r);
            if (r > ROWS || r < 1)
            {
                printf("Wrong input, please repeat\n");
            }
        } while (r > ROWS || r < 1);

        printf("Enter the column of filled cell #%d\n", i + 1);
        do
        {
            getInt(&c);
            if (c > COLUMNS || c < 1)
            {
                printf("Wrong input, please repeat\n");
            }
        } while (c > COLUMNS || c < 1);

        field->field[i] = r * COLUMNS + c;
    }
    return field;
}

bool *toBool(field *someField)
{
    bool *boolField = malloc(ROWS * COLUMNS * sizeof(bool));
    for (int i = 0; i < ROWS * COLUMNS; i++)
    {
        boolField[i] = 0;
    }
    for (int i = 0; i < someField->size; i++)
    {
        boolField[someField->field[i]] = 1;
    }
    return boolField;
}

void printField(field *someField)
{
    bool *boolField = toBool(someField);
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
            if (boolField[r * ROWS + c])
                printf("%c  ", 'o');
            else
                printf("%c  ", '.');
        printf("\n");
    }
    printf("\n");
    free(boolField);
}

field *updateField(field *someField)
{
    bool *boolField = toBool(someField);
    bool updated[ROWS * COLUMNS];
    int n = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLUMNS; c++)
        {
            if (countNeighbors(boolField, r, c) == 3 || (countNeighbors(boolField, r, c) == 2 && boolField[r * COLUMNS + c]))
            {
                updated[r * COLUMNS + c] = 1;
                n += 1;
            }
            else
                updated[r * COLUMNS + c] = 0;
        }
    }
    free(boolField);
    field *new = malloc(sizeof(field));
    new->size = n;
    n = 0;
    new->field = malloc(new->size * sizeof(int));
    for (int i = 0; i < ROWS * COLUMNS; i++)
    {
        if (updated[i])
        {
            new->field[n] = i;
            n++;
        }
    }

    return new;
}

int countNeighbors(bool *field, int r, int c)
{
    return field[(((r - 1) + ROWS) % ROWS) * COLUMNS + (((c - 1) + COLUMNS) % COLUMNS)] +
           field[(((r - 1) + ROWS) % ROWS) * COLUMNS + (((c) + COLUMNS) % COLUMNS)] +
           field[(((r - 1) + ROWS) % ROWS) * COLUMNS + (((c + 1) + COLUMNS) % COLUMNS)] +

           field[(((r) + ROWS) % ROWS) * COLUMNS + (((c - 1) + COLUMNS) % COLUMNS)] +

           field[(((r) + ROWS) % ROWS) * COLUMNS + (((c + 1) + COLUMNS) % COLUMNS)] +

           field[(((r + 1) + ROWS) % ROWS) * COLUMNS + (((c - 1) + COLUMNS) % COLUMNS)] +
           field[(((r + 1) + ROWS) % ROWS) * COLUMNS + (((c) + COLUMNS) % COLUMNS)] +
           field[(((r + 1) + ROWS) % ROWS) * COLUMNS + (((c + 1) + COLUMNS) % COLUMNS)];
}

#if defined(STACK_MODE)

void push(stack *stack, field *someField)
{
    if (stack->top >= STACK_MAX_SIZE)
    {
        moveStack(stack);
    }
    else
    {
        stack->stack[stack->top] = someField;
        stack->top++;
    }
}

field *pop(stack *stack)
{
    if (stack->top == 0)
    {
        printf("stack is empty!\n");
        return NULL;
    }
    else
    {
        stack->top--;
        return stack->stack[stack->top];
    }
}

void moveStack(stack *stack)
{
    free(stack->stack[0]->field);
    free(stack->stack[0]);
    for (int i = 1; i < stack->top; i++)
    {
        stack->stack[i - 1] = stack->stack[i];
    }
    stack->stack[stack->top - 1] = NULL;
    stack->top--;
}

void erase(stack *stack)
{
    for (stack->top; stack->top > 0; stack->top--)
    {
        free(stack->stack[stack->top - 1]->field);
        free(stack->stack[stack->top - 1]);
    }
}

void app()
{
    printf("VECTOR\n");
    stack stack;
    stack.top = 0;
    field *tmp;
    push(&stack, initField());
    printField(stack.stack[stack.top - 1]);

    int key;
    for (;;)
    {
        key = _getch();
        if (key == ',')
        {
            tmp = pop(&stack);
            if (tmp != NULL)
            {
                printField(tmp);
                free(tmp);
            }
        }
        if (key == '.')
        {
            push(&stack, updateField(stack.stack[stack.top - 1]));
            printField(stack.stack[stack.top - 1]);
        }
        if (key == 'q' || key == 'Q')
        {
            break;
        }
    }
    erase(&stack);
}

#else

void push(stack *stack, field *someField)
{
    node *tmp = (node *)malloc(sizeof(node));
    if (tmp == NULL)
    {
        printf("Stack is full!\n");
    }
    else
    {
        tmp->field = someField;
        tmp->next = stack->top;
        stack->top = tmp;
    }
}

field *pop(stack *stack)
{
    if (stack->top == NULL)
    {
        printf("stack is empty!\n");
        return NULL;
    }
    else
    {
        node *tmp;
        field *out;
        tmp = stack->top;
        stack->top = tmp->next;
        out = tmp->field;
        free(tmp);
        return out;
    }
}

void erase(stack *stack)
{
    while (stack->top != NULL)
    {
        field *tmp = pop(stack);
        free(tmp->field);
        free(tmp);
    }
}

void app()
{
    printf("LIST\n");
    stack stack;
    stack.top = NULL;
    field *tmp;
    push(&stack, initField());
    printField(stack.top->field);

    int key;
    for (;;)
    {
        key = _getch();
        if (key == ',')
        {
            tmp = pop(&stack);
            if (tmp != NULL)
            {
                printField(tmp);
                free(tmp);
            }
        }
        if (key == '.')
        {
            push(&stack, updateField(stack.top->field));
            printField(stack.top->field);
        }
        if (key == 'q' || key == 'Q')
        {
            break;
        }
    }
    erase(&stack);
}
#endif