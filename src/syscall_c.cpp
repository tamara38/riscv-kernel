#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void secall() {
    __asm__ volatile("ecall");
}

void* mem_alloc(size_t size){
    size_t blocks = (size % MEM_BLOCK_SIZE) ? (size / MEM_BLOCK_SIZE + 1) : size / MEM_BLOCK_SIZE;
    __asm__ volatile("mv a1, %0" : : "r" (blocks));
    __asm__ volatile("li a0, 0x01");
    secall();

    void* ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int mem_free(void* ptr){
    __asm__ volatile("mv a1, %0" : : "r" (ptr));
    __asm__ volatile("li a0, 0x02");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64 * stack = nullptr;
    if(start_routine != nullptr) {
        stack = new uint64[DEFAULT_STACK_SIZE];
        if(!stack) return -1;
    }
    __asm__ volatile("mv a4, %0" : : "r" (stack));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x11");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_exit() {
    __asm__ volatile("li a0, 0x12");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void thread_dispatch() {
    __asm__ volatile("li a0, 0x13");
    secall();
}

int thread_start(TCB* thread) {
    __asm__ volatile("mv a1, %0" : : "r" (thread));
    __asm__ volatile("li a0, 0x14");
    secall();
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_create_without_start(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64 * stack = nullptr;
    if(start_routine != nullptr) {
        stack = new uint64[DEFAULT_STACK_SIZE];
        if(!stack) return -1;
    }
    __asm__ volatile("mv a4, %0" : : "r" (stack));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x15");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_open(sem_t* handle, unsigned init) {
    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x21");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_close(sem_t handle) {
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("li a0, 0x22");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_wait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x23");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x24");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_trywait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x26");
    secall();

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void putc(char ch){
    __asm__ volatile("mv a1, %0" : : "r" (ch));
    __asm__ volatile("li a0, 0x42");
    secall();
}

char getc(){
    __asm__ volatile("li a0, 0x41");
    secall();

    char ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int time_sleep(time_t time) {
    return 0;
}




