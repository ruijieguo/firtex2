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
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2007/1/9
//
#include "firtex/utility/ZIPFile.h"
#include "../external/zlib/zlib.h"
#include "firtex/utility/File.h"

FX_NS_DEF(utility);

/////////////////////////////////////////////////////////////////////////////
// ZIPFile construction

ZIPFile::ZIPFile()
    : m_pFile(NULL)
    , m_nFile(0)
{		

}

ZIPFile::~ZIPFile()
{
    close();
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile open

bool ZIPFile::open(const tstring& sFile)
{	

    close();

    try
    {
        m_fileReader.open(sFile, BinaryFile::READ);
    }
    catch(const FileIOException& )
    {
        return false;
    }

    if ( locateCentralDirectory() )
    {
        return true;
    }
    else
    {
        close();
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile open test

bool ZIPFile::isOpen() const
{
    return m_fileReader.isFileOpen();
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile close

void ZIPFile::close()
{
    if ( m_fileReader.isFileOpen())
    {							
        m_fileReader.close();
    }

    if ( m_pFile != NULL ) 
        delete [] m_pFile;
    m_pFile = NULL;
    m_nFile = 0;		
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile get the file count

int32_t ZIPFile::getCount() const
{
    return m_nFile;
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile get a particular file

ZIPFile::File* ZIPFile::getFile(int32_t nFile) const
{
    return ( nFile < 0 || nFile >= m_nFile ) ? NULL : m_pFile + nFile;
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile lookup a file by name

ZIPFile::File* ZIPFile::getFile(const tstring& sFile, bool bPartial) const
{
    File* pFile = m_pFile;

    for ( int nFile = m_nFile ; nFile ; nFile--, pFile++ )
    {
        if ( bPartial )
        {					
            const tchar* pszName = strFind(pFile->m_sName.c_str(), '/' );
            pszName = pszName ? pszName + 1 : pFile->m_sName.c_str();
            if (strCompareNoCase(pszName, sFile.c_str() ) == 0 ) return pFile;
        }
        else
        {
            if (strCompareNoCase(pFile->m_sName, sFile) == 0 ) return pFile;
        }
    }

    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile locate the central directory

#pragma pack(1)
typedef struct
{
    uint32_t	nSignature;			// 0x06054b50
    uint16_t	nThisDisk;
    uint16_t	nDirectoryDisk;
    uint16_t	nFilesThisDisk;
    uint16_t	nTotalFiles;
    uint32_t	nDirectorySize;
    uint32_t	nDirectoryOffset;
    uint16_t	nCommentLen;
} ZIP_DIRECTORY_LOC;
#pragma pack()

bool ZIPFile::locateCentralDirectory()
{
    byte pBuffer[4096];
    size_t nBuffer = 0;

    m_fileReader.seek(m_fileReader.getLength() - 4096);
    if ( (nBuffer = m_fileReader.read(pBuffer, 4096)) <= 0 )
        return false;
    if ( nBuffer < sizeof(ZIP_DIRECTORY_LOC) )
        return false;

    ZIP_DIRECTORY_LOC* pLoc = NULL;

    for (size_t nScan = 4 ; nScan < nBuffer ; nScan++ )
    {
        uint32_t* pnSignature = (uint32_t*)( pBuffer + nBuffer - nScan  );

        if ( *pnSignature == 0x06054b50 )
        {
            pLoc = (ZIP_DIRECTORY_LOC*)pnSignature;
            break;
        }
    }

    if ( pLoc == NULL )
        return false;
    FIRTEX_ASSERT(( pLoc->nSignature == 0x06054b50 ), _T("false signature."));

    if ( m_fileReader.getLength() < pLoc->nDirectorySize )
        return false;

    try 
    {
        m_fileReader.seek(pLoc->nDirectoryOffset);
    }
    catch(const FileIOException&)
    {
        return false;
    }

    byte* pDirectory = new byte[ pLoc->nDirectorySize ];
    nBuffer = m_fileReader.read(pDirectory, pLoc->nDirectorySize);

    if ( nBuffer == pLoc->nDirectorySize )
    {
        m_nFile = (int)pLoc->nTotalFiles;
        m_pFile = new File[ m_nFile ];

        if ( ! parseCentralDirectory( pDirectory, pLoc->nDirectorySize ) )
        {
            delete [] m_pFile;
            m_pFile = NULL;
            m_nFile = 0;
        }
    }

    delete [] pDirectory;

    return ( m_nFile > 0 );
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile parse the central directory

#pragma pack(1)
typedef struct
{
    uint32_t	nSignature;		// 0x02014b50
    uint16_t	nWriteVersion;
    uint16_t	nReadVersion;
    uint16_t	nFlags;
    uint16_t	nCompression;
    uint16_t	nFileTime;
    uint16_t	nFileDate;
    uint32_t	nCRC;
    uint32_t	nCompressedSize;
    uint32_t	nActualSize;
    uint16_t	nNameLen;
    uint16_t	nExtraLen;
    uint16_t	nCommentLen;
    uint16_t	nStartDisk;
    uint16_t	nInternalAttr;
    uint32_t	nExternalAttr;
    uint32_t	nLocalOffset;
} ZIP_CENTRAL_FILE;
#pragma pack()

bool ZIPFile::parseCentralDirectory(byte* pDirectory, int32_t nDirectory)
{
    for ( int nFile = 0 ; nFile < m_nFile ; nFile++ )
    {
        ZIP_CENTRAL_FILE* pRecord = (ZIP_CENTRAL_FILE*)pDirectory;

        if ((size_t)nDirectory < sizeof(*pRecord)) return false;
        if (pRecord->nSignature != 0x02014b50) return false;

        pDirectory += sizeof(*pRecord);
        nDirectory -= sizeof(*pRecord);

        int nTailLen = (int)pRecord->nNameLen + (int)pRecord->nExtraLen + (int)pRecord->nCommentLen;
        if ( nDirectory < (int32_t)nTailLen ) return false;

        m_pFile[ nFile ].m_pZIP				= this;
        m_pFile[ nFile ].m_nSize			= pRecord->nActualSize;
        m_pFile[ nFile ].m_nLocalOffset		= pRecord->nLocalOffset;
        m_pFile[ nFile ].m_nCompressedSize	= pRecord->nCompressedSize;
        m_pFile[ nFile ].m_nCompression		= pRecord->nCompression;

        m_pFile[ nFile ].m_sName.resize(pRecord->nNameLen);
        tchar* pszName = (tchar*)(m_pFile[ nFile ].m_sName.c_str());

        for ( uint16_t nChar = 0 ; nChar < pRecord->nNameLen ; nChar++ )
        {
            pszName[ nChar ] = (tchar)pDirectory[ nChar ];
            if ( pszName[ nChar ] == '\\' ) pszName[ nChar ] = '/';
        }				

        pDirectory += (int32_t)nTailLen;
        nDirectory -= (int32_t)nTailLen;//?
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile::File seek to a file

#pragma pack(1)
typedef struct
{
    uint32_t	nSignature;		// 0x04034b50
    uint16_t	nVersion;
    uint16_t	nFlags;
    uint16_t	nCompression;
    uint16_t	nFileTime;
    uint16_t	nFileDate;
    uint32_t	nCRC;
    uint32_t	nCompressedSize;
    uint32_t	nActualSize;
    uint16_t	nNameLen;
    uint16_t	nExtraLen;
} ZIP_LOCAL_FILE;
#pragma pack()	

bool ZIPFile::seekToFile(File* pFile)
{			
    FIRTEX_ASSERT(( pFile != NULL ),_T("Invalidate parameter"));
    FIRTEX_ASSERT(( pFile->m_pZIP == this ),_T("Invalidate parameter"));

    if (m_fileReader.isFileOpen() == false ) return false;

    try
    {
        m_fileReader.seek(pFile->m_nLocalOffset);
    }
    catch(const FileIOException& )
    {
        return false;
    }

    ZIP_LOCAL_FILE pLocal;
    size_t nRead = 0;

    nRead = m_fileReader.read(&pLocal, sizeof(pLocal));
    if ( nRead != sizeof(pLocal) ) return false;

    if ( pLocal.nSignature != 0x04034b50 ) return false;
    if ( pLocal.nCompression != Z_DEFLATED && pLocal.nCompression != 0 ) return false;

    m_fileReader.seekRelative(pLocal.nNameLen + pLocal.nExtraLen);

    return true;
}		

/////////////////////////////////////////////////////////////////////////////
// ZIPFile::File prepare to decompress

bool ZIPFile::File::prepareToDecompress(void* pStream)
{
    memset(pStream,0,sizeof(z_stream));			

    if ( ! m_pZIP->seekToFile( this ) ) return false;

    if ( m_nCompression == 0 )
    {
        return ( m_nSize == m_nCompressedSize );
    }
    else
    {
        FIRTEX_ASSERT(( m_nCompression == Z_DEFLATED ), _T("prepareToDecompress() failed."));
        return Z_OK == inflateInit2( (z_stream*)pStream, -MAX_WBITS );
    }
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile::File decompress to memory

size_t ZIPFile::File::decompress(byte* pOutBuffer, size_t outLen)
{
    z_stream pStream;

    if ( m_nSize > 32*1024*1024 ) 
        return 0;
    if ( ! prepareToDecompress( &pStream ) )
        return 0;

    if ( m_nCompression == 0 )
    {				
        if(!pOutBuffer)
            return (size_t)m_nSize;
        size_t len = (size_t)(m_pZIP->m_fileReader.read(pOutBuffer, (size_t)m_nSize));
        if ( len == (size_t)m_nSize ) 
            return len;				
        return 0;
    }

    size_t nSource = (size_t)m_nCompressedSize;
    byte* pSource = new byte[ nSource ];
    nSource = m_pZIP->m_fileReader.read(pSource, nSource);

    if ( nSource != (uint32_t)m_nCompressedSize )
    {				
        inflateEnd( &pStream );
        delete[] pSource;
        return 0;
    }		

    pStream.next_in		= pSource;
    pStream.avail_in	= (uint32_t)m_nCompressedSize;
    pStream.next_out	= pOutBuffer;
    pStream.avail_out	= (uint32_t)m_nSize;

    inflate( &pStream, Z_FINISH );

    delete [] pSource;

    if ( pStream.avail_out != 0 )
    {
        inflateEnd( &pStream );
        return 0;
    }

    inflateEnd( &pStream );

    return (size_t)m_nSize;
}

/////////////////////////////////////////////////////////////////////////////
// ZIPFile::File decompress to disk

const int32_t BUFFER_IN_SIZE = 64 * 1024u;
const int32_t BUFFER_OUT_SIZE = 128 * 1024u;

bool ZIPFile::File::extract(const tstring& sFile)
{
    z_stream pStream;
    BinaryFile bf;

    try
    {
        if ( FX_NS(utility)::File::exists(sFile) )
        {
            bf.open(sFile, BinaryFile::CRTR);
        }
        else 
        {
            bf.open(sFile, BinaryFile::CREATE);
        }
    }
    catch(const FileIOException& )
    {
        return false;
    }

    if ( ! prepareToDecompress( &pStream ) ) return false;

    int64_t nCompressed = 0, nUncompressed = 0;

    if ( m_nCompression == Z_DEFLATED )
    {
        byte* pBufferIn	= new byte[BUFFER_IN_SIZE];
        byte* pBufferOut = new byte[BUFFER_OUT_SIZE];

        while ( nCompressed < m_nCompressedSize || nUncompressed < m_nSize )
        {
            if ( pStream.avail_in == 0 )
            {
                pStream.avail_in = (uint32_t)((int32_t)(m_nCompressedSize - nCompressed) > BUFFER_IN_SIZE ) ? 
                                   BUFFER_IN_SIZE:(int32_t)(m_nCompressedSize - nCompressed);
                pStream.next_in		= pBufferIn;

                size_t nRead = m_pZIP->m_fileReader.read(pBufferIn, pStream.avail_in);
                if ( nRead != (size_t)pStream.avail_in ) break;
                nCompressed += nRead;
            }

            pStream.avail_out	= BUFFER_OUT_SIZE;
            pStream.next_out	= pBufferOut;

            /*int nInflate =*/ inflate( &pStream, Z_SYNC_FLUSH );

            if ((int32_t)pStream.avail_out < BUFFER_OUT_SIZE )
            {
                uint32_t nWrite = BUFFER_OUT_SIZE - pStream.avail_out;
                try
                {
                    bf.write(pBufferOut, nWrite) ;
                }
                catch(const FileIOException&)
                {
                    break;
                }
                nUncompressed += nWrite;
            }
        }

        delete [] pBufferOut;
        delete [] pBufferIn;

        inflateEnd( &pStream );
    }
    else
    {
        byte* pBufferOut = new byte[BUFFER_OUT_SIZE];

        while ( nUncompressed < m_nSize )
        {
            uint32_t nChunk = (uint32_t)((uint32_t)( m_nSize - nUncompressed) > (uint32_t)BUFFER_OUT_SIZE ) ? 
                              BUFFER_OUT_SIZE : (uint32_t)( m_nSize - nUncompressed);
            uint32_t nProcess = 0;

            nProcess = (uint32_t)m_pZIP->m_fileReader.read(pBufferOut, nChunk);
            if ( nChunk != nProcess ) break;
            try
            {
                bf.write(pBufferOut, nChunk);
            }
            catch(const FileIOException&)
            {
                break;
            }

            nCompressed += nChunk;
            nUncompressed += nChunk;
        }

        delete [] pBufferOut;
    }

    bf.close();

    if ( nUncompressed >= m_nSize ) 
        return true;

    FX_NS(utility)::File file(sFile);
    file.remove();

    return false;
}

FX_NS_END

