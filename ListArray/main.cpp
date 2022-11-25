#include "list.h"

int main()
{
    List list = {};

    ListCtor(&list, 3);
    PushBack(&list, 33);
    PushBack(&list, 43);
    PushBack(&list, 53);
    PushBack(&list, 100);
    PushFront(&list, 333);
    InsertAfter(&list, 3, 3333);
    DeleteElem(&list, 7);
    //ListClear(&list);
    ListDump(&list);
    ListDtor(&list);
    return 0;
}