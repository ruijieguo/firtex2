//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: 郭瑞杰(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/5/4
//
#ifndef _PDFDOCREADER_H
#define _PDFDOCREADER_H

#include <string>
using namespace std;
class PDFDoc;
class TextOutputDev;
class Dict;
class UnicodeMap;
class BaseStream;

FX_NS_DEF(plugin)

class PDFMetadata
{
public:
    PDFMetadata(){}
    ~PDFMetadata(){}
public:
    string m_sTitle;
    string m_sSubject;
    string m_sKeywords;
    string m_sAuthor;
    string m_sCreator;
    string m_sProducer;
    string m_sCreationDate;
    string m_sLastModifiedDate;	
};

class PDFDocReader
{
public:
    PDFDocReader();
    PDFDocReader(const char* filename);
    virtual ~PDFDocReader(void);

public:
    bool open(const char* filename, bool bThrowError = false);
    bool open(const char* filename, bool bMeta, bool bPhysLayout = false, 
              bool bRawOrder = false, bool bThrowError = false);
    void close();

    char* getText(size_t& length);

    PDFMetadata* getMetaData(){return m_pMetadata;}

protected:
    void getInfoString(string& sInfo, Dict *infoDict, const char *key, UnicodeMap *uMap);
    void getInfoDate(string& sInfo, Dict *infoDict,  char *key);

protected:
    static void outputToBuffer(void *stream, char *text, int len);

protected:
    PDFDoc* m_pPDFDoc;
    TextOutputDev* m_pTextOutDev;

    BaseStream* m_pFileStream;
    FILE* m_hFile;

    int m_nCurPage;
    char* m_buffer;
    size_t m_bufferSize;  //buffer 总大小
    size_t m_bufferLength; //buffer当前使用空间大小

    PDFMetadata* m_pMetadata;
};

FX_NS_END

#endif
