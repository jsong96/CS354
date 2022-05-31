// allocation is too big to fit
#include <assert.h>
#include <stdlib.h>
#include "heapAlloc.h"

int main() {
    assert(initHeap(4096)  == 0);
    dumpMem();
    assert(allocHeap(1)    != NULL);
    dumpMem();
    assert(allocHeap(4095) == NULL);
    dumpMem();
    exit(0);
}
