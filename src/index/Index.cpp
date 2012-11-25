 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/10/11
//
#include <memory>
#include "firtex/index/Index.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/CommitList.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/store/FileSystemFactory.h"
#include "firtex/config/XMLConfigurator.h"

FX_NS_USE(config);
FX_NS_USE(analyzer);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(store);
FX_NS_USE(thread);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, Index);

Index::Index()
    : m_accessMode(WRITE)
{				
}

Index::~Index()
{
    close();
}

void Index::open(const std::string& sIndexPath, AccessMode am,
                 const DocumentSchema* pDocSchema)
{
    std::string sFs = GLOBAL_CONF().Storage.filesystem;
    FileSystemPtr pFileSys = FileSystemFactory::instance()->createFileSystem(sFs);
    if (pFileSys.isNull())
    {
        FIRTEX_THROW(InvalidConfigException, "Create file system: [%s] FAILED", sFs.c_str());
    }

    FileSystem::OpenMode om = FileSystem::READ;
    switch (am)
    {
    case READ:
        om = FileSystem::READ;
        break;        
    case WRITE:
        om = FileSystem::CREATE;
        break;
    case APPEND:
    case RDWR:
        om = FileSystem::APPEND;
        break;
    }
    pFileSys->open(sIndexPath, om);

    if ((am == APPEND) && (!pFileSys->fileExists(SCHEMA_FILENAME)))
    {
        am = WRITE;
    }
    open(pFileSys, am, pDocSchema);
}

void Index::open(FileSystemPtr& pFileSys, AccessMode am,
                 const DocumentSchema* pDocSchema)
{
    FIRTEX_ASSERT2(m_pFileSys.isNull());
    m_pFileSys = pFileSys;
    m_accessMode = am;
	
    BarrelsInfoPtr pBarrelsInfo(new BarrelsInfo());
    pBarrelsInfo->read(pFileSys);//read barrels Info

    if (am == WRITE)
    {
        if (!pDocSchema)
        {
            FIRTEX_THROW(InvalidConfigException, "Schema is empty in write mode.");
        }

        m_pDocSchema = new DocumentSchema(*pDocSchema);
        writeSchema(m_pDocSchema, pFileSys);

        pBarrelsInfo->remove(m_pFileSys);

        m_pComponentBuilder = new ComponentBuilder();
        m_pComponentBuilder->init(m_pDocSchema);
        initAnalyzerMapper();

        m_pIndexBarrelKeeper = new IndexBarrelKeeper(m_pFileSys, m_pDocSchema.get(),
                m_pComponentBuilder.get(), m_pAnalyzerMapper.get());
        m_pIndexBarrelKeeper->init(pBarrelsInfo, IndexBarrelKeeper::WRITE);

        openWriter();
    }
    else // READ, APPEND or RDWR mode
    {
        if (pBarrelsInfo->getIndexVersion() != FX_INDEX_VERSION)
        {
            FIRTEX_THROW(VersionException, "Incompatible index version.");
        }
        if (pBarrelsInfo->getBarrelCount() > 0)
        {
            DocumentSchemaPtr pSchemaExist = readSchema(m_pFileSys);
            if (pSchemaExist.isNull())
            {
                FIRTEX_THROW(IndexCollapseException, "Read schema FAILED.");
            }
            if (pDocSchema && !pSchemaExist->isEqual(*pDocSchema))
            {
                FIRTEX_THROW(IllegalArgumentException,
                        "The given document schema is not equal to the existing schema.");
            }
            m_pDocSchema = pSchemaExist;
        }
        else
        {
            if (!pDocSchema)
            {
                FIRTEX_THROW(IllegalArgumentException,
                        "No document schema is specified.");
            }
            m_pDocSchema = new DocumentSchema(*pDocSchema);
        }

        m_pComponentBuilder = new ComponentBuilder();
        m_pComponentBuilder->init(m_pDocSchema);
        initAnalyzerMapper();

        m_pIndexBarrelKeeper = new IndexBarrelKeeper(m_pFileSys, m_pDocSchema.get(),
                m_pComponentBuilder.get(), m_pAnalyzerMapper.get());

        if (am == READ || am == RDWR)
        {
            m_pIndexBarrelKeeper->init(pBarrelsInfo, (am == READ) ?
                    IndexBarrelKeeper::READ : IndexBarrelKeeper::RDWR);

            openReader();
            if (am == RDWR)
            {
                openWriter();
            }
        }
        else if (am == APPEND)
        {
            m_pIndexBarrelKeeper->init(pBarrelsInfo, IndexBarrelKeeper::WRITE);
            openWriter();
        }
    }
}

//static
DocumentSchemaPtr Index::readSchema(FileSystemPtr& pFileSys)
{
    XMLConfigurator configurator;
    configurator.load(SCHEMA_FILENAME, pFileSys);

    DocumentSchemaPtr pDocSchema = new DocumentSchema();
    pDocSchema->configure(configurator);
    return pDocSchema;
}

//static
void Index::writeSchema(DocumentSchemaPtr& pDocSchema,
                        FileSystemPtr& pFileSys)
{
    XMLConfigurator configurator(Configurator::TO_CONF);
    pDocSchema->configure(configurator);
    configurator.save(SCHEMA_FILENAME, pFileSys);
}

//static
void Index::remove(FileSystemPtr& pFileSys)
{
    BarrelsInfoPtr barrelsInfoPtr(new BarrelsInfo());
    try
    {
        barrelsInfoPtr->read(pFileSys); //read barrels Info
        barrelsInfoPtr->remove(pFileSys);
    }
    catch(const IndexCollapseException& e)
    {
        FX_LOG(ERROR, "Remove index FAILED: [%s]", e.what().c_str());
        FIRTEX_RETHROW(e);
    }
}

void Index::remove(const std::string& sIndexPath)
{
    std::string sFs =GLOBAL_CONF().Storage.filesystem;
    FileSystemPtr pFileSys = FileSystemFactory::instance()->createFileSystem(sFs);
    if (pFileSys.isNull())
    {
        FIRTEX_THROW(InvalidConfigException, "Create file system: [%s] FAILED", sFs.c_str());
    }
    pFileSys->open(sIndexPath, FileSystem::CREATE);
    remove(pFileSys);
}

//static
bool Index::exists(const std::string& sIndexPath)
{
    Path p(sIndexPath);
    p.makeDirectory();
    p.setFileName(SCHEMA_FILENAME);
    
    File f(p);
    return f.exists();
}

void Index::close()
{			
    if (m_pReader.isNotNull())
    {
        m_pReader->close();
        m_pReader.reset();
    }

    if (m_pWriter.isNotNull())
    {
        m_pWriter->close();
        m_pWriter.reset();
    }

    m_pDeletedDocFilter.reset();

    m_pComponentBuilder.reset();
    m_pAnalyzerMapper.reset();

    m_pIndexBarrelKeeper.reset();

    if (m_pFileSys.isNotNull())
    {
        m_pFileSys->close();
        m_pFileSys.reset();
    }
}

void Index::openReader()
{
    m_pReader.assign(new IndexReader());
    m_pReader->open(m_pIndexBarrelKeeper);
}

IndexReaderPtr Index::acquireReader(bool bRefresh)
{
    if ((m_accessMode != READ) && (m_accessMode != RDWR))
    {
        FX_LOG(INFO, _T("The access mode is WRITE or APPEND, "
                        "can't aquire reader of the index"));
        return IndexReaderPtr();
    }

    if (bRefresh)
    {
        CommitList commits;
        commits.load(m_pFileSys);
        commitid_t lastCommit = commits.getLastCommit();
        commitid_t curCommit = m_pReader->getBarrelsInfo()->getCommitId();
        if (lastCommit > curCommit)
        {
            m_pIndexBarrelKeeper->refresh();

            IndexReaderPtr pNewReader(m_pReader->clone());
            pNewReader->reopen();
            
            ScopedRWLock lock(m_lock, true);
            m_pReader = pNewReader;
            pNewReader.reset();

            if (m_pIndexBarrelKeeper->getHeldCommitCount() >= 2)
            {
                m_pIndexBarrelKeeper->housekeep();
            }

            return m_pReader;
        }
        if (lastCommit < curCommit)
        {
            FIRTEX_THROW(IndexCollapseException, "Invalid commit id: "
                            "current commit: [%d], last comit: [%d]", 
                         curCommit, lastCommit);
        }
    }

    ScopedRWLock lock(m_lock, false);
    return m_pReader;
}

IndexWriterPtr Index::acquireWriter()
{
    ScopedRWLock lock(m_lock, false);
    if (m_accessMode == READ)
    {
        FX_LOG(INFO, "The index is in READ mode, "
               "can't aquire writer of the index");
        return IndexWriterPtr();
    }
    return m_pWriter;
}

void Index::openWriter()
{
    m_pWriter.assign(new IndexWriter());
    m_pWriter->open(m_pIndexBarrelKeeper);
}

const ComponentBuilder* Index::getComponentBuilder() const
{
    return m_pComponentBuilder.get();
}

void Index::initAnalyzerMapper()
{
    //init analyzer mapper
    m_pAnalyzerMapper = new AnalyzerMapper();
    m_pAnalyzerMapper->init(m_pDocSchema.get());
}

FX_NS_END

