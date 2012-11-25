#ifndef __FX_SIMPLE_ALLOCATOR_H
#define __FX_SIMPLE_ALLOCATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include <vector>

FX_NS_DEF(utility);

class SimpleAllocator
{
public:
    SimpleAllocator();
    ~SimpleAllocator();

    void* allocate(size_t n)
    {
        void* block = new char[n];
        m_blocks.push_back(block);
        return block;
    }
    
    void deallocate(void* pChunk)
    {
    }

    void clear()
    {
        for (BlockVector::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it)
        {
            delete[] (char*)(*it);
        }
        m_blocks.clear();
    }

private:
    typedef std::vector<void*> BlockVector;

    BlockVector m_blocks;
};

DEFINE_TYPED_PTR(SimpleAllocator);

FX_NS_END

#endif
