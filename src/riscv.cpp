#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.h"
#include "../h/memoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/sempahore.hpp"

void RiscV::popSppSpie() { //
    RiscV::mc_sstatus(RiscV::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void RiscV::handleSupervisorTrap() {
    uint64 scause = RiscV::r_scause();
    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){ //ecall from user or supervisor mode
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        uint64 a0;
        __asm__ volatile("ld %0, 80(fp)" : "=r"(a0));
        uint64 a1;
        __asm__ volatile("ld %0, 88(fp)" : "=r"(a1));
        uint64 volatile a2;
        __asm__ volatile("ld %0, 96(fp)" : "=r"(a2));
        uint64 a3;
        __asm__ volatile("ld %0, 104(fp)" : "=r"(a3));
        uint64 a4;
        __asm__ volatile("ld %0, 112(fp)" : "=r"(a4));
        switch (a0) {
            case 0x01:  //MEM_ALLOC
                MemoryAllocator::mem_alloc(a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x02:  //MEM_FREE
                MemoryAllocator::mem_free((void*)a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x11:{ //THREAD_CREATE
                uint64 * stek = (uint64*)a4;
                TCB::create((TCB**)a1, (void(*)(void*))a2, (void*)a3,stek);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            }
            case 0x12: //THREAD_EXIT
                TCB::exit();
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x13: //THREAD_DISPATCH
                TCB::dispatch();
                break;
            case 0x14: {//THREAD_START
                TCB::start((TCB*)a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            }
            case 0x15: {//THREAD_CREATE_WITHOUT_START
                uint64 *stek = (uint64 *) a4;
                TCB::createwoutstart((TCB **) a1, (void (*)(void *)) a2, (void *) a3, stek);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            }
            case 0x21: //SEM_OPEN
                _sem::open((_sem**)a1, a2);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x22: //SEM_CLOSE
                _sem::close((_sem*)a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x23: //SEM_WAIT
                _sem::wait((_sem*)a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x24: //SEM_SIGNAL
                _sem::signal((_sem*)a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x26: //SEM_TRYWAIT
                _sem::trywait((_sem*)a1);
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x41: //GETC
                __getc();
                __asm__ volatile("sd a0, 8*10(fp)");
                break;
            case 0x42: //PUTC
                //ret_a0 = true;
                __putc((char)a1);
                break;
            default:
                break;
        }
        w_sepc(sepc);
        w_sstatus(sstatus);
    }
    else if(scause == 0x8000000000000001UL){ //timer interrupt
        mc_sip(SIP_SSIP);
    }
    else if(scause == 0x8000000000000009UL){ //console interrupt (hardware external interrupt)
        console_handler();
        mc_sip(SIP_SEIP);
    }
    else{
        _printString("error ");
        _printInteger(scause);
        _printString("\n");
        uint64 tmp = r_sepc();
        printHexa(tmp);
        _printString("\n");
    }

}