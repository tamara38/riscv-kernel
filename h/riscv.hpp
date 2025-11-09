#ifndef riscv_hpp
#define riscv_hpp
#include "../lib/hw.h"

class RiscV{
public:

    static void pushRegisters();
    static void popRegisters();

    static void popSppSpie();

    static void supervisorTrap();

    static uint64 r_sepc();            //read sepc register
    static void w_sepc(uint64 value); //write in sepc register

    static uint64 r_sstatus();            //read sstatus register
    static void w_sstatus(uint64 value); //write in sstatus register

    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);

    static uint64 r_scause();            //read scause register
    static void w_scause(uint64 value); //write in scause register

    static uint64 r_stvec();            //read stvec register
    static void w_stvec(uint64 value); //write in stvec register

    static void ms_sip(uint64 mask);
    static void mc_sip(uint64 mask);

    static uint64 r_a0();
    static void w_a0(uint64 value);

    static uint64 r_a1();
    static void w_a1(uint64 value);

    static uint64 r_a2();
    static void w_a2(uint64 value);

    static uint64 r_a3();
    static void w_a3(uint64 value);

    static uint64 r_a4();
    static void w_a4(uint64 value);

    enum BitMaskStatus{
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    enum BitMaskSip{
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

private:

    static void handleSupervisorTrap();


};

inline uint64 RiscV::r_sepc() {
    uint64 volatile value;
    __asm__ volatile("csrr %0, sepc" : "=r"(value));
    return value;
}

inline void RiscV::w_sepc(uint64 value) {
    __asm__ volatile("csrw sepc, %0" : : "r"(value));
}

inline uint64 RiscV::r_sstatus() {
    uint64 volatile value;
    __asm__ volatile("csrr %0, sstatus" : "=r"(value));
    return value;
}

inline void RiscV::w_sstatus(uint64 value) {
    __asm__ volatile("csrw sstatus, %0" : : "r"(value));
}

inline void RiscV::ms_sstatus(uint64 mask) {
    __asm__ volatile("csrs sstatus, %0" : : "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask) {
    __asm__ volatile("csrc sstatus, %0" : : "r"(mask));
}

inline uint64 RiscV::r_scause() {
    uint64 volatile value;
    __asm__ volatile("csrr %0, scause" : "=r"(value));
    return value;
}

inline void RiscV::w_scause(uint64 value) {
    __asm__ volatile("csrw scause, %0" : : "r"(value));
}

inline uint64 RiscV::r_stvec() {
    uint64 volatile value;
    __asm__ volatile("csrr %0, stvec" : "=r"(value));
    return value;
}

inline void RiscV::w_stvec(uint64 value) {
    __asm__ volatile("csrw stvec, %0" : : "r"(value));
}

inline void RiscV::ms_sip(uint64 mask) {
    __asm__ volatile("csrs sip, %0" : : "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask) {
    __asm__ volatile("csrc sip, %0" : : "r"(mask));
}

inline uint64 RiscV::r_a0() {
    uint64 volatile value;
    __asm__ volatile("mv %0, a0" : "=r"(value));
    return value;
}

inline void RiscV::w_a0(uint64 value) {
    __asm__ volatile("mv a0, %0" : : "r"(value));
}

inline uint64 RiscV::r_a1() {
    uint64 volatile value;
    __asm__ volatile("mv %0, a1" : "=r"(value));
    return value;
}

inline void RiscV::w_a1(uint64 value) {
    __asm__ volatile("mv a1, %0" : : "r"(value));
}

inline uint64 RiscV::r_a2() {
    uint64 volatile value;
    __asm__ volatile("mv %0, a2" : "=r"(value));
    return value;
}

inline void RiscV::w_a2(uint64 value) {
    __asm__ volatile("mv a2, %0" : : "r"(value));
}

inline uint64 RiscV::r_a3() {
    uint64 volatile value;
    __asm__ volatile("mv %0, a3" : "=r"(value));
    return value;
}

inline void RiscV::w_a3(uint64 value) {
    __asm__ volatile("mv a3, %0" : : "r"(value));
}

inline uint64  RiscV::r_a4() {
    uint64 volatile value;
    __asm__ volatile("mv %0, a4" : "=r"(value));
    return value;
}

inline void RiscV::w_a4(uint64 value) {
    __asm__ volatile("mv a4, %0" : : "r"(value));
}

#endif //riscv_hpp