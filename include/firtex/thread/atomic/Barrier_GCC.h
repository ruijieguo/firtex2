#ifndef __FX_BARRIER_GCC_H
#define __FX_BARRIER_GCC_H

#define FX_COMPILER_RW_BARRIER  __asm__ __volatile__ ( "" ::: "memory" )
#define FX_COMPILER_R_BARRIER FX_COMPILER_RW_BARRIER
#define FX_COMPILER_W_BARRIER FX_COMPILER_RW_BARRIER

#endif  // __FX_BARRIER_GCC_H
