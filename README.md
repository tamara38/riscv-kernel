# Operating system kernel
Made a fully functional operating system kernel designed for the RISC-V architecture which runs within 
the qemu emulator. Its implementation is entirely self-contained, with no external libraries or 
dependencies on host operating system calls (e.g. threads, memory allocation). The kernel is structured 
into three layers: the C++ API, serving as an object-oriented wrapper around the C API; the C API 
itself; and the ABI layer, which acts as a wrapper for the C API functions.  
