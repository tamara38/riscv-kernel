#ifndef syscall_c_hpp
#define syscall_c_hpp

#include "../lib/hw.h"
#include "tcb.hpp"
#include "sempahore.hpp"


inline void secall();


void* mem_alloc(size_t size);
int mem_free(void*);

class TCB;
typedef TCB* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();
int thread_start(TCB* thread);
int thread_create_without_start(thread_t* handle, void(*start_routine)(void*), void* arg);

class _sem;
typedef _sem* sem_t;

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_trywait(sem_t id);

void putc(char ch);
char getc();

int time_sleep(time_t time);

#endif //syscall_c_hpp