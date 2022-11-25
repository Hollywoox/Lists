#include "list.h"

int main()
{
    List list1 = {};
    ListCtor(&list1);

    PushFront(&list1, 30);
    PushFront(&list1, 56);
    PushFront(&list1, 102);
    PushFront(&list1, 3434);
    PushFront(&list1, 3243432);

    ListDump(&list1);

    //ListClear(&list1);
    //ListDump(&list1);
    ListDtor(&list1);
    return 0;
}