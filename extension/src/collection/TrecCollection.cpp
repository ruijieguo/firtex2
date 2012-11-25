#include <memory>
#include "firtex/extension/collection/TrecCollection.h"
#include "firtex/extension/processor/TrecDocumentProcessor.h"

FX_NS_USE(processor);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, TrecCollection);

TrecCollection::TrecCollection() 
{
}

TrecCollection::~TrecCollection() 
{
}

DocumentProcessor* TrecCollection::createDocumentProcessor()
{
    CollectionConfigurePtr pConfigure = getConfigure();

    TrecDocumentProcessor* pDocProcessor = new TrecDocumentProcessor();
    auto_ptr<TrecDocumentProcessor> ptr(pDocProcessor);

    pDocProcessor->setContentType(getConfigure()->Collection.docTypes);
    pDocProcessor->init(pConfigure->getDocSchema().get(), pConfigure->getTemplate().get());
    return ptr.release();
}

FX_NS_END

