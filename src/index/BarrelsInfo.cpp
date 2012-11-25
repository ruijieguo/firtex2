//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/12/27
//

#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/CommitList.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/Field.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/XMLDocumentWrapper.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/config/XMLConfigurator.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <assert.h>

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(store);
FX_NS_USE(document);
FX_NS_USE(config);
FX_NS_USE(thread);

FX_NS_DEF(index);

//////////////////////////////////////////////////////////////////////////
///BarrelInfo
void BarrelInfo::remove(FileSystemPtr& pFileSys)
{
    string sRev = BARREL_DIRECTORY_PREFIX;
    NumberFormatter::append(sRev, m_commitId);
    pFileSys->deleteDirectory(sRev, true);
}

string BarrelInfo::getSuffix() const
{
    return getSuffix(m_commitId, m_state);
}

std::string BarrelInfo::getSuffix(commitid_t commId, State s)
{
    string str;
    switch (s)
    {
    case BUILDING:
        str = BARREL_BUILDING_STATE_SUFFIX;
        break;
    case MERGING:
        str = BARREL_MERGING_STATE_SUFFIX;
        break;
    case ONDISK:
        break;
    }

    str += BARREL_DIRECTORY_PREFIX;
    NumberFormatter::append(str, commId);
    return str;
}

//////////////////////////////////////////////////////////////////////////
//
SETUP_STREAM_LOGGER(index, BarrelsInfo);

FastMutex BarrelsInfo::COMMIT_FILE_LOCK;

BarrelsInfo::BarrelsInfo()
    : m_commitId(INVALID_COMMIT)
    , m_sIndexVersion(FX_INDEX_VERSION)
{
}

BarrelsInfo::BarrelsInfo(const BarrelsInfo& src)
{
    for (BarrelVector::const_iterator iter = src.m_barrelInfos.begin();
         iter != src.m_barrelInfos.end(); ++iter)
    {
        BarrelInfoPtr pBarrelInfo(new BarrelInfo(*(*iter)));
        m_barrelInfos.push_back(pBarrelInfo);
    }

    m_commitId = src.m_commitId;
    m_sIndexVersion = src.m_sIndexVersion;
    m_sEncoding = src.m_sEncoding;
}

BarrelsInfo::~BarrelsInfo(void)
{
    clear();
}

BarrelsInfo* BarrelsInfo::clone() const
{
    return new BarrelsInfo(*this);
}

void BarrelsInfo::clear()
{
    m_barrelInfos.clear();
    m_commitId = INVALID_COMMIT;
}

#define  ASSERT_EXIST(node, tag)                                        \
    if (node.isNull())                                                  \
    {                                                                   \
        FIRTEX_THROW(IndexCollapseException,                            \
                     "Missing "tag" tag in barrels info file.");        \
    }

void BarrelsInfo::read(FileSystemPtr& pFileSys)
{
    clear();
    CommitList commits;
    commits.load(pFileSys);
    commitid_t lastCommit = commits.getLastCommit();
    if (lastCommit != INVALID_COMMIT)
    {
        read(pFileSys, lastCommit);
    }
}

void BarrelsInfo::read(FileSystemPtr& pFileSys, commitid_t commitId)
{
    string sBarrelName = BARREL_COMMIT_FILENAME_PREFIX;
    NumberFormatter::append(sBarrelName, commitId);
    if (!pFileSys->fileExists(sBarrelName))
    {
        FIRTEX_THROW(IndexCollapseException, "Barrel file: [%s] not exist",
                     sBarrelName.c_str());
    }

    m_commitId = commitId;

    InputStreamPtr pInStream = pFileSys->openFile(sBarrelName);
    string str;
    str.resize((size_t)pInStream->getSize());
    pInStream->read((void*)str.c_str(), str.length());

    XMLDocumentWrapper xmlDoc;
    try 
    {
        xmlDoc.parse(str);
    }
    catch(const BadXmlFormatException& e)
    {
        FIRTEX_THROW(IndexCollapseException, "Read barrels info FAILED: [%s]",
                     e.what().c_str());
    }
    /// find <database> node
    XMLNodeWrapperPtr pDbNode = xmlDoc.firstNode("database");
    ASSERT_EXIST(pDbNode, "<database>");

    ///get <index_version></index_version> element
    XMLNodeWrapperPtr pNode = pDbNode->firstNode("index_version");
    ASSERT_EXIST(pNode, "<index_version>");
    m_sIndexVersion = pNode->getValue();

    ///get <encoding></encoding> element
    pNode = pDbNode->firstNode("encoding");
    if (pNode.isNotNull())
    {
        m_sEncoding = pNode->getValue();
    }

    ///get <barrel_count></barrel_count> element
    pNode = pDbNode->firstNode("barrel_count");
    ASSERT_EXIST(pNode, "<barrel_count>");

    XMLNodeWrapperPtr pBarrelsNode = pDbNode->firstNode("barrels");
    ASSERT_EXIST(pBarrelsNode, "<barrels>");

    ///read barrel information
    for (XMLNodeWrapperPtr pBarrelNode = pBarrelsNode->firstNode();
         pBarrelNode.isNotNull(); pBarrelNode = pBarrelNode->nextSibling())
    {
        BarrelInfoPtr pBarrelInfo = new BarrelInfo();
                
        ///get <suffix></suffix> element
        XMLNodeWrapperPtr pChild = pBarrelNode->firstNode("suffix");
        ASSERT_EXIST(pChild, "<suffix>");

        pBarrelInfo->m_commitId = (commitid_t)NumberParser::parseInt32(
                pChild->getValue() + 1);

        ///get <base_docid></base_docid> element
        pChild = pBarrelNode->firstNode("base_docid");
        ASSERT_EXIST(pChild, "<base_docid>");
        pBarrelInfo->m_baseDocId = NumberParser::parseInt32(pChild->getValue());

        ///get <doc_count></doc_count> element
        pChild = pBarrelNode->firstNode("doc_count");
        ASSERT_EXIST(pChild, "<doc_count>");
        pBarrelInfo->m_nDocCount = NumberParser::parseInt32(pChild->getValue());

        ///get <deletion_count></deletion_count> element
        pChild = pBarrelNode->firstNode("deletion_count");
        ASSERT_EXIST(pChild, "<deletion_count>");
        pBarrelInfo->m_nDeletedDocCount = NumberParser::parseInt32(pChild->getValue());

        ///read field meta information
        XMLNodeWrapperPtr pIndexMetaNode = pBarrelNode->firstNode("index_meta");
        ASSERT_EXIST(pIndexMetaNode, "<index_meta>");
        IndexMeta& indexMeta = pBarrelInfo->getIndexMeta();
        for (XMLNodeWrapperPtr pFieldMetaNode = pIndexMetaNode->firstNode();
             pFieldMetaNode.isNotNull(); pFieldMetaNode = pFieldMetaNode->nextSibling())
        {
            ///get <suffix></suffix> element
            XMLNodeWrapperPtr pChild = pFieldMetaNode->firstNode("field");
            ASSERT_EXIST(pChild, "<field>");
                
            FieldMeta& fieldMeta = indexMeta.fieldMeta(pChild->getValue());

            ///get <total_term_count></total_term_count> element
            pChild = pFieldMetaNode->firstNode("total_term_count");
            ASSERT_EXIST(pChild, "<total_term_count>");
            fieldMeta.totalTermCount = NumberParser::parseUInt64(pChild->getValue());

            ///get <distinct_term_count></distinct_term_count> element
            pChild = pFieldMetaNode->firstNode("distinct_term_count");
            ASSERT_EXIST(pChild, "<distinct_term_count>");
            fieldMeta.distinctTermCount = NumberParser::parseUInt64(pChild->getValue());
        }

        m_barrelInfos.push_back(pBarrelInfo);
    }
}

#undef  ASSERT_EXIST

void BarrelsInfo::write(FileSystemPtr& pFileSys)
{
    string str;
    XMLDocumentWrapper xmlDoc;
    XMLNodeWrapperPtr pDbNode = xmlDoc.appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "database");

    ///add index version element
    pDbNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT, 
                        "index_version", m_sIndexVersion);

    pDbNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT, 
                        "encoding", m_sEncoding);

    ///add barrel count element
    str.clear();
    NumberFormatter::append(str, (int32_t)m_barrelInfos.size());
    pDbNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "barrel_count", str);

    ///add barrels elements
    XMLNodeWrapperPtr pBarrelsNode = pDbNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "barrels");
    BarrelVector::const_iterator iter = m_barrelInfos.begin();
    while (iter != m_barrelInfos.end())
    {
        BarrelInfoPtr pBarrelInfo = *iter;
        XMLNodeWrapperPtr pBarrelNode = pBarrelsNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "barrel");

        ///add <suffix></suffix>
        str = BARREL_DIRECTORY_PREFIX;
        NumberFormatter::append(str, pBarrelInfo->m_commitId);
        pBarrelNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT, 
                                "suffix", str);

        ///add <base_docid></base_docid>
        str.clear();
        NumberFormatter::append(str,pBarrelInfo->m_baseDocId);
        pBarrelNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                                "base_docid", str);

        ///add <doc_count></doc_count>
        str.clear();
        NumberFormatter::append(str, pBarrelInfo->m_nDocCount);
        pBarrelNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT, 
                                "doc_count", str);

        ///add <deletion_count></deletion_count>
        str.clear();
        NumberFormatter::append(str, pBarrelInfo->m_nDeletedDocCount);
        pBarrelNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                                "deletion_count", str);

        ///add index meta element
        XMLNodeWrapperPtr pIndexMetaNode = pBarrelNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "index_meta");
        const IndexMeta& indexMeta = pBarrelInfo->getIndexMeta();
        for (size_t i = 0; i < indexMeta.size(); ++i)
        {
            const FieldMeta& fieldMeta = indexMeta[i];
            XMLNodeWrapperPtr pFieldMetaNode = pIndexMetaNode->appendNode(
                    XMLDocumentWrapper::NODE_ELEMENT, "field_meta");
            pFieldMetaNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                    "field", fieldMeta.fieldName);

            string str;
            NumberFormatter::append(str, fieldMeta.totalTermCount);
            pFieldMetaNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                    "total_term_count", str);

            str.clear();
            NumberFormatter::append(str, fieldMeta.distinctTermCount);
            pFieldMetaNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                    "distinct_term_count", str);
        }

        iter ++;
    }

    string sBarrelFileName = BARREL_COMMIT_FILENAME_PREFIX;
    NumberFormatter::append(sBarrelFileName, m_commitId);
    OutputStreamPtr pOutStream = pFileSys->createFile(sBarrelFileName);
    xmlDoc.addDeclarationNode("UTF-8");
    str.clear();
    xmlDoc.print(str);

    pOutStream->write((const void*)str.c_str(), str.length());
    pOutStream->close();
}

void BarrelsInfo::remove(FileSystemPtr& pFileSys)
{
    for (BarrelVector::iterator iter = m_barrelInfos.begin();
         iter != m_barrelInfos.end(); ++iter)
    {
        (*iter)->remove(pFileSys);
    }

    FileSystem::FileSet files = pFileSys->listFiles();
    for (FileSystem::FileSet::const_iterator it = files.begin();
         it != files.end(); ++it)
    {
        string sFileName = *it;
        if (!strCompare(sFileName.c_str(), BARREL_COMMIT_FILENAME_PREFIX,
                        BARREL_COMMIT_FILENAME_PREFIX_LENGTH))
        {
            pFileSys->deleteDirectory(sFileName);
        }
    }

    clear();
}

BarrelInfo& BarrelsInfo::newBarrel()
{
    docid_t baseDocId = 0;
    if (getBarrelCount() > 0)
    {
        const BarrelInfo& lastBarrel = getLastBarrel();
        baseDocId = lastBarrel.getBaseDocId() + lastBarrel.getDocCount();
    }
    BarrelInfoPtr pNewBarrel(new BarrelInfo(++m_commitId, baseDocId, 0, 0));
    m_barrelInfos.push_back(pNewBarrel);
    return *(pNewBarrel.get());
}

void BarrelsInfo::addBarrel(const BarrelInfo& barrelInfo)
{
    BarrelInfoPtr pNewBarrel(new BarrelInfo(barrelInfo));
    m_barrelInfos.push_back(pNewBarrel);
    if (pNewBarrel->getCommitId() > m_commitId)
    {
        m_commitId = pNewBarrel->getCommitId();
    }

    sort(m_barrelInfos.begin(), m_barrelInfos.end(), BarrelComparator());
}

df_t BarrelsInfo::getDocCount()const
{
    df_t count = 0;
    BarrelVector::const_iterator iter = m_barrelInfos.begin();
    while (iter != m_barrelInfos.end()) 
    {
        count += (*iter)->getDocCount();
        iter++;
    }			
    return count;
}

df_t BarrelsInfo::getDeletedDocCount()const
{
    int32_t count = 0;
    BarrelVector::const_iterator iter = m_barrelInfos.begin();
    while (iter != m_barrelInfos.end()) 
    {
        count += (*iter)->getDeletedDocCount();
        iter++;
    }
    return count;
}

BarrelsInfo::Iterator BarrelsInfo::iterator() const
{
    return BarrelsInfo::Iterator(this);
}

const BarrelInfo* BarrelsInfo::getBarrel(commitid_t comId) const
{
    for (BarrelVector::const_iterator iter = m_barrelInfos.begin();
         iter != m_barrelInfos.end(); ++iter)
    {
        if (comId == (*iter)->getCommitId())
        {
            return (*iter).get();
        }
    }
    return NULL;
}

bool BarrelsInfo::hasBarrel(commitid_t comId) const
{
    for (BarrelVector::const_iterator iter = m_barrelInfos.begin();
         iter != m_barrelInfos.end(); ++iter)
    {
        if (comId == (*iter)->getCommitId())
        {
            return true;
        }
    }
    return false;
}

FX_NS_END

