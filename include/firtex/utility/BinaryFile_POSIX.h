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
#ifndef __BINARYFILE_POSIXIMPL_H
#define __BINARYFILE_POSIXIMPL_H

#include "firtex/utility/BinaryFile.h"

FX_NS_DEF(utility)

class BinaryFileImpl
{
public:
    BinaryFileImpl();		
    virtual ~BinaryFileImpl();	
	
    /**
     * open a binary file 
     * @param filename binary file to be open
     * @param mode open mode
     */
    virtual void open(const tstring &filename,BinaryFile::OpenMode mode) throw(FileIOException);

    /**
     * seek to absolute position in file
     * @param nPos position in file	 
     */
    void seek(int64_t nPos) throw(FileIOException);
	
    /**
     * seek to relative position in file
     * @param nRelPos relative position in file
     */
    void seekRelative(int64_t nRelPos) throw(FileIOException);
	
    /**
     * get current position
     * @return current read/write position
     */
    int64_t getPos(void) const throw(FileIOException);
	
    /**
     * read binary data from file
     * @param pData data read to
     * @param nBytes number of bytes to read
     * @return number of bytes read
     */
    size_t read(void* pData, size_t nBytes) throw(FileIOException);

    /**
     * read binary data from file, make sure /nBytes/ are read.
     * @param pData data read to
     * @param nBytes number of bytes to read
     */
    void readExact(void* pData, size_t nBytes) throw(FileIOException);
	
    /**
     * write binary data to file
     * @param pData data to write
     * @param nBytes number of bytes to write
     */
    void write(const void* pData,size_t nBytes) throw(FileIOException);
	
    /**
     * read data at specific position 
     * @param pData data read to
     * @param nBytes number of data
     * @param nPos position in file
     */
    void pread(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException);
	
    /**
     * write data to specific position 
     * @param pData data to write
     * @param nBytes number of bytes
     * @param nPos position in file
     */
    void pwrite(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException);
	
    /**
     * close file 
     */
    virtual void close(void) throw(FileIOException);

    /** 
     * flush data 
     */
    void flush(void) throw(FileIOException);

    /**
     * truncate file
     * @param 
     */
    void truncate(int64_t newsize) throw(FileIOException);
	
    /** 
     * file length
     */
    int64_t getLength(void) throw(FileIOException);

    /**
     * is file opened
     * @return true if file was opened
     */
    bool isFileOpen(void) const{return(m_handle != -1);}
	
    /**
     * get file open mode
     * @return file open mode
     */	
    BinaryFile::OpenMode getFileMode(void) const throw(FileIOException)
    {return m_fileMode;}
    	
    /**
     * get file name
     */
    tstring  getFileName(void) const{return m_sFileName;} 
private:
    // Unsupported functions. Do not use.
    BinaryFileImpl(const BinaryFileImpl &r);
    BinaryFileImpl& operator=(const BinaryFileImpl &r);
    int operator==(const BinaryFileImpl &r) const;

protected:	
    int m_handle;	// File handle for this file.	
    tstring m_sFileName;// File name (as given by caller).
    BinaryFile::OpenMode m_fileMode;	// File access mode	
};

FX_NS_END

#endif
