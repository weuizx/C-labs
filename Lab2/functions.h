#include <stdbool.h>

#define ROWS 40
#define COLUMNS 40
//#define STACK_MODE

#if defined(STACK_MODE)
#define STACK_MAX_SIZE 30

typedef struct field
{
    int *field;
    int size;
} field;

typedef struct stack
{
    field *stack[STACK_MAX_SIZE];
    int top;
} stack;

void moveStack(stack *stack);
#else
typedef struct field
{
    int *field;
    int size;
} field;

typedef struct node
{
    field *field;
    struct node *next;
} node;

typedef struct stack
{
    node *top;
} stack;
#endif

int getInt(int *a);
field *initField();
bool *toBool(field *someField);
void printField(field *someField);
int countNeighbors(bool *field, int r, int c);
field *updateField(field *someField);

void push(stack *stack, field *someField);
field *pop(stack *stack);
void erase(stack *stack);
void app();
