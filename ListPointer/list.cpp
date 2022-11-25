#include "list.h"

//============================================================

void ListCtor_(List* list, const char* name, const char* func, const char* file, int line)
{
    assert(list != NULL);

    list->size = 0;
    list->error = 0;

    list->list_info.name = name;
    list->list_info.func = func;
    list->list_info.file = file;
    list->list_info.line = line;

    list->fictive = (ListElement*)calloc(1, sizeof(ListElement));
    assert(list->fictive != NULL);

    list->fictive->data = POISON;
    list->fictive->next = list->fictive;
    list->fictive->prev = list->fictive;
}

//================================================================

ListElement* ListBegin(List* list)
{
    assert(list != NULL);

    return list->fictive->next;
}

//================================================================

ListElement* ListEnd(List* list)
{
    assert(list != NULL);

    return list->fictive->prev;
}

//================================================================

ListElement* ListNext(List* list, ListElement* elem)
{
    assert(list != NULL);
    assert(elem != NULL);

    return elem->next;
}

//================================================================

ListElement* ListPrev(List* list, ListElement* elem)
{
    assert(list != NULL);
    assert(elem != NULL);

    return elem->prev;
}

//================================================================

ListElement* PushFront(List* list, elem_t value)
{
    assert(list != NULL);

    ListElement* elem = ListBegin(list);
    
    ListElement* new_elem = (ListElement*)calloc(1, sizeof(ListElement));
    new_elem->data        = value;
    new_elem->prev        = list->fictive;
    new_elem->next        = elem;

    elem->prev          = new_elem;
    list->fictive->next = new_elem;

    ++list->size;

    return new_elem;
}

//================================================================

ListElement* PushBack(List* list, elem_t value)
{
    assert(list != NULL);

    ListElement* elem = ListEnd(list);

    ListElement* new_elem = (ListElement*)calloc(1, sizeof(ListElement));
    new_elem->data        = value;
    new_elem->prev        = elem;
    new_elem->next        = list->fictive;

    elem->next          = new_elem;
    list->fictive->prev = new_elem;

    ++list->size;

    return new_elem;
}

//================================================================

ListElement* InsertBefore(List* list, int index, elem_t value)
{
    assert(list != NULL);

    ListElement* elem      = FindByIndex(list, index);
    ListElement* elem_prev = ListPrev(list, elem);

    ListElement* new_elem = (ListElement*)calloc(1, sizeof(ListElement));
    new_elem->data        = value;
    new_elem->prev        = elem_prev;
    new_elem->next        = elem;

    elem_prev->next = new_elem;
    elem->prev      = new_elem;

    ++list->size;

    return new_elem;
}

//================================================================

ListElement* InsertAfter(List* list, int index, elem_t value)
{
    assert(list != NULL);

    ListElement* elem      = FindByIndex(list, index);
    ListElement* elem_next = ListNext(list, elem);

    ListElement* new_elem = (ListElement*)calloc(1, sizeof(ListElement));
    new_elem->data        = value;
    new_elem->prev        = elem;
    new_elem->next        = elem_next;

    elem->next      = new_elem;
    elem_next->prev = new_elem;

    ++list->size;

    return new_elem;
}

//================================================================

void DeleteElem(List* list, int index)
{
    assert(list != NULL);

    ListElement* elem      = FindByIndex(list, index);
    ListElement* elem_prev = ListPrev(list, elem);
    ListElement* elem_next = ListNext(list, elem);

    elem_prev->next = elem_next;
    elem_next->prev = elem_prev;

    free(elem);
    elem->next = NULL;
    elem->prev = NULL;

    --list->size;
}

//================================================================

ListElement* FindByValue(List* list, elem_t value)
{
    assert(list != NULL);

    ListElement* iter = ListBegin(list);
    for(int i = 1; iter != list->fictive, i <= list->size; iter = ListNext(list, iter), ++i)
    {
        if(iter->data == value) return iter;
    }

    return NULL;
}

//================================================================

ListElement* FindByIndex(List* list, int index)
{
    assert(list != NULL);

    if((index < 1) || (index > list->size)) return NULL;

    ListElement* iter = ListBegin(list);
    for(int i = 1; iter != list->fictive, i <= list->size; iter = ListNext(list, iter), ++i)
    {
        if(i == index) return iter;
    }

    return NULL;
}

//================================================================

void ListDump(List* list)
{
    FILE* log = fopen("dump.txt", "w");

    assert(log  != NULL);
    assert(list != NULL);
    assert(list->fictive != NULL);

    fprintf(log, "digraph List {\n");
    fprintf(log, "\trankdir = LR;\n");
    fprintf(log, "\tnode[color = \"red\", fontsize = 14, style = \"rounded\", style = \"filled\", fillcolor = \"lightgreen\"];\n");
    fprintf(log, "\tedge[color = \"darkgreen\"];\n");
    fprintf(log, "\tinfo [shape = record, fillcolor = \"lightblue\", label = \" list[%p] | name: %s | at %s | at %s(%d) \"];\n",
                   list, list->list_info.name + 1, list->list_info.file, list->list_info.func, list->list_info.line);
    fprintf(log, "\t0 [shape=record,label=\" <1> fictive elem(0) | <2> data = POISON\" ];\n");

    ListElement* iter = ListBegin(list);
    if(iter == list->fictive)
    {
        fprintf(log, "\t0: <1> -> 0;\n");
        fprintf(log, "\t0: <2> -> 0;\n");
    }

    else if(iter != NULL)
    {
        int index = 1;
        for(; index <= list->size, iter != list->fictive; ++index, iter = ListNext(list, iter))
        {
            if(iter == NULL) break;
            fprintf(log, "\t%d [shape=record,label=\" <1> %d | <2> data = ", index, index);
            fprintf(log, elem_fmt, iter->data);
            fprintf(log, "\"];\n");
            fprintf(log, "\t%d: <1> -> %d;\n", index - 1, index);
            if(ListPrev(list, iter) != NULL) fprintf(log, "\t%d: <2> -> %d;\n", index, index - 1);
        }
        if(iter == list->fictive)
        {
            fprintf(log, "\t%d: <1> -> 0;\n", index - 1);
            if(ListEnd(list) != NULL) fprintf(log, "\t0: <2> -> %d;\n", index - 1);
        }
    }

    else
    {
        printf("Error: list is destructed\n");
        abort();
    }

    fprintf(log, "}\n");
    fclose(log);

    system("dot dump.txt -T -O png");
    system("open dump.txt.png");
}

//================================================================

void ListClear(List* list)
{
    assert(list != NULL);

    int  i            = 1;
    ListElement* elem = ListBegin(list);
    while((elem != list->fictive) && (i <= list->size))
    {
        ListElement* next = ListNext(list, elem);
        
        elem->data = POISON;
        elem->prev = NULL;
        elem->next = NULL;
        free(elem);

        elem = next;
        ++i;
    }

    list->fictive->next = list->fictive;
    list->fictive->prev = list->fictive;
    list->size = 0;
}

//================================================================

void ListDtor(List* list)
{
    assert(list != NULL);

    ListClear(list);
    list->fictive->next = NULL;
    list->fictive->prev = NULL;
    free(list->fictive);
}