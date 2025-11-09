#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"


void* operator new(size_t n) {
    return mem_alloc(n);
}

void* operator new[](size_t n) {
    return mem_alloc(n);
}

void operator delete(void* p) noexcept{
    mem_free(p);
}

void operator delete[](void* p) noexcept{
    mem_free(p);
}


