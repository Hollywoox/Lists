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
    static const char* elem_fmt = "%f";
    const double POISON = NAN;
#endif

#ifdef CHAR
    typedef char elem_t;
    static const char* elem_fmt = "%c";
    const int POISON = '\0';
#endif


#define ListCtor(list) ListCtor_((list), #list, __FUNCTION__, __FILE__, __LINE__)

#define ASSERT_OK(list)


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

    ListElement* next;
    ListElement* prev;
}ListElement; 

typedef struct
{
    size_t size;
    int error;
    varinfo list_info;

    ListElement* fictive;
} List;

enum errors
{

};

void ListCtor_(List* list, const char* name, const char* func, const char* file, int line);

ListElement* ListBegin(List* list);

ListElement* ListEnd(List* list);

ListElement* ListNext(List* list, ListElement* elem);

ListElement* ListPrev(List* list, ListElement* elem);

ListElement* PushFront(List* list, elem_t value);

ListElement* PushBack(List* list, elem_t value);

ListElement* InsertBefore(List* list, int index, elem_t value);

ListElement* InsertAfter(List* list, int index, elem_t value);

void DeleteElem(List* list, int index);

ListElement* FindByValue(List* list, elem_t value);

ListElement* FindByIndex(List* list, int index);

void ListDump(List* list);

void ListClear(List* list);

void ListDtor(List* list);

