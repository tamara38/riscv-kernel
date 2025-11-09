#include "../h/tcb.hpp"
#include "../h/print.hpp"

TCB* TCB::running = nullptr;

void TCB::yield() {
    RiscV::w_a0(0x13);
    __asm__ volatile("ecall");
}

void TCB::dispatch() {
    TCB* oldRunning = running;
    if(!oldRunning->isFinished() && !oldRunning->blocked) {
        Scheduler::put(oldRunning);
    }
    TCB::running = Scheduler::get();

    if(oldRunning != running ) {
        contextSwitch(&oldRunning->context, &running->context);
    }
}

void TCB::block() {
    running->blocked = true;
    thread_dispatch();
}

void TCB::deblock() {
    this->blocked = false;
    Scheduler::put(this);
}

int TCB::exit() {
    if(!running || !running->body){ return -1; }
    running->finished = true;
    TCB* old = running;
    thread_dispatch();
    delete old;
    return 0;
}

TCB::TCB(TCB::Body body, void *arg, uint64 *stack) : body(body), arg(arg),
                                                    stack(body != nullptr ? stack : nullptr),
                                                    context({body != nullptr ? (uint64) &wrappper : 0,
                                                             stack != nullptr ? (uint64)((char*)stack + DEFAULT_STACK_SIZE)  : 0
                                                    }),
                                                    finished(false), blocked(false)
{
}

void TCB::wrappper() {
    RiscV::popSppSpie(); //in purposes of going back to user from supervisor mode
    running->body(running->arg);
    thread_exit();
}

int TCB::createwoutstart(TCB **handle, void (*start_routine)(void *), void *arg, void *stack) {
    if(!handle) return -1;
    *handle = new TCB((Body)start_routine, arg, (uint64 *)stack);
    if(!handle) return -1;
    return 0;
}

int TCB::create(TCB **handle, void (*start_routine)(void *), void *arg, void *stack) {
    int ret = createwoutstart(handle, start_routine, arg, stack);
    if( ret == 0 ) {
        start(*handle);
    }
    return ret;
}

int TCB::start(TCB *thread) {
    if(!thread) return -1;
    if(!thread->hasBody()) return -1;
    Scheduler::put(thread);
    return 0;
}
