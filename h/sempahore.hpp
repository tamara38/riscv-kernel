#ifndef _sem_hpp
#define _sem_hpp

#include "list.hpp"

class TCB;

class _sem{
public:

    ~_sem();
    static int open(_sem** handle, unsigned int init);
    static int close(_sem* handle);
    static int signal(_sem* id);
    static int wait(_sem* id);
    static int trywait(_sem* id);

private:

    explicit _sem(int init = 1) : value(init), closed(false), blocked_num(0){}

    int value;
    bool closed;
    int blocked_num;
    List<TCB> blocked;
};



#endif //_sem_hpp