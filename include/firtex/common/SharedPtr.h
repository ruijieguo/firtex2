#ifndef __FXSHAREDPTR_H
#define __FXSHAREDPTR_H


#include <memory>


#define DEFINE_TYPED_PTR(classname)                     \
    typedef std::shared_ptr<classname> classname##Ptr;


#endif
