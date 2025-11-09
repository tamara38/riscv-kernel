#ifndef scheduler_hpp
#define scheduler_hpp

#include "list.hpp"
#include "print.hpp"

class TCB;

class Scheduler{
public:
    static List<TCB> readyThreadQueue;

public:
    static TCB* get();

    static void put(TCB* thread);

};

#endif //scheduler_hpp