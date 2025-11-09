#ifndef list_hpp
#define list_hpp

#include "memoryAllocator.hpp"


template<typename T>
class List{
    struct Elem{
        T* data;
        Elem* next;

        Elem(T* data) : data(data), next(nullptr) {}

        void* operator new(size_t size){
            return MemoryAllocator::mem_alloc(size);
        }

        void* operator new[](size_t size){
            return MemoryAllocator::mem_alloc(size);
        }

        void operator delete(void * ptr){
            MemoryAllocator::mem_free(ptr);
        }

        void operator delete[](void* ptr){
            MemoryAllocator::mem_free(ptr);
        }
    };

    Elem *head, *tail;

public:

    List() : head(nullptr), tail(nullptr) {}

    List(const List<T> &) = delete;

    List<T> & operator=(const List<T> &) = delete;

    void addFirst(T* data){
        Elem* elem = new Elem(data);
        elem->next = head;
        head = elem;

        if(!tail) { tail = head; }
    }

    void addLast(T* data) {
        Elem* elem = new Elem(data);
        if(!tail){
            head = tail = elem;
        }else{
            tail->next = elem;
            tail = elem;
        }
    }

    T* removeFirst() {
        if(!head) { return nullptr; }

        Elem* elem = head;
        head = head->next;
        if(!head) { tail = nullptr; }

        T* ret = elem->data;
        elem->next = nullptr;
        delete elem;
        return ret;
    }

    T* peekFirst() {
        if(!head) { return nullptr; }
        return head->data;
    }

    T* removeLast() {
        if(!head) { return nullptr; }

        Elem* prev = nullptr, *curr = head;
        while(curr != tail){
            prev = curr;
        }

        if(!prev) { head = tail = nullptr; }
        else {
            prev->next = nullptr;
            tail = prev;
        }
        T* ret = curr->data;
        delete curr;
        return ret;

    }

    T* peekLast() {
        if(!tail) { return 0; }
        return tail->data;
    }

};



#endif //list_hpp