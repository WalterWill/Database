#include <iostream>
#include "estrutura.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    list *l = createList();
    DataNode data;
    data.primo = 9;

    final(l, data);


    data.primo = 5;
    final(l, data);

    data.primo = 1997;
    final(l,data);

    printf("Ultimo: %d\n", l->last->number.primo);
    printf("Por index: %d\n", atPos(l, l->size - 1)->number.primo);

    printList(l);

    return 0;
}