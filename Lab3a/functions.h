#define MAX_SIZE 30
typedef struct KeySpace
{
    // ненулевой ключ элемента
    char *key;
    // ключ родительского элемента, может быть нулевым
    char *par;
    // указатель на информацию
    char *info;
    // указатель на следующий элемент
    struct KeySpace *next;
} KeySpace;

typedef struct Table
{
    KeySpace *ks;
    int msize;
    int csize;
} Table;

int getInt(int *a);
int LenStr(char *buf);
char *getStr();
char *getStrFromFile(FILE *file);
void printElement(KeySpace *keySpace);
Table *initTable();
void eraseTable(Table *table);
KeySpace *find(Table *table, char *key);
Table *deepFind(Table *table, char *par);
int add(Table *table, char *key, char *par, char *info);
int load(Table *table, char *fileName);
KeySpace *delete(Table *table, char *key);
Table *deepDelete(Table *table, char *key);
int D_Add(Table *table);
int D_Load(Table *table);
int D_Find(Table *table);
int D_DeepFind(Table *table);
int D_Delete(Table *table);
int D_DeepDelete(Table *table);
int D_Show(Table *table);
int dialog(const char *msgs[], int N);

