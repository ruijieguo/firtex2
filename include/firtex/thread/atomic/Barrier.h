#ifndef __FX_BARRIER_H
#define __FX_BARRIER_H

#include "firtex/common/StdHeader.h"


#if FX_COMPILER == FX_COMPILER_MSVC
#include "firtex/thread/atomic/Barrier_VC.h"
#elif FX_COMPILER == FX_COMPILER_GCC
#include "firtex/thread/atomic/Barrier_GCC.h"
#else
#   error "Undefined compiler"
#endif

#endif //__FX_BARRIER_H
