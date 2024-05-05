typedef struct Line
{
    int n;
    int *a;
} Line;
typedef struct Matrix
{
    int lines;
    Line *matrix;
} Matrix;

int getInt(int *a);
int input(Matrix *rm);
void outputMatrix(const char *msg, Matrix a);
void eraseMatrix(Matrix *a);
Line returnVector(Matrix matrix);
int isHasZero(int a);
void outputVector(const char *msg, Line line);