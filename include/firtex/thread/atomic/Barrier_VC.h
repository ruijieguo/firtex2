#ifndef __FX_BARRIER_VC_H
#define __FX_BARRIER_VC_H

#include <intrin.h>

#pragma intrinsic(_ReadWriteBarrier)
#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_WriteBarrier)

#define FX_COMPILER_RW_BARRIER  _ReadWriteBarrier()
#define FX_COMPILER_R_BARRIER   _ReadBarrier()
#define FX_COMPILER_W_BARRIER   _WriteBarrier()

#endif  // __FX_BARRIER_VC_H
