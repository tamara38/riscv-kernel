#ifndef memoryAllocator_hpp
#define memoryAllocator_hpp

#include "../lib/hw.h"


struct memBlock{
    memBlock* next;
    memBlock* prev;
    size_t size;
    memBlock() : next(nullptr), prev(nullptr), size(0) {}
};

class MemoryAllocator{

public:
    static void* mem_alloc(size_t size);
    static int mem_free(void* blck);
    static void initialize();

private:
    static memBlock* free;
    static memBlock* allocated;

    static memBlock* findFreeBlock(size_t size);
    static void tryToJoin(memBlock* blck);


};

#endif // memoryAllocator_hpp