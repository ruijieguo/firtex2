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

#ifndef _ZIPFILE_H
#define _ZIPFILE_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/BinaryFile.h"

FX_NS_DEF(utility);

class ZIPFile
{
	// Construction
public:
	ZIPFile();
	~ZIPFile();

	// File Class
public:
	class File
	{
	private:				
		inline File(){}

	public:
		size_t decompress(byte* pOutBuffer,size_t outLen);
		bool extract(const tstring& sFile);

	protected:
		bool prepareToDecompress(void* pStream);

	public:
		tstring	m_sName;
		int64_t m_nSize;

	protected:
		int64_t	m_nLocalOffset;
		int64_t	m_nCompressedSize;
		int32_t	m_nCompression;			

	protected:
		ZIPFile* m_pZIP;

		friend class ZIPFile;
	};	

	// Operations
public:
	bool	open(const tstring& sFile);
	bool	attach(int hFile);
	bool	isOpen() const;
	void	close();

public:
	int32_t	getCount() const;
	File*	getFile(int32_t nFile) const;
	File*	getFile(const tstring& sFile, bool bPartial = false) const;

protected:
	bool	locateCentralDirectory();
	bool	parseCentralDirectory(byte* pDirectory, int32_t nDirectory);
	bool	seekToFile(File* pFile);

protected:
	File*	m_pFile;
	int		m_nFile;		
    BinaryFile m_fileReader;
};

FX_NS_END

#endif
