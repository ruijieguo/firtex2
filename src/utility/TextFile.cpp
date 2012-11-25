/**
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
#include "firtex/utility/TextFile.h"
#include "firtex/utility/File.h"
#include <stdio.h>

#ifdef FX_WINDOWS
#include <io.h>
#endif
#if defined(FX_WINDOWS) || defined(HAVE_SYS_STAT_H)
#include <sys/stat.h>
#endif
#if defined(FX_WINDOWS) || defined(HAVE_FCNTL_H)
#include <fcntl.h>
#endif


FX_NS_DEF(utility);

TextFile::TextFile() 
    : m_pFile(NULL)
{
}

TextFile::~TextFile()
{	
    if (m_pFile)
        close();
}

bool TextFile::isFileOpen(void) const
{
    return(m_pFile != NULL);
}

void TextFile::open(const tstring& sFileName,const tstring& sMode) throw(FileIOException)
{
    if (m_pFile) throw FileIOException(sFileName);

    m_pFile = fopen(sFileName.c_str(), sMode.c_str());
    if (!m_pFile) throw FileIOException(sFileName);
    m_sFileName = sFileName;
}

bool TextFile::isEof(void) throw(FileIOException)
{	
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );	
	
    int eofval = feof(m_pFile);
    return(eofval != 0);
}

int32_t TextFile::getPos(void) throw(FileIOException)
{	
    int32_t nPos = 0;
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    long tmp = ftell(m_pFile);
    if (tmp == -1) throw FileIOException(_T("ftell failed."));
    nPos = tmp;
    return nPos;
}

void TextFile::setPos(int32_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    int nSeekResult;		
    nSeekResult = fseek(m_pFile,nPos,SEEK_SET);
    if (nSeekResult) throw FileIOException(_T("fseek failed."));
}

void TextFile::setAppendPos(void) throw(FileIOException)
{	
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    int nSeekResult;		
    nSeekResult = fseek(m_pFile,0,SEEK_END);
    if (nSeekResult) throw FileIOException(_T("fseek failed."));
}

void TextFile::flush(void) throw(FileIOException)
{	
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    int nFlushResult = fflush(m_pFile);
    if (nFlushResult) throw FileIOException(_T("ffush failed."));
}

int32_t TextFile::getSizeInBytes(void) throw(FileIOException)
{	
    File f(m_sFileName);
    return (int32_t)f.getSize();
}

void TextFile::read(char* pBuffer,size_t nBufSize) throw(FileIOException)
{	
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    FIRTEX_ASSERT2(pBuffer);
	
    size_t n = fread(pBuffer,1,nBufSize,m_pFile);	
    if (n != nBufSize )
        FIRTEX_THROW(FileIOException, _T("fread failed, please make sure the "
                        "file is opened in binary mode."));	
}

ssize_t TextFile::readLine(char* pBuffer, size_t nMaxSize, bool bRemoveNewLine)
{
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );	
    if (!fgets(pBuffer,(int)nMaxSize, m_pFile))
    {
        return (ssize_t)-1;
    }
    size_t slen = strlen(pBuffer);
    // Remove newline (if it's there).
    if (slen)
    {
        if (bRemoveNewLine)
        {
            if (pBuffer[slen - 1]=='\n')
            {
                pBuffer[slen - 1]='\0';
                --slen;
            }
        }
        return (ssize_t)slen;
    }
    return (ssize_t)-1;
}

void TextFile::write(const char* pBuffer, size_t nSize) throw(FileIOException)
{	
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    size_t result = fwrite(pBuffer, nSize, 1, m_pFile);
    if (result == nSize ) throw FileIOException(_T("fwrite failed."));
}

void TextFile::write(const char* szString) throw(FileIOException)
{
    FIRTEX_ASSERT(m_pFile, _T("Bad file handle"));
    int result = fputs(szString, m_pFile);
    if (result == EOF) throw FileIOException(_T("fputs failed."));
}

void TextFile::write(const char* szString, bool bNewLine) throw(FileIOException)
{
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    int result;
    if (bNewLine) result = fprintf(m_pFile,"%s\n",szString);
    else result = fputs(szString,m_pFile);
    if (result == EOF) throw FileIOException(_T("write data failed."));
}

void TextFile::close(void) throw(FileIOException)
{	
    FIRTEX_ASSERT(m_pFile,_T("Bad file handle") );
    if ( fclose(m_pFile) ) throw FileIOException(_T("close file failed."));
    m_pFile = NULL;
    m_sFileName = _T("");
}

FX_NS_END

