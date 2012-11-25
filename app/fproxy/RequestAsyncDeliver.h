//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2012-03-02 23:16:11

#ifndef __FX_REQUESTASYNCDELIVER_H
#define __FX_REQUESTASYNCDELIVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/thread/Runnable.h"
#include "firtex/thread/ThreadPool.h"

FX_NS_DEF(app);

template<typename T>
class RequestAsyncDeliver
{
public:
    typedef T ReqType;

    const static size_t DEFAULT_MIN_THREAD_COUNT = 16;
    const static size_t DEFAULT_MAX_THREAD_COUNT = 20;
    const static size_t DEFAULT_QUEUE_SIZE = 20;

public:
    RequestAsyncDeliver();
    ~RequestAsyncDeliver();

public:
    /// Start deliver
    void start();
    
    /// Start deliver
    void stop();

    /// Deliver one request
    void deliver(ReqType* pReq);
    
    /// Wait until all requests delivered
    void waitFinish();

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<ReqType*> ReqQueue;
    
    class Task : public FX_NS(thread)::Runnable
    {
    public:
        Task(RequestAsyncDeliver& deliver, ReqQueue& queue);
        ~Task() {}

    public:
        /**
         * Fetch request and deliver
         */
        void run();
    
    private:
        RequestAsyncDeliver& m_deliver;
        ReqQueue& m_queue;

    private:
        DECLARE_LOGGER();
    };
    DEFINE_TYPED_PTR(Task);

protected:
    void delivered();

private:
    ReqQueue m_queue;
    FX_NS(thread)::ThreadPool m_threadPool;
    std::vector<TaskPtr> m_tasks;

    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(thread)::Condition m_cond;
    int32_t m_nDeliverCount;

private:
    DECLARE_LOGGER();
};

//////////////////////////////////
SETUP_LOGGER_TEMP(app, RequestAsyncDeliver);

template<typename T>
RequestAsyncDeliver<T>::RequestAsyncDeliver() 
    : m_queue(DEFAULT_QUEUE_SIZE)
    , m_threadPool(DEFAULT_MIN_THREAD_COUNT, DEFAULT_MAX_THREAD_COUNT)
    , m_nDeliverCount(0)
{
}

template<typename T>
RequestAsyncDeliver<T>::~RequestAsyncDeliver() 
{
}

template<typename T>
void RequestAsyncDeliver<T>::start()
{
    for (size_t i = 0; i < m_threadPool.capacity(); i++)
    {
        TaskPtr pTask = new Task(*this, m_queue);
        this->m_tasks.push_back(pTask);
        this->m_threadPool.start(*pTask);
    }
}

template<typename T>
void RequestAsyncDeliver<T>::stop()
{
    for (size_t i = 0; i < m_tasks.size(); i++)
    {
        this->m_queue.enqueue(NULL);
    }

    this->m_queue.waitEmpty();
    this->m_threadPool.joinAll();
    this->m_tasks.clear();
}

template<typename T>
inline void RequestAsyncDeliver<T>::delivered()
{
    FX_NS(thread)::FastMutex::Guard g(m_mutex);
    --m_nDeliverCount;
    if (m_nDeliverCount <= 0)
    {
        m_cond.broadcast();
    }
}

template<typename T>
RequestAsyncDeliver<T>::Task::Task(RequestAsyncDeliver& deliver, ReqQueue& queue)
    : m_deliver(deliver)
    , m_queue(queue)
{
}

template<typename T>
void RequestAsyncDeliver<T>::Task::run()
{
    while (true)
    {
        ReqType* pReq = this->m_queue.waitDequeue();
        if (pReq)
        {
            pReq->run();
            m_deliver.delivered();
        }
        else
        {
            break;
        }
    }
}

template<typename T>
void RequestAsyncDeliver<T>::deliver(ReqType* pReq)
{
    {
        FX_NS(thread)::FastMutex::Guard g(m_mutex);
        ++m_nDeliverCount;
    }
    this->m_queue.enqueue(pReq);
}

template<typename T>
void RequestAsyncDeliver<T>::waitFinish()
{
    FX_NS(thread)::FastMutex::Guard g(m_mutex);
    if (m_nDeliverCount > 0)
    {
        m_cond.wait(m_mutex);
    }
}

FX_NS_END

#endif //__FX_REQUESTASYNCDELIVER_H
