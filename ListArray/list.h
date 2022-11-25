#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INT

#ifdef INT
    typedef int elem_t;
    static const char* elem_fmt = "%d";
    const int POISON = 0xDEADF00D;
#endif

#ifdef DOUBLE
    typedef double elem_t;
    static const char* elem_fmt = "%lf";
    const double POISON = NAN;
#endif

#ifdef CHAR
    typedef char elem_t;
    static const char* elem_fmt = "%c";
    const int POISON = '\0';
#endif


#define ListCtor(list, capacity) ListCtor_((list), (capacity), #list, __FILE__, __FUNCTION__, __LINE__)

#define ASSERT_OK(list) if(ListVerify(list) != 0)              \
                        {                                      \
                            ListDump(list);                    \
                            PrintError(list);                  \
                            abort();                           \
                        }


typedef struct 
{
    const char* name;
    const char* func;
    const char* file;
    int line;
} varinfo;

typedef struct ListElement
{
    elem_t data;

    size_t next;
    size_t prev;
}ListElement; 

typedef struct
{
    size_t size;
    size_t capacity;

    size_t fictive;
    size_t free;

    int sorted;

    int error;
    varinfo list_info;

    ListElement* elems;
} List;

enum errors
{
    NULL_LST_POINTER = 1,
    NULL_DATA_POINTER = 2,
    BAD_CAPACITY = 3,
    BAD_SIZE = 4,
    BIG_SIZE = 5,
    WRONG_FICTIVE = 6,
    WRONG_NEXT = 7,
    WRONG_PREV = 8
};

void ListCtor_(List* list, size_t capacity, const char* name, const char* file, const char* func, int line);

size_t ListBegin(List* list);

size_t ListEnd(List* list);

size_t ListNext(List* list, size_t num);

size_t ListPrev(List* list, size_t num);

size_t PushFront(List* list, elem_t value);

size_t PushBack(List* list, elem_t value);

size_t InsertBefore(List* list, size_t index, elem_t value);

size_t InsertAfter(List* list, size_t index, elem_t value);

void DeleteElem(List* list, size_t index);

size_t FindByValue(List* list, elem_t value);

size_t FindByLogNum(List* list, size_t log_num);

size_t FindFree(List* list);

void ListResizeUp(List* list);

void ListSort(List* list);

int ListVerify(List* list);

void PrintError(List* list);

void ListDump(List* list);

void ListClear(List* list);

void ListDtor(List* list);