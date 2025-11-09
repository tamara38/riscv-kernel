#include "../h/memoryAllocator.hpp"

memBlock* MemoryAllocator::free = nullptr;
memBlock* MemoryAllocator::allocated = nullptr;


void MemoryAllocator::initialize() {
    MemoryAllocator::free = (memBlock*)(size_t)HEAP_START_ADDR;
    MemoryAllocator::free->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(memBlock); //at the beginning everythning is free
    MemoryAllocator::free->next = nullptr;
    MemoryAllocator::free->prev = nullptr;
    MemoryAllocator::allocated = nullptr; //there's still no memory allocated
}

void *MemoryAllocator::mem_alloc(size_t size) {
    if(!MemoryAllocator::free) {
        return nullptr; // no free space
    }

    size_t newSize = size*MEM_BLOCK_SIZE + sizeof(memBlock);

    //find free block
    memBlock* newBlck = findFreeBlock(newSize);
    if(!newBlck) return nullptr; // no free space

    if(newBlck->size >= newSize && newBlck->size - newSize >= MEM_BLOCK_SIZE + sizeof(memBlock)) { //creating new free seg for free list (leftover chunk from free block)
        //creating new free seg
        memBlock *newChunk;
        newChunk = (memBlock *) ((char *) newBlck + newSize + sizeof(memBlock));
        newChunk->size = newBlck->size - newSize - sizeof(memBlock);
        newBlck->size = newSize;

        //updating free list
        if (newBlck->prev) newBlck->prev->next = newChunk;
        else MemoryAllocator::free = newChunk;
        if (newBlck->next) newBlck->next->prev = newChunk;
        newChunk->prev = newBlck->prev;
        newChunk->next = newBlck->next;
        newBlck->prev = newBlck->next = nullptr;
    }
    else{ //choosen block is exact same size as requested
        //updating free list
        if(newBlck->prev) newBlck->prev->next = newBlck->next;
        else MemoryAllocator::free = newBlck->next;
        if(newBlck->next) newBlck->next->prev = newBlck->prev;
    } //updating allocated list
    if(!MemoryAllocator::allocated || (char*)newBlck <= (char*)MemoryAllocator::allocated) { //allocated list is empty or
        newBlck->next = MemoryAllocator::allocated;                     //address of the element for inserton comes before address of list head
        newBlck->prev = nullptr;
        if(MemoryAllocator::allocated) MemoryAllocator::allocated->prev = newBlck;
        MemoryAllocator::allocated = newBlck;
    }
    else{ //inserting element in the middle or at the end of the allocated list
        memBlock* currAllocated = MemoryAllocator::allocated;
        while(currAllocated->next && (char*)currAllocated->next < (char*)newBlck)
            currAllocated = currAllocated->next;
        newBlck->next = currAllocated->next;
        if(currAllocated->next) currAllocated->next->prev = newBlck;
        newBlck->prev = currAllocated;
        currAllocated->next = newBlck;
    }
    return (void*)((char*)newBlck + sizeof(memBlock));
}

int MemoryAllocator::mem_free(void *blck) { // suceeded -> 0; error -> negative value
    if(!blck) return -1; //
    if((char*)blck < HEAP_START_ADDR || (char*)blck >= HEAP_END_ADDR) return -2;// out of heap bounds
    if(!MemoryAllocator::allocated) return -3;
    memBlock* block = (memBlock*)((char*)blck - sizeof(memBlock));
    //removing block from allocated list
    if(block->prev) block->prev->next = block->next;
    else MemoryAllocator::allocated = block->next;
    if(block->next) block->next->prev = block->prev;
    else{
        if(!block->prev) MemoryAllocator::allocated = block->next;
    }


    //inserting block to free list
    if(!MemoryAllocator::free || (char*)block <= (char*)MemoryAllocator::free ){
        block->next = MemoryAllocator::free;
        block->prev = nullptr;
        if(MemoryAllocator::free) MemoryAllocator::free->prev = block;
        MemoryAllocator::free = block;
    }else{
        memBlock* curr = MemoryAllocator::free;
        while(curr->next && (char*)curr->next < (char*)block){
            curr = curr->next;
        }
        if(curr->next) curr->next->prev = block;
        block->next = curr->next;
        curr->next = block;
        block->prev = curr;
    }

    tryToJoin(block);
    tryToJoin(block->prev);
    return 0;
}

void MemoryAllocator::tryToJoin(memBlock *blck) {
    if(!blck || !blck->next || (char*)blck + blck->size + sizeof(memBlock) != (char*)blck->next) return;
    blck->size += blck->next->size + sizeof(memBlock);
    blck->next = blck->next->next;
    if(blck->next) blck->next->prev = blck;
}

memBlock *MemoryAllocator::findFreeBlock(size_t size) {
    if(size <= 0) return nullptr;
    memBlock* curr = MemoryAllocator::free;
    while(curr){
        if(curr->size >= size) return curr; //first fit
        curr = curr->next;
    }
    return nullptr;
}





