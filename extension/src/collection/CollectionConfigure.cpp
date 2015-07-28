#include "firtex/extension/collection/CollectionConfigure.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/common/Any.h"

FX_NS_USE(document);
FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(processor);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, CollectionConfigure);

CollectionConfigure::CollectionConfigure()
{
}

CollectionConfigure::CollectionConfigure(const CollectionConfigure& src)
{
    Collection.name = src.Collection.name;
    Collection.identifier = src.Collection.identifier;
    Collection.encoding = src.Collection.encoding;
    Collection.parameter = src.Collection.parameter;

    Collection.inputDocQueueSize = src.Collection.inputDocQueueSize;
    Collection.processThreadCount = src.Collection.processThreadCount;

    if (src.m_pTemplate)
    {
        m_pTemplate.reset(new DocumentTemplate(*(src.m_pTemplate)));
    }
    m_pDocSchema.reset(new DocumentSchema(*src.m_pDocSchema));
}

CollectionConfigure::~CollectionConfigure(void)
{
}

CollectionConfigure* CollectionConfigure::clone() const
{
    return new CollectionConfigure(*this);
}

void CollectionConfigure::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator colConf;        
        colConf.configure("name", Collection.name);
        colConf.configure("identifier", Collection.identifier);

        colConf.configure("source", Collection.source);
        colConf.configure("target", Collection.target);
        colConf.configure("build_mode", Collection.buildMode);

        if (Collection.autoCommit.maxDocs != 0 || Collection.autoCommit.maxTime != 0)
        {
            Configurator autoCommitConf;
            if (Collection.autoCommit.maxDocs != 0)
            {
                autoCommitConf.configure("max_docs", Collection.autoCommit.maxDocs);
            }
            if (Collection.autoCommit.maxTime != 0)
            {
                autoCommitConf.configure("max_time", Collection.autoCommit.maxTime);
            }
            colConf.configure("auto_commit", autoCommitConf.getMap());
        }

        colConf.configure("encoding", Collection.encoding);
        colConf.configure("doc_types", Collection.docTypes);
        colConf.configure("parameter", Collection.parameter);
        colConf.configure("document_queue_size", Collection.inputDocQueueSize);
        colConf.configure("process_thread_count", Collection.processThreadCount);

        if (m_pTemplate)
        {
            m_pTemplate->configure(conf);
        }
        conf.configure("collection", colConf.getMap());
        
        if (m_pDocSchema)
        {
            m_pDocSchema->configure(conf);
        }
    }
    else 
    {
        GLOBAL_CONF().configure(conf);

        Configurator::ConfMap colConfMap;
        conf.configure("collection", colConfMap);

        Configurator colConf(colConfMap);

        colConf.configure("name", Collection.name, "");
        colConf.configure("identifier", Collection.identifier);

        colConf.configure("source", Collection.source);
        colConf.configure("target", Collection.target);
        colConf.configure("build_mode", Collection.buildMode);

        Configurator::Iterator autoCommitIt = colConf.findConf("auto_commit");
        if (autoCommitIt.hasNext())
        {
            Configurator::ConfMap acConfMap =
                AnyCast<Configurator::ConfMap>(autoCommitIt.next().second);
            Configurator c(acConfMap);
            c.configure("max_docs", Collection.autoCommit.maxDocs, 0);
            c.configure("max_time", Collection.autoCommit.maxTime, 0);
        }

        colConf.configure("encoding", Collection.encoding, "");
        colConf.configure("doc_types", Collection.docTypes, "");
        colConf.configure("parameter", Collection.parameter, "");

        colConf.configure("document_queue_size", Collection.inputDocQueueSize,
                          (uint32_t)DEFAULT_DOCUMENT_QUEUE_SIZE);
        colConf.configure("process_thread_count", Collection.processThreadCount,
                          (uint32_t)DEFAULT_PROCESS_THREAD_COUNT);

        Configurator::Iterator tempIt = colConf.findConf("template");
        if (tempIt.hasNext())
        {
            m_pTemplate.reset(new DocumentTemplate());
            m_pTemplate->configure(colConf);
        }

        Configurator::Iterator schemaIt = conf.findConf("index_schema");
        if (schemaIt.hasNext())
        {
            m_pDocSchema.reset(new DocumentSchema());
            m_pDocSchema->configure(conf);
        }
    }
}

FX_NS_END

