#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main()
{
    Matrix *matrix = (Matrix*) malloc(sizeof(Matrix));
    Line *line = (Line*) malloc(sizeof(Line));

    input(matrix);
    outputMatrix("Entered matrix", *matrix);
    *line = returnVector(*matrix);
    outputVector("Result vector", *line);
    
    eraseMatrix(matrix);
    free(line->a);
    free(line);
    free(matrix);

    return 0;
}

