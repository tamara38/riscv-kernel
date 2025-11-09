#include "../h/scheduler.hpp"
#include "../h/print.hpp"
#include "../h/tcb.hpp"



List<TCB> Scheduler::readyThreadQueue;

TCB* Scheduler::get(){
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB* thread){
    readyThreadQueue.addLast(thread);
}