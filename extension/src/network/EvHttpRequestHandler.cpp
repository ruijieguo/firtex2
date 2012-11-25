#include "firtex/extension/network/EvHttpRequestHandler.h"

FX_NS_USE(thread);
FX_NS_USE(utility);

FX_NS_DEF(network);

SETUP_LOGGER(network, EvHttpRequestHandler);

EvHttpRequestHandler::EvHttpRequestHandler(const EvHttpServiceFactoryPtr& pServiceFactory,
                                           size_t nWorkThreadCount)
    : m_pServiceFactory(pServiceFactory)
    , m_requests(MAX_REQUEST_QUEUE_SIZE)
{
    m_pLoopThreads = new ThreadPool(nWorkThreadCount, nWorkThreadCount);
    for (size_t i = 0; i < nWorkThreadCount; ++i)
    {
        m_pLoopThreads->start(*this);
    }
}

EvHttpRequestHandler::~EvHttpRequestHandler() 
{
}

void EvHttpRequestHandler::process(EvHttpRequestContext* pCtx)
{
    FIRTEX_ASSERT2(pCtx != NULL);
    m_requests.enqueue(pCtx);
}

void EvHttpRequestHandler::run()
{
    while (true)
    {
        EvHttpRequestContext* pCtx = m_requests.waitDequeue();
        if (pCtx)
        {
            EvHttpService* pService = m_pServiceFactory->findService(pCtx->getResource());
            if (pService)
            {
                FX_DEBUG("Found request handler for resource: [%s]",
                        pCtx->getResource().c_str());
                try
                {
                    pService->handleRequest(pCtx);
                }
                catch(const std::exception& e)
                {
                    FX_LOG(ERROR, "Run service FAILED for resource: [%s], error: [%s].",
                            pCtx->getResource().c_str(), e.what());
                    pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_INTERNAL_SERVER_ERROR);
                }
                catch(const FirteXException& e)
                {
                    FX_LOG(ERROR, "Run service FAILED for resource: [%s], error: [%s].",
                            pCtx->getResource().c_str(), e.what().c_str());
                    pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_INTERNAL_SERVER_ERROR);
                }
            }
            else
            {
                FX_DEBUG("No such resource: [%s]", pCtx->getResource().c_str());
                pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_NOT_FOUND);
            }
            FX_DEBUG("Request done: [%s]", pCtx->getResource().c_str());
            pCtx->complete();
        }
        else
        {
            FX_DEBUG("Find empty context");
            break;
        }
    }
    FX_DEBUG("Exit processing loop");
}

void EvHttpRequestHandler::stop()
{
    if (m_pLoopThreads.isNotNull())
    {
        for (size_t i = 0; i < m_pLoopThreads->numAllocatedThreads(); ++i)
        {
            FX_DEBUG("Enqueue empty context");
            m_requests.enqueue(NULL);
        }
        m_pLoopThreads->stopAll();
    }
}

void EvHttpRequestHandler::join()
{
    if (m_pLoopThreads.isNotNull())
    {
        m_pLoopThreads->joinAll();
    }
}

FX_NS_END

