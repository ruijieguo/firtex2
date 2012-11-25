//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/5/4
//

#include "firtex/common/StdHeader.h"
#include "PDFDocReader.h"

#include "GString.h"
#include "GlobalParams.h"
#include "Object.h"
#include "Dict.h"
#include "PDFDoc.h"
#include "TextOutputDev.h"
#include "UnicodeMap.h"

FX_NS_DEF(plugin);

PDFDocReader::PDFDocReader(void)
  : m_pPDFDoc(NULL)
  , m_pTextOutDev(NULL)
  , m_pFileStream(NULL)
  , m_hFile(NULL)
  , m_buffer(NULL)
  , m_bufferSize(0)
  , m_bufferLength(0)
  , m_pMetadata(NULL)
{
#ifdef WIN32
    char szPath[512];
    GetModuleFileNameA( NULL, szPath, 512 );

    string sPath = szPath;
    if ( sPath.rfind( '\\' ) >= 0 )
        sPath = sPath.substr(0, sPath.rfind( '\\' ) );

    sPath += "\\data\\xpdf";
    string strCfg = sPath + "\\xpdfrc";
    globalParams = new GlobalParams((char*)strCfg.c_str());
#else
    const char* tmp = "";
    globalParams = new GlobalParams((char*)tmp);//TODO
#endif

    globalParams->setTextPageBreaks(gFalse);
    globalParams->setErrQuiet(gTrue);
}

PDFDocReader::PDFDocReader(const char* filename)
    : m_pPDFDoc(NULL)
    , m_pTextOutDev(NULL)
    , m_pFileStream(NULL)
    , m_hFile(NULL)
    , m_buffer(NULL)
    , m_bufferSize(0)
    , m_bufferLength(0)
    , m_pMetadata(NULL)
{
    const char* tmp = "";
    globalParams = new GlobalParams((char*)tmp);
    globalParams->setTextPageBreaks(gFalse);
    globalParams->setErrQuiet(gTrue);
    open(filename);
}

PDFDocReader::~PDFDocReader(void)
{
    if(globalParams)
    {
        delete globalParams;
        globalParams = NULL;
    }
    close();
}

void PDFDocReader::outputToBuffer(void *stream, char *text, int len) 
{
    PDFDocReader* pThis = (PDFDocReader*)stream;
    if( (pThis->m_bufferLength + len) > pThis->m_bufferSize)
    {
        pThis->m_bufferSize = pThis->m_bufferLength + len;
        char* tmpBuffer = new char[pThis->m_bufferSize];
        if(pThis->m_buffer)
        {
            memcpy(tmpBuffer, pThis->m_buffer, pThis->m_bufferLength);
            delete[] pThis->m_buffer;
        }
        pThis->m_buffer = tmpBuffer;
    }
    memcpy(pThis->m_buffer + pThis->m_bufferLength, text, (size_t)len);
    pThis->m_bufferLength += len;
}

bool PDFDocReader::open(const char* filename, bool bThrowError)
{
    return open(filename, false, false, false, bThrowError);
}

bool PDFDocReader::open(const char* filename, bool bMeta, bool bPhysLayout,
                        bool bRawOrder, bool bThrowError)
{
    close();

    GString *fileName = NULL;
    UnicodeMap *uMap = NULL;
    bool bRet = true;

    try
    {
        if (!(uMap = globalParams->getTextEncoding()))
        {
            /*if(bThrowError)
              FX_STREAM_LOG(ERROR) << fileName << ": Couldn't get text encoding" << FIRTEX_ENDL;
              else 
              FIRTEX_THROW3(FILEIO_ERROR, filename + "bool PDFDocReader::open():Couldn't get text encoding");*/
            return false;
        }

        fileName = new GString(filename);
        m_pPDFDoc = new PDFDoc(fileName, NULL, NULL);

        if (!m_pPDFDoc->isOk()) 
        {
            delete m_pPDFDoc;
            m_pPDFDoc = NULL;
            uMap->decRefCnt();
            uMap = NULL;
            /*if(bThrowError)
              FX_STREAM_LOG(ERROR) << "Open file error: " << filename << FIRTEX_ENDL;
              else 
              FIRTEX_THROW3(FILEIO_ERROR, "bool PDFDocReader::open():Open file error:" + filename);*/
            return false;
        }

        if (!m_pPDFDoc->okToCopy()) 
        {
            delete m_pPDFDoc;
            m_pPDFDoc = NULL;
            uMap->decRefCnt();
            uMap = NULL;
            /*if(bThrowError)
              FX_STREAM_LOG(ERROR) << filename << "Copying of text from this document is not allowed." << FIRTEX_ENDL;
              else 
              FIRTEX_THROW2(FILEIO_ERROR, "bool PDFDocReader::open():Copying of text from this document is not allowed:" + filename);*/
            return false;
        }

        if(bMeta)
        {
            m_pMetadata = new PDFMetadata;
            Object info;
            m_pPDFDoc->getDocInfo(&info);
            if (info.isDict()) 
            {
                getInfoString(m_pMetadata->m_sTitle, info.getDict(), "Title", uMap);
                getInfoString(m_pMetadata->m_sSubject, info.getDict(), "Subject", uMap);
                getInfoString(m_pMetadata->m_sKeywords, info.getDict(), "Keywords", uMap);
                getInfoString(m_pMetadata->m_sAuthor, info.getDict(), "Author", uMap);
                getInfoString(m_pMetadata->m_sCreator, info.getDict(), "Creator", uMap);
                getInfoString(m_pMetadata->m_sProducer, info.getDict(), "Producer", uMap);
                getInfoString(m_pMetadata->m_sCreationDate, info.getDict(), "CreationDate", uMap);
                getInfoString(m_pMetadata->m_sLastModifiedDate, info.getDict(), "LastModifiedDate", uMap);
            }
            info.free();
        }

        // write text file
        m_pTextOutDev = new TextOutputDev(outputToBuffer, this, bPhysLayout?gTrue:gFalse, bRawOrder?gTrue:gFalse);
        if (!m_pTextOutDev->isOk()) 
        {
            delete m_pTextOutDev;
            m_pTextOutDev = NULL;
            bRet = false;

        }
        m_nCurPage = 0;
        uMap->decRefCnt();
        uMap = NULL;
    }
    catch (std::bad_alloc& )
    {
        if(uMap)
            uMap->decRefCnt();
        /*if(bThrowError)
          FIRTEX_THROW2(OUTOFMEM_ERROR, ":bool PDFDocReader::open().");
          else FX_STREAM_LOG(ERROR) << "Out of Memory:bool PDFDocReader::open(): " << filename << FIRTEX_ENDL;*/
    }
    catch (...) 
    {
        if(uMap)
            uMap->decRefCnt();
        /*if(bThrowError)
          FIRTEX_THROW2(UNKNOWN_ERROR, ":bool PDFDocReader::open().");
          else FX_STREAM_LOG(ERROR) << "bool PDFDocReader::open(): unknown error." << FIRTEX_ENDL;*/
    }

    return bRet;
}

void PDFDocReader::close()
{
    if(m_pTextOutDev)
    {
        delete m_pTextOutDev;
        m_pTextOutDev = NULL;
    }
    if(m_pPDFDoc)
    {
        delete m_pPDFDoc;
        m_pPDFDoc = NULL;
    }
    if(m_pMetadata)
    {
        delete m_pMetadata;
        m_pMetadata = NULL;
    }
    if(m_buffer)
    {
        delete m_buffer;
        m_buffer = NULL;
    }
    if(m_pFileStream)
    {
        m_pFileStream = NULL;
    }

    if(m_hFile)
    {
        fclose(m_hFile);
        m_hFile = NULL;
    }
    m_bufferSize = 0;
    m_bufferLength = 0;
}

char* PDFDocReader::getText(size_t& length)
{
    if(m_buffer)
    {
        length = m_bufferLength;
        return m_buffer;
    }
    int nStartPage = 1;
    int nEndPage = 100;
    if(nEndPage > m_pPDFDoc->getNumPages())
        nEndPage = m_pPDFDoc->getNumPages();
    m_pPDFDoc->displayPages(m_pTextOutDev, nStartPage, nEndPage,
                            72, 72, 0, gFalse, gTrue, gFalse);
    delete m_pPDFDoc;
    m_pPDFDoc = NULL;

    length = m_bufferLength;
    return m_buffer;
}
void PDFDocReader::getInfoString(string& sInfo, Dict *infoDict,
                                 const char *key, UnicodeMap *uMap) 
{
    Object obj;
    GString *s1;
    GBool isUnicode;
    Unicode u;
    char buf[8];
    int i, n;
    
    if (infoDict->lookup((char*)key, &obj)->isString()) 
    {
        s1 = obj.getString();
        if ((s1->getChar(0) & 0xff) == 0xfe &&
            (s1->getChar(1) & 0xff) == 0xff) 
        {
            isUnicode = gTrue;
            i = 2;
        }
        else 
        {
            isUnicode = gFalse;
            i = 0;
        }
        while (i < obj.getString()->getLength()) 
        {
            if (isUnicode) 
            {
                u = ((s1->getChar(i) & 0xff) << 8) | (s1->getChar(i+1) & 0xff);
                i += 2;
            }
            else 
            {
                u = s1->getChar(i) & 0xff;
                ++i;
            }
            n = uMap->mapUnicode(u, buf, sizeof(buf));
            sInfo.append(buf, n);                        
        }
    }
    obj.free();
}

void PDFDocReader::getInfoDate(string& sInfo, Dict *infoDict, char *key) 
{
    Object obj;
    char *s;
    if (infoDict->lookup(key, &obj)->isString()) 
    {
        s = obj.getString()->getCString();
        if (s[0] == 'D' && s[1] == ':') 
        {
            s += 2;
        }
        sInfo.append(s);
    }
    obj.free();
}

FX_NS_END

