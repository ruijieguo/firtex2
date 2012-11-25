/*
 * Copyright (C) 2007 Guo Ruijie. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */
#ifndef __TEXTFILE_H
#define __TEXTFILE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Exception.h"

#if _MSC_VER == 1400
#pragma warning(push)
#pragma warning( disable : 4290)
#endif

FX_NS_DEF(utility)

/** 
 * This class provides an interface to read/write text file
 *
 * @class TextFile
 * @author Guo Ruijie
 * @date <2008-06-25> 
 */
class TextFile
{
public:	
    TextFile();	
    virtual ~TextFile();
	
    /**
     * is the file opened
     * @return true if opened
     */
    bool isFileOpen(void) const;

    /**
     * open a text file
     * @param sFileName text file name
     * @param sMode file open mode 
     */
    void open(const tstring& sFileName, const tstring& sMode) throw(FileIOException);

    /**
     * is reach to the end of file 
     * @return true for end of file 
     */
    bool isEof(void) throw(FileIOException);

    /**
     * get current read/write pos
     * @return current position
     */
    int32_t getPos(void) throw(FileIOException);


    /**
     * set current position
     * @param nPos position
     */
    void setPos(int32_t nPos) throw(FileIOException);

    /**
     * set file pointer to end of file
     */
    void setAppendPos(void) throw(FileIOException);

    /**
     * flush data to disk
     */
    void flush(void) throw(FileIOException);

    /**
     * get file name
     */
    tstring getFileName(void)const {return m_sFileName;}

    /**
     * get file size in byte
     * @return file size in byte
     */
    int32_t getSizeInBytes(void) throw(FileIOException);

    /**
     * read data 
     * @param pBuffer data buffer
     * @param nBufSize  size of data to read
     */	
    void read(char* pBuffer, size_t nBufSize) throw(FileIOException);

    /** 
     * read one line 
     * @param pBuffer line buffer
     * @param nMaxSize max size to read
     * @param actual read size, -1 for error
     * @param bRemoveNewLine whether to remove newline ("\n") or not
     */
    ssize_t readLine(char* pBuffer, size_t nMaxSize, bool bRemoveNewLine);

    /**
     * write data 
     * @param pBuffer data buffer
     * @param nSize max size to read
     */	
    void write(const char* pBuffer,size_t nSize) throw(FileIOException);

    /**
     * write data 
     * @param szString string buffer	 
     */	
    void write(const char* szString) throw(FileIOException);

    /**
     * write data 
     * @param szString string buffer	 
     * @param bNewLine as a new line
     */	
    void write(const char* szString, bool bNewLine) throw(FileIOException);
	
    /**
     * close file
     */
    void close(void) throw(FileIOException);

protected:
    tstring m_sFileName;
    FILE* m_pFile;

private:
    // Unsupported functions. Do not use.
    TextFile(const TextFile &r);
    TextFile& operator=(const TextFile &r);
    int operator==(const TextFile &r) const;
};

FX_NS_END

#if _MSC_VER == 1400
#pragma warning(pop)
#endif

#endif
