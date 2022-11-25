#include "list.h"

//==========================================================================

void ListCtor_(List* list, size_t capacity, const char* name, const char* file, const char* func, int line)
{
    if(list == NULL)
    {
        ASSERT_OK(list);
    }

    list->capacity = capacity + 1;
    if((long long)list->capacity < 1)
    {
        ASSERT_OK(list);
    }

    list->size  = 1;
    list->error = 0;
    list->elems = (ListElement*)calloc(sizeof(ListElement), list->capacity);
    if(list->elems == NULL)
    {
        ASSERT_OK(list);
    }

    list->list_info.name = name;
    list->list_info.file = file;
    list->list_info.func = func;
    list->list_info.line = line;

    for(int i = 1; i < list->capacity; ++i)
    {
        list->elems[i].data = POISON;
        list->elems[i].prev = -1;
        list->elems[i].next = -1;
    }

    list->fictive = 0;
    list->elems[list->fictive].data = POISON;
    list->elems[list->fictive].prev = list->fictive;
    list->elems[list->fictive].next = list->fictive;

    if(list->capacity == 1)
    {
        list->free = -1;
    }
    else if(list->capacity == 2)
    {
        list->free = 1;
    }
    else
    {
        list->free = list->capacity / 2;
    }

    list->sorted = 1;

    ASSERT_OK(list);
}

//==========================================================================

size_t ListBegin(List* list)
{
    assert(list != NULL);

    return list->elems[list->fictive].next;
}

//==========================================================================

size_t ListEnd(List* list)
{
    assert(list != NULL);

    return list->elems[list->fictive].prev;
}

//==========================================================================

size_t ListNext(List* list, size_t num)
{
    assert(list != NULL);

    return list->elems[num].next;
}

//==========================================================================

size_t ListPrev(List* list, size_t num)
{
    assert(list != NULL);

    return list->elems[num].prev;
}

//==========================================================================

size_t PushFront(List* list, elem_t value)
{
    ASSERT_OK(list);

    if(list->size == list->capacity)
    {
        ListResizeUp(list);
    }
    size_t begin = ListBegin(list);

    if(list->sorted == 1 && begin > 1 && list->elems[begin - 1].data == POISON)
    {
        list->free = begin - 1;
    }
    else if(list->size == 1)
    {
        //do nothing
    }
    else
    {
        list->sorted = 0;
        list->free   = FindFree(list);
    }
 
    list->elems[list->free].data = value;
    list->elems[list->free].next = begin;
    list->elems[list->free].prev = list->fictive;

    list->elems[begin].prev         = list->free;
    list->elems[list->fictive].next = list->free;

    ++list->size;

    size_t ret = list->free;
    return ret;
}

//==========================================================================

size_t PushBack(List* list, elem_t value)
{
    ASSERT_OK(list);

    if(list->size == list->capacity)
    {
        ListResizeUp(list);
    }
    size_t end = ListEnd(list);

    if(list->sorted == 1 && end < list->capacity - 1 && list->elems[end + 1].data == POISON)
    {
        list->free = end + 1;
    }
    else if(list->size == 1)
    {
        //do nothing
    }
    else
    {
        list->sorted = 0;
        list->free   = FindFree(list);
    }
 
    list->elems[list->free].data = value;
    list->elems[list->free].next = list->fictive;
    list->elems[list->free].prev = end;

    list->elems[end].next           = list->free;
    list->elems[list->fictive].prev = list->free;

    ++list->size;

    size_t ret = list->free;
    return ret;
}

//==========================================================================

size_t InsertBefore(List* list, size_t index, elem_t value)
{
    ASSERT_OK(list);

    if(list->size == list->capacity)
    {
        ListResizeUp(list);
    }

    if(index > 1 && list->elems[index - 1].data == POISON)
    {
        list->free = index - 1;
    }
    else if(list->size == 1)
    {
        //do nothing
    }
    else
    {
        list->free = FindFree(list);
    }

    size_t prev = ListPrev(list, index);

    list->elems[list->free].data = value;
    list->elems[list->free].next = index;
    list->elems[list->free].prev = prev;

    list->elems[prev].next  = list->free;
    list->elems[index].prev = list->free;

    ++list->size;
    list->sorted = 0;

    size_t ret = list->free;
    return ret;
}

//==========================================================================

size_t InsertAfter(List* list, size_t index, elem_t value)
{
    ASSERT_OK(list);

    if(list->size == list->capacity)
    {
        ListResizeUp(list);
    }

    if(index < list->capacity - 1 && list->elems[index + 1].data == POISON)
    {
        list->free = index + 1;
    }
    else if(list->size == 1)
    {
        //do nothing
    }
    else
    {
        list->free = FindFree(list);
    }

    size_t next = ListNext(list, index);

    list->elems[list->free].data = value;
    list->elems[list->free].next = next;
    list->elems[list->free].prev = index;

    list->elems[next].prev  = list->free;
    list->elems[index].next = list->free;

    ++list->size;
    list->sorted = 0;

    size_t ret = list->free;
    return ret;
}

//==========================================================================

void DeleteElem(List* list, size_t index)
{
    ASSERT_OK(list);

    if(index == 0)
    {
        printf("Error: I forbid you to delete the fictive element!\n");
        return;
    }

    if(list->elems[index].data == POISON)
    {
        printf("Error: this element is already deleted!\n");
        return;
    }

    if(!(index == ListBegin(list) || index == ListEnd(list)))
    {
        list->sorted = 0;
    }

    size_t prev = ListPrev(list, index);
    size_t next = ListNext(list, index);
    list->elems[prev].next = next;
    list->elems[next].prev = prev;

    list->elems[index].data = POISON;
    list->elems[index].prev = -1;
    list->elems[index].next = -1;

    --list->size;
}

//==========================================================================

size_t FindByValue(List* list, elem_t value)
{
    ASSERT_OK(list);

    size_t iter = ListBegin(list);
    for(int i = 1; i < list->size; ++i)
    {
        if(list->elems[iter].data == value) return iter;
        iter = ListNext(list, iter);
    }

    return -1;
}

//==========================================================================

size_t FindByLogNum(List* list, size_t log_num)
{
    ASSERT_OK(list);

    if(list->sorted == 1) return ListBegin(list) + log_num - 1;

    size_t iter = ListBegin(list);
    for(int i = 1; i < list->size; ++i)
    {
        if(i == log_num) return iter;
        iter = ListNext(list, iter);
    }

    return -1;
}

//==========================================================================

size_t FindFree(List* list)
{
    for(int i = 1; i < list->capacity; ++i)
    {
        if(list->elems[i].data == POISON) return i;
    }

    return 0;
}

//==========================================================================

void ListResizeUp(List* list)
{
    ASSERT_OK(list);

    if(list->capacity == 1)
    {
        list->capacity = 4;

        ListElement* new_array = (ListElement*)calloc(list->capacity, sizeof(ListElement));
        assert(new_array != NULL);

        for(int i = 0; i < list->capacity; ++i)
        {
            new_array[i].data = POISON;
            new_array[i].prev = -1;
            new_array[i].next = -1;
        }

        new_array[0].next = 0;
        new_array[0].prev = 0;

        list->sorted = 1;

        free(list->elems);
        list->elems = new_array;
        new_array   = NULL;
    }

    else
    {
        ListElement* new_array = (ListElement*)calloc(list->capacity * 3, sizeof(ListElement));
        assert(new_array != NULL);

        for(int i = 0; i < list->capacity * 3; ++i)
        {
            new_array[i].data = POISON;
            new_array[i].prev = -1;
            new_array[i].next = -1;
        }

        size_t iter = ListBegin(list);
        for(int i = list->capacity; i < list->capacity + list->size - 1; ++i)
        {
            new_array[i].data = list->elems[iter].data;
            new_array[i].prev = i - 1;
            new_array[i].next = i + 1;

            iter = ListNext(list, iter);
        }

        new_array[0].next = list->capacity;
        new_array[0].prev = list->capacity + list->size - 2;

        new_array[list->capacity].prev                  = 0;
        new_array[list->capacity + list->size - 2].next = 0;

        list->capacity *= 3;
        list->sorted    = 1;

        free(list->elems);
        list->elems = new_array;
        new_array   = NULL;
    }
}

//==========================================================================

void ListSort(List* list)
{
    ASSERT_OK(list);

    if(list->sorted == 1) return;

    ListElement* new_array = (ListElement*)calloc(list->capacity, sizeof(ListElement));
    assert(new_array != NULL);

    for(int i = 0; i < list->capacity; ++i)
    {
        new_array[i].data = POISON;
        new_array[i].next = -1;
        new_array[i].prev = -1;
    }

    int start = 0;
    for(int i = 1; i < list->capacity; ++i)
    {
        if(list->elems[i].data != POISON)
        {
            start = i;
            break;
        }
    }

    size_t iter = ListBegin(list);
    for(int i = start; i < start + list->size - 1; ++i)
    {
        new_array[i].data = list->elems[iter].data;
        new_array[i].prev = i - 1;
        new_array[i].next = i + 1;

        iter = ListNext(list, iter);
    }

    new_array[0].next = start;
    new_array[0].prev = start + list->size - 2;

    new_array[start].prev                  = 0;
    new_array[start + list->size - 2].next = 0;

    list->sorted = 1;

    free(list->elems);
    list->elems = new_array;
    new_array   = NULL;
}

//==========================================================================

int ListVerify(List* list)
{
    if(list == NULL)
    {
        printf("\x1B[31mError\x1B[0m: list pointer is NULL[%p]\n", list);
        abort();
    }

    if((long long)list->capacity < 1)
    {
        list->error = BAD_CAPACITY;
        return list->error;
    }

    if((long long)list->size < 1)
    {
        list->error = BAD_SIZE;
        return list->error;
    }

    if(list->size > list->capacity)
    {
        list->error = BIG_SIZE;
        return list->error;
    }

    if(list->elems == NULL)
    {
        list->error = NULL_DATA_POINTER;
        return list->error;
    }

    if(list->fictive != 0)
    {
        list->error = WRONG_FICTIVE;
        return list->error;
    }

    size_t iter = ListBegin(list);
    int i = 1;
    for(; i < list->size, iter != list->fictive; ++i)
    {
        size_t next = ListNext(list, iter);

        if(next >= list->capacity)
        {
            list->error = WRONG_NEXT;
            return list->error;
        }

        iter = next;
    }
    
    if(i != list->size || iter != list->fictive)
    {
        list->error = BAD_SIZE;
        return list->error;
    }

    iter = ListEnd(list);
    i = 1;
    for(; i < list->size, iter != list->fictive; ++i)
    {
        size_t prev = ListPrev(list, iter);

        if(prev >= list->capacity)
        {
            list->error = WRONG_PREV;
            return list->error;
        }

        iter = prev;
    }

    if(i != list->size || iter != list->fictive)
    {
        list->error = BAD_SIZE;
        return list->error;
    }

    return list->error;
}

//==========================================================================

void PrintError(List* list)
{
    switch(list->error)
    {
        case NULL_LST_POINTER:
        {
            printf("\x1B[31mError\x1B[0m: list pointer is NULL\n");
            break;
        }
        case NULL_DATA_POINTER:
        {
            printf("\x1B[31mError\x1B[0m: data pointer is NULL\n");
            break;
        }
        case BAD_CAPACITY:
        {
            printf("\x1B[31mError\x1B[0m: capacity is negative\n");
            break;
        }
        case BAD_SIZE:
        {
            printf("\x1B[31mError\x1B[0m: size is negative\n");
            break;
        }
        case BIG_SIZE:
        {
            printf("\x1B[31mError\x1B[0m: size is bigger than capacity\n");
            break;
        }
        case WRONG_FICTIVE:
        {
            printf("\x1B[31mError\x1B[0m: the fictive element is not zero\n");
            break;
        }
        case WRONG_NEXT:
        {
            printf("\x1B[31mError\x1B[0m: the next element is out of the list\n");
            break;
        }
        case WRONG_PREV:
        {
            printf("\x1B[31mError\x1B[0m: the previous element is out of the list\n");
            break;
        }
        default:
        {
            printf("\x1B[31mError\x1B[0m: unknown error\n");
            break;
        }
    }
}

//==========================================================================

void ListDump(List* list)
{
    FILE* log = fopen("dump.txt", "w");

    assert(log  != NULL);
    assert(list != NULL);
    assert(list->elems != NULL);

    fprintf(log, "digraph List {\n");
    fprintf(log, "\trankdir = LR;\n");
    fprintf(log, "\tnode[shape = record, color = \"red\", fontsize = 14, style = \"rounded\", style = \"filled\", fillcolor = \"lightgreen\"];\n");
    fprintf(log, "\tedge[color = \"darkgreen\"];\n");
    fprintf(log, "\tinfo [fillcolor = \"lightblue\", label = \" list[%p] | name: %s | at %s | at %s(%d) | error = %d \"];\n",
                   list, list->list_info.name + 1, list->list_info.file, list->list_info.func, list->list_info.line, list->error);
    fprintf(log, "\t0 [label=\" <1> fictive elem(0) | <2> data = POISON | {prev = %lu | next = %lu}\" ];\n", ListEnd(list), ListBegin(list));

    for(size_t i = 1; i < list->capacity; ++i)
    {
        fprintf(log, "\t%lu [label=\" <1> %lu | <2> data = ", i, i);
        if(list->elems[i].data == POISON)
        {
            fprintf(log, "POISON| {prev = %ld | next = %ld}\", fillcolor = \"green\"];\n", ListPrev(list, i), ListNext(list, i));
        }
        else
        {
            fprintf(log, elem_fmt, list->elems[i].data);
            fprintf(log, "| {prev = %lu | next = %lu}\"];\n", ListPrev(list, i), ListNext(list, i));
        }
    }

    size_t iter = ListBegin(list);
    size_t prev = list->fictive;
    if(iter == list->fictive)
    {
        fprintf(log, "\t0: <1> -> 0;\n");
        fprintf(log, "\t0: <2> -> 0;\n");
    }

    else if(iter < list->capacity)
    {
        size_t index = 1;
        for(; index < list->size, iter != list->fictive; ++index, iter = ListNext(list, iter))
        {
            if(iter >= list->capacity) break;
            fprintf(log, "\t%lu: <1> -> %lu;\n", prev, iter);
            if(prev == ListPrev(list, iter)) fprintf(log, "\t%lu: <2> -> %lu;\n", iter, prev);
            prev = ListNext(list, prev);
        }
        if(iter == list->fictive)
        {
            fprintf(log, "\t%lu: <1> -> 0;\n", prev);
            if(prev == ListEnd(list)) fprintf(log, "\t0: <2> -> %lu;\n", prev);
        }
    }

    fprintf(log, "\tedge[color = \"white\"];\n");
    size_t count_free = 0;
    for(size_t i = 1; i < list->capacity; ++i)
    {
        if(list->elems[i].data == POISON)
        {
            ++count_free;
            fprintf(log, "%lu ", i);
            if(count_free != list->capacity - list->size)
            {
                fprintf(log, "-> ");
            }
            else
            {
                fprintf(log, ";\n");
            }
        }
    }

    fprintf(log, "}\n");
    fclose(log);

    system("dot dump.txt -T -O png");
    system("open dump.txt.png");
}

//==========================================================================

void ListClear(List* list)
{
    ASSERT_OK(list);

    size_t iter = ListBegin(list);
    for(int i = 1; i < list->size; ++i)
    {
        size_t next = ListNext(list, iter);

        list->elems[iter].data = POISON;
        list->elems[iter].prev = -1;
        list->elems[iter].next = -1;

        iter = next;
    }

    list->elems[list->fictive].next = list->fictive;
    list->elems[list->fictive].prev = list->fictive;

    list->size   = 1;
    list->sorted = 1;

    if(list->capacity == 1)
    {
        list->free = -1;
    }
    else if(list->capacity == 2)
    {
        list->free = 1;
    }
    else
    {
        list->free = list->capacity / 2;
    }
}

//==========================================================================

void ListDtor(List* list)
{
    ASSERT_OK(list);

    free(list->elems);
    list->elems = NULL;

    list->capacity = 0;
    list->size     = 0;
    list->error    = 0;
    list->free     = 0;
}