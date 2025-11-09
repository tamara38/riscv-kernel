#include "../h/sempahore.hpp"
#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"

int _sem::open(_sem **handle, unsigned int init) {
    *handle = new _sem(init);
    if(!*handle) return -1;
    return 0;
}

int _sem::close(_sem *handle) {
    if(!handle) return -1;
    if(handle->closed) return -2;
    handle->closed = true;
    TCB* curr = handle->blocked.removeFirst();
        while(curr){
            curr->deblock();
            curr = handle->blocked.removeFirst();
        }
    return 0;
}

int _sem::signal(_sem *id) {
    if(!id) return -1;
    if(id->closed) return -1;
    if(!TCB::running) return -2;

    id->value++;

    if(id->value <= 0) {
        id->blocked_num--;
        TCB* thread = id->blocked.removeFirst();
        thread->deblock();
    }
    return 0;
}

int _sem::wait(_sem *id) {
    if(!id) return -1;
    if(!TCB::running) return -2;

    id->value--;

    if(id->value < 0){
        id->blocked_num++;
        id->blocked.addLast(TCB::running);
        TCB::block();

        if(!id->closed) return 0;
        else{
            if(id->blocked_num == 0) return 0;
            else{
                id->blocked_num--;
                return -1;
            }
        }
    }
    return 0;
}

_sem::~_sem() {
    close(this);
}

int _sem::trywait(_sem *id) {
    if(!id || id->closed) return -1;
    if(!TCB::running) return -2;
    if(id->value > 0) --id->value;
    return id->value > 0 ? 1 : 0;
}
