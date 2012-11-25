#ifndef __FX_COMMON_H
#define __FX_COMMON_H

#	define FX_NS_DEF(sub) namespace firtex{ namespace sub{
#	define FX_ROOTNS_DEF() namespace firtex{ 

#	define FX_NS_END }}
#	define FX_ROOTNS_END() }

#	define FX_NS_USE(sub) using namespace firtex::sub;
#	define FX_ROOTNS_USE() using namespace firtex;

#	define FX_NS(sub) firtex::sub
#	define FX_ROOTNS firtex

#endif
