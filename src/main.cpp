#include "../h/memoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/print.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/scheduler.hpp"



extern void userMain();

void main() {

    MemoryAllocator::initialize();

    RiscV::w_stvec((uint64) &(RiscV::supervisorTrap)); // tells processor what is the trap routine
    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);

    TCB* threads[5];
    int ret = thread_create(&threads[0], nullptr, nullptr);
    TCB::running = threads[0];

    ret += thread_create(&threads[1], (void(*)(void*))&userMain, nullptr );
    while(!threads[1]->isFinished()){
        thread_dispatch();
    }

    _printInteger(ret);
    _printString("\nFinished\n");

    int* end;
    end = (int*)0x100000;
    *end = 0x5555;



}
