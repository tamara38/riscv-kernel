#ifndef tcb_hpp
#define tcb_hpp

#include "../lib/hw.h"
#include "memoryAllocator.hpp"
#include "riscv.hpp"
#include "scheduler.hpp"
#include "syscall_c.h"

class TCB{
public:

    ~TCB() { delete[] stack; }
    bool isFinished() const { return finished; }
    void setFinished(bool status) { finished = status; }
    static void block();
    void deblock();
    bool hasBody() { return this->body != nullptr ? 1 : 0; }


    static int create(TCB** handle, void (*start_routine)(void*), void* arg, void * stack);
    static int createwoutstart(TCB** handle, void (*start_routine)(void*), void* arg, void * stack);
    static int exit();
    static int start(TCB* thread);
    static int threadID;

    using Body = void(*)(void*);

    static TCB* running;

    static void yield();

private:
    TCB(Body body, void* arg, uint64* stack);
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    Body body;
    void* arg;
    uint64* stack;
    Context context;
    bool finished;
    bool blocked;

    friend class RiscV;

    static void contextSwitch(Context* oldContext, Context* newContext);
    static void dispatch();
    static void wrappper();
};

#endif //tcb_hpp