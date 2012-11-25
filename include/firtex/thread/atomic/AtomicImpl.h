#ifndef __FX_ATOMICIMPL_H
#define __FX_ATOMICIMPL_H

#include "firtex/common/StdHeader.h"
#include "firtex/thread/atomic/MemoryOrder.h"
#include "firtex/thread/atomic/Barrier.h"
#include "firtex/thread/atomic/AtomicIntegrals.h"

#if FX_COMPILER == FX_COMPILER_MSVC
#   if FX_PROCESSOR_ARCH == FX_PROCESSOR_X86
#       include <firtex/thread/atomic/Atomic_VC_X86.h>
#   elif FX_PROCESSOR_ARCH == FX_PROCESSOR_X86_64
#       include <firtex/thread/atomic/Atomic_VC_X86_64.h>
#   else
#       error "MS VC++ compiler: unsupported processor architecture"
#   endif
#elif FX_COMPILER == FX_COMPILER_GCC
#   if FX_PROCESSOR_ARCH == FX_PROCESSOR_X86
#       include <firtex/thread/atomic/Atomic_GCC_X86.h>
#   elif FX_PROCESSOR_ARCH == FX_PROCESSOR_X86_64
#       include <firtex/thread/atomic/Atomic_GCC_X86_64.h>
#   elif FX_PROCESSOR_ARCH == FX_PROCESSOR_IA64
#       include <firtex/thread/atomic/Atomic_GCC_IA64.h>
#   elif FX_PROCESSOR_ARCH == FX_PROCESSOR_SPARC
#       include <firtex/thread/atomic/Atomic_GCC_SPARC.h>
#   elif FX_PROCESSOR_ARCH == FX_PROCESSOR_PPC64
#       include <firtex/thread/atomic/Atomic_GCC_PPC64.h>
#   else
#       error "GCC compiler: unsupported processor architecture"
#   endif
#else
#   error "Undefined compiler"
#endif

#endif //__FX_ATOMICIMPL_H
